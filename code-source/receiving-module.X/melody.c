#include <xc.h>
#include "main.h"
#include "interrupts.h"
#include "melody.h"
#include "utils.h"

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


void Wait_ms(unsigned int time)
{
    Timer1=(125*(unsigned long int)time)/32;
    while(Timer1);
}

void GeneratePWM(unsigned long int freq)
{
    PWM_Freq=_XTAL_FREQ/(4*8*2*freq);
}
//e5, c5,d5,g5 g5, d5 ,e5,c5

void Init_CompareMode(void)
{
    CCP2IE=0;                   //CCP1 Interrupt Disable
    T1CONbits.T1CKPS=0b11;      //Prescaller 1:8
    CCP1IE=1;                   //CCP2 Interrupt Enable bit
    CompareSet;                 
     
}

void Init_CaptureMode(void)
{        
    CCP1IE=0;                   //CCP2 Interrupt Disable
    T1CONbits.T1CKPS=0b00;      //Prescaller 1:1
    CCP2IE=1;                   //CCP1 Interrupt Enable
    FALLING_EDGE;
}

 void PlayRing(unsigned char diode)
 {
//     Diode(diode);
     Init_CompareMode();

    unsigned int const melody[] = {E, C, D, g, p, g, D, E, C};
    unsigned char const beats[] = {4, 4, 4, 2, 64, 4, 4, 4, 2};
    const unsigned int tempo=2000;
    
    unsigned char MELODY_LENGTH = sizeof(melody) / sizeof(melody[0]);
    
    for (unsigned char i=0; i<MELODY_LENGTH; i++) 
    {
        GeneratePWM(melody[i]);
        Wait_ms(tempo/beats[i]);
    }
    
    Init_CaptureMode();
//    Diode(CLEAR_DIODE);
 }