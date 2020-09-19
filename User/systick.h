#ifndef _SYSTICK_H_
#define _SYSTICK_H_

#define SYSTICK_VAL() ((uint32_t)SysTick->VAL)

void systick_init(unsigned int);
void Systick_IRQ(void);

#endif //_SYSTICK_H_
