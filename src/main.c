/*
*
*
*/
#include <asf.h>
#include <samd21g18a.h>
#include <stdlib.h>
#include "user_gpio.h"
#include "user_spi.h"
#include "user_clock.h"
#include "def.h"
#include "ft8xx.h"

#include <math.h>




#define CHAPTER_6A 0
#define CHAPTER_6B 0
#define CHAPTER_6C 0
#define CHAPTER_7A 0
#define CHAPTER_7B 0
#define CHAPTER_8A 0
#define CHAPTER_8B 1



#define SIZE_OF_FLOG_16BIT_96X96 (96*2*96)
extern const unsigned char flog_16bit_96x96[];

#define SIZE_OF_MAJI_TEN (0x13540)
extern const unsigned char maji_ten_480_272[];


GPIO_INIT_TABLE test[]=
{
	{ GPIO_PA20, USE_GPIO_FUNC, USE_GPIO_OUTPUT, PA20_USE_SERCOM3_PAD_2 },//nCS
	{ GPIO_PA21, USE_GPIO_FUNC, USE_GPIO_INPUT , PA21_USE_SERCOM3_PAD_3 },//MISO
	{ GPIO_PA22, USE_GPIO_FUNC, USE_GPIO_OUTPUT, PA22_USE_SERCOM3_PAD_0 },//MOSI
	{ GPIO_PA23, USE_GPIO_FUNC, USE_GPIO_OUTPUT, PA23_USE_SERCOM3_PAD_1 },//SCK

	{ GPIO_ENDMARK,0,0,0},
};



GPIO_INIT_TABLE test2[]=
{
	{ GPIO_PA12, USE_GPIO_FUNC, USE_GPIO_OUTPUT,PA12_USE_SERCOM2_PAD_0 },
	{ GPIO_PA13, USE_GPIO_FUNC, USE_GPIO_OUTPUT,PA13_USE_SERCOM2_PAD_1 },
	{ GPIO_PA14, USE_GPIO_PORT, USE_GPIO_OUTPUT,0 },
	{ GPIO_PA15, USE_GPIO_FUNC, USE_GPIO_INPUT ,PA15_USE_SERCOM2_PAD_3 },
	{ GPIO_ENDMARK,0,0,0},
};


volatile UserWait( unsigned long time )
{
	while( time-- )
	{
		
	};
}


void configure_dfll_open_loop(void)
{
    struct system_clock_source_dfll_config config_dfll;
    system_clock_source_dfll_get_config_defaults(&config_dfll);
    system_clock_source_dfll_set_config(&config_dfll);
}

void ClockInitTest(void)
{
	/* Enable the external 32KHz oscillator */
	enum status_code osc32k_status = system_clock_source_enable(SYSTEM_CLOCK_SOURCE_XOSC32K);

	if (osc32k_status != STATUS_OK)
	{
	/* Error enabling the clock source */
	}

	/* Configure the DFLL in open loop mode using default values */
	configure_dfll_open_loop();
	/* Enable the DFLL oscillator */

	enum status_code dfll_status = system_clock_source_enable(SYSTEM_CLOCK_SOURCE_DFLL);
	if(dfll_status != STATUS_OK)
	{
	/* Error enabling the clock source */
	}
	/* Configure flash wait states before switching to high frequency clock */
	system_flash_set_waitstates(2);
	/* Change system clock to DFLL */

	struct system_gclk_gen_config config_gclock_gen;

	system_gclk_gen_get_config_defaults(&config_gclock_gen);
	config_gclock_gen.source_clock    = SYSTEM_CLOCK_SOURCE_DFLL;
	config_gclock_gen.division_factor = 1;
	system_gclk_gen_set_config(GCLK_GENERATOR_0, &config_gclock_gen);
}








