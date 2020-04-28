/* 
 * File:   Interrupts.h
 * Author: KapitanBomba
 *
 * Created on 29 listopada 2019, 14:11
 */

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#define FALLING_EDGE    CCP2CONbits.CCP2M=0b0100
#define RISING_EDGE     CCP2CONbits.CCP2M=0b0101
#define FALL_FLAG       0
#define RISE_FLAG       1
#define CompareSet      CCP1CONbits.CCP1M=0b1000        //Compare mode, set output on match (CCP1IF bit is set)
#define CompareClear    CCP1CONbits.CCP1M=0b1001 

void InterruptConfig(void);
volatile unsigned char ISR_ACK;
volatile unsigned int PWM_Freq, Timer1, g_button_timer, g_generic_timer;
unsigned char g_display_text[4];
unsigned char g_decimal_point;

#endif	/* INTERRUPTS_H */

