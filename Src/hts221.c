#include "hts221.h"
// temperature and humidity

uint8_t hts221_slave_address = HTS221_DEVICE_ADDRESS;

void HTS221_write_byte(uint8_t reg_addr, uint8_t value)
{
	i2c_master_write(value, reg_addr, hts221_slave_address, 0);
}

void HTS221_read_byte_array(uint8_t* read_data, uint8_t reg_address, uint8_t length) {
	i2c_master_read(read_data, length, reg_address, hts221_slave_address, 0);
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
	return reg_status;
}

uint8_t HTS221_init(void) {
	uint8_t status = 1;


	LL_mDelay(100);

	// who am I value
	uint8_t wai_value = HTS221_read_byte(HTS221_WHO_AM_I_ADDRESS);

	if (wai_value == HTS221_WHO_AM_I_VALUE) {
		status = 1;
	} else {
		status = 0;
	}

	HTS221_power_up();
	return status;
}

float HTS221_get_temperature(){
	// read 2 bytes from HTS221_T0_OUT_L make it 16 bit number
	uint8_t t_out_array[2] = {0};
	uint8_t t0_out_array[2] = {0};
	uint8_t t1_out_array[2] = {0};

	HTS221_read_byte_array(t_out_array, HTS221_TEMP_OUT_L, 2);
	HTS221_read_byte_array(t0_out_array, HTS221_T0_OUT_L, 2);
	HTS221_read_byte_array(t1_out_array, HTS221_T1_OUT_L, 2);

	int16_t t_out = t_out_array[0];
	t_out |= (t_out_array[1] << 8);

	int16_t t0_out = t0_out_array[0];
	t0_out |= (t0_out_array[1] << 8);

	int16_t t1_out = t1_out_array[0];
	t1_out |= (t1_out_array[1] << 8);

	uint8_t t0_deg_c_x8 = HTS221_read_byte(HTS221_T0_DEG_C_x8);
	int8_t t1_deg_c_x8 = HTS221_read_byte(HTS221_T1_DEG_C_x8);
	uint8_t t1_t0_msb = HTS221_read_byte(HTS221_T0_DEG_C_x8); 	// Temperature interpolationQ

	float T0_degC = (t0_deg_c_x8 + (1 << 8) * (t1_t0_msb & 0x03)) / 8.0;
	// Value is in 3rd and fourth bit, so we only need to shift this value 6 more bits
	float T1_degC = (t1_deg_c_x8 + (1 << 6) * (t1_t0_msb & 0x0C)) / 8.0;
	float T_DegC = (T0_degC + (t_out - t0_out) * (T1_degC - T0_degC) / (t1_out - t0_out));

	return T_DegC;
}

float HTS221_get_humidity(){
	// read 2 bytes from HTS221_T0_OUT_L make it 16 bit number
	int8_t h0_t0_array[2] = {0};
	int8_t h1_t0_array[2] = {0};
	int8_t h_out_array[2] = {0};
	uint8_t h0_rh_x2 = HTS221_read_byte(HTS221_H0_RH_x2);
	uint8_t h1_rh_x2 = HTS221_read_byte(HTS221_H1_RH_x2);

	HTS221_read_byte_array(h0_t0_array, HTS221_H0_T0_OUT_L, 2);
	HTS221_read_byte_array(h1_t0_array, HTS221_H1_T0_OUT_L, 2);
	HTS221_read_byte_array(h_out_array, HTS221_HUMIDITY_OUT_L, 2);

	int16_t h0_t0_out = HTS221_read_byte(HTS221_H0_T0_OUT_L);
	h0_t0_out |= (HTS221_read_byte(HTS221_H0_T0_OUT_H) << 8);

	int16_t h1_t0_out = HTS221_read_byte(HTS221_H1_T0_OUT_L);
	h1_t0_out |= (HTS221_read_byte(HTS221_H1_T0_OUT_H) << 8);

	int16_t h_out = HTS221_read_byte(HTS221_HUMIDITY_OUT_L);
	h_out |= (HTS221_read_byte(HTS221_HUMIDITY_OUT_H) << 8);

	float humidity;
	float h0 = h0_rh_x2 / 2.0;
	float h1 = h1_rh_x2 / 2.0;
	humidity = (h0 + (h_out - h0_t0_out) * (h1 - h0) / (h1_t0_out - h0_t0_out));

//	return humidity;
}
