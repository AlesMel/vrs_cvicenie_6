#include "i2c.h"

#define HTS221_DEVICE_ADDRESS 0xBE // write

#define HTS221_WHO_AM_I_ADDRESS 0x0F
#define HTS221_WHO_AM_I_VALUE 0xBC

#define HTS221_CTRL_REG1 0x20

// temperature
#define HTS221_TEMP_OUT_L 0x2B
#define HTS221_TEMP_OUT_H 0x2C
#define HTS221_T0_DEG_C_x8 0x32
#define HTS221_T1_DEG_C_x8 0x33
#define HTS221_T1_T0_MSB 0x35
#define HTS221_T0_OUT_L	0x3C
#define HTS221_T0_OUT_H	0x3D
#define HTS221_T1_OUT_L	0x3E
#define HTS221_T1_OUT_H	0x3F

// humidity
#define HTS221_HUMIDITY_OUT_L 0x28
#define HTS221_HUMIDITY_OUT_H 0x29
#define HTS221_H0_RH_x2 0x30
#define HTS221_H1_RH_x2 0x31
#define HTS221_H0_T0_OUT_L 0x36
#define HTS221_H0_T0_OUT_H 0x37
#define HTS221_H1_T0_OUT_L 0x3A
#define HTS221_H1_T0_OUT_H 0x3B

uint8_t HTS221_read_byte(uint8_t reg_address);
uint8_t HTS221_init(void);
uint8_t HTS221_power_up(void);

void HTS221_read_byte_array(uint8_t* read_data, uint8_t reg_address, uint8_t length);
void HTS221_write_byte(uint8_t reg_addr, uint8_t value);

float HTS221_get_temperature();
float HTS221_get_humidity();
float HTS2();
