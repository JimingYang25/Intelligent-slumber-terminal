#include "main.h"
#include "Humidity_Sensor.h"
#include "gpio.h"

#define DHT11_TIMEOUT 2000  // 1. 适当增大超时阈值，容错性更高
#define DHT11_READ_DELAY 45 // 2. 调整比特位判断延时（从60us改为45us，适配慢上升沿）

// 高精度微秒延时（优化：增加空指令，提升延时精度）
void DHT11_Delay_us(uint32_t us)
{
    uint32_t ticks = (HAL_RCC_GetHCLKFreq() / 1000000) * us;
    ticks /= 8;  // 3. 72MHz下调整系数为8，让延时更短（适配慢上升沿）
    __IO uint32_t temp = ticks;
    while(temp--)
    {
        __NOP(); // 空指令，减少编译器优化导致的延时不准
        __NOP();
    }
}

// 读取一个字节的数据（核心优化：调整比特位判断时序）
uint8_t DHT11_Read_Byte(void)
{
    uint8_t i, dat = 0;
    uint32_t timeout;

    for(i = 0; i < 8; i++)
    {
        timeout = DHT11_TIMEOUT;
        // 等待总线拉低（每个比特位起始信号）
        while(HAL_GPIO_ReadPin(DATA_PIN_GPIO_Port, DATA_PIN_Pin) == GPIO_PIN_RESET && timeout--)
        {
            DHT11_Delay_us(1);
        }
        if(timeout == 0) return 0;

        // 4. 缩短判断延时，适配内部上拉的慢上升沿
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

// 读取温湿度数据（增加重试机制）
uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humi)
{
    uint8_t buf[5];
    uint8_t i, retry = 3; // 5. 增加3次重试机制

    while(retry--)
    {
        // 复位传感器
        DHT11_IO_OUT();
        HAL_GPIO_WritePin(DATA_PIN_GPIO_Port, DATA_PIN_Pin, GPIO_PIN_RESET);
        HAL_Delay(25); // 6. 延长复位拉低时间到25ms，确保传感器识别
        HAL_GPIO_WritePin(DATA_PIN_GPIO_Port, DATA_PIN_Pin, GPIO_PIN_SET);
        DHT11_Delay_us(40); // 7. 延长等待时间到40us
        DHT11_IO_IN();

        // 等待应答
        DHT11_Delay_us(10); // 8. 增加短暂延时，等电平稳定
        if(HAL_GPIO_ReadPin(DATA_PIN_GPIO_Port, DATA_PIN_Pin) != GPIO_PIN_RESET)
        {
            continue; // 无应答则重试
        }

        // 读取5字节数据
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

// 保留原有的DHT11_IO_OUT、DHT11_IO_IN、DHT11_Init函数（无需修改）
void DHT11_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DATA_PIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH; // 9. 改为高速输出，提升电平切换速度
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

