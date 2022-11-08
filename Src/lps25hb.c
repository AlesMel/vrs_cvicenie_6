#include "lps25hb.h"

uint8_t LPS25HB_slave_address = LPS25HB_DEVICE_ADDRESS_0;

uint8_t LPS25HB_read_byte(uint8_t reg_address) {
	uint8_t read_data = 0;
	return *(i2c_master_read(&read_data, 1, reg_address, LPS25HB_slave_address, 0));
}

uint8_t LPS25HB_write_byte(uint8_t reg_address, uint8_t value) {
	i2c_master_write(value, reg_address, LPS25HB_slave_address, 0);
}

void LPS25HB_power_up(void) {
	uint8_t status = LPS25HB_read_byte(LPS25HB_CTRL_REG1);
	status |= (1<<7); // power on
	status |= (1<<6); // 1kHz
	LPS25HB_write_byte(LPS25HB_CTRL_REG1, status);
}

uint8_t LPS25HB_init(void) {

	LPS25HB_power_up();

	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t wai_value = LPS25HB_read_byte(LPS25HB_WHO_AM_I_ADDRES);

	if(wai_value == LPS25HB_WHO_AM_I_VALUE) {
		status = 1;
	} else {
		LPS25HB_slave_address = LPS25HB_DEVICE_ADDRESS_1;
		wai_value = LPS25HB_read_byte(LPS25HB_WHO_AM_I_ADDRES);
		if(wai_value == LPS25HB_WHO_AM_I_VALUE) {
			status = 1;
		} else status = 0;

	}
}
