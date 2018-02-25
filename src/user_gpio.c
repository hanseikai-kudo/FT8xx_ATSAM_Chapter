

#include <samd21g18a.h>
#include "stdio.h"
#include "user_gpio.h"



/** \brief PortGroup hardware registers */
/*
typedef union {
  struct {
    uint32_t PINMASK:16;       //!< bit:  0..15  Pin Mask for Multiple Pin Configuration 
    uint32_t PMUXEN:1;         //!< bit:     16  Peripheral Multiplexer Enable      
    uint32_t INEN:1;           //!< bit:     17  Input Enable                       
    uint32_t PULLEN:1;         //!< bit:     18  Pull Enable                        
    uint32_t :3;               //!< bit: 19..21  Reserved                           
    uint32_t DRVSTR:1;         //!< bit:     22  Output Driver Strength Selection   
    uint32_t :1;               //!< bit:     23  Reserved                           
    uint32_t PMUX:4;           //!< bit: 24..27  Peripheral Multiplexing            
    uint32_t WRPMUX:1;         //!< bit:     28  Write PMUX                         
    uint32_t :1;               //!< bit:     29  Reserved                           
    uint32_t WRPINCFG:1;       //!< bit:     30  Write PINCFG                       
    uint32_t HWSEL:1;          //!< bit:     31  Half-Word Select                   
  } bit;                       //!< Structure used for bit  access                  
  uint32_t reg;                //!< Type      used for register access              
} PORT_WRCONFIG_Type;

typedef union {
  struct {
    uint32_t DIR:32;           //!< bit:  0..31  Port Data Direction                
  } bit;                       //!< Structure used for bit  access                  
  uint32_t reg;                //!< Type      used for register access              
} PORT_DIR_Type;

typedef struct {
  __IO PORT_DIR_Type             DIR;         //< \brief Offset: 0x00 (R/W 32) Data Direction 
  __IO PORT_DIRCLR_Type          DIRCLR;      //< \brief Offset: 0x04 (R/W 32) Data Direction Clear 
  __IO PORT_DIRSET_Type          DIRSET;      //< \brief Offset: 0x08 (R/W 32) Data Direction Set 
  __IO PORT_DIRTGL_Type          DIRTGL;      //< \brief Offset: 0x0C (R/W 32) Data Direction Toggle 
  __IO PORT_OUT_Type             OUT;         //< \brief Offset: 0x10 (R/W 32) Data Output Value 
  __IO PORT_OUTCLR_Type          OUTCLR;      //< \brief Offset: 0x14 (R/W 32) Data Output Value Clear 
  __IO PORT_OUTSET_Type          OUTSET;      //< \brief Offset: 0x18 (R/W 32) Data Output Value Set 
  __IO PORT_OUTTGL_Type          OUTTGL;      //< \brief Offset: 0x1C (R/W 32) Data Output Value Toggle 
  __I  PORT_IN_Type              IN;          //< \brief Offset: 0x20 (R/  32) Data Input Value 
  __IO PORT_CTRL_Type            CTRL;        //< \brief Offset: 0x24 (R/W 32) Control 
  __O  PORT_WRCONFIG_Type        WRCONFIG;    //< \brief Offset: 0x28 ( /W 32) Write Configuration 
       RoReg8                    Reserved1[0x4];
  __IO PORT_PMUX_Type            PMUX[16];    //< \brief Offset: 0x30 (R/W  8) Peripheral Multiplexing n
  __IO PORT_PINCFG_Type          PINCFG[32];  //< \brief Offset: 0x40 (R/W  8) Pin Configuration n 
       RoReg8                    Reserved2[0x20];
} PortGroup;
*/



/*
#define	USE_GPIO_INPUT_DISABLE	0x00
#define	USE_GPIO_INPUT_ENABLE	0x02

#define	USE_GPIO_PULL_DISABL	E	0x00
#define	USE_GPIO_PULL_ENABLE		0x04

#define	USE_GPIO_DIRECTION_INPUT	0x00
#define	USE_GPIO_DIRECTION_OUTPUT	0x01

#define	USE_GPIO_DRIVE_NORMAL		0x00
#define	USE_GPIO_DRIVE_STRONG		0x40

#define	USE_GPIO_DISCONNECT			0x00
#define	USE_GPIO_INPUT_PORT			(USE_GPIO_INPUT_ENABLE)
#define	USE_GPIO_OUTPUT_PORT		(USE_GPIO_DIRECTION_OUTPUT)

#define	USE_GPIO_INPUT_WITH_PULL	(USE_GPIO_INPUT_ENABLE|USE_GPIO_PULL_ENABLE)
#define	USE_GPIO_BIDIRECTION_PORT	(USE_GPIO_INPUT_ENABLE|USE_GPIO_DIRECTION_OUTPUT)
#define	USE_GPIO_FORCE				(USE_GPIO_PULL_ENABLE)
*/







/*
bit7:DIR
bit6:INEN
bit5:PULLEN
bit4:DRIVE
bit0:outport
*/







/*
    uint32_t HWSEL:1;          //!< bit:     31  Half-Word Select                   
    uint32_t PINMASK:16;       //!< bit:  0..15  Pin Mask for Multiple Pin Configuration 

    uint32_t WRPMUX:1;         //!< bit:     28  Write PMUX                         
    uint32_t PMUXEN:1;         //!< bit:     16  Peripheral Multiplexer Enable      
    uint32_t PMUX:4;           //!< bit: 24..27  Peripheral Multiplexing            

    uint32_t WRPINCFG:1;       //!< bit:     30  Write PINCFG                       
    uint32_t INEN:1;           //!< bit:     17  Input Enable                       
    uint32_t PULLEN:1;         //!< bit:     18  Pull Enable                        
    uint32_t DRVSTR:1;         //!< bit:     22  Output Driver Strength Selection   
*/










