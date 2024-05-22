#ifndef __BMP280_H__
#define __BMP280_H__

#include "stm32f1xx_hal.h"
#include "i2c.h"

#define BMP280_ADDR				0x76
#define REG_SIZE 				8
#define BMP280_TEMP_XLSB_REG 	0xFC
#define BMP280_TEMP_LSB_REG 	0xFB
#define BMP280_TEMP_MSB_REG		0xFA
#define BMP280_PRESS_XLSB_REG 	0xF9
#define BMP280_PRESS_LSB_REG 	0xF8
#define BMP280_PRESS_MSB_REG 	0xF7
#define BMP280_CONFIG_REG 		0xF5
#define BMP280_CONFIG_CMD		0x10    //t_sb[2:0] filter[2:0] spi3w_en[0]
#define BMP280_CTRL_MEAS_REG 	0xF4
#define BMP280_CTRL_MEAS_CMD   	0x55    //osrs_t[2:0] osrs_p[2:0] mode[1:0]
#define BMP280_STATUS_REG 		0xF3
#define BMP280_RESET_REG 		0xE0
#define BMP280_RESET_CMD 		0xB6
#define BMP280_ID_REG 			0xD0
#define BMP280_REG_ADDR_LSB_T1 	0x88
#define BMP280_REG_ADDR_MSB_T1 	0x89
#define BMP280_REG_ADDR_LSB_T2 	0x8A
#define BMP280_REG_ADDR_MSB_T2 	0x8B
#define BMP280_REG_ADDR_LSB_T3 	0x8C
#define BMP280_REG_ADDR_MSB_T3 	0x8D
#define BMP280_REG_ADDR_LSB_P1 	0x8E
#define BMP280_REG_ADDR_MSB_P1 	0x8F
#define BMP280_REG_ADDR_LSB_P2 	0x90
#define BMP280_REG_ADDR_MSB_P2 	0x91
#define BMP280_REG_ADDR_LSB_P3 	0x92
#define BMP280_REG_ADDR_MSB_P3 	0x93
#define BMP280_REG_ADDR_LSB_P4 	0x94
#define BMP280_REG_ADDR_MSB_P4 	0x95
#define BMP280_REG_ADDR_LSB_P5 	0x96
#define BMP280_REG_ADDR_MSB_P5 	0x97
#define BMP280_REG_ADDR_LSB_P6 	0x98
#define BMP280_REG_ADDR_MSB_P6 	0x99
#define BMP280_REG_ADDR_LSB_P7 	0x9A
#define BMP280_REG_ADDR_MSB_P7 	0x9B
#define BMP280_REG_ADDR_LSB_P8 	0x9C
#define BMP280_REG_ADDR_MSB_P8	0x9D
#define BMP280_REG_ADDR_LSB_P9 	0x9E
#define BMP280_REG_ADDR_MSB_P9 	0x9F


void BMP280_Restart(void);
void BMP280_ReadParameter(void);
void BMP280_Init(void);
void BMP280_Measure_Cmd(void);
void BMP280_ReadData(void);
float BMP280_Calculate_Temp(void);
float BMP280_Calculate_Press(void);

#endif