#include <xc.h>
#include "main.h"
#include "display-7-segment.h"

#define SEG_A    PORTAbits.RA6
#define SEG_B    PORTAbits.RA7
#define SEG_C    PORTAbits.RA5
#define SEG_D    PORTAbits.RA4
#define SEG_E    PORTAbits.RA3
#define SEG_F    PORTAbits.RA2
#define SEG_G    PORTAbits.RA1
#define SEG_DP   PORTAbits.RA0
#define DIG1_CA  PORTBbits.RB7
#define DIG2_CA  PORTBbits.RB6
#define DIG3_CA  PORTBbits.RB5
#define DIG4_CA  PORTBbits.RB4

#define DIGIT1 1
#define DIGIT2 2
#define DIGIT3 3
#define DIGIT4 4

#define DECIMAL_POINT_ON  1
#define DECIMAL_POINT_OFF 0


unsigned char Sign_Anode_Data(unsigned char sign) 
{
    switch (sign) 
    {
        case 0 : return 0b11111100;
        case 1 : return 0b01100000;
        case 2 : return 0b11011010;
        case 3 : return 0b11110010;
        case 4 : return 0b01100110;
        case 5 : return 0b10110110;
        case 6 : return 0b10111110;
        case 7 : return 0b11100100;
        case 8 : return 0b11111110;
        case 9 : return 0b11110110;
        case '0': return 0b11111100;
        case '1': return 0b01100000;
        case '2': return 0b11011010;
        case '3': return 0b11110010;
        case '4': return 0b01100110;
        case '5': return 0b10110110;
        case '6': return 0b10111110;
        case '7': return 0b11100100;
        case '8': return 0b11111110;
        case '9': return 0b11110110;
        case 'a': return 0b11101110;
        case 'b': return 0b00111110;
        case 'c': return 0b10011100;
        case 'd': return 0b01111010;
        case 'e': return 0b10011110;
        case 'f': return 0b10001110;
        case 'g': return 0b10111100;
        case 'h': return 0b01101110;
        case 'i': return 0b01100000;
        case 'j': return 0b01110000;
        case 'k': return 0b01101110;   //same as H
        case 'l': return 0b00011100;
        case 'm': return 0b11101100;   //display as N
        case 'n': return 0b00101010;
        case 'o': return 0b11111100;
        case 'p': return 0b01110011;
        case 'r': return 0b00001010;
        case 's': return 0b10110110;
        case 't': return 0b00011110;
        case 'u': return 0b01111100;
        case 'v': return 0b01111100;   //same as U
        case 'w': return 0b01111100;   //same as U
        case 'x': return 0b01101110;   //same as H
        case 'y': return 0b01110110;
        case 'z': return 0b11011010;   //same as 2
        case '*': return 0b00000000;   //disable digit

     }
    
    return 0;
}

void Set_Anode_Outputs(unsigned char sign, unsigned char decimal_point);
void One_Digit_Handler(unsigned char sign, unsigned char digit_number, unsigned char decimal_point);

void Disable_All_Digits(void)
{
    DIG1_CA=1;
    DIG2_CA=1;
    DIG3_CA=1;
    DIG4_CA=1;

}


void Display_7Seg(unsigned char *text, unsigned char decimal_point)
{
    static unsigned char enable_digit=DIGIT1;
    
    if(enable_digit<=DIGIT1)
    {
        if(decimal_point & 0b1000)
        {    
            One_Digit_Handler(*text,DIGIT1,DECIMAL_POINT_ON);
        }else
        {
            One_Digit_Handler(*text,DIGIT1,DECIMAL_POINT_OFF);
        }
        enable_digit=DIGIT2;
        
    }else if(enable_digit==DIGIT2)
    {
        if(decimal_point & 0b0100)
        {    
            One_Digit_Handler(*(text+1),DIGIT2,DECIMAL_POINT_ON);
        }else
        {
            One_Digit_Handler(*(text+1),DIGIT2,DECIMAL_POINT_OFF);
        }
        enable_digit=DIGIT3;
        
    }else if(enable_digit==DIGIT3)
    {
        if(decimal_point & 0b0010)
        {    
           One_Digit_Handler(*(text+2),DIGIT3,DECIMAL_POINT_ON);
        }else
        {
           One_Digit_Handler(*(text+2),DIGIT3,DECIMAL_POINT_OFF);
        }
        enable_digit=DIGIT4;
        
    }else if(enable_digit>=DIGIT4)
    {  
        if(decimal_point & 0b0001)
        {    
            One_Digit_Handler(*(text+3),DIGIT4,DECIMAL_POINT_ON);
        }else
        {
            One_Digit_Handler(*(text+3),DIGIT4,DECIMAL_POINT_OFF);
        } 
        enable_digit=DIGIT1;
    }
    
}

void One_Digit_Handler(unsigned char sign, unsigned char digit_number, unsigned char decimal_point)
{
    
    Disable_All_Digits();
    Set_Anode_Outputs(Sign_Anode_Data(sign),decimal_point);
    switch (digit_number) 
    {
        case DIGIT1 : DIG1_CA=0; break;
        case DIGIT2 : DIG2_CA=0; break;
        case DIGIT3 : DIG3_CA=0; break;
        case DIGIT4 : DIG4_CA=0; break;
    }
 }

void Set_Anode_Outputs(unsigned char sign, unsigned char decimal_point)
{
    
    if(sign>>7 & 1)
    {
        SEG_A=1;
    }else
    {
        SEG_A=0;
    }
    
    if(sign>>6 & 1)
    {
        SEG_B=1;
    }else
    {
        SEG_B=0;
    }
    
    if(sign>>5 & 1)
    {
        SEG_C=1;
    }else
    {
        SEG_C=0;
    }
    
    if(sign>>4 & 1)
    {
        SEG_D=1;
    }else
    {
        SEG_D=0;
    }
    
    if(sign>>3 & 1)
    {
        SEG_E=1;
    }else
    {
        SEG_E=0;
    }
    
    if(sign>>2 & 1)
    {
        SEG_F=1;
    }else
    {
        SEG_F=0;
    }
    
    if(sign>>1 & 1)
    {
        SEG_G=1;
    }else
    {
        SEG_G=0;
    }
    
    if(decimal_point==1)
    {
        SEG_DP=1;
    }else
    {
        SEG_DP=0;
    }
}
