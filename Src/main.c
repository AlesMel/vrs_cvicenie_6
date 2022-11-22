/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "gpio.h"
#include "usart.h"
#include "lis3mdltr.h"
#include "lsm6ds0.h"
#include "hts221.h"
#include "stdio.h"
#include "string.h"
#include "dma.h"
#include "math.h"
#include "../pressure/lps25hb.h"

#define CHAR_BUFF_SIZE	30
#define LPF_BAR_ALPHA 0.25f /* Filter coefficient */
#define INITIAL_PRESSURE_SAMPLE_COUNT 20
#define DENSITY_AIR_KG_M3 1.225f
#define GRAVITY_EARTH_MPS2 9.81f
#define PRESSURE_SEA_LEVEL_PA 101325.0f

void SystemClock_Config(void);

uint8_t temp = 0;
char formated_text[128];

float current_temperature = 0.0f;
float current_humidity = 0.0f;

float current_pressure = 0.0f; /* procPressure_Pa current pressure (low-pass filtered) */
float ref_avg_pressure = 0.0f; /* procInitPressure_Pa initial average pressure */
float raw_pressure = 0.0f; /*rawPressure_Pa */

float estimated_altitude = 0.0f;
float relative_altitude = 0.0f;
float reference_altitude = 0.0f;

float data_rate = 0.0f;
uint8_t data_count = 0;
uint8_t init_pressure_calculation = 0;

// low-pass filter
void process_pressure(float pressure) {
	raw_pressure = pressure;

	/* Low-pass filter 'raw' pressure measurement */
	current_pressure = LPF_BAR_ALPHA * current_pressure + (1.0f - LPF_BAR_ALPHA) * pressure;

	/* During initialization process, compute average pressure */
	if (!init_pressure_calculation) {
		if (data_count < INITIAL_PRESSURE_SAMPLE_COUNT) {
			ref_avg_pressure += pressure;
			data_count++;
		} else {
			ref_avg_pressure /= ((float) INITIAL_PRESSURE_SAMPLE_COUNT);
			init_pressure_calculation = 1;
		}
	}

	// times 1000 because of cm to m
	//relative_altitude = (-(current_pressure - ref_avg_pressure) / (DENSITY_AIR_KG_M3 * GRAVITY_EARTH_MPS2));
}

void calculate_altitude() {
	// Hypsometric formula
	// https://physics.stackexchange.com/questions/333475/how-to-calculate-altitude-from-current-temperature-and-pressure
	float pressure_ratio = ref_avg_pressure / current_pressure; // reference pressure vs current pressure
	float press_pw = powf(pressure_ratio, (1 / 5.257));
	relative_altitude = ((press_pw - 1) * (current_temperature + 273.15)) / 0.0065;
}

int main(void) {
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	SystemClock_Config();

	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_DMA_Init();
	MX_USART2_UART_Init();

	LPS25HB_init();
	HTS221_init();

	while (1) {
		current_pressure = LPS25HB_get_pressure();
		current_temperature = HTS221_get_temperature();

		if (init_pressure_calculation) {
			current_humidity = HTS221_get_humidity();

			process_pressure(current_pressure);

			calculate_altitude();

			estimated_altitude = -(current_pressure * pow(10, 2) - PRESSURE_SEA_LEVEL_PA) / (DENSITY_AIR_KG_M3 * GRAVITY_EARTH_MPS2);

			memset(formated_text, '\0', sizeof(formated_text));
			sprintf(formated_text, "%4.2f, %2.1f, %2.0f, %3.2f, %3.2f\r", current_pressure, current_temperature, current_humidity, estimated_altitude, relative_altitude);
			USART2_PutBuffer((uint8_t*) formated_text, strlen(formated_text));
			LL_mDelay(25);
		} else {
			process_pressure(current_pressure);
		}
	}
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

  if(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0) {
	  Error_Handler();
  }
  LL_RCC_HSI_Enable();

   /* Wait till HSI is ready */
  while(LL_RCC_HSI_IsReady() != 1) {
    
  }
  LL_RCC_HSI_SetCalibTrimming(16);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
  {
  
  }
  LL_Init1msTick(8000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(8000000);
  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_HSI);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
