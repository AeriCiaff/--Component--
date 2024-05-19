#include "usart.h"

void LoRa_SendData(UART_HandleTypeDef *huart, uint8_t* data, uint16_t size)
{
		HAL_UART_Transmit(huart, data, size, 1000);
}


void LORA_ReceiveData(UART_HandleTypeDef *huart, uint8_t* data, uint16_t size)
{
	HAL_UART_Receive(huart, data, size, 1000);
}