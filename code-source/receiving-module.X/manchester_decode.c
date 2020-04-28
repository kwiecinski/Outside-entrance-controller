#include <xc.h>
#include "main.h"
#include "manchester_decode.h"
#include "circular_buffer.h"
#include "hw_uart.h"
#include "crc16.h"
#include "interrupts.h"
#include "melody.h"
#include "utils.h"

#define	BAUD_RATE		300
#define MANCHESTER_2T	(_XTAL_FREQ/4)/BAUD_RATE

#define MANCHESTER_2T_LOW	MANCHESTER_2T*60/100
#define MANCHESTER_2T_HIGH	MANCHESTER_2T*140/100
#define MANCHESTER_T_LOW	(MANCHESTER_2T/2)*60/100
#define MANCHESTER_T_HIGH	(MANCHESTER_2T/2)*140/100

//errors defines
#define BAT_TIME_AFTER_SHORT_T	0x00
#define BAD_TIME_AFTER_LONG_T	0x01
#define NO_SYNC_TIME			0x02
#define TOO_LONG_WAIT_FOR_SLOPE	0x03
#define SYNC_ERROR			    0x04
#define FRAME_DECODE_ERROR      0x05
#define DATA_SIZE               6



void ManchesterDecode(unsigned char *edge_dir, unsigned int *pulse_time)
{
    static unsigned char decoded_byte, bit_pos, data_counter,f_start_data;
    static unsigned char f_slope_t,f_sync;
    
	if(f_sync==0)
    {   
        if((*pulse_time>MANCHESTER_2T_LOW) && (*pulse_time<MANCHESTER_2T_HIGH))
        {	
            f_slope_t=0;
            decoded_byte=0;
            bit_pos=0;
            f_sync=1; 
            data_counter=0;
         
            return;
            
        }else
        {
            return;
        }
    }else if(f_sync)
    {
        if(f_slope_t==0)
        {
            if((*pulse_time>MANCHESTER_T_LOW) && (*pulse_time<MANCHESTER_T_HIGH))
            {  
                    f_slope_t=1;
                    return;
                    
            }else if((*pulse_time>MANCHESTER_2T_LOW) && (*pulse_time<MANCHESTER_2T_HIGH))
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
                //return BadTime;
                f_sync=0;
                return;
            }	
                
        }else
        {
            if((*pulse_time>MANCHESTER_T_LOW) && (*pulse_time<MANCHESTER_T_HIGH))
            {
                if(*edge_dir==0)
                {
                    decoded_byte^=(0x80>>bit_pos);
                }else
                {
                    decoded_byte^=(0x00>>bit_pos);	
                }
                bit_pos++;
                f_slope_t=0;
                
            }else
            {	
                //return BAT_TIME_AFTER_SHORT_T;
                f_sync=0;
                return;
            }
        }

        if(bit_pos>7)
        {
            if(decoded_byte=='$')
            {
                f_start_data=1;
           }
            if(f_start_data)
            {
                FrameBuffer(&decoded_byte,WRITE);
                data_counter++;
            }
                decoded_byte=0;
                bit_pos=0;
                
            if(data_counter>=DATA_SIZE)
            {
                f_start_data=0;
                f_sync=0;
                return;
            }
        }
	}//else if(!f_sync)
}//void ManchesterDecode


///////////////////////////////////////////////////////////////////////////////
///
/// The function checks if receiving has been completed and if so, processes data from the buffer.
/// ISR_ACK - Global flag in CCP ICS
/// State_Check - Global var for TIMER0 ISR
///
///////////////////////////////////////////////////////////////////////////////

unsigned int FrameDecode(DataStruct *DataRCV)
{
    unsigned char data, i, data_counter;
    
    while(1)
    {
        if(FrameBuffer(&data,READ)==NoData)
        {
            return NoData;
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
                if(FrameBuffer(&data,READ)==NoData)
                {
                    return NoData;
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

unsigned char CRC_check(DataStruct *DataRCV)
{
	unsigned int crc_val;
	crc_val=CRC16(&DataRCV->frame[0],DATA_SIZE-4);											
	if( (DataRCV->frame[DATA_SIZE-4]==crc_val>>8) && (DataRCV->frame[DATA_SIZE-3]==(crc_val&0x00FF)) )
    {
		return 1;
	}else
	{
		return 0;
	}
}

unsigned char CheckEvent(DataStruct *DataRCV)
{
    static char a;
    
    if(DataRCV->frame[0]=='G' && DataRCV->frame[1]=='0')
    {
        PlayRing(GATE);
        SendUART('G');
        while(FrameDecode(DataRCV)!=NoData);
        return 1;
    }else if(DataRCV->frame[0]=='W' && DataRCV->frame[1]=='0')
    {
       // PlayRing(WICKET);
        SendUART('W');
        if(a)
        {
            PORTCbits.RC3=1;
            a=0;
        }else
        {
             PORTCbits.RC3=0;
             a=1;
        }
        while(FrameDecode(DataRCV)!=NoData);
        return 1;
        
    }else if(DataRCV->frame[0]=='W' && DataRCV->frame[1]=='B')
    {
        //PlayRing(WICKET_BELL);
        SendUART('B');
        while(FrameDecode(DataRCV)!=NoData);
        return 1;
    }
    
    return 0;
}


///////////////////////////////////////////////////////////////////////////////
///
/// The function checks if receiving has been completed and if so, processes data from the buffer.a
/// ISR_ACK - Global flag in CCP ICS
/// State_Check - Global var for TIMER0 ISR
///
///////////////////////////////////////////////////////////////////////////////

void ProcessRCVData(DataStruct *DataRCV)
{
    if(ISR_ACK==1)                      //Checking if interrupt fire          
    {
            
        //check if the high state on the communication port lasts longer than MANCHESTER_2T_HIGH
        //if so, it means that the transmission has ended and the received data can be processed
        if(PORTCbits.RC1 == 0)          
        { 
            Timer1=80;
        }
        //receiving completed, now data processing
        if(Timer1==0)
        {
            while(FrameDecode(DataRCV)!=NoData)
            {
                if(DataRCV->RCV_Frame)
                {
                    if(CRC_check(DataRCV))
                    {
                     
                        if(CheckEvent(DataRCV))
                        {
                            return;
                        }
                    }else
                    {
                        SendUART('C');
                    }

                    DataRCV->RCV_Frame=0;
                }
            }
            ISR_ACK=0;
        }
      
    }
    
}