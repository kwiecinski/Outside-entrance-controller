/* 
 * 
 * 
 *
 * 
 */

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#define FALLING_EDGE    CCP2CONbits.CCP2M=0b0100
#define RISING_EDGE     CCP2CONbits.CCP2M=0b0101
#define FALL_FLAG       0
#define RISE_FLAG       1
#define COMPARE_SET     CCP1CONbits.CCP1M=0b1000        //Compare mode, set output on match (CCP1IF bit is set)
#define COMPARE_CLEAR   CCP1CONbits.CCP1M=0b1001 

void Interrupt_Init(void);

volatile unsigned char g_reciver_ccp2_isr_fire_flag, g_display_controll;
volatile unsigned int g_pwm_freq, g_button_timer, g_generic_timer;

unsigned char g_display_text[4];
unsigned char g_decimal_point;

#endif	/* INTERRUPTS_H */

