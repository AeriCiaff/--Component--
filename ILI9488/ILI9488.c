#include "ILI9488.h"
#include <string.h>
#include <stdio.h>

static ILI9488_DMA_State dma_state = ILI9488_DMA_IDLE;
static ILI9488_DMATxCpltCallback dma_callback = NULL;
static volatile uint8_t dma_complete_flag = 0;
static ILI9488_Rotation lcd_rotation = ILI9488_ROTATION_0;

static void LCD_Color565To666(uint16_t color, uint8_t *rgb){
	rgb[0] = ((color >> 11) & 0x1F) << 3;
	rgb[1] = ((color >> 5) & 0x3F) << 2;
	rgb[2] = (color & 0x1F) << 3;
}

static uint16_t LCD_GetWidth(void){
    return (lcd_rotation == ILI9488_ROTATION_90 || lcd_rotation == ILI9488_ROTATION_270) ? LCD_HEIGHT : LCD_WIDTH;
}

static uint16_t LCD_GetHeight(void){
    return (lcd_rotation == ILI9488_ROTATION_90 || lcd_rotation == ILI9488_ROTATION_270) ? LCD_WIDTH : LCD_HEIGHT;
}

/**
 * @brief 写命令
 */
void LCD_WriteCommand(uint8_t command){
	DC_LOW();
  CS_LOW();
  HAL_SPI_Transmit(&hspi1, &command, 1, HAL_MAX_DELAY);
  CS_HIGH();
}

/**
 * @brief 写数据
 */
void LCD_WriteData(uint8_t data){
	DC_HIGH();
	CS_LOW();
	HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
	CS_HIGH();
}

/**
 * @brief 写数据16位
 */
void LCD_WriteData16(uint16_t data){
	uint8_t rgb[3];
	LCD_Color565To666(data, rgb);
	DC_HIGH();
  CS_LOW();
  HAL_SPI_Transmit(&hspi1, rgb, 3, HAL_MAX_DELAY);
  CS_HIGH();
}

/**
 * @brief 写数据16位缓冲区
 */
void LCD_WriteDataBuffer16(const uint16_t* buffer, uint32_t len){
	if (len == 0) return;

  DC_HIGH();
  CS_LOW();

  for (uint32_t i = 0; i < len; i++) {
		uint8_t rgb[3];
		LCD_Color565To666(buffer[i], rgb);
		HAL_SPI_Transmit(&hspi1, rgb, 3, HAL_MAX_DELAY);
  }

  CS_HIGH();
}

/**
 * @brief SPI DMA传输完成回调函数
 */
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi){
	if (hspi->Instance == SPI1) {
		dma_complete_flag = 1;
		dma_state = ILI9488_DMA_IDLE;

		if (dma_callback != NULL) {
				dma_callback();
		}
  }
}

/**
 * @brief 等待DMA传输完成
 */
void LCD_WaitDMAComplete(void){
	uint32_t start_time = HAL_GetTick();

	while(dma_complete_flag == 0){
		if(HAL_GetTick() - start_time > 2000){
			dma_state = ILI9488_DMA_ERROR;
			break;
		}
		for(volatile int i = 0; i < 100; i++);
	}
}

/**
 * @brief 获取DMA状态
 */
ILI9488_DMA_State ILI9488_GetDMAState(void) {
    return dma_state;
}

/**
 * @brief 设置DMA状态
 */
void ILI9488_SetDMAState(ILI9488_DMA_State state) {
    dma_state = state;
}

/**
 * @brief 设置DMA传输完成回调函数
 */
void ILI9488_SetDMACallback(ILI9488_DMATxCpltCallback callback) {
    dma_callback = callback;
}

/**
 * @brief 设置显示旋转角度
 */
void LCD_SetRotation(ILI9488_Rotation rotation) {
    uint8_t madctl;

    switch (rotation) {
        case ILI9488_ROTATION_0:
            madctl = 0x48;
            break;
        case ILI9488_ROTATION_90:
            madctl = 0x28;
            break;
        case ILI9488_ROTATION_180:
            madctl = 0x88;
            break;
        case ILI9488_ROTATION_270:
            madctl = 0xE8;
            break;
        default:
            madctl = 0x48;
            rotation = ILI9488_ROTATION_0;
            break;
    }

    LCD_WriteCommand(0x36);
    LCD_WriteData(madctl);
    lcd_rotation = rotation;
}

/**
 * @brief 重置ILI9488
 */
void LCD_Reset(void) {
    RST_HIGH();
    HAL_Delay(100);
    RST_LOW();
    HAL_Delay(200);
    RST_HIGH();
    HAL_Delay(200);
}

