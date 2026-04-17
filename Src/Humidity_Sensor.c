#include "main.h"
#include "Humidity_Sensor.h"
#include "gpio.h"

//部分厂商的出品模块响应速度可能不同，在这里可修改宏定义
#define DHT11_TIMEOUT 2000  // 
#define DHT11_READ_DELAY 45 // 

void DHT11_Delay_us(uint32_t us)
{
    uint32_t ticks = (HAL_RCC_GetHCLKFreq() / 1000000) * us;
    ticks /= 8;  
    __IO uint32_t temp = ticks;
    while(temp--)
    {
        __NOP(); 
        __NOP();
    }
}


uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat = 0;
    uint32_t timeout;

    for(i = 0; i < 8; i++)
    {
        timeout = DHT11_TIMEOUT;
        
        while(HAL_GPIO_ReadPin(DATA_PIN_GPIO_Port, DATA_PIN_Pin) == GPIO_PIN_RESET && timeout--)
        {
            DHT11_Delay_us(1);
        }
        if(timeout == 0) return 0;

        
        DHT11_Delay_us(DHT11_READ_DELAY);

        if(HAL_GPIO_ReadPin(DATA_PIN_GPIO_Port, DATA_PIN_Pin) == GPIO_PIN_SET)
        {
            dat |= (1 << (7 - i));
        }
        else
        {
            dat &= ~(1 << (7 - i));
        }

        timeout = DHT11_TIMEOUT;
        while(HAL_GPIO_ReadPin(DATA_PIN_GPIO_Port, DATA_PIN_Pin) == GPIO_PIN_SET && timeout--)
        {
            DHT11_Delay_us(1);
        }
        if(timeout == 0) return 0;
    }
    return dat;
}


uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i, retry = 3; 

    while(retry--)
    {
        // 复位传感器
        DHT11_IO_OUT();
        HAL_GPIO_WritePin(DATA_PIN_GPIO_Port, DATA_PIN_Pin, GPIO_PIN_RESET);
        HAL_Delay(25); 
        HAL_GPIO_WritePin(DATA_PIN_GPIO_Port, DATA_PIN_Pin, GPIO_PIN_SET);
        DHT11_Delay_us(40); 
        DHT11_IO_IN();

        // 等待应答
        DHT11_Delay_us(10); 
        if(HAL_GPIO_ReadPin(DATA_PIN_GPIO_Port, DATA_PIN_Pin) != GPIO_PIN_RESET)
        {
            continue; // 无应答则重试
        }

        
        for(i = 0; i < 5; i++)
        {
            buf[i] = DHT11_Read_Byte();
        }

        // 校验数据
        if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
        {
            *humi = buf[0];
            *temp = buf[2];
            return 0; // 读取成功
        }
    }

    return 1; // 所有重试失败
}


void DHT11_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DATA_PIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; 
    HAL_GPIO_Init(DATA_PIN_GPIO_Port, &GPIO_InitStruct);
}

void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DATA_PIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DATA_PIN_GPIO_Port, &GPIO_InitStruct);
}

uint8_t DHT11_Init(void)
{
    uint32_t timeout = DHT11_TIMEOUT;

    DHT11_IO_OUT();
    HAL_GPIO_WritePin(DATA_PIN_GPIO_Port, DATA_PIN_Pin, GPIO_PIN_RESET);
    HAL_Delay(25); // 延长复位时间
    HAL_GPIO_WritePin(DATA_PIN_GPIO_Port, DATA_PIN_Pin, GPIO_PIN_SET);
    DHT11_Delay_us(40); // 延长等待时间
    DHT11_IO_IN();

    while(HAL_GPIO_ReadPin(DATA_PIN_GPIO_Port, DATA_PIN_Pin) == GPIO_PIN_SET && timeout--)
    {
        DHT11_Delay_us(1);
    }
    if(timeout == 0) return 1;

    timeout = DHT11_TIMEOUT;
    while(HAL_GPIO_ReadPin(DATA_PIN_GPIO_Port, DATA_PIN_Pin) == GPIO_PIN_RESET && timeout--)
    {
        DHT11_Delay_us(1);
    }
    if(timeout == 0) return 1;

    return 0;
}

