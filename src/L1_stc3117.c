/****************************************************************************/
/** 
* SenseGuard gmbh
* @file L1_stc3117.c
* @{
* @details
* 
* This file contains the \e function for the <b> STC3117  </b>().
* 
* <b> Major Tasks </b> of this file includes
*1. Initialization function for STC3117 basic configuration and voltage mode gas guage configuration 
*2. Other supportive functions which are used by the initialization funtions
* 
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date        Changes
* ----- ------ --------    ---------------------------------------------------------------------
* 1.0   sk                 First version
*
* </pre>
*****************************************************************************/
/** @} */


/****************************Includes***********************************************************/
#include<stdio.h>
#include"L1_stc3117.h"
#include<stdint.h>
#include"mcal_i2c.h"
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
/**********************************************************************************************/

/**************************************Preprocessor********************************************/
#define VOLTAGE_FACRTOR 9011
#define INTERNAL_RESISTANCE_BATTERY 90
#define BATTERY_CAPACITY 820
/**********************************************************************************************/

/************************************global variables******************************************/
component_stc3117 stc3117 = {0};
static const char *TAG = "STC3117_EVENT";
uint16_t OCV_table[16] = {0x1770,0x1926,0x19B2,0x19FB,0x1A3E,0x1A6D,0x1A9D,0x1AB6,0x1AD5,0x1B01,0x1B70,0x1BB1,0x1BE8,0x1C58,0x1CF3,0x1DA9};
uint8_t SOC_table[16 ] = {0x00,0x06,0x0C,0x14,0x1E,0x28,0x32,0x3C,0x50,0x64,0x78,0x82,0x8C,0xA0,0xB4,0xC8};
/**********************************************************************************************/


/*****************************************************************************/
/**
* \brief Read STC3117 device ID
*
* \description
*   This function reads the device ID of the chip, this confirm the 
    chip is live and the communication between the master and the slave 
    is proper.
*
* @param      None
*  
* @return     Id matches or not (Either 0(success) or 1(FAIL))
*
* @author     SK 
*
* @date       20/06/2022
*
* @note
********************************************************************************/

uint8_t L1_stc3117_ReadDeviceId(void)
{
    uint8_t status,id[1];

    status = mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_READ,STC3117_REG_ID,id);   // reads device ID
    if(status == I2C_SUCCESS)
    {
        if(id[0]==STC3117_DEVICE_ID)                                      // Match the read device ID with the expected value
        {
            return STC3117_SUCCESS;
        }
        else
        {
            return STC3117_FAIL;
        }
    }
    else
    {
        return 0;
    }

}

/*****************************************************************************/
/**
* \brief clear power on reset.
*
* \description
*   when STC3117 is powered up for the first time the reset bit in control register is set.
*   This function clears the power on reset bit from the control register.
*
* @param      None
*  
* @return     POR bit is cleared or not (either 0 (success) or 1 (fail))
*
* @author     SK 
*
* @date       20/06/2022
*
* @note
********************************************************************************/

uint8_t L1_stc3117_clear_POR(void)
{
    uint16_t wait_counter= 0;

    mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_READ,STC3117_REG_CTRL,&stc3117.stc3117_register_struct.ctrl_reg );    // reads control register

    if(stc3117.stc3117_register_struct.ctrl_reg_bits.pordet == 1)    // checks the power on reset bit
    {
        stc3117.stc3117_register_struct.ctrl_reg_bits.pordet = 0;
        mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_WRITE,STC3117_REG_CTRL,&stc3117.stc3117_register_struct.ctrl_reg ); // clear the POR bit
        while(wait_counter<10)                   // 10 count delay
        {
            wait_counter++;
        }
        mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_READ,STC3117_REG_CTRL,&stc3117.stc3117_register_struct.ctrl_reg ); // verify whether POR bit is cleared
        if(stc3117.stc3117_register_struct.ctrl_reg_bits.pordet == 0)
        {
            return STC3117_SUCCESS;
        }
        else
        {
            return STC3117_FAIL;
        }
        
    }
    else
    {
        return STC3117_FAIL;
    }


}

