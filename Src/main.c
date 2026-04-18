/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "Humidity_Sensor.h"
#include <stdint.h>
#include "BH1750_Sensor.h"
#include <stdlib.h> 
#include "AI_Record.h"

//参考语音命令对应的寄存器单字节数据
#define ACTIVATE 0x81  //启动 -- 开始收集各传感器数据
#define OLED_ON  0x12  //开灯 -- 亮起LED
#define	OLED_OFF 0x13  //关灯 -- 熄灭LED
#define AWAKE 	 0x80  //唤醒词 -- 会激活OLED的表情表现

#define ADC_BUFFER_SIZE  10    
#define ADC_REF_VOLTAGE  3300  
uint16_t adc_dma_buffer[ADC_BUFFER_SIZE] = {0};
uint8_t score1,score2,score3,score4;
uint16_t adc_voltage; // 存储转换后的电压值
/* USER CODE END Includes */


void ADC_Start_Continuous(void)
{
  
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_dma_buffer, ADC_BUFFER_SIZE) != HAL_OK)
  {
    Error_Handler(); 
  }
}

uint16_t ADC_Get_Average_Voltage(void)
{
  uint32_t adc_sum = 0;
  
  for (uint8_t i = 0; i < ADC_BUFFER_SIZE; i++)
  {
    adc_sum += adc_dma_buffer[i];
  }
  

  uint16_t adc_avg = adc_sum / ADC_BUFFER_SIZE;
  

  uint16_t voltage_mv = (adc_avg * ADC_REF_VOLTAGE) / 4095;
  
  return voltage_mv;
}
	
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */


/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint8_t temp,humi;
	
	uint8_t Compute_Score(void)
	{
		uint8_t total;
		score1=25-abs(temp-24)*1.5;
		if (score1<=0){score1=0;};
		
		score2=25-abs(humi-55)*0.4;
		if (score2<=0){score2=0;};
		
		score3=(uint8_t)(BH1750_ReadLux());
		if (score3<10){score3=25;}
		else if(score3>=10 && score3<50){score3=25-(score3-10)*0.2;}
		else{score3=0;};
		
		score4=adc_voltage/10;
		if (score4<40){score4=25;}
		else if(score4>=40 && score4<70){score4=25-(score4-40)*0.5;}
		else{score4=0;};
		
		total=score1+score2+score3+score4;
		return total;
		
	}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	 
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2C2_Init();
  MX_ADC1_Init();
	
  /* USER CODE BEGIN 2 */
	OLED_Init();
	ADC_Start_Continuous(); ）
	//BH1750_Init();
	
	
	if(DHT11_Init() == 0)
  {
    OLED_ShowString(0, 0, "DHT11 OK"); 
  }
  else
  {
    OLED_ShowString(0, 0, "DHT11 ERR");
    HAL_Delay(2000);
  }
	HAL_Delay(1100);
  /* USER CODE END 2 */
	OLED_Clear();
	uint32_t num;
  /* Infinite loop */
	uint8_t dt;
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
		adc_voltage = ADC_Get_Average_Voltage();
		dt=Asr_Result();
		if(dt!=0) //在识别成功之前寄存器数据会默认为0
		{
			OLED_Expression_Speaking(500);
			
			if (dt==ACTIVATE)
			{OLED_Expression_Speaking(500);
				
				while (1){
									
							if (DHT11_ReadData(&temp,&humi)==0)
								{
									OLED_Clear();
									OLED_Expression_Speaking(500);
									OLED_Clear();
									OLED_ShowString(0,0,"temp:");OLED_ShowString(62,0,"C");
									OLED_ShowString(0,1,"humi:");OLED_ShowString(62,1,"%");
									OLED_ShowNum(42,0,temp,2);
									OLED_ShowNum(42,1,humi,2);
									OLED_ShowString(0,2,"soun:");OLED_ShowNum(42,2,adc_voltage,2);OLED_ShowString(62,2,"DB");
									
									OLED_ShowString(0,3,"Brig:");OLED_ShowNum(42,3,(uint8_t)(BH1750_ReadLux()),2);OLED_ShowString(62,3,"Lux");
									
									OLED_ShowString(0,7,"Slum_Score:");OLED_ShowNum(82,7,Compute_Score(),3);
									while(1){};
			
								}
								else
									{
									OLED_Clear();
									OLED_Processing();
									}
    /* USER CODE BEGIN 3 */
									
			
									}
			
			
			
			}
			if(dt==OLED_ON){OLED_Expression_Speaking(500);HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);};
			
			if(dt==OLED_OFF){OLED_Expression_Speaking(500);HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);};
			
			
			if (dt==AWAKE) //以下沿用上方主逻辑代码
			{OLED_Expression_Wait();
			
			  while (1)
  {
    /* USER CODE END WHILE */
		adc_voltage = ADC_Get_Average_Voltage();
		dt=Asr_Result();
		if(dt!=0)
		{
			
			
			if (dt==ACTIVATE)
			{while (1){
			
							if (DHT11_ReadData(&temp,&humi)==0)
								{
									OLED_Clear();
									OLED_Expression_Speaking(500);
									OLED_Clear();
									OLED_ShowString(0,0,"temp:");OLED_ShowString(62,0,"C");
									OLED_ShowString(0,1,"humi:");OLED_ShowString(62,1,"%");
									OLED_ShowNum(42,0,temp,2);
									OLED_ShowNum(42,1,humi,2);
									OLED_ShowString(0,2,"soun:");OLED_ShowNum(42,2,adc_voltage,2);OLED_ShowString(62,2,"DB");
									
									OLED_ShowString(0,3,"Brig:");OLED_ShowNum(42,3,(uint8_t)(BH1750_ReadLux()),2);OLED_ShowString(62,3,"Lux");
									
									OLED_ShowString(0,7,"Slum_Score:");OLED_ShowNum(82,7,Compute_Score(),3);
									while(1){};
			
								}
								else
									{
									OLED_Clear();
									OLED_Processing();
									}
    /* USER CODE BEGIN 3 */
									
			
									}
			
			
			
			}
			if(dt==OLED_ON){OLED_Expression_Speaking(500);HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);};
			
			if(dt==OLED_OFF){OLED_Expression_Speaking(500);HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);};
			
			
			num+=1;
			HAL_Delay(5);
			if (num%45==0){OLED_Expression_Blink();};
			
				
				
			
			
			
			
			
			
			
			
			
		}
				
			}
				
			}
			
				
				
			
			
			
			
			
			
			
			
			
		}
				
			}
			
		
		}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
		OLED_ShowString(32,3,"ERROR");
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
