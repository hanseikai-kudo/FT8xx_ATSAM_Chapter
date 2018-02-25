#include <asf.h>
#include "ft8xx.h"
#include "user_gpio.h"
#include "user_spi.h"
#include "def.h"

#include <samd21g18a.h>
#include <stdbool.h>




#define ATSAM_SPI_TRANS(p) \
				SERCOM_FT8XX->SPI.DATA.reg = ((unsigned char)(p) & SPI_DATA_MASK);\
				while( !(SERCOM_FT8XX->SPI.INTFLAG.reg & SPI_INTFLAG_TXC) );\
				rx = (SERCOM_FT8XX->SPI.DATA.reg & 0xFF);




unsigned char TransByte( unsigned char byte )
{
	unsigned short rcv;

	rcv = SercomSPI_TransByte( SERCOM_FT8XX, byte );

	return rcv;
}



bool HostCommand( unsigned char cmd, unsigned char p1, unsigned char p2 )
{
	GpioBitOut( FT8XX_nCS,0 );

		TransByte( cmd );
		TransByte( p1 );
		TransByte( p2 );

	GpioBitOut( FT8XX_nCS,1 );

	return true;
}



bool HostMemWriteByte( unsigned long adr, unsigned char p1 )
{
	unsigned long addres = adr | 0x00800000;

	GpioBitOut( FT8XX_nCS,0 );

		TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
		TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
		TransByte( (unsigned char)(addres & 0x000000FF) );
		TransByte( (unsigned char)(p1) );

	GpioBitOut( FT8XX_nCS,1 );

	return true;
}



bool HostMemWriteWord( unsigned long adr, unsigned short word )
{
	unsigned long addres = adr | 0x00800000;

	GpioBitOut( FT8XX_nCS,0 );

		TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
		TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
		TransByte( (unsigned char)(addres & 0x000000FF) );
		TransByte( (unsigned char)(word&0x00FF) );
		TransByte( (unsigned char)((word&0xFF00)>>8) );

	GpioBitOut( FT8XX_nCS,1 );

	return true;
}



bool HostMemWriteDoubleWord( unsigned long adr, unsigned long dword )
{
	int i;
	unsigned char rx;
	unsigned long addres = adr | 0x00800000;
	unsigned char buf[8];

	buf[0]= (unsigned char)((addres & 0x00FF0000)>>16);
	buf[1]= (unsigned char)((addres & 0x0000FF00)>>8);
	buf[2]= (unsigned char)(addres & 0x000000FF);
	buf[3]= (unsigned char)(dword&0x000000FF);
	buf[4]= (unsigned char)((dword&0x0000FF00)>> 8);
	buf[5]= (unsigned char)((dword&0x00FF0000)>>16);
	buf[6]= (unsigned char)((dword&0xFF000000)>>24);

	GpioBitOut( FT8XX_nCS,0 );

		for( i=0;i<7;i++ )
		{
			ATSAM_SPI_TRANS( buf[i] );
		}

	GpioBitOut( FT8XX_nCS,1 );
	return true;
}



unsigned char HostMemReadByte( unsigned long adr )
{
	unsigned long addres = adr | 0x00000000;
	unsigned char data;

	GpioBitOut( FT8XX_nCS,0 );

		TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
		TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
		TransByte( (unsigned char)(addres & 0x000000FF) );

		TransByte( 0x00 );//dummy byte

		data = TransByte( 0x00 );

	GpioBitOut( FT8XX_nCS,1 );

	return data;
}



unsigned short HostMemReadWord( unsigned long adr )
{
	unsigned long addres = adr | 0x00000000;
	unsigned char H_byte;
	unsigned char L_byte;

	GpioBitOut( FT8XX_nCS,0 );

		TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
		TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
		TransByte( (unsigned char)(addres & 0x000000FF) );

		TransByte( 0x00 );//dummy byte

		L_byte = TransByte( 0x00 );//Low byte
		H_byte = TransByte( 0x00 );//Hi  byte

	GpioBitOut( FT8XX_nCS,1 );

	return ((unsigned short)(H_byte * 256 + L_byte)) ;
}



unsigned long HostMemReadDoubleDWord( unsigned long adr )
{
	unsigned long addres = adr | 0x00000000;
	unsigned char ary[4];
	unsigned long value;

	GpioBitOut( FT8XX_nCS,0 );

		TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
		TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
		TransByte( (unsigned char)(addres & 0x000000FF) );

		TransByte( 0x00 );//dummy byte

		ary[3]= TransByte( 0x00 );
		ary[2]= TransByte( 0x00 );
		ary[1]= TransByte( 0x00 );
		ary[0]= TransByte( 0x00 );

	GpioBitOut( FT8XX_nCS,1 );

	value = (((unsigned long)(ary[0]))<<24)|(((unsigned long)(ary[1]))<<16)|(((unsigned long)(ary[2]))<<8)|((unsigned long)(ary[3]));

	return value;
}



bool HostMemWrite_Burst( unsigned long adr,unsigned char *ptr,unsigned long size )
{
	unsigned char rx;
	unsigned long addres = adr | 0x00800000;

	GpioBitOut( FT8XX_nCS,0 );

		TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
		TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
		TransByte( (unsigned char)(addres & 0x000000FF) );
		while( size )
		{
			ATSAM_SPI_TRANS( *ptr++ );

			size--;
		};

	GpioBitOut( FT8XX_nCS,1 );

	return true;
}




//---------------------------------------------------------------------------------------------------------------------------



#define FT_CMD_FIFO_SIZE     (4*1024L)  //4KB coprocessor Fifo size


