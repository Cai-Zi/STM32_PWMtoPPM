# STM32_PWMtoPPM
使用STM32F103C8T6最小系统板（蓝色开发板），捕获8路PWM转换为PPM输出，弥补无PPM输出的接收机
# 引脚分配
接收机的PWM输出通道 --> 最小系统板的引脚 <br>
  CH1 --> PA0 <br>
	CH2 --> PA1 <br>
	CH3 --> PA2 <br>
	CH4 --> PA3 <br>
	CH5 --> PB6 <br>
	CH6 --> PB7 <br>
	CH7 --> PB8 <br>
	CH8 --> PB9 <br>
最小系统板PPM输出在PB1脚，LED灯在PC13脚
# SBUS输出
暂未开发，先挖个坑，有空再填
