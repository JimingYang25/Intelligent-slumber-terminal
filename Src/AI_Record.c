#include "main.h"
#include "i2c.h"   // HAL I2C 头文件


#define Asr_Addr        0x34        // 从机地址
#define ASR_RESULT_ADDR 0x64        // 结果寄存器



extern I2C_HandleTypeDef hi2c2;    
int Asr_Result(void)
{
    uint8_t read_data;
    HAL_StatusTypeDef status;

    
   
    status = HAL_I2C_Mem_Read(
        &hi2c2,             
        Asr_Addr << 1,         
        ASR_RESULT_ADDR,       // 寄存器地址
        I2C_MEMADD_SIZE_8BIT,  
        &read_data,            // 读取到的数据!!!!!一位
        1,                     
        100                  
    );

    if (status == HAL_OK)
    {
        HAL_Delay(20);
        return read_data;  
    }
    else
    {
        HAL_Delay(20);
        return 0;          
    }
}
