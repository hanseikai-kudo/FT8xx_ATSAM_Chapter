#include <asf.h>
#include "ft8xx.h"
#include "user_gpio.h"
#include "user_spi.h"
#include "def.h"

#include <samd21g18a.h>
#include <stdbool.h>

















unsigned short JpegLoad( int handle, unsigned char *src, unsigned long dst_offset, unsigned long size )
{
	unsigned int cmdBufferRd = 0x0000;
	unsigned int cmdBufferWr = 0x0000;
    unsigned short cmdOffset;

    unsigned long count=0;
    unsigned long ofs=0;

//--------------------------------------
	do
	{
		cmdBufferRd = HostMemReadWord(REG_CMD_READ);
		cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
	}while (cmdBufferWr != cmdBufferRd);
	cmdOffset = cmdBufferWr;

//	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, (CMD_DLSTART));
//	cmdOffset = incCMDOffset(cmdOffset, 4);

//	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, (DL_CLEAR_RGB | BLACK));
//	cmdOffset = incCMDOffset(cmdOffset, 4);
//
//	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, (DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
//	cmdOffset = incCMDOffset(cmdOffset, 4);

//--------------------------------------

	HostMemWriteDoubleWord( RAM_CMD + cmdOffset, (CMD_LOADIMAGE));
	cmdOffset = incCMDOffset(cmdOffset, 4);

	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, dst_offset);
	cmdOffset = incCMDOffset(cmdOffset, 4);								
	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, 2);//OPT_NODL);
	cmdOffset = incCMDOffset(cmdOffset, 4);								

//	i= 79776;
	ofs=0;
	while(size > 0)
	{

		if(size >= 8192)
		{
			count=8192;
		}
		else
		{
			count=size;
		}
		cmdOffset = WrCmdBuf( RAM_CMD + cmdOffset, cmdOffset, &src[ofs],count );
		ofs=ofs+count;
		size -= count;
	}
/*
	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, BITMAP_HANDLE(handle));
	cmdOffset = incCMDOffset(cmdOffset, 4);

	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, CMD_SETBITMAP);
	cmdOffset = incCMDOffset(cmdOffset, 4);

	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, dst_offset);
	cmdOffset = incCMDOffset(cmdOffset, 4);

	HostMemWriteWord(RAM_CMD + cmdOffset, RGB565);
	cmdOffset = incCMDOffset(cmdOffset, 2);

	HostMemWriteWord(RAM_CMD + cmdOffset, 480);
	cmdOffset = incCMDOffset(cmdOffset, 2);
	
    HostMemWriteWord(RAM_CMD + cmdOffset, 272);
	cmdOffset = incCMDOffset(cmdOffset, 2);

	HostMemWriteWord(RAM_CMD + cmdOffset, 0x0000);
	cmdOffset = incCMDOffset(cmdOffset, 2);
*/
//--------------------------------------
/*
	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, (DL_DISPLAY));
	cmdOffset = incCMDOffset(cmdOffset, 4);
 	cmdBufferRd = HostMemReadWord(REG_CMD_READ);
	
	HostMemWriteDoubleWord(RAM_CMD + cmdOffset, (CMD_SWAP));
	cmdOffset = incCMDOffset(cmdOffset, 4);
*/
	HostMemWriteWord(REG_CMD_WRITE, (cmdOffset));

	do
	{
		cmdBufferRd = HostMemReadWord(REG_CMD_READ);
		cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
	}while (cmdBufferWr != cmdBufferRd);

	cmdOffset = cmdBufferWr;
}





/*
	HostMemWriteWord(REG_CMD_WRITE, (cmdOffset));					// Update the ring buffer pointer so the graphics processor starts executing

	do
	{
		cmdBufferRd = HostMemReadWord(REG_CMD_READ);					// Read the graphics processor read pointer

		cmdBufferWr = HostMemReadWord(REG_CMD_WRITE); 				// Read the graphics processor write pointer
	}while (cmdBufferWr != cmdBufferRd);									// Wait until the two registers match
	cmdOffset = cmdBufferWr;															// The new starting point the first location after the last command
*/















