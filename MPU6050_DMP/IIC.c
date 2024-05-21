#include "stm32f1xx_hal.h"
#include "IIC.h"


#define GPIO_PORT_IIC     GPIOB // 更改需要的引脚编号       
#define RCC_IIC_ENABLE    __HAL_RCC_GPIOB_CLK_ENABLE()
#define IIC_SCL_PIN       GPIO_PIN_8  // 更改需要的引脚编号                  
#define IIC_SDA_PIN       GPIO_PIN_9  // 更改需要的引脚编号          


#if 1	
#define IIC_SCL_1()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SCL_PIN, GPIO_PIN_SET)		/* SCL = 1 */
#define IIC_SCL_0()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SCL_PIN, GPIO_PIN_RESET)		/* SCL = 0 */

#define IIC_SDA_1()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SDA_PIN, GPIO_PIN_SET)		/* SDA = 1 */
#define IIC_SDA_0()  HAL_GPIO_WritePin(GPIO_PORT_IIC, IIC_SDA_PIN, GPIO_PIN_RESET)		/* SDA = 0 */

#define IIC_SDA_READ()  HAL_GPIO_ReadPin(GPIO_PORT_IIC, IIC_SDA_PIN)	
#else	
#define IIC_SCL_1()  GPIO_PORT_IIC->BSRR = IIC_SCL_PIN				/* SCL = 1 */
#define IIC_SCL_0()  GPIO_PORT_IIC->BRR = IIC_SCL_PIN				/* SCL = 0 */

#define IIC_SDA_1()  GPIO_PORT_IIC->BSRR = IIC_SDA_PIN				/* SDA = 1 */
#define IIC_SDA_0()  GPIO_PORT_IIC->BRR = IIC_SDA_PIN				/* SDA = 0 */

#define IIC_SDA_READ()  ((GPIO_PORT_IIC->IDR & IIC_SDA_PIN) != 0)
#endif

void IIC_GPIO_Init(void);

static void IIC_Delay(void)
{
    uint8_t i;
    for (i = 0; i < 10; i++);
}

void IIC_Start(void)
{

    IIC_SDA_1();
    IIC_SCL_1();
    IIC_Delay();
    IIC_SDA_0();
    IIC_Delay();
    IIC_SCL_0();
    IIC_Delay();
}


void IIC_Stop(void)
{
    IIC_SDA_0();
    IIC_SCL_1();
    IIC_Delay();
    IIC_SDA_1();
}

void IIC_Send_Byte(uint8_t _ucByte)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        if (_ucByte & 0x80)
        {
            IIC_SDA_1();
        }
        else
        {
            IIC_SDA_0();
        }
        IIC_Delay();
        IIC_SCL_1();
        IIC_Delay();
        IIC_SCL_0();
        if (i == 7)
        {
            IIC_SDA_1();
        }
        _ucByte <<= 1;
        IIC_Delay();
    }
}

uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i;
    uint8_t value;

    value = 0;
    for (i = 0; i < 8; i++)
    {
        value <<= 1;
        IIC_SCL_1();
        IIC_Delay();
        if (IIC_SDA_READ())
        {
            value++;
        }
        IIC_SCL_0();
        IIC_Delay();
    }
    if(ack==0)
        IIC_NAck();
    else
        IIC_Ack();
    return value;
}


uint8_t IIC_Wait_Ack(void)
{
    uint8_t re;

    IIC_SDA_1();
    IIC_Delay();
    IIC_SCL_1();
    IIC_Delay();
    if (IIC_SDA_READ())
    {
        re = 1;
    }
    else
    {
        re = 0;
    }
    IIC_SCL_0();
    IIC_Delay();
    return re;
}

void IIC_Ack(void)
{
    IIC_SDA_0();
    IIC_Delay();
    IIC_SCL_1();	
    IIC_Delay();
    IIC_SCL_0();
    IIC_Delay();
    IIC_SDA_1();	
}

void IIC_NAck(void)
{
    IIC_SDA_1();	
    IIC_Delay();
    IIC_SCL_1();	
    IIC_Delay();
    IIC_SCL_0();
    IIC_Delay();
}


void IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_IIC_ENABLE;

    GPIO_InitStructure.Pin = IIC_SCL_PIN | IIC_SDA_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
    HAL_GPIO_Init(GPIO_PORT_IIC, &GPIO_InitStructure);

    IIC_Stop();
}

uint8_t IIC_CheckDevice(uint8_t _Address)
{
    uint8_t ucAck;

    IIC_GPIO_Init();

    IIC_Start();

    IIC_Send_Byte(_Address|IIC_WR);
    ucAck = IIC_Wait_Ack();

    IIC_Stop();	

    return ucAck;
}