static unsigned short fifo_Freespace(unsigned short cmd_fifo_wp );
static void CheckCmdBuffer( unsigned long now , unsigned short count );
static unsigned short Updatecmdfifo( unsigned short cmd_fifo_wp, unsigned short count);



unsigned int incCMDOffset(unsigned int currentOffset, unsigned short commandSize)
{
    unsigned int newOffset;

    newOffset = currentOffset + commandSize;
    if(newOffset > 4095)
    {
        newOffset = (newOffset - 4096);
    }
    return newOffset;
}



static unsigned short fifo_Freespace(unsigned short cmd_fifo_wp )
{
 
	unsigned short fullness,retval;
    fullness = ( cmd_fifo_wp - HostMemReadWord(REG_CMD_READ)) & 4095;
    retval = (FT_CMD_FIFO_SIZE - 4) - fullness;


    return (retval);
}



static void CheckCmdBuffer( unsigned long now , unsigned short count )
{
	unsigned short getfreespace;
	do
	{
		getfreespace = fifo_Freespace( now );
	}while( getfreespace < count );
}



static unsigned short Updatecmdfifo( unsigned short cmd_fifo_wp, unsigned short count )
{
     cmd_fifo_wp = ( cmd_fifo_wp + count) & 4095;
     cmd_fifo_wp = ( cmd_fifo_wp + 3) & 0xffc;

    HostMemWriteWord( REG_CMD_WRITE, cmd_fifo_wp);

	return cmd_fifo_wp;
}



unsigned short WaitCmdfifo_empty( void )
{
	unsigned short cmd_fifo_wp,cmdBufferRd,cmdBufferWr;

	do
	{
		cmdBufferRd = HostMemReadWord(REG_CMD_READ);
		cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
	}while (cmdBufferWr != cmdBufferRd);

    cmd_fifo_wp = HostMemReadWord( REG_CMD_WRITE);

	return cmd_fifo_wp;
}



unsigned short WrCmdBuf( unsigned long adr, unsigned short cmd_fifo_wp, unsigned char *buffer,unsigned short count )
{
  unsigned char rx;
	unsigned long length =0, SizeTransfered = 0;
	unsigned long addres = adr | 0x00800000;

    do
	{
        length = count;

        if (length > fifo_Freespace(cmd_fifo_wp) )
		{
            length = fifo_Freespace(cmd_fifo_wp);
        }
		CheckCmdBuffer( cmd_fifo_wp, length);

		GpioBitOut( FT8XX_nCS,0 );

			addres = (RAM_CMD + cmd_fifo_wp) | 0x00800000;
			TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
			TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
			TransByte( (unsigned char)(addres & 0x000000FF) );
 
			SizeTransfered = 0;
			while (length--)
			{
				ATSAM_SPI_TRANS(*buffer++);

				SizeTransfered ++;
	        }
			length = SizeTransfered;
 
		GpioBitOut( FT8XX_nCS,1 );
		cmd_fifo_wp = Updatecmdfifo( cmd_fifo_wp, length);
		cmd_fifo_wp = WaitCmdfifo_empty();
 
        count -= length;

    }while (count > 0);

	return cmd_fifo_wp;
}






unsigned short burst( unsigned char *buffer,unsigned short count )
{
  unsigned char rx;
	unsigned long length =0, SizeTransfered = 0;
	unsigned long addres = 0,spc=0;

	unsigned short cmdBufferRd,cmdBufferWr;

	cmdBufferRd = HostMemReadWord(REG_CMD_READ);



    do
	{
        length = count;
		spc = HostMemReadDoubleDWord(REG_CMDB_SPACE);
		if( length > spc )
		{
			length = spc;
		}

		GpioBitOut( FT8XX_nCS,0 );

			addres = (REG_CMDB_WRITE) | 0x00800000;
			TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
			TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
			TransByte( (unsigned char)(addres & 0x000000FF) );
 
			SizeTransfered = 0;
			while (length--)
			{
				ATSAM_SPI_TRANS(*buffer++);

				SizeTransfered ++;
	        }
			length = SizeTransfered;
 
		GpioBitOut( FT8XX_nCS,1 );

		spc = HostMemReadDoubleDWord(REG_CMDB_SPACE);
	    HostMemWriteWord( REG_CMD_WRITE, cmdBufferRd + length );

		cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);

		WaitCmdfifo_empty();
 
        count -= length;


    }while (count > 0);

	return cmdBufferWr;
}






bool Boxfill_Burst( unsigned long adr,unsigned long size,int mode, unsigned char _r,unsigned char _g, unsigned char _b )
{
	unsigned char rx;
	unsigned long addres = adr | 0x00800000;
	unsigned short rgb=0;

	switch( mode )
	{
		case RGB565:
			rgb = (((_r&0xF8)>>3) * 0x0800) + (((_g&0xFC)>>2) * 0x0020) + (((_b&0xF8)>>3) * 0x0001);
			break;
		case ARGB1555:
			rgb = (((_r&0xF8)>>3) * 0x0400) + (((_g&0xF8)>>3) * 0x0020) + (((_b&0xF8)>>3) * 0x0001);
			break;
		default:
			rgb = 0;
	}



	GpioBitOut( FT8XX_nCS,0 );

		TransByte( (unsigned char)((addres & 0x00FF0000)>>16) );
		TransByte( (unsigned char)((addres & 0x0000FF00)>>8) );
		TransByte( (unsigned char)(addres & 0x000000FF) );
		while( size )
		{
			ATSAM_SPI_TRANS( (rgb&0x00FF) );
			ATSAM_SPI_TRANS( ((rgb&0xFF00)>>8) );

			size--;
		};

	GpioBitOut( FT8XX_nCS,1 );

	return true;
}

