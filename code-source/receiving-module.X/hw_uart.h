/* 
 * File:   UART.h
 * Author: KapitanBomba
 *
 * Created on 29 listopada 2019, 15:44
 */


void UART_Init(void);
void SendUART(char data);
void SendDigitUART(unsigned int data);
void SendArrayUART(unsigned char *data, unsigned char size);