/*****************************************************************************/
/**
* \brief software reset for stc3117   
*
* \description
*   This function sets the software reset bit in control register 
*
* @param      None
*  
* @return     Either 0(success) or 1(fail)
*
* @author     SK 
*
* @date       20/06/2022
*
* @note       cross verify the functionality
********************************************************************************/
uint8_t L1_stc3117_soft_reset(void)
{
    uint8_t i2c_status = I2C_ERROR;
    stc3117.stc3117_register_struct.ctrl_reg = 0x10;

    i2c_status = mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_WRITE,STC3117_REG_CTRL,&stc3117.stc3117_register_struct.ctrl_reg); // make sof reset bit low
    if(i2c_status == I2C_SUCCESS)
    {
       while(stc3117.stc3117_register_struct.ctrl_reg_bits.pordet == 1)    // wait till chip gets reset and get ready for operation 
       {
        mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_READ,STC3117_REG_CTRL,&stc3117.stc3117_register_struct.ctrl_reg);
       }
       return I2C_SUCCESS;
    }
    else
    {
        return I2C_ERROR;
    }
}

/*****************************************************************************/
/**
* \brief Enter operation mpde
*
* \description
*   This function set the chip to enter into the operational mode and verify the chip counter is getting incremented (just 5 times check)
*
* @param      None
*  
* @return     increment_value the value should be greater than or equal to 2 
*
* @author     SK 
*
* @date       20/06/2022
*
* @note
********************************************************************************/
uint16_t L1_enter_operation_mode(void)
{
    uint16_t present_counter=0;
    uint16_t previous_counter=0;
    uint16_t increment_value= 0;

     mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_READ,STC3117_REG_MODE,&stc3117.stc3117_register_struct.mode_reg); // read mode register value
     if (stc3117.stc3117_register_struct.mode_reg_bits.gg_run == 0)     // check whether it is operation mode or not
     {
        stc3117.stc3117_register_struct.mode_reg_bits.gg_run = 1;      // bit value to configure for operation mode
         mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_WRITE,STC3117_REG_MODE,&stc3117.stc3117_register_struct.mode_reg);  // write back the updated mode register value

          for(int i=0;i<5;i++)                // check counter value for five times 
          {
            stc3117.stc3117_register_struct.reg_counter =  L1_read_reg_counter();  // read chip counter value
            present_counter = stc3117.stc3117_register_struct.reg_counter;     // present read counter value
            if(present_counter != previous_counter)                           // check present counter value with the previous counter value
            {
                previous_counter = present_counter;                           // present counter value to previous counter value
                increment_value++;
            }
            vTaskDelay(500/portTICK_RATE_MS);                               // 500ms delay because chip updates the counter for every 500 ms 
          }

     }
     return increment_value;
}


/*****************************************************************************/
/**
* \brief disables the chip operation mode
*
* \description
*   This function set the chip disables the operational mode and verify the chip counter is not getting increment (just 5 times check)
*
* @param      None
*  
* @return     increment_value the value should be less than or equal to 1 
*
* @author     SK 
*
* @date       20/06/2022
*
* @note
********************************************************************************/
uint16_t L1_disable_operation_mode(void)
{
    uint16_t present_counter=0;
    uint16_t previous_counter=0;
    uint16_t increment_value= 0;

     mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_READ,STC3117_REG_MODE,&stc3117.stc3117_register_struct.mode_reg);  // reads mode register 
     if (stc3117.stc3117_register_struct.mode_reg_bits.gg_run == 1)    //check whether operation enable bit is set previously
     {
        stc3117.stc3117_register_struct.mode_reg_bits.gg_run = 0;     // set the bit to 0
         mcal_I2C_Master_register_operation(I2C0,STC3117_I2C_ADDRESS,I2C_WRITE,STC3117_REG_MODE,&stc3117.stc3117_register_struct.mode_reg); // update the mode register with latet value

          for(int i=0;i<5;i++)                       // check counter value for five times
          {
            stc3117.stc3117_register_struct.reg_counter =  L1_read_reg_counter();      // reads counter register 
            present_counter = stc3117.stc3117_register_struct.reg_counter;            // present counter value
            if(present_counter != previous_counter)                                   // check whether counter register
            {
                previous_counter = present_counter;                                // present counter value is stored to previous counter value
                increment_value++;
            }
            vTaskDelay(500/portTICK_RATE_MS);                             // 500 ms delay
          }

     }
     return increment_value;
}


