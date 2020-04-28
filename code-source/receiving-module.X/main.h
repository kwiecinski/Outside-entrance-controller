#ifndef DEFINE_H
#define	DEFINE_H

#define _XTAL_FREQ 8000000

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

typedef struct 
{
	unsigned char frame[4];
    unsigned char RCV_Frame;
    
}DataStruct;


typedef struct 
{
	unsigned char seconds,minutes,hours,day,month,year;	
	
}TimeStruct;

typedef struct
{
    unsigned char klock, pin, lock_long_press;
    volatile unsigned char *port;
    void (*button_short_function)(void); 
    void (*button_long_function)(void);
 
}KeyStruct;

typedef struct
{
    KeyStruct *set_rtc;
    KeyStruct *set_time1;
    KeyStruct *set_time2;
    KeyStruct *set_right;
    KeyStruct *set_up;
    KeyStruct *set_down;
    
}KeyPointerStruct;

/*
#define SW_TIME1    PORTCbits.RC7
#define SW_TIME2    PORTCbits.RC5
#define SW_SET_RTC  PORTBbits.RB0
#define SW_RIGHT    PORTBbits.RB1
#define SW_DOWN     PORTBbits.RB2
#define SW_UP       PORTBbits.RB3

*/
#define SW_TIME1_PIN       7
#define SW_TIME1_PORT      PORTC

#define SW_TIME2_PIN       5
#define SW_TIME2_PORT      PORTC

#define SW_SET_RTC_PIN     0
#define SW_SET_RTC_PORT    PORTB

#define SW_RIGHT_PIN       1
#define SW_RIGHT_PORT      PORTB

#define SW_DOWN_PIN        2
#define SW_DOWN_PORT       PORTB

#define SW_UP_PIN          3
#define SW_UP_PORT         PORTB


#endif	/* DEFINE_H */