/*
GPIO_INIT_TABLE test[]=
{
	{ PORT_PA00, USE_GPIO_PORT, USE_GPIO_OUTPUT,0 },
	{ PORT_PA01, USE_GPIO_PORT, USE_GPIO_OUTPUT,0 },
	{ PORT_PA02, USE_GPIO_PORT, USE_GPIO_OUTPUT,0 },
	{ PORT_PA03, USE_GPIO_PORT, USE_GPIO_OUTPUT,0 },


	{ PORT_PA04, USE_GPIO_FUNC, USE_GPIO_OUTPUT, PA04_USE_SERCOM0_PAD_0 },
	{ PORT_PA05, USE_GPIO_FUNC, USE_GPIO_OUTPUT, PA05_USE_SERCOM0_PAD_1 },
	{ PORT_PA06, USE_GPIO_FUNC, USE_GPIO_OUTPUT, PA06_USE_SERCOM0_PAD_2 },
	{ PORT_PA07, USE_GPIO_FUNC, USE_GPIO_OUTPUT, PA07_USE_SERCOM0_PAD_3 },
	{ PORT_ENDMARK,0,0},
};
*/




#define PORTA             PORT->Group[0]
#define PORTB             PORT->Group[1]
#define PORTC             PORT->Group[2]
#define PORTD             PORT->Group[3]



static PortGroup *GetPortOffset(GPIO port)
{
  PortGroup *ptr=NULL;

  if( port & GPIO_BITIGNORE )
  {
    return NULL;
  } 

  switch( port & GPIO_PORTMASK )
  {
    case PORTA_GROUP://		0x00
      ptr = &PORTA;
      break;
    case PORTB_GROUP://		0x40
      ptr = &PORTB;
      break;
    case PORTC_GROUP://		0x60
      ptr = &PORTC;
      break;
    case PORTD_GROUP://		0x80
      ptr = &PORTD;
      break;
    default:
      break;
  }
  return ptr;
}


int GpioPortInit( GPIO_INIT_TABLE *tbl )
{
	unsigned long value = 0;

	unsigned short low = 0;
	unsigned short hi = 0;

  PortGroup *ptr;
  
//	gpio bit position [15..0]
//	port->WRCONFIG.bit.PINMASK = (0x00000001<<(tbl->port & 0x0F));
//	port->WRCONFIG.bit.HWSEL = ((tbl->port & 0x10)==0?0:1);

  if( tbl == NULL )return -1;

  ptr = GetPortOffset( tbl->port );
  if( ptr == NULL )return -1;



	value = (0x00000001<<(tbl->port & GPIO_BITMASK));
	low = (value & 0x0000FFFF);
	hi = ((value & 0xFFFF0000)>>16);
	value = 0;


	//gpio bit position [15..0]
//	value |= (0x00000001<<(tbl->port & 0x0F));
	//gpio bit position bit * 32 + pos
//	value |= ((tbl->port & 0x10)==0?0x00000000:WRCONFIG_HWSEL);



	if( tbl->mode == USE_GPIO_FUNC )
	{
//		use function
//		port->WRCONFIG.bit.PMUXEN = tbl->func;
//		port->WRCONFIG.bit.PMUXEN = 1;
//		port->WRCONFIG.bit.WRPMUX = 1;
		value |= (((unsigned long)(tbl->func))<<24) | WRCONFIG_PMUXEN | WRCONFIG_WRPMUX | WRCONFIG_WRPINCFG;

	}
	else
	{
//		use gpio
//		port->WRCONFIG.bit.INEN = ((tbl->config & 0x40)==0 ? 0:1) 
//		port->WRCONFIG.bit.PULLEN = ((tbl->config & 0x20)==0 ? 0:1) 
//		port->WRCONFIG.bit.DRIVE = ((tbl->config & 0x10)==0 ? 0:1) 
//		port->WRCONFIG.bit.WRPINCFG = 1;

		value |= ((tbl->config & USE_GPIO_INPUT)==0 ? 0x00000000:WRCONFIG_INEN) 
				| ((tbl->config & USE_GPIO_FORCE_PULL)==0 ? 0x00000000:WRCONFIG_PULLEN)
				| ((tbl->config & USE_GPIO_DRIVE_STRONG)==0 ? 0x00000000:WRCONFIG_DRVSTR); 
	}

	ptr->DIR.reg |= ( ((tbl->config & 0x80)==0?0x00000000:0x00000001) << (tbl->port & GPIO_BITMASK) );

//	port->WRCONFIG.reg = value;



	value |= low ;
	ptr->WRCONFIG.reg = (value | low);

	ptr->WRCONFIG.reg = (value | hi | WRCONFIG_HWSEL);


  return 0;
}





int GpioBitOut( GPIO port,unsigned char bit )
{
	PortGroup *ptr;
  unsigned long value;

	ptr = GetPortOffset( port );
	if( ptr == NULL )return -1;

  value = ((bit & 0x01) == 0 ? 0x00000000 : 0x00000001);
  value = ( value << (port & GPIO_BITMASK) );

	ptr->OUT.bit.OUT = value;
  return 0;
}

int GpioBitIn( GPIO port )
{
	PortGroup *ptr;
  unsigned long value;

	ptr = GetPortOffset( port );
	if( ptr == NULL )return -1;

  value = ptr->IN.bit.IN & (0x00000001<<(port & GPIO_BITMASK));

  return (value == 0 ? 0:1);
}










