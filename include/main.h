#ifndef _MAIN_H__
#define _MAIN_H__
#include<stdint.h>
#define _HTS221___

#ifdef _HTS221___
#include "L1_hts221.h"
#define HTS221_DEVICEID      0x5F
#endif

typedef enum 
{
  ERROR =0,
  PASS,

}ERROR_STATUS;

typedef struct
{
  int16_t temperature_data;
  int16_t humidity_data;

}sensors;

typedef struct
{
    sensors sensor_data;
}product;





#endif