/*****************************************************************************/
/**
* \brief read the battery voltage
*
* \description
*   This function reads the battery volatage and apply the required calculation to convert the register raw value into readable voltage value
*
* @param      data
*  
* @return     void
*
* @author     SK 
*
* @date       20/06/2022
*
* @note
********************************************************************************/
void L1_read_voltage(float *data)
{
    float temp_voltage = 0;
    mcal_i2c_read_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_VOLTAGE,&stc3117.stc3117_register_struct.reg_voltage,2);    // cross verify
    stc3117.stc3117_register_struct.reg_voltage &= 0x0FFF;                         // cleared the unused bits
    stc3117.stc3117_register_struct.reg_voltage = (float)(stc3117.stc3117_register_struct.reg_voltage * VOLTAGE_FACRTOR)/4096;  // expression to calculate the voltage
    temp_voltage= (float)(stc3117.stc3117_register_struct.reg_voltage/1000.00);    //convert mV to V 
    *data = temp_voltage;                                              // final battery voltage value in volts

}


/*****************************************************************************/
/**
* \brief read OCV register
*
* \description
*   This function reads the OCV register value
*
* @param      None
*  
* @return     ocvvalue (16 bit)
*
* @author     SK 
*
* @date       22/06/2022
*
* @note
********************************************************************************/
uint16_t L1_read_OCV(void)
{
    uint8_t status =I2C_ERROR;
    uint8_t tempdata[2]={0};
    uint16_t ocvvalue = 0;
    status = mcal_i2c_read_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_OCV,tempdata,2);     // read 2- 8 bit ocv reg
    if(status == I2C_SUCCESS)                   
    {
        ocvvalue= tempdata[1];                                              //LSB                      
        ocvvalue = (ocvvalue << 8) | tempdata[0];                          // build 16-bit ocv value (ocv (LSB and HSB))    
    }
    return ocvvalue;
}


/*****************************************************************************/
/**
* \brief write ocv register
*
* \description
*   This function writes the ocv value to the ocv register.
*
* @param      ocv_value
*  
* @return     None
*
* @author     SK 
*
* @date       22/06/2022
*
* @note
********************************************************************************/
void L1_write_OCV(uint16_t ocv_value)
{
    uint8_t tempdata[2] = {0};

    tempdata[0] = ocv_value & 0x00FF;                   // LSB of the 16 bit ocv value
    tempdata[1] = (ocv_value & 0xFF00) >> 8;           // MSB of the 16 bit ocv value

    mcal_i2c_read_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_OCV,tempdata,2);     // write back the ocv value to the 2- 8 bit ocv register

}

/*****************************************************************************/
/**
* \brief Read counter register
*
* \description
*   This function reads the counter register
*
* @param      None
*  
* @return     countervalue
*
* @author     SK 
*
* @date       22/06/2022
*
* @note
********************************************************************************/
uint16_t L1_read_reg_counter(void)
{
      uint8_t temp_data[2]={0};
      uint8_t status=I2C_ERROR;
      uint16_t countervalue=0;
      
      
        status = mcal_i2c_read_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_COUNTER,temp_data,2);  // reads 2- 8 bit counter register
        if(status == I2C_SUCCESS)
        {
        countervalue= temp_data[1];                                // counter MSB value
        countervalue = (countervalue<<8)|temp_data[0];             // counter LSB value
        }
        
        
        return countervalue;                                      // complete 16 bit register value.
}

/*****************************************************************************/
/**
* \brief Update OCV table
*
* \description
*   This function updates the chip ocv table (total 8 bit 32 register ) 
*
* @param      ocv_table_data
*  
* @return     write_reg_error_count
*
* @author     SK 
*
* @date       20/06/2022
*
* @note
********************************************************************************/
uint16_t L1_update_ocvtable(uint16_t *ocv_table_data)
{
    uint8_t ocv_reg_address = STC3117_REG_OCVTAB;
    uint16_t verifydata[16] = {0};
    uint16_t write_reg_error_count= 0;
    uint8_t tempdata[2]= {0};

    for(int i=0;i<16;i++)                                         // writes the OCV table
    {
        tempdata[0]= ocv_table_data[i] & 0x00FF;                                 
        tempdata[1]= (ocv_table_data[i] & 0xFF00) >> 8;        
        mcal_i2c_write_multi(I2C0,STC3117_I2C_ADDRESS,ocv_reg_address,tempdata,2);    // writes the OCV table
        ocv_reg_address +=2; 
    }
    ocv_reg_address = STC3117_REG_OCVTAB;
    for(int i=0; i<16; i++)
    {
        mcal_i2c_read_multi(I2C0,STC3117_I2C_ADDRESS,ocv_reg_address,tempdata,2);       // reads back the ocv table
        verifydata[i] = tempdata[1];
        verifydata[i] = (verifydata[i]<<8)|tempdata[0];
        ocv_reg_address +=2; 
    }
    for(int i=0;i<16;i++)                                   // verify the write data and read data.
    {
        if(ocv_table_data[i] != verifydata[i])
        {
            write_reg_error_count++;
        }

    }
    return write_reg_error_count;                          // write error count

}


