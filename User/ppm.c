#include "stm32f10x.h"
#include <stdio.h>
#include "ppm.h"
#include "systick.h"

enum {
    RC_CH1 = 0,
    RC_CH2,
    RC_CH3,
    RC_CH4,
    RC_CH5,
    RC_CH6,
    RC_CH7,
    RC_CH8, 
    RC_CH_NUM
};

// {36000, 2} ~ 1000us
// {36000, 1} ~ 500us
#define TIM_COUNTER 36000
#define TIM_PSC 2

static uint32_t start[RC_CH_NUM] = {0};
static uint32_t end[RC_CH_NUM] = {0};
uint32_t interval[RC_CH_NUM] = {0};
static uint32_t ch_cnt[RC_CH_NUM] = {0};

void LedInit(void)
{
    // PB0 for LED, PB1 for ouput
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*
*   Timer4:
*      CH5 ~ PB6
*      CH6 ~ PB7
*      CH7 ~ PB8
*      CH8 ~ PB9
*/
void Timer4Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    // TIM3 IC1 PB.6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    // TIM3 IC2 PB.7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TIM3 IC3 PB.8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // TIM3 IC4 PB.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* TIM4 basic Init */
	TIM_TimeBaseStructure.TIM_Period = TIM_COUNTER - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = TIM_PSC - 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    /* TIM3 CH1 */
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
    TIM_ICInitStructure.TIM_ICFilter = 0x0; 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
 
    TIM_ClearFlag(TIM4, TIM_IT_CC1);
    TIM_ITConfig(TIM4,TIM_IT_CC1,ENABLE);
    
    /* TM3 CH2*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
    TIM_ICInitStructure.TIM_ICFilter = 0x0; 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
 
    TIM_ClearFlag(TIM4, TIM_IT_CC2);
    TIM_ITConfig(TIM4,TIM_IT_CC2,ENABLE);

    /* TM3 CH3*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
    TIM_ICInitStructure.TIM_ICFilter = 0x0; 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
 
    TIM_ClearFlag(TIM4, TIM_IT_CC3);
    TIM_ITConfig(TIM4,TIM_IT_CC3,ENABLE);

    /* TM3 CH4*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
    TIM_ICInitStructure.TIM_ICFilter = 0x0; 
    TIM_ICInit(TIM4, &TIM_ICInitStructure);
 
    TIM_ClearFlag(TIM4, TIM_IT_CC4);
    TIM_ITConfig(TIM4,TIM_IT_CC4,ENABLE);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}

/*
*	Timer2:
*    	CH1 ~ PA0
*    	CH2 ~ PA1
*    	CH3 ~ PA2
*    	CH4 ~ PA3
*/
void Timer2Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    // TIM2 IC1 PA.0
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // TIM2 IC2 PA.1
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM2 IC3 PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // TIM2 IC4 PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* TIM2 basic init */
	TIM_TimeBaseStructure.TIM_Period = TIM_COUNTER - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_PSC - 1;		 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* TIM2 CH1 */
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
    TIM_ICInitStructure.TIM_ICFilter = 0x0; 
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
 
    TIM_ClearFlag(TIM2, TIM_IT_CC1);
    TIM_ITConfig(TIM2, TIM_IT_CC1,ENABLE);
    
    /* TM2 CH2*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
    TIM_ICInitStructure.TIM_ICFilter = 0x0; 
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
 
    TIM_ClearFlag(TIM2, TIM_IT_CC2);
    TIM_ITConfig(TIM2, TIM_IT_CC2,ENABLE);

    /* TM2 CH3*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
    TIM_ICInitStructure.TIM_ICFilter = 0x0; 
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
 
    TIM_ClearFlag(TIM2, TIM_IT_CC3);
    TIM_ITConfig(TIM2, TIM_IT_CC3,ENABLE);

    /* TM2 CH4*/
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;    
    TIM_ICInitStructure.TIM_ICFilter = 0x0; 
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
 
    TIM_ClearFlag(TIM2, TIM_IT_CC4);
    TIM_ITConfig(TIM2, TIM_IT_CC4,ENABLE);
	
	TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}


