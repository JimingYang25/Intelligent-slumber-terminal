#ifndef __HUMIDITY_SENSOR_H_
#define __HUMIDITY_SENSOR_H_
#include "main.h"
#include "gpio.h"

void DHT11_IO_OUT(void);
void DHT11_IO_IN(void);
void delay_us(uint32_t us);
uint8_t DHT11_Init(void);
uint8_t DHT11_Read_Byte(void);
uint8_t DHT11_ReadData(uint8_t *temp, uint8_t *humi);















#endif