/*****************************************************************************/
/**
* \brief writes the vm_cnf register
*
* \description
*   This function writes the calculated vm_cnf value to the register
*
* @param      vm_cnf
*  
* @return     void
*
* @author     SK 
*
* @date       22/06/2022
*
* @note
********************************************************************************/
void L1_write_reg_vm_cnf (uint16_t vm_cnf)
{
    uint8_t tempdata[2]= {0};
    

    tempdata[0] = vm_cnf & 0x00FF;              // cm_cnf LSB 
    tempdata[1] = (vm_cnf & 0xFF00) >> 8;      // cm_cnf MSB

    mcal_i2c_write_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_VM_CNF,tempdata,2);    // writes vm_cnf
}

/*****************************************************************************/
/**
* \brief writes the SOC table
*
* \description
*   This function writes the estimated SOC table value for LIPO battery
*
* @param      soc_table_data
*  
* @return     write_reg_error_count
*
* @author     SK 
*
* @date       22/06/2022
*
* @note
********************************************************************************/
uint16_t L1_update_soctable(uint8_t *soc_table_data)
{
    uint8_t status = I2C_ERROR;
    uint8_t verifydata[16] = {0};
    uint16_t write_reg_error_count= 0;
    

   status =  mcal_i2c_write_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_SOCTAB,soc_table_data,16);  // writes the soc table
    if(status == I2C_SUCCESS)
    {
       status = mcal_i2c_read_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_SOCTAB,verifydata,16);    // reads SOC table
       if(status == I2C_SUCCESS)
       {
            for(int i=0;i<16;i++)                 // verify the SOC table.
            {
                if(soc_table_data[i] != verifydata[i])
                {
                write_reg_error_count++;
                }   

            }
       }
       else
       {
        ESP_LOGI(TAG,"I2C ERROR \n\r");
        
       }

    }
    else
    {
        ESP_LOGI(TAG,"I2C ERROR \n\r");
    }

    return write_reg_error_count;

}


/*****************************************************************************/
/**
* \brief chip initialization
*
* \description
*   This function does the basic initialization of the chip without enabling gas guage algorithm 
*
* @param      None
*  
* @return     None
*
* @author     SK 
*
* @date       22/06/2022
*
* @note
********************************************************************************/
void chip_initialization_test(void)
{
    uint8_t status=STC3117_FAIL;
    uint16_t incremented_counter_value = 0;
    stc3117.stc3117_register_struct.reg_counter = 0;

    status = L1_stc3117_ReadDeviceId();                       // reads device id
    if(status == STC3117_SUCCESS)
    {
        ESP_LOGI(TAG,"stc3117 device ID success \n\r");
        status = L1_stc3117_clear_POR();                     // cears power on reset
        if(status == STC3117_SUCCESS)
        {
            L1_stc3117_soft_reset();                         // soft reset
            while(stc3117.stc3117_register_struct.reg_counter < 3)              
            {
                stc3117.stc3117_register_struct.reg_counter = L1_read_reg_counter();   // waits until counter value reads more than 3 
                //mcal_i2c_read_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_COUNTER,&stc3117.stc3117_register_struct.reg_counter,2);
            }
            incremented_counter_value = L1_enter_operation_mode();          // chip enters the operational mode
            if(incremented_counter_value > 2)                               // checks whether counter value is greater thann 2
            {
                ESP_LOGI(TAG,"stc3117 device enters the operation mode \n\r");
                ESP_LOGI(TAG,"stc3117 intialization is completed\n\r");

            }
            else
            {
                 ESP_LOGI(TAG,"stc3117 device failed to enter the operation mode \n\r");
            }

        }
        else
        {
            incremented_counter_value = L1_enter_operation_mode();
            if(incremented_counter_value > 2)
            {
                ESP_LOGI(TAG,"stc3117 device enters the operation mode \n\r");
                ESP_LOGI(TAG,"stc3117 intialization is completed\n\r");

            }
            else
            {
                 ESP_LOGI(TAG,"stc3117 device already in operation mode or failed to enter \n\r");
            }

        }
    }
    else
    {
        ESP_LOGI(TAG,"stc3117 device ID failed \n\r");
    }

}


