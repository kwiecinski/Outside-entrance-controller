/* 
 * Counting 16bit CRC from 8bit data tab
 * CRC Polynomial: 0x1021
 * 
 */

unsigned int CRC16(unsigned char *data_tab_ptr, unsigned char size)
{
    const unsigned int generator = 0x1021;
    unsigned int crc = 0; 
    unsigned char k,i;
    
    for(k=0;k<size;k++)
    {
        crc ^= (*(data_tab_ptr+k) << 8); //move byte into MSB of 16bit CRC
        
        for (i=0;i<8;i++)
        {
            if ((crc&0x8000)!=0)     //test for MSB = bit 15
            {
                 crc=(crc<<1)^generator;
            }else
            {
                crc<<=1;
            }
        }
    }
    
    return crc;
}

