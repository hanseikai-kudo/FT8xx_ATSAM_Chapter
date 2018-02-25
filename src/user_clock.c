#include <asf.h>
#include <samd21g18a.h>
#include "user_gpio.h"
#include "user_spi.h"
#include "user_clock.h"










GCLKGENCTRL GclkCtrlTbl[9]=
{
	{	USE_GCLKGEN00,GCLK_SRC_OSC8M,		GCLK_CTRL_ENABLE	,1},
	{	USE_GCLKGEN01,GCLK_SRC_DFLL48M,		GCLK_CTRL_ENABLE	,1},
	{	USE_GCLKGEN02,GCLK_SRC_OSCULP32K,	GCLK_CTRL_ENABLE	,1},
	{	USE_GCLKGEN03,GCLK_SRC_XOSC,		0	,1},
	{	USE_GCLKGEN04,GCLK_SRC_XOSC,		0	,1},
	{	USE_GCLKGEN05,GCLK_SRC_XOSC,		0	,1},
	{	USE_GCLKGEN06,GCLK_SRC_XOSC,		0	,1},
	{	USE_GCLKGEN07,GCLK_SRC_XOSC,		0	,1},
	{	USE_GCLKGEN08,GCLK_SRC_XOSC,		0	,1},
};




MduleClock ModuleClkTbl[24]=
{
	{	MODULE_CLK_DFLL48M_REF,	USE_GCLKGEN00,	USE_CLKCTRL_CLKEN	},
	{	MODULE_CLK_DPLL,		USE_GCLKGEN00,	0	},
	{	MODULE_CLK_DPLL_32K,	USE_GCLKGEN00,	0	},
	{	MODULE_CLK_WDT,			USE_GCLKGEN02,	0	},
	{	MODULE_CLK_RTC,			USE_GCLKGEN00,	0	},
	{	MODULE_CLK_EIC,			USE_GCLKGEN00,	0	},
	{	MODULE_CLK_USB,			USE_GCLKGEN00,	0	},
	{	MODULE_CLK_EVSYS_CH00,	USE_GCLKGEN00,	0	},
	{	MODULE_CLK_EVSYS_CH01,	USE_GCLKGEN00,	0	},
	{	MODULE_CLK_EVSYS_CH02,	USE_GCLKGEN00,	0	},
	{	MODULE_CLK_EVSYS_CH03,	USE_GCLKGEN00,	0	},
	{	MODULE_CLK_EVSYS_CH04,	USE_GCLKGEN00,	0	},
	{	MODULE_CLK_EVSYS_CH05,	USE_GCLKGEN00,	0	},
	{	MODULE_CLK_SERCOMX,		USE_GCLKGEN00,	USE_CLKCTRL_CLKEN	},
	{	MODULE_CLK_SERCOM0,		USE_GCLKGEN00,	USE_CLKCTRL_CLKEN	},
	{	MODULE_CLK_SERCOM1,		USE_GCLKGEN00,	USE_CLKCTRL_CLKEN	},
	{	MODULE_CLK_SERCOM2,		USE_GCLKGEN01,	USE_CLKCTRL_CLKEN	},
	{	MODULE_CLK_SERCOM3,		USE_GCLKGEN01,	USE_CLKCTRL_CLKEN	},
	{	MODULE_CLK_TCC_0_1,		USE_GCLKGEN00,	0	},
	{	MODULE_CLK_TCC_2_3,		USE_GCLKGEN00,	0	},
	{	MODULE_CLK_ADC,			USE_GCLKGEN00,	0	},
	{	MODULE_CLK_AC_DIG,		USE_GCLKGEN00,	0	},
	{	MODULE_CLK_AC_ANA,		USE_GCLKGEN00,	0	},
	{	MODULE_CLK_DAC,			USE_GCLKGEN00,	0	},
	{	MODULE_CLK_PTC,			USE_GCLKGEN00,	0	},
};

















static inline bool system_gclk_is_syncing(void)
{
	if (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY){
		return true;
	}

	return false;
}

/*
typedef struct
{
	GCLKGEN id;
	GCLKSRC src;
	unsigned long flag;
	unsigned short div;
}GCLKGENCTRL;
*/

void GClkGenInit( GCLKGENCTRL *tbl )
{
	unsigned long genctrl = 0;
	unsigned long gendiv = 0;

	genctrl = ((tbl->src << USE_GCLKSRC_SHIFT) | tbl->id) | tbl->flag ;
	gendiv = ((tbl->div<<USE_GCLKDIV_SHIFT)|tbl->id);


	while (system_gclk_is_syncing()) {
		/* Wait for synchronization */
	};

	system_interrupt_enter_critical_section();

	GCLK->GENDIV.reg = gendiv;

	while (system_gclk_is_syncing()) {
		/* Wait for synchronization */
	};
	GCLK->GENCTRL.reg = genctrl;

	system_interrupt_leave_critical_section();
}


/*
typedef struct MODULECLOCK
{		
	MODULE_CLK_ID id;
	GCLKGEN src;
	unsigned short flag;
}MduleClock;
*/


void ModuleClkInit( MduleClock *tbl )
{
	unsigned short clkctrl=0;

	clkctrl = ((tbl->src<<USE_GCLKGEN_SHIFT) | tbl->id) | tbl->flag;

	system_interrupt_enter_critical_section();

	GCLK->CLKCTRL.reg = clkctrl;

	system_interrupt_leave_critical_section();
}




void UserClockInit(void)
{
	int i;
	for( i=0;i<9;i++ )
	{
		GClkGenInit( &GclkCtrlTbl[i] );
	}


	for( i=0;i<24;i++ )
	{
		ModuleClkInit( &ModuleClkTbl[i] );
	}
}
