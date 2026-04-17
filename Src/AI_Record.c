#include "main.h"
#include "i2c.h"   // HAL I2C 头文件

// 你原来的宏定义不变
#define Asr_Addr        0x34        // 从机地址
#define ASR_RESULT_ADDR 0x64        // 结果寄存器
#define ASR_SPEAK_ADDR  0xXX        // 你原来的播放寄存器（自行补全）

// 全局变量
extern I2C_HandleTypeDef hi2c2;    // 你用的是 I2C2
int Asr_Result(void)
{
    uint8_t read_data;
    HAL_StatusTypeDef status;

    // ==================== HAL 库 1行 = 原厂全部时序 ====================
    // 起始 + 写地址 + 寄存器地址 + 重启 + 读地址 + 读数据 + 停止
    status = HAL_I2C_Mem_Read(
        &hi2c2,                // 你的I2C句柄
        Asr_Addr << 1,         // 7位地址左移1位
        ASR_RESULT_ADDR,       // 寄存器地址
        I2C_MEMADD_SIZE_8BIT,  // 8位寄存器
        &read_data,            // 读取到的数据
        1,                     // 读取1字节
        100                    // 超时100ms
    );

    if (status == HAL_OK)
    {
        HAL_Delay(20);
        return read_data;  // 成功返回数据
    }
    else
    {
        HAL_Delay(20);
        return 0;          // 失败返回0
    }
}
