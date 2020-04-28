#include <xc.h>
#include "interrupts.h"
#include "pcf8583.h"
#include "main.h"
#include "menu.h"
#include "hw_uart.h"


void Menu(void)
{
    
}



void Display_2Dig(unsigned char data)
{
    g_display_text[2]=data/10;
    g_display_text[3]=data%10;
}


void Set_RTC_Button_Short(void)
{

    
}

void Set_Time1_Button_Short(void)
{
    
}

void Set_Time2_Button_Short(void)
{
    
}

void Down_Button_Short(void)
{
    
}

void Up_Button_Short(void)
{
    
}

void Right_Button_Short(void)
{
    
}
