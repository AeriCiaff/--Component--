#ifndef ILI9488_H
#define ILI9488_H

#include "main.h"
#include "spi.h"
#include "gpio.h"
#include <stdint.h>

#define CS_Pin GPIO_PIN_11
#define CS_GPIO_Port GPIOE
#define DC_Pin GPIO_PIN_12
#define DC_GPIO_Port GPIOE
#define RST_Pin GPIO_PIN_13
#define RST_GPIO_Port GPIOE

#define LCD_WIDTH 320
#define LCD_HEIGHT 480

#define CS_LOW()    HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET)
#define CS_HIGH()   HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET)
#define DC_LOW()    HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_RESET)
#define DC_HIGH()   HAL_GPIO_WritePin(DC_GPIO_Port, DC_Pin, GPIO_PIN_SET)
#define RST_LOW()   HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_RESET)
#define RST_HIGH()  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET)

#define LCD_WHITE       0xFFFF
#define LCD_BLACK      	0x0000	  
#define LCD_BLUE       	0x001F  
#define LCD_BRED        0XF81F
#define LCD_GRED 		0XFFE0
#define LCD_GBLUE		0X07FF
#define LCD_RED         0xF800
#define LCD_MAGENTA     0xF81F
#define LCD_GREEN       0x07E0
#define LCD_CYAN        0x7FFF
#define LCD_YELLOW      0xFFE0
#define LCD_BROWN 		0XBC40
#define LCD_BRRED 		0XFC07
#define LCD_GRAY  		0X8430 

typedef enum {
    ILI9488_DMA_IDLE = 0,
    ILI9488_DMA_BUSY,
    ILI9488_DMA_ERROR
} ILI9488_DMA_State;

typedef enum {
    ILI9488_ROTATION_0   = 0,    // 正常方向
    ILI9488_ROTATION_90  = 1,    // 顺时针90度
    ILI9488_ROTATION_180 = 2,    // 180度
    ILI9488_ROTATION_270 = 3     // 顺时针270度
} ILI9488_Rotation;

typedef void (*ILI9488_DMATxCpltCallback)(void);

void LCD_WriteCommand(uint8_t command);
void LCD_WriteData(uint8_t data);
void LCD_WriteData16(uint16_t data);
void LCD_WriteDataBuffer16(const uint16_t* buffer, uint32_t len);
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void LCD_WaitDMAComplete(void);
ILI9488_DMA_State ILI9488_GetDMAState(void);
void ILI9488_SetDMAState(ILI9488_DMA_State state);
void ILI9488_SetDMACallback(ILI9488_DMATxCpltCallback callback);
void LCD_Reset(void);
void LCD_Init(void);
void LCD_SetRotation(ILI9488_Rotation rotation);
void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawImageRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *colors);
void LCD_FillColor(uint16_t color);
void LCD_InitDMA(void);
uint16_t LCD_ReadID(void);
#endif