#include "stm32f1xx_hal.h"
#include "i2c.h"


#define BMP280_ADDR				0x76
#define REG_SIZE 				I2C_MEMADD_SIZE_8BIT
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



//定义一个结构体，存放BMP280数值
struct BMP280Paramter{
	uint16_t T1;
	int16_t T2;
	int16_t T3;
	uint16_t P1;
	int16_t P2;
	int16_t P3;
	int16_t P4;
	int16_t P5;
	int16_t P6;
	int16_t P7;
	int16_t P8;
	int16_t P9;
	int32_t ADC_T;
	int32_t ADC_P;
	int32_t T_FINE;
}BMP280;

//BMP280软重启
void BMP280_Restart(void){
	uint8_t reset_cmd = BMP280_RESET_CMD;
	HAL_I2C_Mem_Write(&hi2c1, (BMP280_ADDR << 1), BMP280_RESET_REG, REG_SIZE, &reset_cmd, 1, HAL_MAX_DELAY);
}

//修正参数读取函数
void BMP280_ReadParameter(void){
	uint8_t buffer[2];
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_T1, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.T1 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_T2, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.T2 = (uint16_t)(buffer[1] << 8 | buffer[0]); 
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_T3, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.T3 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P1, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P1 = (uint16_t)(buffer[1] << 8 | buffer[0]);	
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P2, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P2 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P3, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P3 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P4, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P4 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P5, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P5 = (uint16_t)(buffer[1] << 8 | buffer[0]);	
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P6, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P6 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P7, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P7 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P8, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P8 = (uint16_t)(buffer[1] << 8 | buffer[0]);
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_REG_ADDR_LSB_P9, REG_SIZE, buffer, 2, HAL_MAX_DELAY);
	BMP280.P9 = (uint16_t)(buffer[1] << 8 | buffer[0]);
}


//BMP280初始化
void BMP280_Init(void){
	BMP280_Restart();
	HAL_Delay(200);
	
	uint8_t ctrl_meas_cmd = BMP280_CTRL_MEAS_CMD;
	uint8_t config_cmd = BMP280_CONFIG_CMD;

	HAL_I2C_Mem_Write(&hi2c1, (BMP280_ADDR << 1), BMP280_CTRL_MEAS_REG, REG_SIZE, &ctrl_meas_cmd, 1, HAL_MAX_DELAY);
	HAL_I2C_Mem_Write(&hi2c1, (BMP280_ADDR << 1), BMP280_CONFIG_REG, REG_SIZE, &config_cmd, 1, HAL_MAX_DELAY);
	BMP280_ReadParameter();
}


void BMP280_Measure_Cmd(void){
	uint8_t measure_cmd = BMP280_CTRL_MEAS_CMD;
	HAL_I2C_Mem_Write(&hi2c1, (BMP280_ADDR << 1), BMP280_CTRL_MEAS_REG, REG_SIZE, &measure_cmd, 1, HAL_MAX_DELAY);
}

//获取BMP280气压和温度数据
void BMP280_ReadData(void){
	uint8_t data[6];
	
	HAL_I2C_Mem_Read(&hi2c1, (BMP280_ADDR << 1), BMP280_PRESS_MSB_REG, REG_SIZE, data, 6, HAL_MAX_DELAY);
	BMP280.ADC_P = (uint32_t)((data[0] << 12) | (data[1] << 4) | (data[2] >> 4));
	BMP280.ADC_T = (uint32_t)((data[3] << 12) | (data[4] << 4) | (data[5] >> 4));
}


//计算温度
float BMP280_Calculate_Temp(void){
	int32_t var1, var2, T;
	
	var1 = ((((BMP280.ADC_T >> 3) - ((int32_t)BMP280.T1 << 1))) *((int32_t)BMP280.T2)) >>11;
	var2 = (((((BMP280.ADC_T >> 4) - ((int32_t)BMP280.T1)) *((BMP280.ADC_T >> 4) - ((int32_t)BMP280.T1))) >>12) *((int32_t)BMP280.T3)) >>14;
	BMP280.T_FINE = var1 + var2;
	T = (BMP280.T_FINE * 5 +128) >> 8;
	
	return (float)T/100;
}


//计算气压
float BMP280_Calculate_Press(void){
	BMP280_Calculate_Temp();
  int64_t var1, var2, p;
  var1 = ((int64_t)BMP280.T_FINE) - 128000;
  var2 = var1 * var1 * (int64_t)BMP280.P6;
  var2 = var2 + ((var1 * (int64_t)BMP280.P5) << 17);
  var2 = var2 + (((int64_t)BMP280.P4) << 35);
  var1 = ((var1 * var1 * (int64_t)BMP280.P3) >> 8) +((var1 * (int64_t)BMP280.P2) << 12);
  var1 =(((((int64_t)1) << 47) + var1)) * ((int64_t)BMP280.P1) >> 33;
  if (var1 == 0)
  {
    return 0;
  }
  else
  {
    p = 1048576 - BMP280.ADC_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)BMP280.P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)BMP280.P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)BMP280.P7) << 4);
    return (float)p/256;
  } 
}