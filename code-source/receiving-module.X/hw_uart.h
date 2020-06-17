/* 
 * 
 * 
 *
 * 
 */

#ifndef HW_UART_H
#define	HW_UART_H

void UART_Init(void);
void SendUART(char data);
void SendDigitUART(unsigned int data);
void SendArrayUART(unsigned char *data, unsigned char size);

#endif	/* HW_URAT_H */