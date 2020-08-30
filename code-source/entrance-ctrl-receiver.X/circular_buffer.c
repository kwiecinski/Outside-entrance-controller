/* 
 * 
 * 
 *
 * 
 */

#include <xc.h>
#include "circular_buffer.h"

#define BUFFER_SIZE	60

unsigned char Frame_Buffer(unsigned char *data, unsigned char mode)
{
	static unsigned char buffer[BUFFER_SIZE];
	static unsigned char read_pointer, write_pointer, write_pointer_mem;

	if(mode==WRITE)
	{
		write_pointer_mem=write_pointer;
		write_pointer++;
        
		if(write_pointer==BUFFER_SIZE)
		{
			write_pointer=0;
		}
		if(write_pointer==read_pointer)
		{
			write_pointer=write_pointer_mem;
			return BUFF_FULL;
		}	

		buffer[write_pointer]=*data;

		return 1;
			
	}else //READ
	{
		if(write_pointer!=read_pointer)
		{
			read_pointer++;		
			if(read_pointer==BUFFER_SIZE)
			{
				read_pointer=0;
			}
			*data=buffer[read_pointer];
			return 1;

		}else
		{
			return NO_DATA;
		}
	}
}
