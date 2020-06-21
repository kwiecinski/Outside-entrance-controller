#include <xc.h>
#include "main.h"
#include "manchester_decode.h"
#include "circular_buffer.h"
#include "hw_uart.h"
#include "crc16.h"
#include "interrupts.h"
#include "melody.h"
#include "utils.h"
#include "pcf8583.h"
#include "menu.h"

#define	BAUD_RATE		300

//XTAL_FREQ divided by 4 becouse of internal prescaler in Timer1
//see datasheet
#define MANCHESTER_T	(_XTAL_FREQ/4)/BAUD_RATE

//adding time window of 80% and 120%
#define MANCHESTER_2T_LOW	(MANCHESTER_T*2)*80/100
#define MANCHESTER_2T_HIGH	(MANCHESTER_T*2)*120/100
#define MANCHESTER_T_LOW	MANCHESTER_T*80/100
#define MANCHESTER_T_HIGH	MANCHESTER_T*120/100

//frame data size, example of frame: $G0xx& has 6 byte
#define DATA_SIZE               6


//function prototypes
unsigned char Check_Time_Date(TimeStruct *time, 
                              MenuParamPonterStruct *time_limit);

///////////////////////////////////////////////////////////////////////////////
///
/// Alghoritm from Atmel application note - Manchester Coding Basics
/// PURPOSE: Decode bytes - base on edge direction and pulse time, writes to 
///          circular buffer
/// INPUTS: edge_dir - edge direction of received slope (0 or 1)
//          pulse_time - time betweet to edges
//
///////////////////////////////////////////////////////////////////////////////

void Manchester_Decode(unsigned char *edge_dir, unsigned int *pulse_time)
{
    static unsigned char decoded_byte, bit_pos, data_counter, start_data_flag;
    static unsigned char next_edge_also_T_flag,sync_flag;
    
	if(sync_flag==0)
    {   
        //Waiting for 2T to synchronize witch data clock
        
        if((*pulse_time>MANCHESTER_2T_LOW) && (*pulse_time<MANCHESTER_2T_HIGH))
        {	
            next_edge_also_T_flag=0;
            decoded_byte=0;
            bit_pos=0;
            sync_flag=1; 
            data_counter=0;
           
            return;
            
        }else
        {
            //SYNC_ERROR
            return;
        }
    }else if(sync_flag)
    {
        
        if(next_edge_also_T_flag==0)
        {
            if((*pulse_time>MANCHESTER_T_LOW) && 
               (*pulse_time<MANCHESTER_T_HIGH))
            {  
                    next_edge_also_T_flag=1;
                    return;
                    
            }else if((*pulse_time>MANCHESTER_2T_LOW) && 
                     (*pulse_time<MANCHESTER_2T_HIGH))
            {
                if(*edge_dir==0)
                {
                    decoded_byte^=(0x80>>bit_pos);
                }else
                {
                    decoded_byte^=(0x00>>bit_pos);	
                }
                bit_pos++;
                
            }else        
            {	
                //BAD_TIME_AFTER_LONG_T_ERROR
                sync_flag=0;
                return;
            }	
                
        }else
        {
            if((*pulse_time>MANCHESTER_T_LOW) && 
               (*pulse_time<MANCHESTER_T_HIGH))
            {
                if(*edge_dir==0)
                {
                    decoded_byte^=(0x80>>bit_pos);
                }else
                {
                    decoded_byte^=(0x00>>bit_pos);	
                }
                bit_pos++;
                next_edge_also_T_flag=0;
                
            }else
            {	
                //BAT_TIME_AFTER_SHORT_T_ERROR;
                sync_flag=0;
                return;
            }
        }

        if(bit_pos>7)
        {
            if(decoded_byte=='$')
            {
                start_data_flag=1;
            } 
            
            if(start_data_flag)
            {
               //Write byte to circular buffer
                Frame_Buffer(&decoded_byte,WRITE);
                data_counter++;
            }
            
            decoded_byte=0;
            bit_pos=0;
                
            if(data_counter>=DATA_SIZE)
            {
                start_data_flag=0;
                sync_flag=0;
                return;
            }
        }
	}//else if(!sync_flag)
}//void ManchesterDecode


///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