int main (void)
{
	unsigned int cmdOffset = 0x0000;
	unsigned int cmdBufferRd = 0x0000;
	unsigned int cmdBufferWr = 0x0000;
	
	int i,j,k;





	system_clock_init();
	system_gclk_init();

	ClockInitTest();

	UserClockInit();

	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 <<  (2 + PM_APBCMASK_SERCOM0_Pos));
	system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 <<  (3 + PM_APBCMASK_SERCOM0_Pos));

	GpioPortInit( &test2[0] );
	GpioPortInit( &test2[1] );
	GpioPortInit( &test2[2] );
	GpioPortInit( &test2[3] );

	SercomSPI_Init( SERCOM2, (SPI_CTRLA_MODE00|SPI_CTRLA_PINMODE00|SPI_CTRLA_MSB_FIRST|SPI_CTRLA_FORM_SPI), (SPI_CTRLB_CHSIZE8BIT|SPI_CTRLB_RXEN),0 );
	SercomSPI_Enable( SERCOM2 );



	UserWait(100000);
	UserWait(100000);
	UserWait(100000);
	UserWait(100000);

	HostCommand(HOST_CLKINT,0,0);//send command to "CLKEXT" to FT81X 
	HostCommand(HOST_ACTIVE,0,0);//send host command "ACTIVE" to FT81X 

	UserWait(100000);
	UserWait(100000);
	UserWait(100000);
	UserWait(100000);
	UserWait(100000);
	UserWait(100000);
	UserWait(100000);
	UserWait(100000);

	HostMemWriteByte(REG_CPURESET, 0x01);
	HostMemWriteWord(REG_CMD_READ, 0x0000);
	HostMemWriteWord(REG_CMD_WRITE, 0x0000);
	HostMemWriteByte(REG_CPURESET, 0x00); 

/* Configure display registers - demonstration for WQVGA resolution */ 
	HostMemWriteWord(REG_HSIZE, 480); 
	HostMemWriteWord(REG_HCYCLE, 548); 
	HostMemWriteWord(REG_HOFFSET, 43); 
	HostMemWriteWord(REG_HSYNC0, 0); 
	HostMemWriteWord(REG_HSYNC1, 41); 
	HostMemWriteWord(REG_VSIZE, 272); 
	HostMemWriteWord(REG_VCYCLE, 292); 
	HostMemWriteWord(REG_VOFFSET, 12); 
	HostMemWriteWord(REG_VSYNC0, 0); 
	HostMemWriteWord(REG_VSYNC1, 10); 
	HostMemWriteByte(REG_SWIZZLE, 0); 
	HostMemWriteByte(REG_PCLK_POL, 1); 

	/* write first display list */ 
	HostMemWriteDoubleWord( RAM_DL+0,DL_CLEAR_RGB );
	HostMemWriteDoubleWord( RAM_DL+4,DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG );
	HostMemWriteDoubleWord( RAM_DL+8,DL_DISPLAY );

	HostMemWriteByte(REG_DLSWAP, DLSWAP_FRAME);

	HostMemWriteByte(REG_GPIO_DIR, 0x80); 
	HostMemWriteByte(REG_GPIO, 0x80); 
	HostMemWriteByte(REG_PWM_DUTY, 128); 

	HostMemWriteByte(REG_PCLK, 7);
	HostMemWriteByte(REG_PWM_DUTY, 64);

	do
	{
		cmdBufferRd = HostMemReadWord(REG_CMD_READ);
		cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
	}while (cmdBufferWr != cmdBufferRd);
	cmdOffset = cmdBufferWr;
//--------------------------------------














#if CHAPTER_8B
	signed short move_x,move_y;
	unsigned long offset = 0;
	float f_sin,f_cos,rad;
	float f_deg;

	JpegLoad( 1, &maji_ten_480_272[0], RAM_G+0x00000000, SIZE_OF_MAJI_TEN );

	do
	{
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_DLSTART) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR_RGB | BLUE) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset, VERTEX_FORMAT(0) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_HANDLE(7) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BEGIN_BITMAPS );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SOURCE( RAM_G + 0 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_LAYOUT(RGB565, 480*2, 272) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SIZE(NEAREST, 0, 0, 480, 272) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,VERTEX2F( 0 ,0 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,DL_END );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		f_deg += 5;
		rad = f_deg * 3.14159265359 / 180;
		
		f_sin = sin(rad);
		f_cos = cos(rad);

		move_x = f_sin * 120 + (240-48);
		move_y = f_cos * 80 + (136-48);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BEGIN_BITMAPS );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SOURCE( RAM_G + (move_y)*(480*2)+(move_x*2) ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_LAYOUT(RGB565, 480*2, 272) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SIZE(NEAREST, 0, 0, 96, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,VERTEX2F( move_x-8 ,move_y-8 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,DL_END );
		cmdOffset = incCMDOffset(cmdOffset, 4);


		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_DISPLAY) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_SWAP) );
		cmdOffset = incCMDOffset(cmdOffset, 4);


		HostMemWriteWord(REG_CMD_WRITE, cmdOffset);

		do
		{
			cmdBufferRd = HostMemReadWord(REG_CMD_READ);
			cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
		}while (cmdBufferWr != cmdBufferRd);
		cmdOffset = cmdBufferWr;
	}while(1);

