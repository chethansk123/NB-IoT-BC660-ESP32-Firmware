


#include"main.h"
#include "L1_hts221.h"
#include "mcal_i2c.h"

component_hts221 hts221 = {0};

 /*******************************************************************************
 * @brief
 *  Initialization of the temperature/humidity sensor HTS221. Configuration is made 
 *  in such way that the measurements are carried out in 1Hz continuous mode.
 *
 * @note
 *  -
 *
 * @param[out] state of initialization True=Init ok, False=Init failed
 *  
 *   
 ******************************************************************************/
bool HTS221_init(void) {
	uint8_t ui8_tmp = 0;
	
	// Read ASIC ID to check.
	if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_WHO_AM_I, &hts221.hts221_register_struct.who_am_i) == I2C_ERROR) {
		return false;
	}
	
	if (hts221.hts221_register_struct.who_am_i== HTS221_WHO_AM_I_VALUE) { //check whether the received ID is only the required ID
		// Read calibration data 
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_2, &ui8_tmp) == I2C_ERROR) {
			return false; // T0_degC_x8
		}
        
		hts221.hts221_cal_data.T0_degC = ui8_tmp;
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_5, &ui8_tmp) == I2C_ERROR) {
			return false; // T0_msb
		}
		hts221.hts221_cal_data.T0_degC |= ((uint16_t)(ui8_tmp & 0x03) << 8);
		hts221.hts221_cal_data.T0_degC /= 8;
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_3, &ui8_tmp) == I2C_ERROR) {
			return false; // T1_degC_x8
		}
		hts221.hts221_cal_data.T1_degC = ui8_tmp;
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_5, &ui8_tmp) == I2C_ERROR) {
			return false; // T1_msb
		}
		hts221.hts221_cal_data.T1_degC |= ((uint16_t)(ui8_tmp & 0x0C) << 6);
		hts221.hts221_cal_data.T1_degC /= 8;
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_D, &ui8_tmp) == I2C_ERROR) {
			return false; // T0_OUT High-Byte
		}
		hts221.hts221_cal_data.T0_OUT = ((uint16_t)ui8_tmp << 8);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_CALIB_C, &ui8_tmp) == I2C_ERROR) {
			return false; // T0_OUT Low-Byte
		}
		hts221.hts221_cal_data.T0_OUT |= (uint16_t)(ui8_tmp);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_F, &ui8_tmp) == I2C_ERROR) {
			return false; // T1_OUT High-Byte
		}
		hts221.hts221_cal_data.T1_OUT = ((uint16_t)ui8_tmp << 8);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_E, &ui8_tmp) == I2C_ERROR) {
			return false; // T1_OUT Low-Byte
		}
		hts221.hts221_cal_data.T1_OUT |= (uint16_t)(ui8_tmp);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_0, &ui8_tmp) == I2C_ERROR) {
			return false; // H0_rH_x2
		}
		hts221.hts221_cal_data.H0_rH = (ui8_tmp / 2);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_1, &ui8_tmp) == I2C_ERROR) {
			return false; // H1_rH_x2
		}
		hts221.hts221_cal_data.H1_rH = (ui8_tmp / 2);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_7, &ui8_tmp) == I2C_ERROR) {
			return false; // H0_T0_OUT High-Byte
		}
		hts221.hts221_cal_data.H0_T0_OUT = ((uint16_t)ui8_tmp << 8);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_CALIB_6, &ui8_tmp) == I2C_ERROR) {
			return false; // H0_T0_OUT Low-Byte
		}
		hts221.hts221_cal_data.H0_T0_OUT |= (uint16_t)(ui8_tmp);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_B, &ui8_tmp) == I2C_ERROR) {
			return false; // H1_T0_OUT High-Byte
		}
		hts221.hts221_cal_data.H1_T0_OUT = ((uint16_t)ui8_tmp << 8);
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_READ, HTS221_REG_CALIB_A, &ui8_tmp) == I2C_ERROR) {
			return false; // H1_T0_OUT Low-Byte
		}
		hts221.hts221_cal_data.H1_T0_OUT |= (uint16_t)(ui8_tmp);
		
		// Set settings
		hts221.hts221_register_struct.av_conf_bits.avgt = 3; //  Temperature, 16 Samples per measurement =
		hts221.hts221_register_struct.av_conf_bits.avgh = 3; //  Humidity, 32 Samples per Message
		// -> 2.1uA current at ODR 1Hz
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_WRITE,HTS221_REG_AV_CONF, &hts221.hts221_register_struct.av_conf) == I2C_ERROR) {
			return false;
		}
		
		hts221.hts221_register_struct.ctrl_reg1 = 0;
		hts221.hts221_register_struct.ctrl_reg1_bits.bdu = 0; // Continuous Update Mode
		hts221.hts221_register_struct.ctrl_reg1_bits.odr = 1; // 1Hz Operation
		hts221.hts221_register_struct.ctrl_reg1_bits.pd  = HTS221_POWER_MODE_POWER_DOWN; // power down mode activation
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_WRITE, HTS221_REG_CTRL_REG1, &hts221.hts221_register_struct.ctrl_reg1) == I2C_ERROR) {
			return false;
		}
		
		hts221.hts221_register_struct.ctrl_reg2 = 0;
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_WRITE, HTS221_REG_CTRL_REG2, &hts221.hts221_register_struct.ctrl_reg2) == I2C_ERROR) {
			return false;
		}
		
		hts221.hts221_register_struct.ctrl_reg3 = 0;
		hts221.hts221_register_struct.ctrl_reg3_bits.drdy = 0; // disable Data Ready interrupt on pin 3
		
		if (mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS,I2C_WRITE, HTS221_REG_CTRL_REG3, &hts221.hts221_register_struct.ctrl_reg3) == I2C_ERROR) {
			return false;
		}
		
		hts221.hts221_register_struct.status_reg_bits.t_da = 0;
		hts221.hts221_register_struct.status_reg_bits.h_da = 0;
	 }
	else {
		return false;
	}
	
	return true;
}


