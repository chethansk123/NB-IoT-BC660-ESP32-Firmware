/****************************************************************************/
/** 
* SenseGuard gmbh
* @file mcal_i2c.c
* @{
* @details
* 
* This file contains the \e mcal function for the <b> ESP32 I2C </b>().
* 
* <b> Major Tasks </b> of this file includes
*1. Initialize the requested i2c configuration 
*2. i2c based external device register read write operation
*3. i2c send 
*4. i2c receive
* 
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date        Changes
* ----- ------ --------    ---------------------------------------------------------------------
* 1.0   sk    18-03-2022   First version
*
* </pre>
*****************************************************************************/
/** @} */

/***************************************includes*****************************/
#include"mcal_i2c.h"
#include "driver/i2c.h"


/*******************************preprocessor**********************************/
#define READ_OPERATION      1 
#define WRITE_OPERATION     0




/*****************************************************************************/
/**
* \brief i2c initialization
*
* \description
*   This function sets the various parameter of the i2c and bus configuration
*
* @param      None
*  
* @return     initialization status
*
* @author     SK 
*
* @date       18/03/2022
*
* @note
********************************************************************************/
int i2c_master_init(void)
{
    int i2c_master_port = I2C0;          // i2c 0 is choosen for configuration

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,            
        .sda_io_num = I2C0_MASTER_SDA_IO,       // set i2c data pin 
        .scl_io_num = I2C0_MASTER_SCL_IO,       // set i2c clock pin 
        .sda_pullup_en = GPIO_PULLUP_ENABLE,     // enable internal pull up on data pin 
        .scl_pullup_en = GPIO_PULLUP_ENABLE,     // enable internal pull up on i2c clock pin 
        .master.clk_speed = I2C0_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);       // configures the requested i2c parameter

    return i2c_driver_install(i2c_master_port, conf.mode, I2C0_MASTER_RX_BUF_DISABLE, I2C0_MASTER_TX_BUF_DISABLE, 0);    // install the i2c master driver
}


/*****************************************************************************/
/**
* \brief external device register operation
*
* \description
*   This function wtires and reads the internal register of the external i2c based ASIC
*
* @param      i2c_num, deviceid,read_write,reg_agg and data
*  
* @return     0(SUCCESS) or 1(ERROR)
*
* @author     SK 
*
* @date       18/03/2022
*
* @note
********************************************************************************/
int8_t mcal_I2C_Master_register_operation(mcal_i2c_num_t i2c_num, uint8_t deviceid,uint8_t read_write, uint8_t reg_addr, uint8_t *data)
{
    int8_t status = I2C_SUCCESS;
    
    if(read_write == READ_OPERATION)                 // checks whether requested opertion is read operation
    {
        status = mcal_i2c_read(i2c_num,deviceid,reg_addr,data);  
    }
    else if(read_write == WRITE_OPERATION)           // checks whether requested operation is write operation
    {
        status = mcal_i2c_write(i2c_num,deviceid,reg_addr,data);   

    }
    return status;
}

/*****************************************************************************/
/**
* \brief i2c read function
*
* \description
*   This function performs the i2c read operation, the fuction performs the 
    i2c bus sequence as mentioned in the esp32 IDF official website.
*
* @param      i2c_num, deviceid,reg_adr and data
*  
* @return      0(SUCCESS) or 1(ERROR)
*
* @author     SK 
*
* @date       18/03/2022
*
* @note
********************************************************************************/
int8_t mcal_i2c_read (mcal_i2c_num_t i2c_num, uint8_t deviceid, uint8_t reg_addr, uint8_t *data)
{
    uint8_t deviceid_RW;
    int8_t status=I2C_SUCCESS;
    deviceid_RW = (deviceid<<1)| (WRITE_OPERATION);     // device id last bit is padded with the write operation
    i2c_cmd_handle_t read_cmd;
    read_cmd =  i2c_cmd_link_create();                 // i2c command link is created to start the i2c operation
      if(read_cmd == NULL)                             // checks for link creation 
      {
          return I2C_ERROR;
      }
      else
      {
        status |= i2c_master_start(read_cmd);                 // enables the start bit of i2c operation
        status |= i2c_master_write_byte(read_cmd,deviceid_RW,1);     // writes device id with the requested operation
        status |=i2c_master_write_byte(read_cmd,reg_addr,1);          // writes the requested register address
        status |=i2c_master_start(read_cmd);                         // again enables the start bit of i2c operation
        deviceid_RW =  (deviceid<<1)|READ_OPERATION;       // device id last bit is padded with the read operation
        status |=i2c_master_write_byte(read_cmd,deviceid_RW,1);       // writes device id with the requested operation
     
        status |=i2c_master_read(read_cmd,data,1,1);             // reads the data stored in the requested register
        status |=i2c_master_stop(read_cmd);                      // enables the stop bit of i2c operation
        status |=i2c_master_cmd_begin(0,read_cmd,0xFFFF);        // initiate the i2c bus operation based on above configuration
        i2c_cmd_link_delete(read_cmd);                          // i2c created command link is deleted to release the resource
      }  
    return status;  
}

