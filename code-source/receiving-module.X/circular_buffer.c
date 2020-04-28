#include <xc.h>
#include "circular_buffer.h"

#define BufferSize	30

unsigned char FrameBuffer(unsigned char *data,unsigned char mode)
{
	static unsigned char Buffer[BufferSize];
	static unsigned char ReadPointer,WritePointer,
		 		WritePointerMem;

	if(mode==WRITE)
	{
		WritePointerMem=WritePointer;
		WritePointer++;
		if(WritePointer==BufferSize)
		{
			WritePointer=0;
		}
		if(WritePointer==ReadPointer)
		{
			WritePointer=WritePointerMem;
			return BuffFull;
		}	

		Buffer[WritePointer]=*data;

		return 1;
			
	}else //READ
	{
		if(WritePointer!=ReadPointer)
		{
			ReadPointer++;		
			if(ReadPointer==BufferSize)
			{
				ReadPointer=0;
			}
			*data=Buffer[ReadPointer];
			return 1;

		}else
		{
			return NoData;
		}
	}
}