/*******************************************************************************
 * @brief
 *  Reads the current temperature
 *
 * @note
 * Interpolation formula
   y = y1 + (((x-x1)*(y2-y1))/(x2-x1))

 * MODIFICATION HISTORY:
   @author : SK
   @date : 18/03/2022
   1.	All i2c operationfuctions are replace as 
   1.	mcal_timer_delay_ms is commented in the fucntion
   2.	break is added inside first while loop which is checking for status resgister which monitor change in the temperature data

 * @param[out] Temperature value (16 bit integer) with one decimal place temperature.
 * 
 *   
 ******************************************************************************/
bool read_HTS221_temperature(int16_t* result_ptr) {
	uint8_t i2c_state;                                      // changed the data type
	static int16_t temperature_result = 0;
	static int16_t t_out = 0;
	int32_t zaehler;
	int32_t nenner;
	
	hts221.hts221_register_struct.status_reg = 0;
	
	// wait in the loop until the measurement result is available
	while(hts221.hts221_register_struct.status_reg_bits.t_da == 0) {
		i2c_state = mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_STATUS_REG, &hts221.hts221_register_struct.status_reg);
		
		if (i2c_state != I2C_SUCCESS) {
			return false;
		}
        break;                        // added for testing
		
		//mcal_timer_delay_ms(100);         // commented for testing
	}
	
	// Read temperature register
	i2c_state = mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_TEMP_OUT_L, &hts221.hts221_register_struct.temp_out_l);
	
	if (i2c_state != I2C_SUCCESS) {
		return false;
	}
	
	i2c_state = mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_TEMP_OUT_H, &hts221.hts221_register_struct.temp_out_h);
	
	if (i2c_state != I2C_SUCCESS) {
		return false;
	}
	
	// combine to 16bit value
	t_out = (int16_t)(((uint16_t)hts221.hts221_register_struct.temp_out_l) | (uint16_t)(hts221.hts221_register_struct.temp_out_h << 8));
	
	// calculate actual temperature from calibration data.
	zaehler = ((10*hts221.hts221_cal_data.T1_degC - hts221.hts221_cal_data.T0_degC)*(t_out - hts221.hts221_cal_data.T0_OUT));
	nenner = (hts221.hts221_cal_data.T1_OUT - hts221.hts221_cal_data.T0_OUT);
	temperature_result = (zaehler/nenner) + ( 10*hts221.hts221_cal_data.T0_degC);
	//temperature_result = ( (10*(hts221_cal_data.T1_degC - hts221_cal_data.T0_degC)*(t_out - hts221_cal_data.T0_OUT))/
	//						(hts221_cal_data.T1_OUT - hts221_cal_data.T0_OUT) ) + (10 * hts221_cal_data.T0_degC);
	
	*result_ptr = temperature_result;
	
	hts221.hts221_register_struct.status_reg_bits.t_da = 0;
	
	return true;
}

 /*******************************************************************************
 * @brief
 *  Liest die aktuelle Feuchte aus
 *
 * @note
 * MODIFICATION HISTORY:
   @author : SK
   @date : 18/03/2022
   1.	mcal_timer_delay_ms is commented in the fucntion
   2.	break is added inside first while loop which is checking for status resgister which monitor change in the temperature data
 * 
 * @param[out] Humidity value (16 bit Integer) with one decimal place
 *  
 * Interpolation formula
 * y = y1 + (((x-x1)*(y2-y1))/(x2-x1))
 * 
 * 
 *   
 ******************************************************************************/

