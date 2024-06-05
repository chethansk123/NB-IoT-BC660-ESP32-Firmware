#ifndef __MCAL_I2C_H__
#define __MCAL_I2C_H__

#include<stdint.h>

#define I2C_ERROR               1
#define I2C_SUCCESS             0

#define I2C_READ  1 
#define I2C_WRITE 0

typedef enum
{
    I2C0_MASTER_SDA_IO = 21,      //21
    I2C0_MASTER_SCL_IO,
}mcal_i2c_gpionum_t;

typedef enum
{
    I2C0,
    I2C1,

}mcal_i2c_num_t;


typedef enum 
{
    I2C0_MASTER_FREQ_HZ = 90000,
    I2C0_MASTER_TX_BUF_DISABLE = 0,
    I2C0_MASTER_RX_BUF_DISABLE = 0,

}mcal_i2c0_configuration_t;


int8_t mcal_I2C_Master_register_operation(mcal_i2c_num_t i2c_num, uint8_t deviceid,uint8_t read_write, uint8_t reg_addr, uint8_t *data);
int8_t mcal_i2c_write (mcal_i2c_num_t i2c_num, uint8_t deviceid, uint8_t reg_addr, uint8_t *data);
int8_t mcal_i2c_read (mcal_i2c_num_t i2c_num, uint8_t deviceid,uint8_t reg_addr, uint8_t *data);
int i2c_master_init(void);
int8_t mcal_i2c_read_multi (mcal_i2c_num_t i2c_num, uint8_t deviceid, uint8_t reg_addr, uint8_t *data, uint32_t datalen);
int8_t mcal_i2c_write_multi (mcal_i2c_num_t i2c_num, uint8_t deviceid, uint8_t reg_addr, uint8_t *data, uint8_t datalen);
#endif