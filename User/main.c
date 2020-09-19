/*
≤∂ªÒPWM ‰»Î£∫
*    	CH1 ~ PA0
*    	CH2 ~ PA1
*    	CH3 ~ PA2
*    	CH4 ~ PA3
*		CH5 ~ PB6
*		CH6 ~ PB7
*		CH7 ~ PB8
*		CH8 ~ PB9
 ‰≥ˆPPM£∫PB1
LED£∫PC13
*/
#include "stm32f10x.h"
#include <stdio.h>
#include "SystemClock.h"
#include "Nvic.h"
#include "ppm.h"
#include "systick.h"
#include "uart.h"

extern uint32_t interval[];

void delay(unsigned n)
{
    unsigned i;
    for (; n > 0; n--) {
        for (i = 0xffff; i > 0; --i);
    }
}

int main(void)
{
    RCC_Configuration();
    NVIC_Configuration();
    /* Init UART1 */
    uart_init();    
    /* Init LED */
    LedInit();
    /*
    * Init timer2 and timer4 for input capture
    */
    Timer2Init();
    
    Timer4Init();
    
    /* Init systick with a random number to startup systick timer */
    systick_init(10000);
    
    while(1)
	{
        delay(40);
//        printf("ch value: 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\r\n",\
//                interval[0], interval[1], interval[2], interval[3], \
//                interval[4], interval[5], interval[6], interval[7]);
	}
}
