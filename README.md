# 智能睡眠终端/Intelligent Slumber Terminal
        
      JimingYang(Independent developer)
产品描述:
这是一个基于stm32f103c8t6型号单片机的项目，用于为用户提供语音操作的睡眠指标收集与服务。
复刻此项目之前你需要自行完成:
a.模块清单的硬件购买

b.keil以及STM32CubeMX的开发环境搭建

c.STLink或其他烧录方案的工具准备和驱动安装

1.模块清单

(1)stm32f103c8t6最小电路板 购买连接：https://item.taobao.com/item.htm?id=559305712053


(2)0.96英寸 OLED 显示器  购买连接:https://ic-item.jd.com/10119772531086.html
注：本项目使用四引脚，I2C通信协议型号


(3)幻尔一体式AI语音交互模块 购买链接：https://item.jd.com/10124291590000.html?pcdk=GiOq1zJEf3-S10KWw8fa2GKPR1blVU4ar0WUwFoXS1Umlhr6PjiTj36rn3HVOnqU.NnCI.sen1

官方使用文件:https://github.com/JimingYang25/Intelligent-slumber-terminal/blob/main/1.1%20%E8%AF%AD%E9%9F%B3%E4%BA%A4%E4%BA%92%E6%A8%A1%E5%9D%97%E7%AE%80%E4%BB%8B.pdf

(4)声音传感器模块 购买链接：https://ic-item.jd.com/10118917477833.html

(5)登仕唐 DHT11温湿度传感器模块 购买链接:https://ic-item.jd.com/10099445838473.html

2.接线说明
                    5V              --STM32--                                           GND      
       
       语音模块(5V)->|             B12      GND----------------------------------------->|
       
       DHT11(1)----->|             B13      GND                 3.3V                     |<-OLED(GND)
       
                     |             B14      R                    |<-OLED(3.3V)           |<-语音模块(GND)
                    
                     |             B15      B11<-语音模块(SCL)   |<--声音传感器(3.3V)     |<-DTH11(4)
                     
                     |             A9       B10<-语音模块(SDA)   |                        |<-语音传感器(GND)
                     
                     |             A10      B1                   |                        |<-OLED(-)
                     
                     |             A11      B0                   |                        |
                     
                     |             A12      A7                   |                        |
                     
                     |             A15      A6                   |                        |
                     
                     |             B3       A5                   |                        |
                     
                     |             B4       A4                   |                        |
                     
                     |             B5       A3 <-DHT11(2)        |                        |
                     
                     |  OLED(SCL)->B6       A2 <-DHT11(3)        |                        |
                     
                     |  OLED(SDA)->B7       A1                   |                        |
                     
                     |             B8       A0<-声音传感器(AO)   |                        |
                     
                     |             B9       C15                  |                        |
                     
                     |------------>5V       C14                  |                        |
                     
                     |             GND      C13<-LED(+)          |                        |
                     
                     |             3.3V     VB                   |                        |
                     
                                     |-------------------------->|                        |




3.接线实物图片:

<img width="560" height="400" alt="微信图片_20260414140655_299_4" src="https://github.com/user-attachments/assets/a7895d3b-1b5f-4550-8378-1d658e95a0b1" />

4.仓库源工程文件的CubeMx配置如下:

<img width="475" height="390" alt="屏幕截图 2026-04-17 162627" src="https://github.com/user-attachments/assets/c0f640b5-1bca-4fa4-b88a-679eb3558935" />



重要参数说明:



1.在HSE和LSE来源均为Crystal/Ceramic Resonator的条件下，时钟树配置为HCLK:72MHZ,APB1主频:36MHZ,APB2主频:72MHZ

<img width="700" height="500" alt="屏幕截图 2026-04-17 163814" src="https://github.com/user-attachments/assets/cc162e20-0879-4acf-9c85-3c56e81e64c3" />



2.I2C：I2C1/I2C2均为标准模式下的100000HZ，两者配置参数完全相同，如下图：

<img width="500" height="381" alt="屏幕截图 2026-04-17 163255" src="https://github.com/user-attachments/assets/8ee82333-24ad-4717-bcef-1740747d309a" />



