#include <xc.h>
#include "main.h"
#include "interrupts.h"
#include "hw_uart.h"
#include "menu.h"

#define KEY_SHORT_PRESS_TIME  78
#define KEY_LONG_PRESS_TIME   7800

void Key_Press(KeyStruct *button)
{
	if(button->klock==0 && (*button->port & 1<<button->pin)==0)		
	{
        button->klock=1;
        button->lock_long_press=1;
        g_button_timer=KEY_LONG_PRESS_TIME;

        if(button->button_short_function)
        {
            button->button_short_function();
        }

	}else if((*button->port & 1<<button->pin)!=0 && button->klock==1)    
	{
        if(g_button_timer<(KEY_LONG_PRESS_TIME-KEY_SHORT_PRESS_TIME))
        {
            button->klock=0;
            button->lock_long_press=0;
            SendUART('1');
        }
        
    }else if(button->lock_long_press==1 && g_button_timer==0)
    { 
        if(button->button_long_function)
        {
            button->button_long_function();
        }
        
        button->lock_long_press=0;
    }
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
    set_rtc.button_short_function=Set_RTC_Button_Short;
    set_rtc.button_long_function=0;
    set_rtc.klock=0;
    set_rtc.lock_long_press=0;
    
    set_time1.pin=SW_TIME1_PIN;
    set_time1.port=&SW_TIME1_PORT;
    set_time1.button_short_function=Set_Time1_Button_Short;
    set_time1.button_long_function=0;
    set_time1.klock=0;
    set_time1.lock_long_press=0;
    
    set_time2.pin=SW_TIME2_PIN;
    set_time2.port=&SW_TIME2_PORT;
    set_time2.button_short_function=Set_Time2_Button_Short;
    set_time2.button_long_function=0;
    set_time2.klock=0;
    set_time2.lock_long_press=0;
    
    set_right.pin=SW_RIGHT_PIN;
    set_right.port=&SW_RIGHT_PORT;
    set_right.button_short_function=Right_Button_Short;
    set_right.button_long_function=0;
    set_right.klock=0;
    set_right.lock_long_press=0;
    
    set_up.pin=SW_UP_PIN;
    set_up.port=&SW_UP_PORT;
    set_up.button_short_function=Up_Button_Short;
    set_up.button_long_function=0;
    set_up.klock=0;
    set_up.lock_long_press=0;
    
    set_down.pin=SW_DOWN_PIN;
    set_down.port=&SW_DOWN_PORT;
    set_down.button_short_function=Down_Button_Short;
    set_down.button_long_function=0;
    set_down.klock=0;
    set_down.lock_long_press=0;
  
}

void Button_Handler(KeyPointerStruct *keydef)
{
    Key_Press(keydef->set_rtc);
    Key_Press(keydef->set_down);
    Key_Press(keydef->set_right);
    Key_Press(keydef->set_time1);
    Key_Press(keydef->set_time2);
    Key_Press(keydef->set_up);
}