#ifndef _USER_SPI_H_

#define _USER_SPI_H_


//-----------------------------------------------------------------------------------
#define SPI_CTRLA_DORD 0x40000000
#define SPI_CTRLA_CPOL 0x20000000
#define SPI_CTRLA_CPHA 0x10000000
#define SPI_CTRLA_IBON 0x00000010
#define SPI_CTRLA_RUNSTDBY 0x00000080
#define SPI_CTRLA_MASTER 0x0000000C  //011 xx
#define SPI_CTRLA_SLAVE 0x00000008   //010 xx
#define SPI_CTRLA_ENABLE 0x00000002
#define SPI_CTRLA_SWRST 0x00000001

#define SPI_CTRLA_DIN_PAD00 (0x00000000)
#define SPI_CTRLA_DIN_PAD01 (0x00100000)
#define SPI_CTRLA_DIN_PAD02 (0x00200000)
#define SPI_CTRLA_DIN_PAD03 (0x00300000)

#define SPI_CTRLA_DOUT_MODE00 (0x00000000)
#define SPI_CTRLA_DOUT_MODE01 (0x00010000)
#define SPI_CTRLA_DOUT_MODE02 (0x00020000)
#define SPI_CTRLA_DOUT_MODE03 (0x00030000)

//-----------------------------------------------------------------------------------
#define SPI_CTRLA_MODE00 (0x00000000)
#define SPI_CTRLA_MODE01 (0x00000000|SPI_CTRLA_CPHA)
#define SPI_CTRLA_MODE02 (SPI_CTRLA_CPOL|0x00000000)
#define SPI_CTRLA_MODE03 (SPI_CTRLA_CPOL|SPI_CTRLA_CPHA)
#define SPI_CTRLA_MSB_FIRST (0x00000000)
#define SPI_CTRLA_LSB_FIRST (SPI_CTRLA_DORD)
#define SPI_CTRLA_FORM_SPI (0x00000000)
#define SPI_CTRLA_FORM_SPIADR (0x02000000)

#define SPI_CTRLA_PINMODE00 (SPI_CTRLA_DOUT_MODE00|SPI_CTRLA_DIN_PAD03)
#define SPI_CTRLA_PINMODE01 (SPI_CTRLA_DOUT_MODE01|SPI_CTRLA_DIN_PAD00)
#define SPI_CTRLA_PINMODE02 (SPI_CTRLA_DOUT_MODE02|SPI_CTRLA_DIN_PAD00)
#define SPI_CTRLA_PINMODE03 (SPI_CTRLA_DOUT_MODE03|SPI_CTRLA_DIN_PAD02)



//DOPO   DO     SCK    DI     Slave_SS Master_SS
//0x0    PAD[0] PAD[1] PAD[3] PAD[2]   System configuration
//0x1    PAD[2] PAD[3] PAD[0] PAD[1]   System configuration
//0x2    PAD[3] PAD[1] PAD[0] PAD[2]   System configuration
//0x3    PAD[0] PAD[3] PAD[2] PAD[1]   System configuration

#define SPI_CTRLB_RXEN (0x00020000)
#define SPI_CTRLB_AMODE_MASK (0x00000000)
#define SPI_CTRLB_AMODE_2ADDRS (0x00004000)
#define SPI_CTRLB_AMODE_RANGE  (0x00008000)
#define SPI_CTRLB_AMODE_RESERVED (0x0000C000)

#define SPI_CTRLB_MSSEN (0x00002000)
#define SPI_CTRLB_SSDE  (0x00000200)
#define SPI_CTRLB_PLOADEN (0x00000040)
#define SPI_CTRLB_CHSIZE8BIT (0x00000000)
#define SPI_CTRLB_CHSIZE9BIT (0x00000001)

#define SPI_INTFLAG_RXC 0x04
#define SPI_INTFLAG_TXC 0x02
#define SPI_INTFLAG_DRE 0x01
#define SPI_DATA_MASK 0x000001FF



//bool SercomSPI_TransByte( Sercom *sercom, unsigned short tx_data, unsigned short *rx_data );
bool SercomSPI_UnitReady( Sercom *sercom );
bool SercomSPI_WriteComplete( Sercom *sercom );
bool SercomSPI_Init( Sercom *sercom, unsigned long ctrl_a, unsigned long ctrl_b, unsigned char baud );
bool SercomSPI_Disable( Sercom *sercom );
bool SercomSPI_Enable( Sercom *sercom );
unsigned char SercomSPI_TransByte( Sercom *sercom, unsigned short tx_data );

#endif