void Timer2_IRQ(void)
{
    static uint32_t cnt = 0; 
    
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        cnt++;
        return;
    }
    // CH1 PA0 PPM_CH1 
    if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {  
        /* Clear TIM2_CH1 Capture compare interrupt pending bit */  
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);  
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == SET) {
            start[RC_CH1] = TIM_GetCapture1(TIM2);
            GPIO_SetBits(GPIOB, GPIO_Pin_0);
            
            TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            
            ch_cnt[RC_CH1] = cnt;
            return;
        } else {
            end[RC_CH1] = TIM_GetCapture1(TIM2);
                   
            interval[RC_CH1] = (cnt - ch_cnt[RC_CH1]) * TIM_COUNTER + end[RC_CH1] - start[RC_CH1];

            // limit the scale to 2sec.
            if (interval[RC_CH1] > 72000)
                interval[RC_CH1] = 72000;
            
            GPIO_ResetBits(GPIOB, GPIO_Pin_0);
            TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising); 
            return;
        }
    }
    
    // CH2 PA1 PPM_CH2
    if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) {  
        /* Clear TIM2_CH2 Capture compare interrupt pending bit */  
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);  
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == SET) {
            start[RC_CH2] = TIM_GetCapture2(TIM2);
            
            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH2] = cnt;
            return;
        } else {
            end[RC_CH2] = TIM_GetCapture2(TIM2);
                   
            interval[RC_CH2] = (cnt - ch_cnt[RC_CH2]) * TIM_COUNTER + end[RC_CH2] - start[RC_CH2];
            // limit the scale within 2 secs.
            if (interval[RC_CH2] > 72000)
                interval[RC_CH2] = 72000;

            TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
            return;
        }
    }  
    
    // CH3 PA2 PPM_CH3 
    if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET) {  
        /* Clear TIM2_CH3 Capture compare interrupt pending bit */  
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);  
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2) == SET) {
            start[RC_CH3] = TIM_GetCapture3(TIM2);
            
            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH3] = cnt;
            return;
        } else {
            end[RC_CH3] = TIM_GetCapture3(TIM2);
                   
            interval[RC_CH3] = (cnt - ch_cnt[RC_CH3]) * TIM_COUNTER + end[RC_CH3] - start[RC_CH3];
            // limit the scale within 2 secs.
            if (interval[RC_CH3] > 72000)
                interval[RC_CH3] = 72000;

            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
            return;
        }
    }  
    
    // CH4 PA3 PPM_CH4 
    if (TIM_GetITStatus(TIM2, TIM_IT_CC4) != RESET) {  
        /* Clear TIM2_CH4 Capture compare interrupt pending bit */  
        TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);  
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3) == SET) {
            start[RC_CH4] = TIM_GetCapture4(TIM2);
            
            TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH4] = cnt;
            return;
        } else {
            end[RC_CH4] = TIM_GetCapture4(TIM2);
                   
            interval[RC_CH4] = (cnt - ch_cnt[RC_CH4]) * TIM_COUNTER + end[RC_CH4] - start[RC_CH4];
            // limit the scale within 2 secs.
            if (interval[RC_CH4] > 72000)
                interval[RC_CH4] = 72000;

            TIM_OC4PolarityConfig(TIM2,TIM_ICPolarity_Rising); 
            return;
        }
    }
}

