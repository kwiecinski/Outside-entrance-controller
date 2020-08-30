#include <xc.h>
#include "interrupts.h"
#include "pcf8583.h"
#include "main.h"
#include "menu.h"
#include "hw_uart.h"
#include "key-handler.h"
#include "flash.h"
#include "display-7-segment.h"

#define MENU_WORK_DAY 0
#define MENU_SET_RTC  1

#define WORK_DAY_MINUTES1_ADD    0x00
#define WORK_DAY_HOURS1_ADD      0x01
#define WORK_DAY_MINUTES2_ADD    0x02
#define WORK_DAY_HOURS2_ADD      0x03

//Function prototypes
void Menu_Set_Param_RTC(MenuParamStruct *menu, unsigned char check_button);
void Menu_Set_Param_Time_Limit(MenuParamStruct *menu, 
                               unsigned char check_button);
void Write_Limit_To_EEprom(MenuParamStruct *limit1, MenuParamStruct *limit2);
void Read_Limit_From_EEprom(MenuParamStruct *limit1, MenuParamStruct *limit2);
void Send_7Seg_Text(char *text, unsigned char decimal_point);

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Menu_Init(MenuParamPonterStruct *menudef)
{
     g_display_controll=0;
     Disable_All_Digits();
    
    static MenuParamStruct hours_minutes;
    static MenuParamStruct time_limit_work_day_1;
    static MenuParamStruct time_limit_work_day_2;
    
    menudef->hours_minutes_ptr=&hours_minutes;
    menudef->time_limit_work_day_1_ptr=&time_limit_work_day_1;
    menudef->time_limit_work_day_2_ptr=&time_limit_work_day_2;
    
    Read_Limit_From_EEprom(menudef->time_limit_work_day_1_ptr,
                           menudef->time_limit_work_day_2_ptr);
    
    hours_minutes.max_limit=23;
    hours_minutes.max_limit1=59;
    hours_minutes.min_limit=0;
    hours_minutes.min_limit1=0;
    hours_minutes.next_menu=0;

    
    time_limit_work_day_1.max_limit=23;
    time_limit_work_day_1.max_limit1=59;
    time_limit_work_day_1.min_limit=0;
    time_limit_work_day_1.min_limit1=0;
    time_limit_work_day_1.next_menu=&time_limit_work_day_2;
    
    time_limit_work_day_2.max_limit=23;
    time_limit_work_day_2.max_limit1=59;
    time_limit_work_day_2.min_limit=0;
    time_limit_work_day_2.min_limit1=0;
    time_limit_work_day_2.next_menu=0;
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Show_Time(TimeStruct *time,  KeyPointerStruct *keydef)
{
    
    PCF8583_Read_Time_Date(time);
    g_display_controll=1; 
    
    g_display_text[0]=time->hours/10;
    g_display_text[1]=time->hours%10;
    g_display_text[2]=time->minutes/10;
    g_display_text[3]=time->minutes%10;

    g_decimal_point=0b0100;  

    while(Button_Handler(keydef)!=k_set_right_short)
    {
    }

    g_display_controll=0;
    Disable_All_Digits();
 
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Menu_Set_Time(MenuParamPonterStruct *menudef, KeyPointerStruct *keydef, 
                   unsigned char menu_type)
{  
    unsigned char check_button, key;
    MenuParamStruct current_menu;
    MenuParamStruct *current_menu_ptr=&current_menu; 
    TimeStruct time;
    
    g_display_controll=1;

    if(menu_type==MENU_WORK_DAY)
    {
        Send_7Seg_Text("set1", 0);
        __delay_ms(1000);
        Read_Limit_From_EEprom(menudef->time_limit_work_day_1_ptr,
                               menudef->time_limit_work_day_2_ptr);
        
        current_menu_ptr=menudef->time_limit_work_day_1_ptr;
        key=k_set_time1_short;
        
    }else if(menu_type==MENU_SET_RTC)
    {
        Send_7Seg_Text("srtc", 0);
        __delay_ms(1000);
        
        PCF8583_Read_Time_Date(&time);
   
        menudef->hours_minutes_ptr->param1=time.minutes;
        menudef->hours_minutes_ptr->param=time.hours;
        
        current_menu_ptr=menudef->hours_minutes_ptr;
        key=k_set_rtc_short;
    }
            
    g_decimal_point=0b0100;
    
    while(1)
    {
        Menu_Set_Param_Time_Limit(current_menu_ptr, check_button);

        //exit from menu - save data 
        check_button=Button_Handler(keydef);
        if(check_button==key)
        {
           if(current_menu_ptr->next_menu==0)
           {
                if(menu_type==MENU_WORK_DAY)
                {
                    Write_Limit_To_EEprom(menudef->time_limit_work_day_1_ptr,
                                          menudef->time_limit_work_day_2_ptr);

                }else if(menu_type==MENU_SET_RTC)
                {
                    time.seconds=0;
                    time.minutes=menudef->hours_minutes_ptr->param1;
                    time.hours=menudef->hours_minutes_ptr->param;

                    PCF8583_Set_Date_Time(&time);
                }
                
                    Send_7Seg_Text("save",0);
                    __delay_ms(2000);
                    Send_7Seg_Text("****",0);

                    return; 
            }
              current_menu_ptr=(current_menu_ptr->next_menu);
        }
        
     
    }  
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Menu_Set_Param_Time_Limit(MenuParamStruct *menu, 
                               unsigned char check_button)
{
    static unsigned char flag=0, change_digit=0;
    
    if(flag==0 && g_generic_timer==0)
    {
        g_generic_timer=2000;
        flag=1;
        
        g_display_text[0]=(menu->param)/10; 
        g_display_text[1]=(menu->param)%10;
        g_display_text[2]=(menu->param1)/10; 
        g_display_text[3]=(menu->param1)%10;

    }else if(g_generic_timer==0)
    {
        g_display_text[change_digit]='*';
        flag=0;
        g_generic_timer=400;
    }
     
     if(check_button==k_set_right_short)
    {
        change_digit++;
        
        if(change_digit>3)
        {
            change_digit=0;
            
            g_display_text[3]=(menu->param1)%10;
            
        }else if(change_digit==2)
        {
            g_display_text[2]=(menu->param1)/10;
            
        }else if(change_digit==1)
        {
            g_display_text[1]=(menu->param)%10;   
        }else if(change_digit==0)
        {
            g_display_text[0]=(menu->param)/10;  
        }
    }

    if(check_button==k_set_up_short)
    {
        if(change_digit==0)
        {
            (menu->param)=(menu->param)+10;

            if(menu->param>menu->max_limit)
            {
                (menu->param)=(menu->param)-((menu->max_limit)-(menu->max_limit)%10+10);
            }
            
            if(menu->max_limit/10==menu->min_limit/10)
            {
                (menu->param)=(menu->param%10)+((menu->max_limit)-(menu->max_limit)%10);
            }
             
        }else if(change_digit==1)
        {
            (menu->param)=(menu->param)+1;

            if((menu->param)>(menu->max_limit))
            {
                (menu->param)=(menu->min_limit);
            }
        }else if(change_digit==2)
        {
            (menu->param1)=(menu->param1)+10;

            if(menu->param1>menu->max_limit1)
            {
                (menu->param1)=(menu->param1)-((menu->max_limit1)-(menu->max_limit1)%10+10);
            }
        }else if(change_digit==3)
        {
            (menu->param1)=(menu->param1)+1;

            if((menu->param1)>(menu->max_limit1))
            {
                (menu->param1)=(menu->min_limit1);
            }
        }
        
        g_display_text[0]=(menu->param)/10; 
        g_display_text[1]=(menu->param)%10;
        g_display_text[2]=(menu->param1)/10; 
        g_display_text[3]=(menu->param1)%10;

        g_generic_timer=7000;
    }

    if(check_button==k_set_down_short)
    {
        if(change_digit==0)
        {
            (menu->param)=(menu->param)-10;

            if((menu->param)<(menu->min_limit))
            {
                (menu->param)=(menu->param)+((menu->max_limit)-(menu->max_limit)%10+10);
            }
            /*
            if(menu->max_limit/10==menu->min_limit/10)
            {
                (menu->param)=(menu->param%10)+((menu->max_limit)-(menu->max_limit)%10);
            }
            */
        }else if(change_digit==1)
        {
            (menu->param)=(menu->param)-1;

            if(menu->param<(menu->min_limit))
            {
                (menu->param)=(menu->max_limit);
            }
        }else if(change_digit==2)
        {
            (menu->param1)=(menu->param1)-10;

            if(menu->param1<(menu->min_limit1))
            {
                (menu->param1)=(menu->param1)+((menu->max_limit1)-(menu->max_limit1)%10+10);
            }
        }else if(change_digit==3)
        {
            (menu->param1)=(menu->param1)-1;

            if(menu->param1<(menu->min_limit1))
            {
                (menu->param1)=(menu->max_limit1);
            }
        }
        
        g_display_text[0]=(menu->param)/10; 
        g_display_text[1]=(menu->param)%10;
        g_display_text[2]=(menu->param1)/10; 
        g_display_text[3]=(menu->param1)%10;

        g_generic_timer=7000;
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Menu_Handler(MenuParamPonterStruct *menudef, KeyPointerStruct *keydef, 
                 TimeStruct *time)
{
    unsigned char check_button;
    
    check_button=Button_Handler(keydef);
    if(check_button==k_set_right_short)
    {
        Show_Time(time,keydef);
        
    }else if(check_button==k_set_rtc_short)
    {
        Menu_Set_Time(menudef,keydef,MENU_SET_RTC);
        
    }else if(check_button==k_set_time1_short)
    {
        Menu_Set_Time(menudef,keydef,MENU_WORK_DAY);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Write_Limit_To_EEprom(MenuParamStruct *limit1, MenuParamStruct *limit2)
{

    Write_EEprom(limit1->param, WORK_DAY_MINUTES1_ADD);
    Write_EEprom(limit1->param1,WORK_DAY_HOURS1_ADD);
    Write_EEprom(limit2->param, WORK_DAY_MINUTES2_ADD);
    Write_EEprom(limit2->param1, WORK_DAY_HOURS2_ADD);
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Read_Limit_From_EEprom(MenuParamStruct *limit1, MenuParamStruct *limit2)
{
    limit1->param=Read_EEprom(WORK_DAY_MINUTES1_ADD);
    limit1->param1=Read_EEprom(WORK_DAY_HOURS1_ADD);
    limit2->param=Read_EEprom(WORK_DAY_MINUTES2_ADD);
    limit2->param1=Read_EEprom(WORK_DAY_HOURS2_ADD);
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Send_7Seg_Text(char *text, unsigned char decimal_point)
{
    unsigned char i;
    
    for(i=0;i<4;i++)
    {
        g_display_text[i]=*(text+i);
    }
    
    g_decimal_point=decimal_point;
}
