#ifndef _PPM_H_
#define _PPM_H_
#include "stm32f10x.h"

/*        __             __
*   s_p  |  |d_p        |  | 
*    ____|  |___________|  |__
*/



void LedInit(void);
void Timer2Init(void);
void Timer2_IRQ(void);
    
void Timer4Init(void);
void Timer4_IRQ(void); 

void ppm_output(void);

#endif // _PPM_H_

