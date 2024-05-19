#ifndef __LORA_H
#define __LORA_H

#include "usart.h"


/*

		*@: 使用LoRa模块发送数据(USART)
		*@: LoRa_SendData(&huart1, "hello", sizeof("hello"))
		*@: &huart1: 使用的串口编号
		*@: "hello": 输入要发送的数据
		*@: sizeof("hello"): 数据的大小

*/
void LoRa_SendData(UART_HandleTypeDef *huart, uint8_t* data, uint16_t size)


/*

		*@: 使用LoRa模块接收数据(USART)
		*@: LoRa_ReceiveData(&huart1, receiveData, sizeof(receiveData))
		*@: &huart1: 使用的串口编号
		*@: receiveData: 接收的数据(数组)
		*@: sizeof(receiveDate): 数据的大小

*/
void LORA_ReceiveData(UART_HandleTypeDef *huart, uint8_t* data, uint16_t size)

#endif