bool read_HTS221_humidity(int16_t* result_ptr) {
	uint8_t i2c_state;
	static int16_t humidity_result = 0;
	static int16_t h_t_out = 0;
	int32_t zaehler;
	int32_t nenner;
	
	hts221.hts221_register_struct.status_reg = 0;
	
	// wait in the loop until the measurement result is available
	while(hts221.hts221_register_struct.status_reg_bits.h_da == 0) {
		i2c_state = mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_STATUS_REG, &hts221.hts221_register_struct.status_reg);
		
		if (i2c_state != I2C_SUCCESS) {
			return false;
		}

        break; // added for testing
		
		//mcal_timer_delay_ms(100);
	}
	
	// Read humidity register
	i2c_state = mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_HUMIDITY_OUT_L, &hts221.hts221_register_struct.humidity_out_l);
	
	if (i2c_state != I2C_SUCCESS) {
		return false;
	}
	
	i2c_state = mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_READ,HTS221_REG_HUMIDITY_OUT_H, &hts221.hts221_register_struct.humidity_out_h);
	
	if (i2c_state != I2C_SUCCESS) {
		return false;
	}
	
	// combine to 16-bit value
	h_t_out = (int16_t)(((uint16_t)hts221.hts221_register_struct.humidity_out_l) | (uint16_t)(hts221.hts221_register_struct.humidity_out_h << 8));
	
//	#ifdef DEBUG_HTS221
//		uint8_t tempbuf[32] = {0};
//		printf("H_OUT: %02X%02X\r\n", (uint8_t)(h_t_out>>8), (uint8_t)(h_t_out&0x00FF));
//	#endif
	
	// Calculate actual humidity from calibration data
	zaehler = ((10*hts221.hts221_cal_data.H1_rH - hts221.hts221_cal_data.H0_rH)*(h_t_out - hts221.hts221_cal_data.H0_T0_OUT));
	nenner = (hts221.hts221_cal_data.H1_T0_OUT - hts221.hts221_cal_data.H0_T0_OUT);
	
	humidity_result = (zaehler/nenner) + (10*hts221.hts221_cal_data.H0_rH);
	//humidity_result = ( (uint32_t)(10*(hts221_cal_data.H1_rH - hts221_cal_data.H0_rH)*(h_t_out - hts221_cal_data.H0_T0_OUT))/
	//		(hts221_cal_data.H1_T0_OUT - hts221_cal_data.H0_T0_OUT) ) + (10 * hts221_cal_data.H0_rH);
	
	*result_ptr = humidity_result;
	
	hts221.hts221_register_struct.status_reg_bits.h_da = 0;
	
	return true;
}

/*******************************************************************************
 * @brief
 *  Setzt Power-Mode des HTS221
 *
 * @note
 *  -
 *
 * @param[in] power_mode (Sleep-Mode oder Active-Mode)
 *   
 *   
 ******************************************************************************/
bool set_HTS221_power_mode(hts221_power_mode_t power_mode) {
	uint8_t i2c_state;
	
	hts221.hts221_register_struct.ctrl_reg1_bits.pd = power_mode;
	
	i2c_state = mcal_I2C_Master_register_operation(I2C0, HTS221_I2C_ADDRESS, I2C_WRITE,HTS221_REG_CTRL_REG1, &hts221.hts221_register_struct.ctrl_reg1);
	
	if (i2c_state == I2C_SUCCESS) {
		return true;
	}
	else 
	{
		return false;
	}
}