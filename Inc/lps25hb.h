#include "i2c.h"

// barometer

#define LPS25HB_DEVICE_ADDRESS_0 0xB8
#define LPS25HB_DEVICE_ADDRESS_1 0xBA

#define LPS25HB_WHO_AM_I_VALUE 0xBD
#define LPS25HB_WHO_AM_I_ADDRES 0x0F

#define LPS25HB_CTRL_REG1 0x20

// PRESSURE BUFFER [3 bytes]
#define LPS25HB_REF_P_H 0x0A
#define LPS25HB_REF_P_L 0x09
#define LPS25HB_REF_P_XL 0x08

uint8_t LPS25HB_init(void);
uint8_t LPS25HB_read_byte(uint8_t reg_address);
uint8_t LPS25HB_write_byte(uint8_t reg_address, uint8_t value);

void LPS25HB_get_pressure();
void LPS25HB_power_up(void);