#endif


#if CHAPTER_8A

	JpegLoad( 1, &gear2_jpg_480_272[0], RAM_G+0x00000000, SIZE_OF_GEAR );

	do
	{
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_DLSTART) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR_RGB | BLUE) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset, VERTEX_FORMAT(0) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_HANDLE(7) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BEGIN_BITMAPS );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SOURCE( RAM_G + 0 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_LAYOUT(RGB565, 480*2, 272) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SIZE(NEAREST, 0, 0, 480, 272) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,VERTEX2F( 0 ,0 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,DL_END );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_DISPLAY) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_SWAP) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteWord(REG_CMD_WRITE, cmdOffset);

		do
		{
			cmdBufferRd = HostMemReadWord(REG_CMD_READ);
			cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
		}while (cmdBufferWr != cmdBufferRd);
		cmdOffset = cmdBufferWr;
	}while(1);

#endif





#if CHAPTER_7B

	signed short move_x,move_y;
	unsigned long offset = 0;
	float f_sin,f_cos,rad;
	float f_deg;



	HostMemWrite_Burst( RAM_G+0x00000000, &flog_16bit_96x96[0],(SIZE_OF_FLOG_16BIT_96X96+3)/4*4 );


	do
	{
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_DLSTART) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR_RGB | BLUE) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset, VERTEX_FORMAT(0) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_HANDLE(7) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		f_deg += 5;
		rad = f_deg * 3.14159265359 / 180;
	
		f_sin = sin(rad);
		f_cos = cos(rad);

		move_x = f_sin * 120 + (240-48);
		move_y = f_cos * 120 + (136-48);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BEGIN_BITMAPS );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SOURCE( RAM_G + 0 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_LAYOUT(RGB565, 96*2, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SIZE(NEAREST, 0, 0, 96, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,VERTEX2F( move_x ,move_y ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,DL_END );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_DISPLAY) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_SWAP) );
		cmdOffset = incCMDOffset(cmdOffset, 4);


		HostMemWriteWord(REG_CMD_WRITE, cmdOffset);

		do
		{
			cmdBufferRd = HostMemReadWord(REG_CMD_READ);
			cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
		}while (cmdBufferWr != cmdBufferRd);
		cmdOffset = cmdBufferWr;
	}while(1);

#endif


#if CHAPTER_7A

	HostMemWrite_Burst( RAM_G+0x00000000, &flog_16bit_96x96[0],(SIZE_OF_FLOG_16BIT_96X96+3)/4*4 );

	do
	{
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_DLSTART) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR_RGB | BLUE) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset, VERTEX_FORMAT(0) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_HANDLE(7) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BEGIN_BITMAPS );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SOURCE( RAM_G + 0 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_LAYOUT(RGB565, 96*2, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SIZE(NEAREST, 0, 0, 96, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,VERTEX2F( 240-48 ,136-48 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,DL_END );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_DISPLAY) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_SWAP) );
		cmdOffset = incCMDOffset(cmdOffset, 4);


		HostMemWriteWord(REG_CMD_WRITE, cmdOffset);

		do
		{
			cmdBufferRd = HostMemReadWord(REG_CMD_READ);
			cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
		}while (cmdBufferWr != cmdBufferRd);
		cmdOffset = cmdBufferWr;
	}while(1);

#endif


#if CHAPTER_6C

	signed short move_x,move_y;
	unsigned long offset = 0;
	float f_sin,f_cos,rad;
	float f_deg;

	offset = 0;
	Boxfill_Burst( offset, (96*96), RGB565, 255,255,255 );
	offset += (96*96*2);

	do
	{
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_DLSTART) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR_RGB | BLUE) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset, VERTEX_FORMAT(0) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_HANDLE(7) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		f_deg += 5;
		rad = f_deg * 3.14159265359 / 180;
	
		f_sin = sin(rad);
		f_cos = cos(rad);

		move_x = f_sin * 120 + (240-48);
		move_y = f_cos * 120 + (136-48);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BEGIN_BITMAPS );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SOURCE( RAM_G + 0 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_LAYOUT(RGB565, 96*2, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SIZE(NEAREST, 0, 0, 96, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,VERTEX2F( move_x ,move_y ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,DL_END );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_DISPLAY) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_SWAP) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteWord(REG_CMD_WRITE, cmdOffset);

		do
		{
			cmdBufferRd = HostMemReadWord(REG_CMD_READ);
			cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
		}while (cmdBufferWr != cmdBufferRd);
		cmdOffset = cmdBufferWr;
	}while(1);

