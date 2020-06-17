#include <xc.h>
#include "hw_i2c.h"
#include "main.h"
#include "hw_uart.h"

#define PCF8583_WRITE_ADDRESS      0xA0
#define PCF8583_READ_ADDRESS       0xA1

// Rejestry
#define PCF8583_CTRL_STATUS_REG    0x00
#define PCF8583_SECONDS_REG        0x02
#define PCF8583_MINUTES_REG        0x03
#define PCF8583_HOURS_REG          0x04
#define PCF8583_DATE_REG           0x05
#define PCF8583_MONTHS_REG         0x06

// NVRAM address to save year
#define PCF8583_YEAR_REG           0x10

//START/STOP clock
#define PCF8583_START_COUNTING     0x00
#define PCF8583_STOP_COUNTING      0x80


unsigned char BCD_2_DEC(unsigned char bcd) 
{
    return (((bcd>>4)*10) + (bcd&0x0F));
}

unsigned char DEC_2_BCD(unsigned char dec) 
{
    return (((dec/10)<<4)+(dec%10));
}

void PCF8583_Write_Byte(unsigned char address, unsigned char data)
{
	I2C_Master_Start();
	I2C_Master_Write(PCF8583_WRITE_ADDRESS);
	I2C_Master_Write(address);
	I2C_Master_Write(data);
	I2C_Master_Stop();
}   


unsigned char PCF8583_Read_Byte(unsigned char address)
{
	unsigned char retval;
	I2C_Master_Start();
	I2C_Master_Write(PCF8583_WRITE_ADDRESS);
	I2C_Master_Write(address);
	I2C_Master_Start();
	I2C_Master_Write(PCF8583_READ_ADDRESS);
	retval = I2C_Master_Read(0);
	I2C_Master_Stop();
	return(retval);
}   


void PCF8583_Read_Time_Date(TimeStruct *time_struct_ptr)
{		
	unsigned char year_bits;
	unsigned char year;
	unsigned char bcd_sec;
	unsigned char bcd_min;
	unsigned char bcd_hrs;
	unsigned char bcd_day;
	unsigned char bcd_mon;

    INTCONbits.GIE=0;
    
	I2C_Master_Start();
	I2C_Master_Write(PCF8583_WRITE_ADDRESS);
	I2C_Master_Write(PCF8583_SECONDS_REG); 
	I2C_Master_Start();
	I2C_Master_Write(PCF8583_READ_ADDRESS);
	bcd_sec = I2C_Master_Read(1);
	bcd_min = I2C_Master_Read(1);     
	bcd_hrs = I2C_Master_Read(1);   
	bcd_day = I2C_Master_Read(1);  
	bcd_mon = I2C_Master_Read(0); 
    I2C_Master_Stop();
 
	time_struct_ptr->seconds = BCD_2_DEC(bcd_sec);     
	time_struct_ptr->minutes = BCD_2_DEC(bcd_min);     
	time_struct_ptr->hours   = BCD_2_DEC(bcd_hrs & 0x3F); 
	time_struct_ptr->day     = BCD_2_DEC(bcd_day & 0x3F);
	time_struct_ptr->month   = BCD_2_DEC(bcd_mon & 0x1F);
    time_struct_ptr->weekday = bcd_mon>>5;
    
	year_bits   = bcd_day >> 6;   

	//odczyt roku
	year = PCF8583_Read_Byte(PCF8583_YEAR_REG);
	while(year_bits != (year & 3))
	{
		year++;
	}
	time_struct_ptr->year = year;
	PCF8583_Write_Byte(PCF8583_YEAR_REG, year);
    
    INTCONbits.GIE=1;
}


void PCF8583_Set_Date_Time(TimeStruct *time_struct_ptr)
{
	unsigned char bcd_sec;
	unsigned char bcd_min;
	unsigned char bcd_hrs;
	unsigned char bcd_day;
	unsigned char bcd_mon;
    
	INTCONbits.GIE=0;
    
	bcd_sec = DEC_2_BCD(time_struct_ptr->seconds);
	bcd_min = DEC_2_BCD(time_struct_ptr->minutes);
	bcd_hrs = DEC_2_BCD(time_struct_ptr->hours); 
	bcd_day = DEC_2_BCD(time_struct_ptr->day) | (time_struct_ptr->year<<6);
	bcd_mon = DEC_2_BCD(time_struct_ptr->month & 0b00011111) | 
              (time_struct_ptr->weekday<<5);
    
	PCF8583_Write_Byte(PCF8583_CTRL_STATUS_REG, PCF8583_STOP_COUNTING);
	
	I2C_Master_Start();
	I2C_Master_Write(PCF8583_WRITE_ADDRESS);
	I2C_Master_Write(PCF8583_SECONDS_REG);
	I2C_Master_Write(bcd_sec); 
	I2C_Master_Write(bcd_min);
	I2C_Master_Write(bcd_hrs);   
	I2C_Master_Write(bcd_day);
	I2C_Master_Write(bcd_mon);
	I2C_Master_Stop();

	PCF8583_Write_Byte(PCF8583_YEAR_REG,time_struct_ptr->year);
	PCF8583_Write_Byte(PCF8583_CTRL_STATUS_REG, PCF8583_START_COUNTING);
    
    INTCONbits.GIE=1;
}