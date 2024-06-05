#ifndef HTS221_H__
#define HTS221_H__
#include <stdint.h>
#include <stdbool.h>

#define HTS221_WHO_AM_I_VALUE	0xBC

#define HTS221_I2C_ADDRESS	 0x5F


typedef enum hts221_power_mode_t_ {
	HTS221_POWER_MODE_POWER_DOWN = 0,
	HTS221_POWER_MODE_ACTIVE = 1,
} hts221_power_mode_t;

typedef enum 
{
    HTS221_REG_WHO_AM_I		= 0x0F,
	HTS221_REG_AV_CONF		= 0x10,
	HTS221_REG_CTRL_REG1	= 0x20,
	HTS221_REG_CTRL_REG2	= 0x21,
	HTS221_REG_CTRL_REG3	= 0x22,
	HTS221_REG_STATUS_REG	= 0x27,
	HTS221_REG_HUMIDITY_OUT_L = 0x28,
	HTS221_REG_HUMIDITY_OUT_H = 0x29,
	HTS221_REG_TEMP_OUT_L	= 0x2A,
	HTS221_REG_TEMP_OUT_H	= 0x2B,
	HTS221_REG_CALIB_0		= 0x30,
	HTS221_REG_CALIB_1		= 0x31,
	HTS221_REG_CALIB_2		= 0x32,
	HTS221_REG_CALIB_3		= 0x33,
	HTS221_REG_CALIB_4		= 0x34,
	HTS221_REG_CALIB_5		= 0x35,
	HTS221_REG_CALIB_6		= 0x36,
	HTS221_REG_CALIB_7		= 0x37,
	HTS221_REG_CALIB_8		= 0x38,
	HTS221_REG_CALIB_9		= 0x39,
	HTS221_REG_CALIB_A		= 0x3A,
	HTS221_REG_CALIB_B		= 0x3B,
	HTS221_REG_CALIB_C		= 0x3C,
	HTS221_REG_CALIB_D		= 0x3D,
	HTS221_REG_CALIB_E		= 0x3E,
	HTS221_REG_CALIB_F		= 0x3F,
}hts221_register_map_addresses_t;

typedef struct hts221_register_map_t_ {
	uint8_t who_am_i;
	
	union {
		uint8_t av_conf;
		struct {
			uint8_t avgh: 3;
			uint8_t avgt: 3;
			uint8_t reserved: 2;
		} av_conf_bits;
	};
	
	union {
		uint8_t ctrl_reg1;
		struct {
			uint8_t odr: 2;
			uint8_t bdu: 1;
			uint8_t reserved: 4;
			uint8_t pd: 1;
		} ctrl_reg1_bits;
	};
	
	union {
		uint8_t ctrl_reg2;
		struct {
			uint8_t one_shot: 1;
			uint8_t heater: 1;
			uint8_t reserved: 5;
			uint8_t boot: 1;
		} ctrl_reg2_bits;
	};
	
	union {
		uint8_t ctrl_reg3;
		struct {
			uint8_t reserved1: 2;
			uint8_t drdy: 1;
			uint8_t reserved2: 3;
			uint8_t pp_od: 1;
			uint8_t drdy_h_l: 1;
		} ctrl_reg3_bits;
	};
	
	union {
		uint8_t status_reg;
		struct {
			uint8_t t_da: 1;
			uint8_t h_da: 1;
			uint8_t reserved: 6;
		} status_reg_bits;
	};
	
	uint8_t humidity_out_l;
	uint8_t humidity_out_h;
	uint8_t temp_out_l;
	uint8_t temp_out_h;
} hts221_register_map_t;

typedef struct hts221_calibration_data_t_ {
	int16_t T0_degC;
	int16_t T1_degC;
	int16_t T0_OUT;
	int16_t T1_OUT;
	int16_t H0_rH;
	int16_t H1_rH;
	int16_t H0_T0_OUT;
	int16_t H1_T0_OUT;
} hts221_calibration_data_t;


typedef struct
{
    hts221_register_map_t hts221_register_struct;
    hts221_calibration_data_t hts221_cal_data;  
   	
}component_hts221;

bool HTS221_init(void);
bool read_HTS221_temperature(int16_t* result_ptr);
bool read_HTS221_humidity(int16_t* result_ptr);
bool set_HTS221_power_mode(hts221_power_mode_t power_mode);


#endif