#include <xc.h>
#include "main.h"
#include "interrupts.h"
#include "hw_uart.h"
#include "menu.h"

#define KEY_SHORT_PRESS_TIME  800
#define KEY_LONG_PRESS_TIME   7800

enum button_press Key_Press(KeyStruct *button)
{
	if(button->klock==0 && (*button->port & 1<<button->pin)==0)		
	{
        button->klock=1;
        button->lock_long_press=1;
        g_button_timer=KEY_LONG_PRESS_TIME;

        return button->button_short_function;

	}else if((*button->port & 1<<button->pin)!=0 && button->klock==1)    
	{
        if(g_button_timer<(KEY_LONG_PRESS_TIME-KEY_SHORT_PRESS_TIME))
        {
            button->klock=0;
            button->lock_long_press=0;
        }
        
    }else if(button->lock_long_press==1 && g_button_timer==0)
    { 
        button->lock_long_press=0;
        return button->button_long_function;
    }
    
    return k_no_key_press;
}   


void Button_Init (KeyPointerStruct *keydef)
{
    static KeyStruct set_rtc;
    static KeyStruct set_time1;
    static KeyStruct set_time2;
    static KeyStruct set_right;
    static KeyStruct set_up;
    static KeyStruct set_down;

    keydef->set_rtc=&set_rtc;
    keydef->set_time1=&set_time1;
    keydef->set_time2=&set_time2;
    keydef->set_right=&set_right;
    keydef->set_up=&set_up;
    keydef->set_down=&set_down;
    
    set_rtc.pin=SW_SET_RTC_PIN;
    set_rtc.port=&SW_SET_RTC_PORT;
    set_rtc.button_short_function=k_set_rtc_short;
    set_rtc.button_long_function=k_set_rtc_long;
    set_rtc.klock=0;
    set_rtc.lock_long_press=0;
    
    set_time1.pin=SW_TIME1_PIN;
    set_time1.port=&SW_TIME1_PORT;
    set_time1.button_short_function=k_set_time1_short;
    set_time1.button_long_function=k_set_time1_long;
    set_time1.klock=0;
    set_time1.lock_long_press=0;
    
    set_time2.pin=SW_TIME2_PIN;
    set_time2.port=&SW_TIME2_PORT;
    set_time2.button_short_function=k_set_time2_short;
    set_time2.button_long_function=k_set_time2_long;
    set_time2.klock=0;
    set_time2.lock_long_press=0;
    
    set_right.pin=SW_RIGHT_PIN;
    set_right.port=&SW_RIGHT_PORT;
    set_right.button_short_function= k_set_right_short;
    set_right.button_long_function=k_set_right_long;
    set_right.klock=0;
    set_right.lock_long_press=0;
    
    set_up.pin=SW_UP_PIN;
    set_up.port=&SW_UP_PORT;
    set_up.button_short_function=k_set_up_short;
    set_up.button_long_function=k_set_up_long;
    set_up.klock=0;
    set_up.lock_long_press=0;
    
    set_down.pin=SW_DOWN_PIN;
    set_down.port=&SW_DOWN_PORT;
    set_down.button_short_function=k_set_down_short;
    set_down.button_long_function=k_set_down_long;
    set_down.klock=0;
    set_down.lock_long_press=0;
}

unsigned char Button_Handler(KeyPointerStruct *keydef)
{   
    unsigned char button;
    
    button=Key_Press(keydef->set_rtc);
    if(button!=k_no_key_press)
    {
        return button;
    }
    button=Key_Press(keydef->set_down);
    if(button!=k_no_key_press)
    {
        return button;
    }
    button=Key_Press(keydef->set_right);
    if(button!=k_no_key_press)
    {
        return button;
    }
    button=Key_Press(keydef->set_time1);
    if(button!=k_no_key_press)
    {
        return button;
    }
    button=Key_Press(keydef->set_time2);
    if(button!=k_no_key_press)
    {
        return button;
    }
    button=Key_Press(keydef->set_up);
    if(button!=k_no_key_press)
    {
        return button;
    }
    
    return k_no_key_press;
}