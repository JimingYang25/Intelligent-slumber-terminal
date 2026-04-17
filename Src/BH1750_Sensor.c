#include "main.h"
#include "BH1750_Sensor.h"
#include "i2c.h"
#include "oled.h"
#define BH1750_ADDR 0x23<<1
#define BH1750_POWER_ON 0x01//POWER_ON（上电）
#define	BH1750_POWER_DOWN 0x00//POWER_DOWN（休眠）
#define	BH1750_RESET 0x07//RESET（清寄存器）
#define	BH1750_CONT_H 0x10//连续高分辨率（1lx，120ms，推荐）
#define	BH1750_CONT_L			0x13//连续低分辨率（4lx，16ms）
#define	BH1750_SING_H			0x20//单次高分辨率（测完休眠）



void BH1750_Init(void)
{
	uint8_t cmd=BH1750_POWER_ON;
	if (HAL_I2C_Master_Transmit(&hi2c2, BH1750_ADDR, &cmd, 1, 100) != HAL_OK) 
		{
        OLED_ShowString(0,0,"BH_ERROR");
    }
	HAL_Delay(10);
	
	cmd=BH1750_RESET;
	if (HAL_I2C_Master_Transmit(&hi2c2, BH1750_ADDR, &cmd, 1, 100) != HAL_OK) 
		{
        OLED_ShowString(0,0,"BH_ERROR");
    }	
	HAL_Delay(10);
	
	 cmd = BH1750_CONT_H;
    if (HAL_I2C_Master_Transmit(&hi2c2, BH1750_ADDR, &cmd, 1, 100) != HAL_OK) {
        Error_Handler();
    }
    HAL_Delay(130); // 等待第一次测量完成!!部分出品厂商的模块响应速度更快，这里只是推荐的等待时长
}

float BH1750_ReadLux(void)
{
    uint8_t data[2];
    uint16_t raw_value; 
    float lux;

    if (HAL_I2C_Master_Receive(&hi2c2, BH1750_ADDR | 0x01, data, 2, 100) == HAL_OK) 
			{
        raw_value = (data[0] << 8) | data[1];
        lux = (float)raw_value / 1.2; // BH1750 高分辨率模式下的转换系数
        return lux;
			}
		else 
		{
        return -1.0f; // 读取失败返回错误值
    }
}

