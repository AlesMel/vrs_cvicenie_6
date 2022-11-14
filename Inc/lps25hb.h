#include "i2c.h"

// barometer

#define LPS25HB_DEVICE_ADDRESS_0 0xB9
#define LPS25HB_DEVICE_ADDRESS_1 0xBB

#define LPS25HB_WHO_AM_I_VALUE 0xBD
#define LPS25HB_WHO_AM_I_ADDRES 0x0F

#define LPS25HB_CTRL_REG1 0x20

// PRESSURE BUFFER [3 bytes]
#define LPS25HB_PRESS_OUT_H 0x2A
#define LPS25HB_PRESS_OUT_L 0x29
#define LPS25HB_PRESS_OUT_XL 0x28

uint8_t LPS25HB_init(void);
uint8_t LPS25HB_read_byte(uint8_t reg_address);
uint8_t LPS25HB_power_up(void);

void LPS25HB_read_byte_array(uint8_t* data, uint8_t reg_adress, uint8_t size);
void LPS25HB_write_byte(uint8_t reg_address, uint8_t value);
float LPS25HB_get_pressure();
