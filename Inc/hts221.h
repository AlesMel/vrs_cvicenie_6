#include "i2c.h"

#define HTS221_DEVICE_ADDRESS 0xBE // write

#define HTS221_WHO_AM_I_ADDRESS 0x0F
#define HTS221_WHO_AM_I_VALUE 0xBC

#define HTS221_CTRL_REG1 0x20

// temperature
#define HTS221_TEMP_OUT_L 0x2B
#define HTS221_TEMP_OUT_H 0x2C

// humidity
#define HTS221_HUMIDITY_OUT_L 0x28
#define HTS221_HUMIDITY_OUT_H 0x29

uint8_t HTS221_read_byte(uint8_t reg_address);
uint8_t HTS221_init(void);
uint8_t HTS221_power_up(void);

void HTS221_read_byte_array(uint8_t* read_data, uint8_t reg_address, uint8_t length);
void HTS221_write_byte(uint8_t reg_addr, uint8_t value);

float HTS221_get_temperature();
float HTS221_get_humidity();