/**
 * @brief 初始化ILI9488
 */
void LCD_Init(void){
	LCD_Reset();

	LCD_WriteCommand(0x01);
	HAL_Delay(10);

	LCD_WriteCommand(0xE0);
	uint8_t gammaP[] = {0x00, 0x03, 0x09, 0x08, 0x16, 0x0A, 0x3F, 0x78, 0x4C, 0x09, 0x0A, 0x08, 0x16, 0x1A, 0x0F};
	for(uint8_t i = 0; i < 15; i++) LCD_WriteData(gammaP[i]);

	LCD_WriteCommand(0xE1);
	uint8_t gammaN[] = {0x00, 0x16, 0x19, 0x03, 0x0F, 0x05, 0x32, 0x45, 0x46, 0x04, 0x0E, 0x0D, 0x35, 0x37, 0x0F};
	for(uint8_t i = 0; i < 15; i++) LCD_WriteData(gammaN[i]);

	LCD_WriteCommand(0xC0);
	LCD_WriteData(0x17);
	LCD_WriteData(0x15);

	LCD_WriteCommand(0xC1);
	LCD_WriteData(0x41);

	LCD_WriteCommand(0xC5);
	LCD_WriteData(0x00);
	LCD_WriteData(0x12);
	LCD_WriteData(0x80);

	LCD_SetRotation(ILI9488_ROTATION_0);

	LCD_WriteCommand(0x3A);
	LCD_WriteData(0x66);

	LCD_WriteCommand(0xB0);
	LCD_WriteData(0x00);

	LCD_WriteCommand(0xB1);
	LCD_WriteData(0xA0);

	LCD_WriteCommand(0xB4);
	LCD_WriteData(0x02);

	LCD_WriteCommand(0xB6);
	LCD_WriteData(0x02);
	LCD_WriteData(0x02);

	LCD_WriteCommand(0xE9);
	LCD_WriteData(0x00);

	LCD_WriteCommand(0xF7);
	LCD_WriteData(0xA9);
	LCD_WriteData(0x51);
	LCD_WriteData(0x2C);
	LCD_WriteData(0x82);

	LCD_WriteCommand(0x11);
	HAL_Delay(120);

	LCD_WriteCommand(0x29);
	HAL_Delay(20);
	LCD_FillColor(LCD_BLACK);
}

/**
 * @brief 读取ILI9488 ID（通常为0x9488或0x9489）
 */
uint16_t LCD_ReadID(void) {
    uint8_t rx[3] = {0};
    uint8_t cmd = 0x04;
    DC_LOW();
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    DC_HIGH();
    HAL_SPI_Receive(&hspi1, rx, 3, HAL_MAX_DELAY);
    CS_HIGH();
    return (rx[1] << 8) | rx[2];
}

/**
 * @brief 设置显示窗口
 */
void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	LCD_WriteCommand(0x2A);
	LCD_WriteData(x0 >> 8);
	LCD_WriteData(x0 & 0xFF);
	LCD_WriteData(x1 >> 8);
	LCD_WriteData(x1 & 0xFF);
    
	LCD_WriteCommand(0x2B);
	LCD_WriteData(y0 >> 8);
	LCD_WriteData(y0 & 0xFF);
	LCD_WriteData(y1 >> 8);
	LCD_WriteData(y1 & 0xFF);
}

void LCD_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    uint16_t width = LCD_GetWidth();
    uint16_t height = LCD_GetHeight();
    if (x >= width || y >= height) return;
    LCD_SetWindow(x, y, x, y);
    LCD_WriteCommand(0x2C);
    LCD_WriteData16(color);
}

void LCD_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    if (x1 > x2) {
        uint16_t t = x1;
        x1 = x2;
        x2 = t;
    }
    if (y1 > y2) {
        uint16_t t = y1;
        y1 = y2;
        y2 = t;
    }

    uint16_t width = LCD_GetWidth();
    uint16_t height = LCD_GetHeight();

    if (x1 >= width || y1 >= height) return;
    if (x2 >= width) x2 = width - 1;
    if (y2 >= height) y2 = height - 1;

    LCD_SetWindow(x1, y1, x2, y2);
    LCD_WriteCommand(0x2C);

    uint8_t rgb[3];
    LCD_Color565To666(color, rgb);

    enum { PIXELS_PER_CHUNK = 1024 };
    static uint8_t tx_chunk[PIXELS_PER_CHUNK * 3];

    for (uint32_t i = 0; i < PIXELS_PER_CHUNK; i++) {
        uint32_t idx = i * 3;
        tx_chunk[idx] = rgb[0];
        tx_chunk[idx + 1] = rgb[1];
        tx_chunk[idx + 2] = rgb[2];
    }

    DC_HIGH();
    CS_LOW();

    uint32_t remain = (uint32_t)(x2 - x1 + 1) * (uint32_t)(y2 - y1 + 1);
    while (remain > 0) {
        uint32_t pixels = (remain > PIXELS_PER_CHUNK) ? PIXELS_PER_CHUNK : remain;

        dma_complete_flag = 0;
        dma_state = ILI9488_DMA_BUSY;

        if (HAL_SPI_Transmit_DMA(&hspi1, tx_chunk, (uint16_t)(pixels * 3)) != HAL_OK) {
            dma_state = ILI9488_DMA_ERROR;
            break;
        }

        LCD_WaitDMAComplete();
        if (dma_state == ILI9488_DMA_ERROR) {
            break;
        }

        remain -= pixels;
    }

    CS_HIGH();
}

