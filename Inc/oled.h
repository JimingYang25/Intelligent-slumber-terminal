#ifndef __OLED_H
#define __OLED_H

#include "main.h"
extern I2C_HandleTypeDef hi2c1;//声明I2C的句柄变量，定义在外部文件中，方便多个文件访问i2c接口。


void MX_I2C1_Init(void);


ErrorStatus OLED_CheckDevice(uint8_t slave_addr);
ErrorStatus OLED_WriteByte(uint8_t cmd,uint8_t byte);
ErrorStatus OLED_WriteBuffer(uint8_t cmd,uint8_t* buffer,uint32_t num);
void OLED_SetPos(uint8_t y,uint8_t x);
void OLED_Fill(uint8_t fill_data);
void OLED_Clear(void);
void OLED_FullFill(void);
ErrorStatus OLED_Init(void);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t );
void OLED_ShowString(uint8_t x, uint8_t y, char *str);
void OLED_ShowChinese(uint8_t x, uint8_t y, uint8_t index);
void OLED_ShowNum(uint8_t x, uint8_t y, int32_t num, uint8_t len);
void OLED_ShowFloat(uint8_t x, uint8_t y, float num, uint8_t int_len, uint8_t dec_len);
void OLED_Expression_Wait(void);
void OLED_Expression_Blink(void);
void OLED_Expression_Speaking(int16_t time);
void OLED_Expression_Browse(void);
void OLED_Processing(void);


#endif



