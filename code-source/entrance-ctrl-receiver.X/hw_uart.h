/* 
 * 
 * 
 *
 * 
 */

#ifndef HW_UART_H
#define	HW_UART_H

void UART_Init(void);
void Send_UART(char data);
void Send_Digit_UART(unsigned int data);
void Send_Array_UART(unsigned char *data, unsigned char size);

#endif	/* HW_URAT_H */