#ifndef L1_STC3117_H_
#define L1_STC3117_H_


#define STC3117_SUCCESS   0
#define STC3117_FAIL    1
#define STC3117_DEVICE_ID   0x16
#define STC3117_I2C_ADDRESS  0x70  
#define RAM_SIZE    16


typedef enum
{
    STC3117_REG_MODE = 0x00,
    STC3117_REG_CTRL,
    STC3117_REG_SOC,
    STC3117_REG_COUNTER = 0x04,
    STC3117_REG_CURRENT = 0x06,
    STC3117_REG_VOLTAGE = 0x08,
    STC3117_REG_TEMPERATURE = 0x0A,
    STC3117_REG_OCV    = 0x0D,
    STC3117_REG_CC_CNF = 0x0F,
    STC3117_REG_VM_CNF = 0x11,
    STC3117_REG_ALARM_SOC = 0x13,
    STC3117_REG_ALARM_VOLTAGE,
    STC3117_REG_CURRENT_THRES,
    STC3117_REG_RELAX_COUNT,
    STC3117_REG_RELAX_MAX,
    STC3117_REG_ID,
    STC3117_REG_CC_ADJ_LOW = 0x1B,
    STC3117_REG_CC_ADJ_HIGH,
    STC3117_REG_VM_ADJ_LOW, 
    STC3117_REG_VM_ADJ_HIGH,
    STC3117_REG_RAM  = 0x20, 
    STC3117_REG_OCVTAB = 0x30,
    STC3117_REG_SOCTAB = 0x50,

}StC3117_register_map_addresses_t;


typedef struct stc3117_register_map_t_ 
{	
	union {
		uint8_t ctrl_reg;
		struct {
			uint8_t io0data: 1;
			uint8_t cg_rst: 1;
			uint8_t cg_vm: 1;
            uint8_t batfail: 1;
            uint8_t pordet: 1;
            uint8_t alm_soc: 1;
            uint8_t alm_volt: 1;
            uint8_t uvlod: 1;
		} ctrl_reg_bits;
	};

    union{
         uint8_t mode_reg;
         struct
         {
            uint8_t vmode: 1;
            uint8_t batd_pu: 1;
            uint8_t force_cd: 1;
            uint8_t alm_ena: 1;
            uint8_t gg_run: 1;
            uint8_t force_cc: 1;
            uint8_t force_vm: 1;
            uint8_t unused: 1; 
         }mode_reg_bits;

    };
   
    uint16_t reg_counter;
    uint16_t reg_voltage;
    uint16_t reg_ocv;
    uint16_t reg_soc;
	
} stc3117_register_map_t;


typedef struct
{
    stc3117_register_map_t stc3117_register_struct;
   	
}component_stc3117;

void L1_stc3117_RAMRead(uint8_t *RamData);
void L1_stc3117_RAMWrite(uint8_t *RamData);
uint8_t L1_stc3117_ReadDeviceId(void);
uint8_t L1_stc3117_clear_POR(void);
void chip_initialization_test(void);
uint16_t L1_enter_operation_mode(void);
void L1_read_voltage(float *data);
uint16_t L1_update_ocvtable(uint16_t *data);
uint16_t L1_read_reg_counter(void);
void L1_voltage_GasGuage_intialization(void);
uint16_t L1_read_OCV(void);
void L1_write_reg_vm_cnf (uint16_t vm_cnf);
void L1_write_OCV(uint16_t ocv_value);
uint16_t L1_read_soc(void);


#endif