unsigned int Frame_Decode(DataStruct *DataRCV)
{
    unsigned char data, i;
    
    while(1)
    {
        if(Frame_Buffer(&data,READ)==NO_DATA)
        {
            return NO_DATA;
        }
       
        if(data=='$')
        {   
            for(i=0;i<(DATA_SIZE-2);i++)
			{
				DataRCV->frame[i]=0;
			}
            i=0;
            
            while(1)
            {
                if(Frame_Buffer(&data,READ)==NO_DATA)
                {
                    return NO_DATA;
                }
                if(data=='&' && i==(DATA_SIZE-2))
                {
                    DataRCV->RCV_Frame=1;
                    return 10;
                }
                DataRCV->frame[i]=data;
                i++;
                if(i>(DATA_SIZE-2))
                {
                    break;
                }
            }
        }//if(data=='$')       
    }//while(1)
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////


unsigned char CRC_check(DataStruct *DataRCV)
{
	unsigned int crc_val;
    
	crc_val=CRC16(&DataRCV->frame[0],DATA_SIZE-4);	
    
	if((DataRCV->frame[DATA_SIZE-4]==crc_val>>8) && 
       (DataRCV->frame[DATA_SIZE-3]==(crc_val&0x00FF)))
    {
		return 1;
	}else
	{
		return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
///
/// 
/// 
/// 
///
///////////////////////////////////////////////////////////////////////////////

unsigned char Check_Event(DataStruct *DataRCV)
{
    if(DataRCV->frame[0]=='G' && DataRCV->frame[1]=='0')
    {
        
        Send_7Seg_Text("bram",0);
        g_display_controll=0;   //disable 7seg function (stack ovf)
        Play_Ring();
        while(Frame_Decode(DataRCV)!=NO_DATA);
        g_display_controll=1;
        Send_7Seg_Text("****",0);
        
        return 1;
        
    }else if(DataRCV->frame[0]=='W' && DataRCV->frame[1]=='0')
    {
    
        Send_7Seg_Text("furt",0);
        g_display_controll=0;   //disable 7seg function (stack ovf)
        Play_Ring();
        while(Frame_Decode(DataRCV)!=NO_DATA);
        g_display_controll=1;
        Send_7Seg_Text("****",0);
        
        return 1;
        
    }else if(DataRCV->frame[0]=='W' && DataRCV->frame[1]=='B')
    {
 
        Send_7Seg_Text("dzwo",0);
        g_display_controll=0;   //disable 7seg function (stack ovf)
        Play_Ring();
        while(Frame_Decode(DataRCV)!=NO_DATA);
        g_display_controll=1;
        Send_7Seg_Text("****",0);
           
        return 1;
    }
    
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
///
/// The function checks if receiving has been completed and if so, 
/// processes data from the buffer
/// 
///
///////////////////////////////////////////////////////////////////////////////

void Check_RCV_Data(DataStruct *DataRCV, TimeStruct *time, 
                    MenuParamPonterStruct *time_limit)
{
    if(g_reciver_ccp2_isr_fire_flag==1)        //Checking if interrupt fire          
    {
        
        /*
        * check if the high state on the communication port lasts longer
        * than MANCHESTER_2T_HIGH if so, it means that the transmission has 
        * ended and the received data can be processed
        */
        
        if(PORTCbits.RC1 == 0)          
        { 
            g_generic_timer=80;
            
        }
        //receiving completed, now data processing
        if(g_generic_timer==0)
        {
            while(Frame_Decode(DataRCV)!=NO_DATA)
            {
                if(DataRCV->RCV_Frame)
                {
                    if(CRC_check(DataRCV) && Check_Time_Date(time, time_limit))
                    {                  
                        if(Check_Event(DataRCV))
                        {       
                            return;
                        }
                    }else
                    {
                        //error
                    }

                    DataRCV->RCV_Frame=0;
                }
            }
            
            g_reciver_ccp2_isr_fire_flag=0;
        }
    }
}

unsigned char Check_Time_Date(TimeStruct *time, MenuParamPonterStruct *time_limit)
{
    unsigned int time_limit_min_1, time_limit_min_2, current_time_min;
    
    PCF8583_Read_Time_Date(time);
    
    if(time->weekday==saturday || time->weekday==sunday)
    {
        time_limit_min_1=time_limit->time_limit_free_day_1_ptr->param*60+
                         time_limit->time_limit_free_day_1_ptr->param1;
        time_limit_min_2=time_limit->time_limit_free_day_2_ptr->param*60+
                         time_limit->time_limit_free_day_2_ptr->param1;
        current_time_min=time->hours*60+time->minutes;
        
        
        if(time_limit_min_1>current_time_min && 
           time_limit_min_2>time_limit_min_1)
        {
            return 1;
        }else
        {
            return 0;
        } 
        
    }else
    {
        time_limit_min_1=time_limit->time_limit_work_day_1_ptr->param*60+
                         time_limit->time_limit_free_day_1_ptr->param1;
        
        time_limit_min_2=time_limit->time_limit_work_day_2_ptr->param*60+
                         time_limit->time_limit_free_day_2_ptr->param1;
        
        current_time_min=time->hours*60+time->minutes;

        if(time_limit_min_1>current_time_min && 
           time_limit_min_2>time_limit_min_1)
        {
            return 1;
        }else
        {
            return 0;
        }
        
        
    }
}
 