/*****************************************************************************/
/**
* \brief reads the SOC 
*
* \description
*   This function reads the SOC register raw value and coverts it to the readable voltage value
*
* @param      None
*  
* @return     stc3117.stc3117_register_struct.reg_soc
*
* @author     SK 
*
* @date       22/06/2022
*
* @note
********************************************************************************/
uint16_t L1_read_soc(void)
{
    uint8_t tempdata[2] ={0};
    mcal_i2c_read_multi(I2C0,STC3117_I2C_ADDRESS,STC3117_REG_SOC,tempdata,2);      // read the soc 2- 8bit register 

    stc3117.stc3117_register_struct.reg_soc = tempdata[1];                                // SOC MSB
    stc3117.stc3117_register_struct.reg_soc = (stc3117.stc3117_register_struct.reg_soc << 8) | tempdata[0]; //SOC LSB

    stc3117.stc3117_register_struct.reg_soc /= 512;      // final SOC percentage 

    return stc3117.stc3117_register_struct.reg_soc;
}


/*****************************************************************************/
/**
* \brief voltage gasguage initialization
*
* \description
*   This function initializes the chip with gas guage algorithm option
*
* @param      None
*  
* @return     None
*
* @author     SK 
*
* @date       22/06/2022
*
* @note
********************************************************************************/
void L1_voltage_GasGuage_intialization(void)
{
    uint8_t status=STC3117_FAIL;
    uint16_t incremented_counter_value = 0;
    stc3117.stc3117_register_struct.reg_counter = 0;
    uint8_t VM_CNF = 0;


      status = L1_stc3117_ReadDeviceId();               // read device ID
      if(status == STC3117_SUCCESS)
      {
        incremented_counter_value = L1_disable_operation_mode();    // disables the chip from the operational mode
        if(incremented_counter_value < 2)                           
            {
                incremented_counter_value = 0;
                ESP_LOGI(TAG,"stc3117 device exits from the operation mode \n\r");
                stc3117.stc3117_register_struct.reg_ocv = L1_read_OCV();      //read ocv value
                L1_stc3117_clear_POR();                                       // optional
                while(stc3117.stc3117_register_struct.reg_counter < 3)          // check untill the counter value reaches greater than 3 
                {
                    stc3117.stc3117_register_struct.reg_counter = L1_read_reg_counter();      // read counter register value
               
                } 
                status = L1_update_ocvtable(OCV_table);            // update ocv cure value to ocv registers (total 16 values for 16- 16 bit resisters)
                if(status == 0)
                {
                   ESP_LOGI(TAG,"Battery OCV table write success \n\r"); 
                }
                status = L1_update_soctable(SOC_table);          // update SOC values respective to ocv values to the total 16- 8 bit registers
                if(status == 0)
                {
                   ESP_LOGI(TAG,"Battery SOC table write success \n\r"); 
                }

                VM_CNF = (INTERNAL_RESISTANCE_BATTERY * BATTERY_CAPACITY)/ 977.78;      // calculate voltage mode configuration value.
                L1_write_reg_vm_cnf(VM_CNF);                                             // writes the vm_cnf value to the register
                L1_write_OCV(stc3117.stc3117_register_struct.reg_ocv);                   // writes back the SOC value to the register
                
               incremented_counter_value =  L1_enter_operation_mode();                   // enters into the operational mode
               if(incremented_counter_value > 2)                                         // checks the counter reg value which should be greater than 2
                {
                ESP_LOGI(TAG,"stc3117 device enters the operation mode \n\r");
                ESP_LOGI(TAG,"stc3117 intialization is completed\n\r");
                }     
            }

      }

}




