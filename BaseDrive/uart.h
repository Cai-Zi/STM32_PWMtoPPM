#ifndef _UART_H_
#define _UART_H_

#include "stm32f10x.h"

void uart_init(void);
void USART_SendString(USART_TypeDef* USARTx, uint8_t *str);

#endif /* _UART_H_ */

/******************* (C) COPYRIGHT 2012 WangBao *****END OF FILE****/