/*****************************************************************************/
/**
* \brief i2c write function
*
* \description
*   This function performs the i2c write operation, the fuction performs the 
    i2c bus sequence as mentioned in the esp32 IDF official website.
*
* @param      i2c_num, deviceid,reg_adr and data
*  
* @return     0(SUCCESS) or 1(ERROR)
*
* @author     SK 
*
* @date       18/03/2022
*
* @note
********************************************************************************/
int8_t mcal_i2c_write (mcal_i2c_num_t i2c_num, uint8_t deviceid, uint8_t reg_addr, uint8_t *data)
{
    uint8_t deviceid_RW,status=I2C_SUCCESS;
    deviceid_RW = (deviceid<<1)| (WRITE_OPERATION);       // device id last bit is padded with the write operation
    i2c_cmd_handle_t write_cmd;
    write_cmd =  i2c_cmd_link_create();                    // i2c command link is created to start the i2c operation
    if(write_cmd == NULL)                                  // checks for link creation 
    {
        return I2C_ERROR;
    }
    else
    {
    status |=i2c_master_start(write_cmd);                   // enables the start bit of i2c operation
    status |=i2c_master_write_byte(write_cmd,deviceid_RW,1);    // writes device id with the requested operation
    status |=i2c_master_write_byte(write_cmd,reg_addr,1);       // writes the requested register address
    status |=i2c_master_write(write_cmd,data,1,1);               // writes required data to requested register
    status |=i2c_master_stop(write_cmd);                           // enables the stop bit of i2c operation
    status |=i2c_master_cmd_begin(0,write_cmd,0xFFFF);              // initiate the i2c bus operation based on above configuration
    i2c_cmd_link_delete(write_cmd);
    }
    return status;

}


/*****************************************************************************/
/**
* \brief i2c read function
*
* \description
*   This function performs the i2c read operation, the fuction performs the 
    i2c bus sequence as mentioned in the esp32 IDF official website.
*
* @param      i2c_num, deviceid,reg_adr and data
*  
* @return      0(SUCCESS) or 1(ERROR)
*
* @author     SK 
*
* @date       18/03/2022
*
* @note
********************************************************************************/
int8_t mcal_i2c_read_multi (mcal_i2c_num_t i2c_num, uint8_t deviceid, uint8_t reg_addr, uint8_t *data, uint32_t datalen)
{
    uint8_t deviceid_RW;
    uint8_t temp_data[datalen];
    uint16_t final_data=0; 
    int8_t status=I2C_SUCCESS;
    i2c_cmd_handle_t read_cmd;

    deviceid_RW = (deviceid<<1)| (WRITE_OPERATION);     // device id last bit is padded with the write operation
  
    read_cmd =  i2c_cmd_link_create();                 // i2c command link is created to start the i2c operation
      if(read_cmd == NULL)                             // checks for link creation 
      {
          return I2C_ERROR;
      }
      else
      {
        status |= i2c_master_start(read_cmd);                 // enables the start bit of i2c operation
        status |= i2c_master_write_byte(read_cmd,deviceid_RW,1);     // writes device id with the requested operation
        status |=i2c_master_write_byte(read_cmd,reg_addr,1);          // writes the requested register address
        status |=i2c_master_start(read_cmd);                         // again enables the start bit of i2c operation
        deviceid_RW =  (deviceid<<1)|READ_OPERATION;       // device id last bit is padded with the read operation
        status |=i2c_master_write_byte(read_cmd,deviceid_RW,1);       // writes device id with the requested operation
        status |=i2c_master_read(read_cmd,data,datalen,I2C_MASTER_LAST_NACK);             // reads the data stored in the requested register

        #if 0
        while(datalen > 1)
        {
            status |=i2c_master_read(read_cmd,(uint8_t *)data,1,I2C_MASTER_ACK);             // reads the data stored in the requested register
            datalen--;
        }
            status |=i2c_master_read(read_cmd,(uint8_t *)data,1,I2C_MASTER_NACK);             // reads the data stored in the requested register
        #endif
        
        status |=i2c_master_stop(read_cmd);                      // enables the stop bit of i2c operation
        status |=i2c_master_cmd_begin(0,read_cmd,0xFFFF);        // initiate the i2c bus operation based on above configuration
        i2c_cmd_link_delete(read_cmd);                          // i2c created command link is deleted to release the resource

        #if 0
        final_data = temp_data[1];
        final_data = (final_data<<8)|temp_data[0];
        *data= final_data;
        #endif

        

      }  
    return status;  
}

/*****************************************************************************/
/**
* \brief i2c write function
*
* \description
*   This function performs the i2c write operation, the fuction performs the 
    i2c bus sequence as mentioned in the esp32 IDF official website.
*
* @param      i2c_num, deviceid,reg_adr and data
*  
* @return     0(SUCCESS) or 1(ERROR)
*
* @author     SK 
*
* @date       18/03/2022
*
* @note
********************************************************************************/
int8_t mcal_i2c_write_multi (mcal_i2c_num_t i2c_num, uint8_t deviceid, uint8_t reg_addr, uint8_t *data, uint8_t datalen)
{
    uint8_t deviceid_RW,status=I2C_SUCCESS;
    deviceid_RW = (deviceid<<1)| (WRITE_OPERATION);       // device id last bit is padded with the write operation
    i2c_cmd_handle_t write_cmd;
    write_cmd =  i2c_cmd_link_create();                    // i2c command link is created to start the i2c operation
    if(write_cmd == NULL)                                  // checks for link creation 
    {
        return I2C_ERROR;
    }
    else
    {
    status |=i2c_master_start(write_cmd);                   // enables the start bit of i2c operation
    status |=i2c_master_write_byte(write_cmd,deviceid_RW,1);    // writes device id with the requested operation
    status |=i2c_master_write_byte(write_cmd,reg_addr,1);       // writes the requested register address
    status |=i2c_master_write(write_cmd,data,datalen,I2C_MASTER_LAST_NACK);               // writes required data to requested register
    status |=i2c_master_stop(write_cmd);                           // enables the stop bit of i2c operation
    status |=i2c_master_cmd_begin(0,write_cmd,0xFFFF);              // initiate the i2c bus operation based on above configuration
    i2c_cmd_link_delete(write_cmd);
    }
    return status;

}