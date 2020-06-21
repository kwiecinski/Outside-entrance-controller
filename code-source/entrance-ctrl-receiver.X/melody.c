#include <xc.h>
#include "main.h"
#include "interrupts.h"
#include "melody.h"
#include "utils.h"
#include "display-7-segment.h"

#define  c     261
#define  d     293
#define  e     329
#define  f     349
#define  g     392
#define  a     440
#define  b     493
#define  C     523
#define  D     587
#define  E     659
#define  F     698
#define  G     784
#define  p      1

void Generate_PWM(unsigned long int freq)
{
    g_pwm_freq=_XTAL_FREQ/(4*8*2*freq);
}

void Init_CompareMode(void)
{
    CCP2IE=0;                   //CCP1 Interrupt Disable
    T1CONbits.T1CKPS=0b11;      //Prescaller 1:8
    CCP1IE=1;                   //CCP2 Interrupt Enable bit
    COMPARE_SET;                 
}

void Init_CaptureMode(void)
{        
    CCP1IE=0;                   //CCP2 Interrupt Disable
    T1CONbits.T1CKPS=0b00;      //Prescaller 1:1
    CCP2IE=1;                   //CCP1 Interrupt Enable
    FALLING_EDGE;
}

 void Play_Ring(void)
 {
    Init_CompareMode();

    unsigned int const melody[] = {E, C, D, g, p, g, D, E, C};
    unsigned char const beats[] = {4, 4, 4, 2, 64, 4, 4, 4, 2};
    const unsigned int tempo=1800;
    
    unsigned char MELODY_LENGTH = sizeof(melody) / sizeof(melody[0]);
    
    for (unsigned char i=0; i<MELODY_LENGTH; i++) 
    {
        Generate_PWM(melody[i]);
        
        for(unsigned int j=0;j<(tempo/beats[i])/4;j++)
        {
            Display_7Seg(&g_display_text[0],g_decimal_point);
            Wait_ms(4);
        }
    }
    Init_CaptureMode();
 }