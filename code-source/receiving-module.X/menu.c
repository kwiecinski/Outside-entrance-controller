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
#define MENU_FREE_DAY 1
#define MENU_SET_RTC  2

#define WORK_DAY_MINUTES1_ADD    0x00
#define WORK_DAY_HOURS1_ADD      0x01
#define WORK_DAY_MINUTES2_ADD    0x02
#define WORK_DAY_HOURS2_ADD      0x03
#define FREE_DAY_MINUTES1_ADD    0x04
#define FREE_DAY_HOURS1_ADD      0x05
#define FREE_DAY_MINUTES2_ADD    0x06
#define FREE_DAY_HOURS2_ADD      0x07

//Function prototypes
void Menu_Set_Param_RTC(MenuParamStruct *menu, unsigned char check_button);
void Menu_Set_Param_Time_Limit(MenuParamStruct *menu, unsigned char check_button);
void Write_Limit_To_EEprom(MenuParamStruct *limit1, MenuParamStruct *limit2, unsigned char limit_type);
void Read_Limit_From_EEprom(MenuParamStruct *limit1, MenuParamStruct *limit2, unsigned char limit_type);
void Display_7Seg_Text(char *text, unsigned char decimal_point);
void Display_Weekday(unsigned char weekday);
//////////////////////


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
    static MenuParamStruct day_month;
    static MenuParamStruct year;
    static MenuParamStruct time_limit_work_day_1;
    static MenuParamStruct time_limit_work_day_2;
    static MenuParamStruct time_limit_free_day_1;
    static MenuParamStruct time_limit_free_day_2;
    
    menudef->hours_minutes_ptr=&hours_minutes;
    menudef->day_month_ptr=&day_month;
    menudef->year_ptr=&year;
    
    menudef->time_limit_free_day_1_ptr=&time_limit_free_day_1;
    menudef->time_limit_free_day_2_ptr=&time_limit_free_day_2;
    menudef->time_limit_work_day_1_ptr=&time_limit_work_day_1;
    menudef->time_limit_work_day_2_ptr=&time_limit_work_day_2;
    
    Read_Limit_From_EEprom(menudef->time_limit_free_day_1_ptr,menudef->time_limit_free_day_2_ptr,MENU_FREE_DAY);
    Read_Limit_From_EEprom(menudef->time_limit_work_day_1_ptr,menudef->time_limit_work_day_2_ptr,MENU_WORK_DAY);
    
    hours_minutes.max_limit=23;
    hours_minutes.max_limit1=59;
    hours_minutes.min_limit=0;
    hours_minutes.min_limit1=0;
    hours_minutes.next_menu=&day_month;
    
    day_month.max_limit=31;
    day_month.max_limit1=12;
    day_month.min_limit=1;
    day_month.min_limit1=1;
    day_month.next_menu=&year;
        
    year.max_limit=21;
    year.max_limit1=99;
    year.min_limit=20;
    year.min_limit1=0;
    year.next_menu=0;
    
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
    
    
    time_limit_free_day_1.max_limit=23;
    time_limit_free_day_1.max_limit1=59;
    time_limit_free_day_1.min_limit=0;
    time_limit_free_day_1.min_limit1=0;
    time_limit_free_day_1.next_menu=&time_limit_free_day_2;
    
    time_limit_free_day_2.max_limit=23;
    time_limit_free_day_2.max_limit1=59;
    time_limit_free_day_2.min_limit=0;
    time_limit_free_day_2.min_limit1=0;
    time_limit_free_day_2.next_menu=0;
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
    unsigned char check_button;
    
    PCF8583_Read_Time_Date(time);
    
    g_display_controll=1; 
    
    g_display_text[0]=time->hours/10;
    g_display_text[1]=time->hours%10;
    g_display_text[2]=time->minutes/10;
    g_display_text[3]=time->minutes%10;

    g_decimal_point=0b0100;  

    while(1)
    {
        check_button=Button_Handler(keydef);
        if(check_button==k_set_right_short)
        {
            g_decimal_point=0b0100; 
            g_display_text[0]=time->day/10;
            g_display_text[1]=time->day%10;
            g_display_text[2]=time->month/10;
            g_display_text[3]=time->month%10;
             

            while(1)
            {
                check_button=Button_Handler(keydef);
                if(check_button==k_set_right_short)
                {
                    g_decimal_point=0b0000;
                    g_display_text[0]=2;
                    g_display_text[1]=time->year/100;
                    g_display_text[2]=time->year%100/10;
                    g_display_text[3]=time->year%10;
                  

                    while(1)
                    {
                       check_button=Button_Handler(keydef);
                       if(check_button==k_set_right_short)
                       {

                           Display_Weekday(time->weekday);
                           
                           while(Button_Handler(keydef)!=k_set_right_short);
                           
                           g_display_controll=0;
                           Disable_All_Digits();
                           
                           return;
                       }
                    }
                }
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///
///  Return true if year is a multiple pf 4 and not multiple of 100. 
///  OR year is multiple of 400. 
/// 
///////////////////////////////////////////////////////////////////////////////
  

unsigned char Is_Leap(unsigned int year) 
{
    return (((year%4==0) &&  (year%100!=0)) || (year%400==0)); 
} 

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

unsigned char Is_Valid_Date(unsigned char d, unsigned char m, unsigned int y) 
{ 
    
    const unsigned int k_max_valid_year = 2999; 
    const unsigned int k_min_valid_year = 2000; 
    
    // If year, month and day  are not in given range 
    if (y>k_max_valid_year || y<k_min_valid_year) 
    {
        return 0; 
    }
    if (m<1 || m>12) 
    {
        return 0; 
    }
    if (d<1 || d>31) 
    {
        return 0; 
    }
  
    // Handle February month  with leap year 
    if (m==2) 
    { 
        if (Is_Leap(y)) 
        {
            return (d<=29);
        }else
        {
            return (d<=28); 
        }
    } 
  
    //Months of April, June,  Sept and Nov must have  number of days less than 
    //or equal to 30. 
    if (m==4 || m==6 || m==9 || m==11)
    {
        return (d<=30); 
    }
  
    return 1; 
} 

///////////////////////////////////////////////////////////////////////////////
///
/// This function will return week day number from 0 to 6
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

unsigned char Find_Weekday(unsigned int year, unsigned int month, unsigned int day) 
{
    unsigned char wday=0;
    wday=(day += month < 3 ? year-- : year - 2, 23*month/9 + day + 4 + 
          year/4- year/100 + year/400)%7-1;
    
    return wday;   
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

    if(menu_type==MENU_FREE_DAY)
    {
         Display_7Seg_Text("set2", 0);
        __delay_ms(1000);
        
        Read_Limit_From_EEprom(menudef->time_limit_free_day_1_ptr,
                               menudef->time_limit_free_day_2_ptr,
                               MENU_FREE_DAY);
        
        current_menu_ptr=menudef->time_limit_free_day_1_ptr;
        key=k_set_time2_short;
        
    }else if(menu_type==MENU_WORK_DAY)
    {
        Display_7Seg_Text("set1", 0);
        __delay_ms(1000);
        Read_Limit_From_EEprom(menudef->time_limit_work_day_1_ptr,
                               menudef->time_limit_work_day_2_ptr,
                               MENU_WORK_DAY);
        
        current_menu_ptr=menudef->time_limit_work_day_1_ptr;
        key=k_set_time1_short;
        
    }else if(menu_type==MENU_SET_RTC)
    {
        Display_7Seg_Text("srtc", 0);
        __delay_ms(1000);
        PCF8583_Read_Time_Date(&time);
   
        menudef->hours_minutes_ptr->param1=time.minutes;
        menudef->hours_minutes_ptr->param=time.hours;
        menudef->day_month_ptr->param=time.day;
        menudef->day_month_ptr->param1=time.month;
        
        menudef->year_ptr->param1=time.year%100;
        menudef->year_ptr->param=20+time.year/100;
        
        current_menu_ptr=menudef->hours_minutes_ptr;
        key=k_set_rtc_short;

    }
            
    g_decimal_point=0b0100;
    
    while(1)
    {
        Menu_Set_Param_Time_Limit(current_menu_ptr, check_button);
        
        check_button=Button_Handler(keydef);
        
        if(check_button==key)
        {
           if(current_menu_ptr->next_menu==0)
           {
                if(menu_type==MENU_FREE_DAY)
                {
                
                    Write_Limit_To_EEprom(menudef->time_limit_free_day_1_ptr,
                                          menudef->time_limit_free_day_2_ptr,
                                          MENU_FREE_DAY);
                    
                    Display_7Seg_Text("save",0);

                }else if(menu_type==MENU_WORK_DAY)
                {
                    Write_Limit_To_EEprom(menudef->time_limit_work_day_1_ptr,
                                          menudef->time_limit_work_day_2_ptr,
                                          MENU_WORK_DAY);
                    
                    Display_7Seg_Text("save",0);
                    
                }else if(menu_type==MENU_SET_RTC)
                {
                    time.seconds=0;
                    time.minutes=menudef->hours_minutes_ptr->param1;
                    time.hours=menudef->hours_minutes_ptr->param;
                    time.day=menudef->day_month_ptr->param;
                    time.month=menudef->day_month_ptr->param1;
                    
                    time.year=((menudef->year_ptr->param1)+100*
                              ((menudef->year_ptr->param)%10));
                    
                    time.weekday=Find_Weekday((2000+time.year),
                                               time.month,time.day);
                    
                    if(Is_Valid_Date(time.day,time.month,(2000+time.year)))
                    {
                        PCF8583_Set_Date_Time(&time);
                         Display_7Seg_Text("save",0);
                        
                    }else
                    {
                         Display_7Seg_Text("badd",0);
                    }      
                }

                __delay_ms(2000);
                Display_7Seg_Text("****",0);

                return; 
            }
           
            current_menu_ptr=(current_menu_ptr->next_menu);
            if(current_menu_ptr==menudef->year_ptr)
            {
                 g_decimal_point=0b0000;
            }
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

void Select_Menu(MenuParamPonterStruct *menudef, KeyPointerStruct *keydef, 
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
        
    }else if(check_button==k_set_time2_short)
    {
        Menu_Set_Time(menudef,keydef,MENU_FREE_DAY);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Write_Limit_To_EEprom(MenuParamStruct *limit1, MenuParamStruct *limit2, 
                           unsigned char limit_type)
{
    if(limit_type==MENU_WORK_DAY)
    {
        Write_EEprom(limit1->param, WORK_DAY_MINUTES1_ADD);
        Write_EEprom(limit1->param1,WORK_DAY_HOURS1_ADD);
        Write_EEprom(limit2->param, WORK_DAY_MINUTES2_ADD);
        Write_EEprom(limit2->param1, WORK_DAY_HOURS2_ADD);
        
    }else if(limit_type==MENU_FREE_DAY)
    {
        Write_EEprom(limit1->param,FREE_DAY_MINUTES1_ADD);
        Write_EEprom(limit1->param1,FREE_DAY_HOURS1_ADD);
        Write_EEprom(limit2->param,FREE_DAY_MINUTES2_ADD);
        Write_EEprom(limit2->param1,FREE_DAY_HOURS2_ADD);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Read_Limit_From_EEprom(MenuParamStruct *limit1, MenuParamStruct *limit2, 
                            unsigned char limit_type)
{
    if(limit_type==MENU_WORK_DAY)
    {
        limit1->param=Read_EEprom(WORK_DAY_MINUTES1_ADD);
        limit1->param1=Read_EEprom(WORK_DAY_HOURS1_ADD);
        limit2->param=Read_EEprom(WORK_DAY_MINUTES2_ADD);
        limit2->param1=Read_EEprom(WORK_DAY_HOURS2_ADD);
        
    }else if(limit_type==MENU_FREE_DAY)
    {
        limit1->param=Read_EEprom(FREE_DAY_MINUTES1_ADD);
        limit1->param1=Read_EEprom(FREE_DAY_HOURS1_ADD);
        limit2->param=Read_EEprom(FREE_DAY_MINUTES2_ADD);
        limit2->param1=Read_EEprom(FREE_DAY_HOURS2_ADD);
    }
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Display_7Seg_Text(char *text, unsigned char decimal_point)
{
    unsigned char i;
    
    for(i=0;i<4;i++)
    {
        g_display_text[i]=*(text+i);
    }
    
    g_decimal_point=decimal_point;
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Display_Weekday(unsigned char weekday)
{
    switch (weekday)
    {
        case monday:     Display_7Seg_Text("mond",0); break;
        case tuesday:    Display_7Seg_Text("tues",0); break;
        case wedenesday: Display_7Seg_Text("wede",0); break;
        case thursday:   Display_7Seg_Text("thur",0); break;
        case friday:     Display_7Seg_Text("frid",0); break;
        case saturday:   Display_7Seg_Text("satu",0); break;
        case sunday:     Display_7Seg_Text("sund",0); break;
    }
}