void Timer4_IRQ(void)
{
    static uint32_t cnt = 0; 
    
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        cnt++;
        return;
    }   
    // CH1 PB6 RC_CH5
    if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET) {  
        /* Clear TIM2_CH5 Capture compare interrupt pending bit */  
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);  
        
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == SET) {
            start[RC_CH5] = TIM_GetCapture1(TIM4);
            
            TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH5] = cnt;
            return;
        } else {
            end[RC_CH5] = TIM_GetCapture1(TIM4);
                   
            interval[RC_CH5] = (cnt - ch_cnt[RC_CH5]) * TIM_COUNTER + end[RC_CH5] - start[RC_CH5];
            // limit the scale within 2 secs.
            if (interval[RC_CH5] > 72000)
                interval[RC_CH5] = 72000;

            TIM_OC1PolarityConfig(TIM4,TIM_ICPolarity_Rising); 
			return;
        }
    }  
    
    // CH2 PB7 RC_CH6
    if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET) {  
        /* Clear TIM2_CH6 Capture compare interrupt pending bit */  
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);  
        
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == SET) {
            start[RC_CH6] = TIM_GetCapture2(TIM4);
            
            TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH6] = cnt;
            return;
        } else {
            end[RC_CH6] = TIM_GetCapture2(TIM4);
                   
            interval[RC_CH6] = (cnt - ch_cnt[RC_CH6]) * TIM_COUNTER + end[RC_CH6] - start[RC_CH6];
            // limit the scale within 2 secs.
            if (interval[RC_CH6] > 72000)
                interval[RC_CH6] = 72000;

            TIM_OC2PolarityConfig(TIM4,TIM_ICPolarity_Rising); 
			return;
        }
    }

    // CH3 PB8 RC_CH7
    if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET) {  
        /* Clear TIM2_CH7 Capture compare interrupt pending bit */  
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC3);  
        
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == SET) {
            start[RC_CH7] = TIM_GetCapture3(TIM4);
            
            TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH7] = cnt;
            return;
        } else {
            end[RC_CH7] = TIM_GetCapture3(TIM4);
                   
            interval[RC_CH7] = (cnt - ch_cnt[RC_CH7]) * TIM_COUNTER + end[RC_CH7] - start[RC_CH7];
            // limit the scale within 2 secs.
            if (interval[RC_CH7] > 72000)
                interval[RC_CH7] = 72000;

            TIM_OC3PolarityConfig(TIM4,TIM_ICPolarity_Rising); 
			return;
        }
    }

    // CH4 PB9 RC_CH8
    if (TIM_GetITStatus(TIM4, TIM_IT_CC4) != RESET) {  
        /* Clear TIM2_CH8 Capture compare interrupt pending bit */  
        TIM_ClearITPendingBit(TIM4, TIM_IT_CC4);  
        
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) == SET) {
            start[RC_CH8] = TIM_GetCapture4(TIM4);
            
            TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);
            ch_cnt[RC_CH8] = cnt;
            return;
        } else {
            end[RC_CH8] = TIM_GetCapture4(TIM4);
                   
            interval[RC_CH8] = (cnt - ch_cnt[RC_CH8]) * TIM_COUNTER + end[RC_CH8] - start[RC_CH8];
            // limit the scale within 2 secs.
            if (interval[RC_CH8] > 72000)
                interval[RC_CH8] = 72000;

            TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising); 
			return;
        }
    }    
}

enum PPM_OUTPUT_CH_STATE {
    CH_DOWN_STATE,
    CH_UP_STATE,
};

static enum PPM_OUTPUT_CH_STATE state = CH_DOWN_STATE;

/* 20ms */
#define MS20 (72000 * 20)
/* 0.5ms */
#define MS05  (36000)

static uint64_t total_value = 0;
static uint8_t ch_idx = 0;

/*
*	- Continuously send 8 PWMs through GPIO_PB1 pin.
*	- each pwm pulse's low level width is 0.5ms
*	  the high level width shouldn't less than
*	  0.5ms, and greater than 1.5ms.
*	- the frequence of PPM output signal is 50Hz,
*	  so the period is 20ms.
*/
void ppm_output(void){
    uint32_t ch_val = 0;
    
    /* for CH1 ~ CH8 and the last one low level interval */
    if (CH_DOWN_STATE == state) {
		/* next systick interrupt after 0.5ms */
        systick_init(MS05);
        total_value += MS05;
        state = CH_UP_STATE;
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);
    } else {
        /* for channedl CH1 ~ CH8 high level interval */
        if (ch_idx < RC_CH_NUM) {
            if (interval[ch_idx] < (MS05))
                ch_val = MS05;
            else {
                ch_val = (interval[ch_idx] << 1) - MS05;
            }
            systick_init(ch_val);
            total_value += ch_val;
            ch_idx++;
        } else {
            /* the last long high level interval */
            systick_init(MS20 - total_value);
            total_value = 0;
            ch_idx = 0;
        }
        state = CH_DOWN_STATE;
        GPIO_SetBits(GPIOB, GPIO_Pin_1);
    }     
}

