/* 
 * 
 * 
 *
 * 
 */

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


enum button_press 
{
    k_set_rtc_short,
    k_set_rtc_long,
    k_set_time1_short,
    k_set_time1_long,
    k_set_time2_short,
    k_set_time2_long,
    k_set_right_short,
    k_set_right_long,
    k_set_up_short,
    k_set_up_long,
    k_set_down_short,
    k_set_down_long,
    k_no_key_press       
};

enum days
{
    monday,
    tuesday,
    wedenesday,
    thursday,
    friday,
    saturday,
    sunday
};

typedef struct 
{
	unsigned char frame[4];
    unsigned char RCV_Frame;
    
}DataStruct;


typedef struct 
{
	signed char seconds,minutes,hours,day,month,year,weekday;	
	
}TimeStruct;

typedef struct
{
    unsigned char klock, pin, lock_long_press;
    volatile unsigned char *port;
    unsigned char button_short_function;
    unsigned char button_long_function;
 
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

typedef struct MenuParamStruct
{
    unsigned char max_limit,max_limit1,letter,min_limit,min_limit1;
    signed char param, param1;
    struct MenuParamStruct *next_menu;
        
}MenuParamStruct;

typedef struct
{
    MenuParamStruct *hours_minutes_ptr;
    MenuParamStruct *day_month_ptr;
    MenuParamStruct *year_ptr;
    MenuParamStruct *time_limit_work_day_1_ptr;
    MenuParamStruct *time_limit_work_day_2_ptr;
    MenuParamStruct *time_limit_free_day_1_ptr;
    MenuParamStruct *time_limit_free_day_2_ptr;
    
}MenuParamPonterStruct;
    


#endif	/* DEFINE_H */
