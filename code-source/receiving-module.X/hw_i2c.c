#include <xc.h>
#include "main.h"

void Init_I2C(void)
{
    const unsigned long int k_i2c_clock=100000;
    
    //Configures the SDA and SCL pins as the source of the serial port pins
    SSPCONbits.SSPEN=1;
    //I2C Master mode, clock = FOSC / (4 * (SSPADD+1))
    SSPCONbits.SSPM=0b1000;

    //SSPADD = (_XTAL_FREQ/(4*k_i2c_clock))-1;
    SSPADD=19;    
    SSPCON2=0;
    SSPSTAT=0;

}


void I2C_Master_Wait(void)
{

    while (SSPCON2bits.RCEN || SSPCON2bits.PEN || SSPCON2bits.RSEN || 
          SSPCON2bits.SEN || SSPCON2bits.ACKEN || SSPSTATbits.R_nW); 
}

void I2C_Master_Start(void)
{
  I2C_Master_Wait();  
  SSPCON2bits.SEN = 1;
}


void I2C_Master_Stop(void)
{
  I2C_Master_Wait();
  SSPCON2bits.PEN = 1;
}


void I2C_Master_Write(unsigned data)
{   
  I2C_Master_Wait();
  SSPBUF = data;
}


unsigned char I2C_Master_Read(unsigned char ack)
{
    unsigned short data;
    I2C_Master_Wait();
    RCEN = 1;                 //Receive Enable bit 
    I2C_Master_Wait();
    data = SSPBUF;           //Read data from SSPBUF
    I2C_Master_Wait();
    ACKDT = (ack)?0:1;    //check if ack bit received  
    ACKEN = 1;         
    return data;
}

