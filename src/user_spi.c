
#include <samd21g18a.h>
#include <stdbool.h>
#include "user_spi.h"



/* * \brief SERCOM_SPI hardware registers */
/*
#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef struct { // SPI Mode 
  __IO SERCOM_SPI_CTRLA_Type     CTRLA;       //< \brief Offset: 0x00 (R/W 32) SPI Control A 
  __IO SERCOM_SPI_CTRLB_Type     CTRLB;       //< \brief Offset: 0x04 (R/W 32) SPI Control B 
       RoReg8                    Reserved1[0x4];
  __IO SERCOM_SPI_BAUD_Type      BAUD;        //< \brief Offset: 0x0C (R/W  8) SPI Baud Rate 
       RoReg8                    Reserved2[0x7];
  __IO SERCOM_SPI_INTENCLR_Type  INTENCLR;    //< \brief Offset: 0x14 (R/W  8) SPI Interrupt Enable Clear 
       RoReg8                    Reserved3[0x1];
  __IO SERCOM_SPI_INTENSET_Type  INTENSET;    //< \brief Offset: 0x16 (R/W  8) SPI Interrupt Enable Set 
       RoReg8                    Reserved4[0x1];
  __IO SERCOM_SPI_INTFLAG_Type   INTFLAG;     //< \brief Offset: 0x18 (R/W  8) SPI Interrupt Flag Status and Clear 
       RoReg8                    Reserved5[0x1];
  __IO SERCOM_SPI_STATUS_Type    STATUS;      //< \brief Offset: 0x1A (R/W 16) SPI Status 
  __I  SERCOM_SPI_SYNCBUSY_Type  SYNCBUSY;    //< \brief Offset: 0x1C (R/  32) SPI Syncbusy 
       RoReg8                    Reserved6[0x4];
  __IO SERCOM_SPI_ADDR_Type      ADDR;        //< \brief Offset: 0x24 (R/W 32) SPI Address 
  __IO SERCOM_SPI_DATA_Type      DATA;        //< \brief Offset: 0x28 (R/W 32) SPI Data 
       RoReg8                    Reserved7[0x4];
  __IO SERCOM_SPI_DBGCTRL_Type   DBGCTRL;     //< \brief Offset: 0x30 (R/W  8) SPI Debug Control 
} SercomSpi;
#endif


#if !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__))
typedef union {
       SercomI2cm                I2CM;        //< \brief Offset: 0x00 I2C Master Mode 
       SercomI2cs                I2CS;        //< \brief Offset: 0x00 I2C Slave Mode 
       SercomSpi                 SPI;         //< \brief Offset: 0x00 SPI Mode 
       SercomUsart               USART;       //< \brief Offset: 0x00 USART Mode 
} Sercom;
#endif // !(defined(__ASSEMBLY__) || defined(__IAR_SYSTEMS_ASM__)) 


typedef union {
  struct {
    uint32_t SWRST:1;          //!< bit:      0  Software Reset                     
    uint32_t ENABLE:1;         //!< bit:      1  Enable                             
    uint32_t MODE:3;           //!< bit:  2.. 4  Operating Mode                     
    uint32_t :2;               //!< bit:  5.. 6  Reserved                           
    uint32_t RUNSTDBY:1;       //!< bit:      7  Run during Standby                 
    uint32_t IBON:1;           //!< bit:      8  Immediate Buffer Overflow Notification 
    uint32_t :7;               //!< bit:  9..15  Reserved                           
    uint32_t DOPO:2;           //!< bit: 16..17  Data Out Pinout                    
    uint32_t :2;               //!< bit: 18..19  Reserved                           
    uint32_t DIPO:2;           //!< bit: 20..21  Data In Pinout                     
    uint32_t :2;               //!< bit: 22..23  Reserved                           
    uint32_t FORM:4;           //!< bit: 24..27  Frame Format                       
    uint32_t CPHA:1;           //!< bit:     28  Clock Phase                        
    uint32_t CPOL:1;           //!< bit:     29  Clock Polarity                     
    uint32_t DORD:1;           //!< bit:     30  Data Order                         
    uint32_t :1;               //!< bit:     31  Reserved                           
  } bit;                       //!< Structure used for bit  access                  
  uint32_t reg;                //!< Type      used for register access              
} SERCOM_SPI_CTRLA_Type;

Sercom *sercom->SPI.CTRLA

typedef union {
  struct {
    uint32_t CHSIZE:3;         //!< bit:  0.. 2  Character Size                     
    uint32_t :3;               //!< bit:  3.. 5  Reserved                           
    uint32_t PLOADEN:1;        //!< bit:      6  Data Preload Enable                
    uint32_t :2;               //!< bit:  7.. 8  Reserved                           
    uint32_t SSDE:1;           //!< bit:      9  Slave Select Low Detect Enable     
    uint32_t :3;               //!< bit: 10..12  Reserved                           
    uint32_t MSSEN:1;          //!< bit:     13  Master Slave Select Enable         
    uint32_t AMODE:2;          //!< bit: 14..15  Address Mode                       
    uint32_t :1;               //!< bit:     16  Reserved                           
    uint32_t RXEN:1;           //!< bit:     17  Receiver Enable                    
    uint32_t :14;              //!< bit: 18..31  Reserved                           
  } bit;                       //!< Structure used for bit  access                  
  uint32_t reg;                //!< Type      used for register access              
} SERCOM_SPI_CTRLB_Type;
Sercom *sercom->SPI.CTRLB



typedef union {
  struct {
    uint8_t  BAUD:8;           //!< bit:  0.. 7  Baud Rate Value                    
  } bit;                       //!< Structure used for bit  access                  
  uint8_t reg;                 //!< Type      used for register access              
} SERCOM_SPI_BAUD_Type;
Sercom *sercom->SPI.BAUD







typedef union {
  struct {
    uint32_t DATA:9;           //!< bit:  0.. 8  Data Value                         
    uint32_t :23;              //!< bit:  9..31  Reserved                           
  } bit;                       //!< Structure used for bit  access                  
  uint32_t reg;                //!< Type      used for register access              
} SERCOM_SPI_DATA_Type;
Sercom *sercom->SPI.DATA

typedef union {
  struct {
    uint8_t  DRE:1;            //!< bit:      0  Data Register Empty Interrupt      
    uint8_t  TXC:1;            //!< bit:      1  Transmit Complete Interrupt        
    uint8_t  RXC:1;            //!< bit:      2  Receive Complete Interrupt         
    uint8_t  SSL:1;            //!< bit:      3  Slave Select Low Interrupt Flag    
    uint8_t  :3;               //!< bit:  4.. 6  Reserved                           
    uint8_t  ERROR:1;          //!< bit:      7  Combined Error Interrupt           
  } bit;                       //!< Structure used for bit  access                  
  uint8_t reg;                 //!< Type      used for register access              
} SERCOM_SPI_INTFLAG_Type;
Sercom *sercom->SPI.INTFLAG
Sercom *sercom->SPI.INTENSET
Sercom *sercom->SPI.INTENCLR

typedef union {
  struct {
    uint16_t :2;               //!< bit:  0.. 1  Reserved                           
    uint16_t BUFOVF:1;         //!< bit:      2  Buffer Overflow                    
    uint16_t :13;              //!< bit:  3..15  Reserved                           
  } bit;                       //!< Structure used for bit  access                  
  uint16_t reg;                //!< Type      used for register access              
} SERCOM_SPI_STATUS_Type;
Sercom *sercom->SPI.STATUS


*/