#endif

#if CHAPTER_6B
	unsigned long offset = 0;

	offset = 0;
	Boxfill_Burst( offset, (96*96), RGB565, 255,255,255 );
	offset += (96*96*2);
	Boxfill_Burst( offset, (96*96), RGB565, 255,255,0 );
	offset += (96*96*2);
	Boxfill_Burst( offset, (96*96), RGB565, 255,0,255 );
	offset += (96*96*2);
	Boxfill_Burst( offset, (96*96), RGB565, 255,0,0 );
	offset += (96*96*2);
	Boxfill_Burst( offset, (96*96), RGB565, 0,255,255 );
	offset += (96*96*2);
	Boxfill_Burst( offset, (96*96), RGB565, 0,255,0 );
	offset += (96*96*2);
	Boxfill_Burst( offset, (96*96), RGB565, 0,0,255 );
	offset += (96*96*2);
	Boxfill_Burst( offset, (96*96), RGB565, 5,5,5 );

	k=0;	
	do
	{
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_DLSTART) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR_RGB | BLACK) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset, VERTEX_FORMAT(0) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_HANDLE(7) );
		cmdOffset = incCMDOffset(cmdOffset, 4);


		for( i=0;i<3;i++ )
		{
			for( j=0;j<5;j++ )
			{
				HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SOURCE( RAM_G + (96*96*2)* ((k+(i*5+j))%8) ) );
				cmdOffset = incCMDOffset(cmdOffset, 4);

				HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_LAYOUT(RGB565, 96*2, 96) );
				cmdOffset = incCMDOffset(cmdOffset, 4);
				HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SIZE(NEAREST, 0, 0, 96, 96) );
				cmdOffset = incCMDOffset(cmdOffset, 4);

				HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BEGIN_BITMAPS );
				cmdOffset = incCMDOffset(cmdOffset, 4);

				HostMemWriteDoubleWord( RAM_CMD + cmdOffset,VERTEX2F( j*96 , i*96 ) );
				cmdOffset = incCMDOffset(cmdOffset, 4);

				HostMemWriteDoubleWord( RAM_CMD + cmdOffset,DL_END );
				cmdOffset = incCMDOffset(cmdOffset, 4);
			}
		}
		k=k+1;
		k=k%8;

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_DISPLAY) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_SWAP) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteWord(REG_CMD_WRITE, cmdOffset);

		do
		{
			cmdBufferRd = HostMemReadWord(REG_CMD_READ);
			cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
		}while (cmdBufferWr != cmdBufferRd);
		cmdOffset = cmdBufferWr;

		UserWait(100000);
	}while(1);

#endif


#if CHAPTER_6A

	Boxfill_Burst( 0, (96*96), RGB565, 255,255,255 );

	do
	{
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_DLSTART) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR_RGB | BLUE) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_CLEAR | CLR_COL | CLR_STN | CLR_TAG));
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset, VERTEX_FORMAT(0) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_HANDLE(7) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BEGIN_BITMAPS );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SOURCE( RAM_G + 0 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_LAYOUT(RGB565, 96*2, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,BITMAP_SIZE(NEAREST, 0, 0, 96, 96) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,VERTEX2F( 240-48 ,136-48 ) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,DL_END );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(DL_DISPLAY) );
		cmdOffset = incCMDOffset(cmdOffset, 4);
		HostMemWriteDoubleWord( RAM_CMD + cmdOffset,(CMD_SWAP) );
		cmdOffset = incCMDOffset(cmdOffset, 4);

		HostMemWriteWord(REG_CMD_WRITE, cmdOffset);

		do
		{
			cmdBufferRd = HostMemReadWord(REG_CMD_READ);
			cmdBufferWr = HostMemReadWord(REG_CMD_WRITE);
		}while (cmdBufferWr != cmdBufferRd);
		cmdOffset = cmdBufferWr;
	}while(1);

#endif









}
