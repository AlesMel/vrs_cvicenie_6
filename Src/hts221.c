#include "hts221.h"
// temperature and humidity

uint8_t hts221_slave_address = HTS221_DEVICE_ADDRESS;

void HTS221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, hts221_slave_address, 0);
}

uint8_t HTS221_read_byte(uint8_t reg_address) {
	uint8_t read_data = 0;
	return *(i2c_master_read(&read_data, 1, reg_address, hts221_slave_address, 0));
}

uint8_t HTS221_power_up(void) {
	uint8_t reg_status = HTS221_read_byte(HTS221_CTRL_REG1);
	reg_status |= (1<<7); // power on
	reg_status |= (1<<0); // 1kHz
	HTS221_write_byte(HTS221_CTRL_REG1, reg_status);
	if (reg_status != HTS221_read_byte(HTS221_CTRL_REG1)) {
		return 0;
	}
	return 0;
}

uint8_t HTS221_init(void) {
	uint8_t status = 1;

	HTS221_power_up();

	LL_mDelay(100);

	// who am I value
	uint8_t wai_value = HTS221_read_byte(HTS221_WHO_AM_I_ADDRESS);

	if (wai_value == HTS221_WHO_AM_I_VALUE) {
		status = 1;
	} else {
		status = 0;
	}

}