//SPI_CTRLA_DIPO[1:0]
//SPI_CTRLA_DOPO[1:0]
//SPI_CTRLA_MODE[2:0]










bool SercomSPI_Enable( Sercom *sercom )
{
	sercom->SPI.CTRLA.reg |= SPI_CTRLA_ENABLE;
	return true;
}


bool SercomSPI_Disable( Sercom *sercom )
{
	sercom->SPI.CTRLA.reg = (sercom->SPI.CTRLA.reg & (~SPI_CTRLA_ENABLE));
	return true;
}


bool SercomSPI_Init( Sercom *sercom, unsigned long ctrl_a, unsigned long ctrl_b, unsigned char baud )
{
//	sercom->SPI.CTRLA.reg = (sercom->SPI.CTRLA.reg & (~SPI_CTRLA_ENABLE));
	sercom->SPI.CTRLA.reg = (SPI_CTRLA_MODE00|SPI_CTRLA_PINMODE00|SPI_CTRLA_MSB_FIRST|SPI_CTRLA_FORM_SPI|SPI_CTRLA_MASTER);
	sercom->SPI.CTRLB.reg = (SPI_CTRLB_CHSIZE8BIT|SPI_CTRLB_RXEN);
	sercom->SPI.BAUD.reg = baud;
	return true;
}


bool inline SercomSPI_WriteComplete( Sercom *sercom )
{
	return (sercom->SPI.INTFLAG.reg & SPI_INTFLAG_TXC);
}


bool inline SercomSPI_UnitReady( Sercom *sercom )
{
	return (sercom->SPI.INTFLAG.reg & SPI_INTFLAG_DRE);
}


//bool SercomSPI_TransByte( Sercom *sercom, unsigned short tx_data, unsigned short *rx_data )
unsigned char SercomSPI_TransByte( Sercom *sercom, unsigned short tx_data )
{
	unsigned char rx;
	//while( !SercomSPI_UnitReady(sercom) )
	{
	};

	sercom->SPI.DATA.reg = (tx_data & SPI_DATA_MASK);


	while( !SercomSPI_WriteComplete( sercom ) )
	{
	};

	rx = (sercom->SPI.DATA.reg & 0xFF);//SPI_DATA_MASK;

	return rx;
}






























