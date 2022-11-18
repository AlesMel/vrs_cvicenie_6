#include "lps25hb.h"

uint8_t LPS25HB_slave_address = LPS25HB_DEVICE_ADDRESS_0;

void LPS25HB_read_byte_array(uint8_t* read_data, uint8_t reg_address, uint8_t size) {
	i2c_master_read(read_data, size, reg_address, LPS25HB_slave_address, 1);
}

uint8_t LPS25HB_read_byte(uint8_t reg_address) {
	uint8_t read_data = 0;
	return *(i2c_master_read(&read_data, 1, reg_address, LPS25HB_slave_address, 0));
}

void LPS25HB_write_byte(uint8_t reg_address, uint8_t value) {
	i2c_master_write(value, reg_address, LPS25HB_slave_address, 0);
}

uint8_t LPS25HB_power_up(void) {
	uint8_t reg_status = LPS25HB_read_byte(LPS25HB_CTRL_REG1);
	reg_status |= (1 << 7); // power on
	reg_status |= (1 << 4); // 1kHz

	LPS25HB_write_byte(LPS25HB_CTRL_REG1, reg_status);

	if (reg_status != LPS25HB_read_byte(LPS25HB_CTRL_REG1)) {
		return 0;
	}
	return 1;
}

uint8_t LPS25HB_init(void) {
	uint8_t status = 1;

	LL_mDelay(100);

	uint8_t wai_value = LPS25HB_read_byte(LPS25HB_WHO_AM_I_ADDRESS);

	if(wai_value == LPS25HB_WHO_AM_I_VALUE) {
		status = 1;
	} else {
		LPS25HB_slave_address = LPS25HB_DEVICE_ADDRESS_1;
		wai_value = LPS25HB_read_byte(LPS25HB_WHO_AM_I_ADDRESS);
		if(wai_value == LPS25HB_WHO_AM_I_VALUE) {
			status = 1;
		} else status = 0;
	}

	LPS25HB_power_up();

	return status;
}

float LPS25HB_get_pressure(void) {
//	The pressure output value is a 24-bit data that contains the measured pressure. It is
//	composed of PRESS_OUT_H (2Ah), PRESS_OUT_L (29h) and PRESS_OUT_XL (28h).
//	The value is expressed as 2’s complement.
	uint8_t pressure_reading[3] = {0};
	LPS25HB_read_byte_array(pressure_reading, LPS25HB_PRESS_OUT_XL, 3);


	float pressure_sensitivity = 4096.0;
//	float pressure = (pressure_reading[2]<<16 | pressure_reading[1]<<8 | pressure_reading[0]);
	float calc_pressure = (pressure_reading[2] * 65536 + pressure_reading[1] * 256 + pressure_reading[0]);

	return calc_pressure / pressure_sensitivity;
}