/**
 * @brief 通过坐标来画框
 */
void LCD_DrawImageRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, const uint16_t *colors) {
    if (colors == NULL) return;

    if (x1 > x2) {
        uint16_t t = x1;
        x1 = x2;
        x2 = t;
    }
    if (y1 > y2) {
        uint16_t t = y1;
        y1 = y2;
        y2 = t;
    }

    uint16_t width = LCD_GetWidth();
    uint16_t height = LCD_GetHeight();

    if (x1 >= width || y1 >= height) return;
    if (x2 >= width) x2 = width - 1;
    if (y2 >= height) y2 = height - 1;

    LCD_SetWindow(x1, y1, x2, y2);
    LCD_WriteCommand(0x2C);

    enum { PIXELS_PER_CHUNK = 512 };
    static uint8_t tx_chunk[PIXELS_PER_CHUNK * 3];

    uint32_t remain = (uint32_t)(x2 - x1 + 1) * (uint32_t)(y2 - y1 + 1);
    uint32_t offset = 0;

    DC_HIGH();
    CS_LOW();

    while (remain > 0) {
        uint32_t pixels = (remain > PIXELS_PER_CHUNK) ? PIXELS_PER_CHUNK : remain;

        for (uint32_t i = 0; i < pixels; i++) {
            uint8_t *rgb = &tx_chunk[i * 3];
            LCD_Color565To666(colors[offset + i], rgb);
        }

        dma_complete_flag = 0;
        dma_state = ILI9488_DMA_BUSY;

        if (HAL_SPI_Transmit_DMA(&hspi1, tx_chunk, (uint16_t)(pixels * 3)) != HAL_OK) {
            dma_state = ILI9488_DMA_ERROR;
            break;
        }

        LCD_WaitDMAComplete();
        if (dma_state == ILI9488_DMA_ERROR) {
            break;
        }

        offset += pixels;
        remain -= pixels;
    }

    CS_HIGH();
}

void LCD_FillColor(uint16_t color) {
    uint16_t width = LCD_GetWidth();
    uint16_t height = LCD_GetHeight();
    LCD_SetWindow(0, 0, width - 1, height - 1);
    LCD_WriteCommand(0x2C);

    uint8_t rgb[3];
    LCD_Color565To666(color, rgb);

    enum { PIXELS_PER_CHUNK = 1024 };
    static uint8_t tx_chunk[PIXELS_PER_CHUNK * 3];

    for (uint32_t i = 0; i < PIXELS_PER_CHUNK; i++) {
        uint32_t idx = i * 3;
        tx_chunk[idx] = rgb[0];
        tx_chunk[idx + 1] = rgb[1];
        tx_chunk[idx + 2] = rgb[2];
    }

    DC_HIGH();
    CS_LOW();

    uint32_t remain = (uint32_t)width * height;
    while (remain > 0) {
        uint32_t pixels = (remain > PIXELS_PER_CHUNK) ? PIXELS_PER_CHUNK : remain;

        dma_complete_flag = 0;
        dma_state = ILI9488_DMA_BUSY;

        if (HAL_SPI_Transmit_DMA(&hspi1, tx_chunk, (uint16_t)(pixels * 3)) != HAL_OK) {
            dma_state = ILI9488_DMA_ERROR;
            break;
        }

        LCD_WaitDMAComplete();
        if (dma_state == ILI9488_DMA_ERROR) {
            break;
        }

        remain -= pixels;
    }

    CS_HIGH();
}

/**
 * @brief 初始化DMA传输
 */
void LCD_InitDMA(void) {
    LCD_Init();

    dma_state = ILI9488_DMA_IDLE;
    dma_callback = NULL;
    dma_complete_flag = 0;
    
    LCD_FillColor(LCD_BLACK);
}