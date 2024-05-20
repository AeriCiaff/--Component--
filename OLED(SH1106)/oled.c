#include "oled.h"
#include "oledfont.h"
#include "i2c.h"	   


static void OLED_Write_Cmd(uint8_t cmd)
{
	uint8_t buf[2];
	buf[0] = 0x00;
	buf[1] = cmd;
	

	HAL_I2C_Master_Transmit(&hi2c1, 0x78, buf, 2, 0xFFFF);
}

static void OLED_Write_Dat(uint8_t dat)
{
	uint8_t buf[2];
	buf[0] = 0x40;
	buf[1] = dat;
	

	HAL_I2C_Master_Transmit(&hi2c1, 0x78, buf, 2, 0xFFFF);
}

void OLED_Clear(void)
{  
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_Write_Cmd(0xb0+i);
		OLED_Write_Cmd(0x02);
		OLED_Write_Cmd(0x10);
		for(n=0;n<128;n++)
		{
			OLED_Write_Dat(0);
		}			
	}
}

void OLED_Init(void)
{
	HAL_Delay(100);

	OLED_Write_Cmd(0xAE); // 关闭显示
	OLED_Write_Cmd(0x02); // 设置低列地址
	OLED_Write_Cmd(0x10); // 设置高列地址
	OLED_Write_Cmd(0x40); // 设置起始行地址
	OLED_Write_Cmd(0x81); // 对比度控制
	OLED_Write_Cmd(0xFF); // 对比度设置为128
	OLED_Write_Cmd(0xA1); // 设置段重映射
	OLED_Write_Cmd(0xC8); // 设置COM扫描方向
	OLED_Write_Cmd(0xA6); // 设置正常/反向显示
	OLED_Write_Cmd(0xA8); // 设置多路复用比（1到64）
	OLED_Write_Cmd(0x3F); // 设置1/32占空比
	OLED_Write_Cmd(0xD3); // 设置显示偏移
	OLED_Write_Cmd(0x00); // 显示偏移值为0
	OLED_Write_Cmd(0xD5); // 设置振荡器分频
	OLED_Write_Cmd(0x80); // 默认设置
	OLED_Write_Cmd(0xD9); // 设置预充电周期
	OLED_Write_Cmd(0xF1); // 预充电周期值
	OLED_Write_Cmd(0xDA); // 设置COM引脚配置
	OLED_Write_Cmd(0x12); // COM引脚配置值
	OLED_Write_Cmd(0xDB); // 设置Vcomh
	OLED_Write_Cmd(0x40); // Vcomh值
	OLED_Write_Cmd(0x20); // 设置内存地址模式
	OLED_Write_Cmd(0x02); // 页面地址模式
	OLED_Write_Cmd(0x8D); // 设置充电泵启用
	OLED_Write_Cmd(0x14); // 充电泵设置为开启
	OLED_Write_Cmd(0xA4); // 整个显示开启
	OLED_Write_Cmd(0xA6); // 正常显示模式
	OLED_Write_Cmd(0xAF); // 打开显示

	OLED_Clear();
}

void OLED_Set_Pos(uint8_t x, uint8_t y)
{
	OLED_Write_Cmd(0xb0+y);
	OLED_Write_Cmd(((x&0xf0)>>4)|0x10);
	OLED_Write_Cmd((x&0x0f)|0x01);
}   	  
 
void OLED_Display_On(void)
{
	OLED_Write_Cmd(0X8D);
	OLED_Write_Cmd(0X14);
	OLED_Write_Cmd(0XAF);
}

void OLED_Display_Off(void)
{
	OLED_Write_Cmd(0X8D);
	OLED_Write_Cmd(0X10);
	OLED_Write_Cmd(0XAE);
}		   			 

void OLED_ShowChar_EN(uint8_t x,uint8_t y,uint8_t chr,uint8_t size)
{   
	uint8_t c=0,i=0;
	
	c = chr-' ';	
	if(x > 128-1)
	{
		x=0;
		y++;
	}
	
	if(size ==16)
	{
		OLED_Set_Pos(x,y);	
		for(i=0;i<8;i++)
		{
			OLED_Write_Dat(F8X16[c*16+i]);
		}
		OLED_Set_Pos(x,y+1);
		for(i=0;i<8;i++)
		{
			OLED_Write_Dat(F8X16[c*16+i+8]);
		}
	}
	else
	{	
		OLED_Set_Pos(x,y);
		for(i=0;i<6;i++)
		{
			OLED_Write_Dat(F6x8[c][i]);
		}
	}
}


static uint32_t oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}	



void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar_EN(x+(size/2)*t,y,' ',size);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar_EN(x+(size/2)*t,y,temp+'0',size); 
	}
} 


void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size)
{
	uint8_t j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar_EN(x,y,chr[j],size);
			x+=8;
			if(x>120){x=0;y+=2;}
			j++;
	}
}


void OLED_ShowChar_CN(uint8_t x,uint8_t y,uint8_t no)
{      			    
	uint8_t t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_Write_Dat(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_Write_Dat(Hzk[2*no+1][t]);
				adder+=1;
      }					
}


void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{ 	
 uint16_t j=0;
 uint8_t x,y;
  
  if(y1%8==0)
	{
		y=y1/8;
	}		
  else
	{
		y=y1/8+1;
	}
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	  {      
	    	OLED_Write_Dat(BMP[j++]);	    	
	  }
	}
} 