# 1 "crc16.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "/opt/microchip/xc8/v2.10/pic/include/language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "crc16.c" 2






unsigned int CRC16(unsigned char *data_tab_ptr, unsigned char size)
{
    const unsigned int generator = 0x1021;
    unsigned int crc = 0;
    unsigned char k,i;

    for(k=0;k<size;k++)
    {
        crc ^= (*(data_tab_ptr+k) << 8);

        for (i=0;i<8;i++)
        {
            if ((crc&0x8000)!=0)
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
