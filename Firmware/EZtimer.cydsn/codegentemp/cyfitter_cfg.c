/*******************************************************************************
* File Name: cyfitter_cfg.c
* 
* PSoC Creator  3.3
*
* Description:
* This file contains device initialization code.
* Except for the user defined sections in CyClockStartupError(), this file should not be modified.
* This file is automatically generated by PSoC Creator.
*
********************************************************************************
* Copyright (c) 2007-2015 Cypress Semiconductor.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
********************************************************************************/

#include <string.h>
#include "cytypes.h"
#include "cydevice_trm.h"
#include "cyfitter.h"
#include "CyLib.h"
#include "CyLFClk.h"
#include "cyfitter_cfg.h"

#define CY_NEED_CYCLOCKSTARTUPERROR 1


#if defined(__GNUC__) || defined(__ARMCC_VERSION)
    #define CYPACKED 
    #define CYPACKED_ATTR __attribute__ ((packed))
    #define CYALIGNED __attribute__ ((aligned))
    #define CY_CFG_UNUSED __attribute__ ((unused))
    #define CY_CFG_SECTION __attribute__ ((section(".psocinit")))
    
    #if defined(__ARMCC_VERSION)
        #define CY_CFG_MEMORY_BARRIER() __memory_changed()
    #else
        #define CY_CFG_MEMORY_BARRIER() __sync_synchronize()
    #endif
    
#elif defined(__ICCARM__)
    #include <intrinsics.h>

    #define CYPACKED __packed
    #define CYPACKED_ATTR 
    #define CYALIGNED _Pragma("data_alignment=4")
    #define CY_CFG_UNUSED _Pragma("diag_suppress=Pe177")
    #define CY_CFG_SECTION _Pragma("location=\".psocinit\"")
    
    #define CY_CFG_MEMORY_BARRIER() __DMB()
    
#else
    #error Unsupported toolchain
#endif


CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n);
CY_CFG_UNUSED
static void CYMEMZERO(void *s, size_t n)
{
	(void)memset(s, 0, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPY(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n);
CY_CFG_UNUSED
static void CYCONFIGCPYCODE(void *dest, const void *src, size_t n)
{
	(void)memcpy(dest, src, n);
}



/* Clock startup error codes                                                   */
#define CYCLOCKSTART_NO_ERROR    0u
#define CYCLOCKSTART_XTAL_ERROR  1u
#define CYCLOCKSTART_32KHZ_ERROR 2u
#define CYCLOCKSTART_PLL_ERROR   3u

#ifdef CY_NEED_CYCLOCKSTARTUPERROR
/*******************************************************************************
* Function Name: CyClockStartupError
********************************************************************************
* Summary:
*  If an error is encountered during clock configuration (crystal startup error,
*  PLL lock error, etc.), the system will end up here.  Unless reimplemented by
*  the customer, this function will stop in an infinite loop.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode);
CY_CFG_UNUSED
static void CyClockStartupError(uint8 errorCode)
{
    /* To remove the compiler warning if errorCode not used.                */
    errorCode = errorCode;

    /* `#START CyClockStartupError` */

    /* If we have a clock startup error (bad MHz crystal, PLL lock, etc.),  */
    /* we will end up here to allow the customer to implement something to  */
    /* deal with the clock condition.                                       */

    /* `#END` */

    /* If nothing else, stop here since the clocks have not started         */
    /* correctly.                                                           */
    while(1) {}
}
#endif

#define CY_CFG_BASE_ADDR_COUNT 8u
CYPACKED typedef struct
{
	uint8 offset;
	uint8 value;
} CYPACKED_ATTR cy_cfg_addrvalue_t;



/*******************************************************************************
* Function Name: cfg_write_bytes32
********************************************************************************
* Summary:
*  This function is used for setting up the chip configuration areas that
*  contain relatively sparse data.
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[]);
static void cfg_write_bytes32(const uint32 addr_table[], const cy_cfg_addrvalue_t data_table[])
{
	/* For 32-bit little-endian architectures */
	uint32 i, j = 0u;
	for (i = 0u; i < CY_CFG_BASE_ADDR_COUNT; i++)
	{
		uint32 baseAddr = addr_table[i];
		uint8 count = (uint8)baseAddr;
		baseAddr &= 0xFFFFFF00u;
		while (count != 0u)
		{
			CY_SET_XTND_REG8((void CYFAR *)(baseAddr + data_table[j].offset), data_table[j].value);
			j++;
			count--;
		}
	}
}


/*******************************************************************************
* Function Name: ClockSetup
********************************************************************************
*
* Summary:
*   Performs the initialization of all of the clocks in the device based on the
*   settings in the Clock tab of the DWR.  This includes enabling the requested
*   clocks and setting the necessary dividers to produce the desired frequency. 
*
* Parameters:
*   void
*
* Return:
*   void
*
*******************************************************************************/
static void ClockSetup(void);
CY_CFG_SECTION
static void ClockSetup(void)
{
	uint8 ecoStatus;

	/* Set Flash Cycles based on max possible frequency in case a glitch occurs during ClockSetup(). */
	CY_SET_REG32((void CYXDATA *)(CYREG_CPUSS_FLASH_CTL), (0x0012u));

	/* Start the WCO */
	CySysClkWcoStart();
	CyDelayCycles(12000000u); /* WCO may take up to 500ms to start */
	(void)CySysClkWcoSetPowerMode(CY_SYS_CLK_WCO_LPM);    /* Switch to the low power mode */

	/* Setup and trim IMO based on desired frequency. */
	CySysClkWriteImoFreq(12u);
	/* CYDEV_CLK_ILO_CONFIG Starting address: CYDEV_CLK_ILO_CONFIG */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_ILO_CONFIG), 0x80000006u);

	/* CYDEV_WDT_CONFIG Starting address: CYDEV_WDT_CONFIG */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_WDT_CONFIG), 0x40000000u);


	/* Enable fast start mode for XO */
	CY_SET_REG32((void*)CYREG_BLE_BLERD_BB_XO, CY_GET_REG32((void*)CYREG_BLE_BLERD_BB_XO) | (uint32)0x02u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_BLE_BLERD_BB_XO_CAPTRIM), 0x00003E2Du);
	/* Disable Crystal Stable Interrupt before enabling ECO */
	CY_SET_REG32((void*)CYREG_BLE_BLESS_LL_DSM_CTRL, CY_GET_REG32((void*)CYREG_BLE_BLESS_LL_DSM_CTRL) & (~(uint32)0x08u));
	/* Start the ECO and check status since it is needed for HFCLK */
	ecoStatus = CySysClkEcoStart(2000u);
	if (ecoStatus != CYRET_SUCCESS)
	{
		CyClockStartupError(CYCLOCKSTART_XTAL_ERROR);
	}
	CyDelayUs(1500u); /* Wait to stabalize */

	/* Setup phase aligned clocks */
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL2, 0x00001700u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF42u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL3, 0x00095F00u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF43u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL1, 0x00001000u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF41u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_16_CTL0, 0x00000100u);
	CY_SET_REG32((void *)CYREG_PERI_DIV_CMD, 0x8000FF40u);

	/* CYDEV_CLK_SELECT Starting address: CYDEV_CLK_SELECT */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_SELECT), 0x00000002u);

	/* CYDEV_CLK_IMO_CONFIG Starting address: CYDEV_CLK_IMO_CONFIG */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_CLK_IMO_CONFIG), 0x80000000u);

	/* CYDEV_PERI_PCLK_CTL11 Starting address: CYDEV_PERI_PCLK_CTL11 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL11), 0x00000042u);

	/* CYDEV_PERI_PCLK_CTL8 Starting address: CYDEV_PERI_PCLK_CTL8 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL8), 0x00000042u);

	/* CYDEV_PERI_PCLK_CTL7 Starting address: CYDEV_PERI_PCLK_CTL7 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL7), 0x00000043u);

	/* CYDEV_PERI_PCLK_CTL2 Starting address: CYDEV_PERI_PCLK_CTL2 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL2), 0x00000041u);

	/* CYDEV_PERI_PCLK_CTL1 Starting address: CYDEV_PERI_PCLK_CTL1 */
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_PERI_PCLK_CTL1), 0x00000040u);

	(void)CyIntSetVector(8u, &CySysWdtIsr);
	CyIntEnable(8u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_WDT_CONFIG), 0x40000000u);
	/* Set Flash Cycles based on newly configured 24.00MHz HFCLK. */
	CY_SET_REG32((void CYXDATA *)(CYREG_CPUSS_FLASH_CTL), (0x0011u));
}


/* Analog API Functions */


/*******************************************************************************
* Function Name: AnalogSetDefault
********************************************************************************
*
* Summary:
*  Sets up the analog portions of the chip to default values based on chip
*  configuration options from the project.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
static void AnalogSetDefault(void);
static void AnalogSetDefault(void)
{
	CY_SET_XTND_REG32((void CYFAR *)CYREG_CTBM0_DFT_CTRL, 0x00000003u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_CTBM1_DFT_CTRL, 0x00000003u);
	CY_SET_XTND_REG32((void CYFAR *)CYREG_PASS_DSAB_DSAB_CTRL, 0x00000000u);
}

#define CY_AMUX_UNUSED CYREG_CM0_ROM_DWT


/*******************************************************************************
* Function Name: cyfitter_cfg
********************************************************************************
* Summary:
*  This function is called by the start-up code for the selected device. It
*  performs all of the necessary device configuration based on the design
*  settings.  This includes settings from the Design Wide Resources (DWR) such
*  as Clocks and Pins as well as any component configuration that is necessary.
*
* Parameters:  
*   void
*
* Return:
*   void
*
*******************************************************************************/
CY_CFG_SECTION
void cyfitter_cfg(void)
{
	/* Disable interrupts by default. Let user enable if/when they want. */
	CyGlobalIntDisable;

	CY_SET_XTND_REG32((void CYFAR *)(CYREG_BLE_BLESS_LL_DSM_CTRL), 0x00000000u);

	{
		static const uint32 CYCODE cy_cfg_addr_table[] = {
			0x400F0002u, /* Base address: 0x400F0000 Count: 2 */
			0x400F3042u, /* Base address: 0x400F3000 Count: 66 */
			0x400F3125u, /* Base address: 0x400F3100 Count: 37 */
			0x400F3241u, /* Base address: 0x400F3200 Count: 65 */
			0x400F3326u, /* Base address: 0x400F3300 Count: 38 */
			0x400F4003u, /* Base address: 0x400F4000 Count: 3 */
			0x400F410Bu, /* Base address: 0x400F4100 Count: 11 */
			0x400F6002u, /* Base address: 0x400F6000 Count: 2 */
		};

		static const cy_cfg_addrvalue_t CYCODE cy_cfg_data_table[] = {
			{0x81u, 0x0Fu},
			{0x83u, 0x0Fu},
			{0x40u, 0x30u},
			{0x41u, 0x02u},
			{0x49u, 0xFFu},
			{0x4Au, 0xFFu},
			{0x4Bu, 0xFFu},
			{0x4Du, 0xA0u},
			{0x4Fu, 0x04u},
			{0x50u, 0x18u},
			{0x5Au, 0x04u},
			{0x5Fu, 0x01u},
			{0x60u, 0x40u},
			{0x62u, 0xC0u},
			{0x64u, 0x40u},
			{0x65u, 0x40u},
			{0x66u, 0xC0u},
			{0x68u, 0xC0u},
			{0x6Au, 0xC0u},
			{0x6Cu, 0xC0u},
			{0x6Eu, 0xC0u},
			{0x83u, 0x0Cu},
			{0x8Au, 0x04u},
			{0x91u, 0x02u},
			{0x96u, 0x0Eu},
			{0x99u, 0x08u},
			{0x9Eu, 0x08u},
			{0xA6u, 0x02u},
			{0xA7u, 0x03u},
			{0xAEu, 0x01u},
			{0xB0u, 0x04u},
			{0xB1u, 0x02u},
			{0xB2u, 0x01u},
			{0xB3u, 0x08u},
			{0xB4u, 0x02u},
			{0xB5u, 0x04u},
			{0xB6u, 0x08u},
			{0xB7u, 0x01u},
			{0xBEu, 0x10u},
			{0xC0u, 0x30u},
			{0xC1u, 0x02u},
			{0xC5u, 0xD0u},
			{0xC6u, 0x20u},
			{0xC7u, 0x51u},
			{0xC8u, 0x3Au},
			{0xC9u, 0xFFu},
			{0xCAu, 0xFFu},
			{0xCBu, 0xFFu},
			{0xCDu, 0xAFu},
			{0xCEu, 0x0Fu},
			{0xCFu, 0x04u},
			{0xD0u, 0x18u},
			{0xD2u, 0x80u},
			{0xD4u, 0x07u},
			{0xD8u, 0x04u},
			{0xD9u, 0x04u},
			{0xDAu, 0x04u},
			{0xDBu, 0x04u},
			{0xDFu, 0x01u},
			{0xE0u, 0x40u},
			{0xE2u, 0xC0u},
			{0xE4u, 0x40u},
			{0xE5u, 0x40u},
			{0xE6u, 0xC0u},
			{0xE8u, 0xC0u},
			{0xEAu, 0xC0u},
			{0xECu, 0xC0u},
			{0xEEu, 0xC0u},
			{0x06u, 0x08u},
			{0x0Du, 0x08u},
			{0x0Fu, 0x02u},
			{0x16u, 0x20u},
			{0x17u, 0x40u},
			{0x1Cu, 0x10u},
			{0x1Du, 0x88u},
			{0x1Eu, 0x08u},
			{0x1Fu, 0x01u},
			{0x24u, 0x40u},
			{0x26u, 0x20u},
			{0x27u, 0x22u},
			{0x2Eu, 0x08u},
			{0x36u, 0x20u},
			{0x37u, 0x02u},
			{0x3Fu, 0x02u},
			{0x40u, 0x10u},
			{0x41u, 0x20u},
			{0x45u, 0x28u},
			{0x4Fu, 0x01u},
			{0x55u, 0x20u},
			{0x56u, 0x28u},
			{0x57u, 0x01u},
			{0x5Cu, 0x40u},
			{0x5Eu, 0x08u},
			{0x5Fu, 0x21u},
			{0x77u, 0x40u},
			{0x83u, 0x01u},
			{0x85u, 0x80u},
			{0xC0u, 0x40u},
			{0xC2u, 0xA0u},
			{0xC4u, 0xA0u},
			{0xCAu, 0x40u},
			{0xCCu, 0xA0u},
			{0xCEu, 0x80u},
			{0xD0u, 0x66u},
			{0xD6u, 0xF0u},
			{0x40u, 0x60u},
			{0x41u, 0x05u},
			{0x49u, 0xFFu},
			{0x4Au, 0xFFu},
			{0x4Bu, 0xFFu},
			{0x4Du, 0xA0u},
			{0x4Fu, 0x04u},
			{0x50u, 0x18u},
			{0x5Au, 0x04u},
			{0x5Fu, 0x01u},
			{0x60u, 0x40u},
			{0x62u, 0xC0u},
			{0x64u, 0x40u},
			{0x65u, 0x40u},
			{0x66u, 0xC0u},
			{0x68u, 0xC0u},
			{0x6Au, 0xC0u},
			{0x6Cu, 0xC0u},
			{0x6Eu, 0xC0u},
			{0x86u, 0x0Eu},
			{0x89u, 0x02u},
			{0x8Du, 0x08u},
			{0x8Eu, 0x02u},
			{0x92u, 0x01u},
			{0x97u, 0x03u},
			{0x9Bu, 0x0Cu},
			{0xA6u, 0x08u},
			{0xAAu, 0x04u},
			{0xB0u, 0x08u},
			{0xB1u, 0x04u},
			{0xB2u, 0x02u},
			{0xB3u, 0x01u},
			{0xB4u, 0x01u},
			{0xB5u, 0x08u},
			{0xB6u, 0x04u},
			{0xB7u, 0x02u},
			{0xBEu, 0x04u},
			{0xC0u, 0x20u},
			{0xC1u, 0x05u},
			{0xC5u, 0xD5u},
			{0xC7u, 0x21u},
			{0xC8u, 0x33u},
			{0xC9u, 0xFFu},
			{0xCAu, 0xFFu},
			{0xCBu, 0xFFu},
			{0xCDu, 0xAFu},
			{0xCEu, 0x0Fu},
			{0xCFu, 0x04u},
			{0xD0u, 0x18u},
			{0xD2u, 0x80u},
			{0xD4u, 0x07u},
			{0xD8u, 0x04u},
			{0xD9u, 0x04u},
			{0xDAu, 0x04u},
			{0xDBu, 0x04u},
			{0xDFu, 0x01u},
			{0xE0u, 0x40u},
			{0xE2u, 0xC0u},
			{0xE4u, 0x40u},
			{0xE5u, 0x40u},
			{0xE6u, 0xC0u},
			{0xE8u, 0xC0u},
			{0xEAu, 0xC0u},
			{0xECu, 0xC0u},
			{0xEEu, 0xC0u},
			{0x00u, 0x04u},
			{0x02u, 0x02u},
			{0x0Au, 0x40u},
			{0x10u, 0x28u},
			{0x18u, 0x05u},
			{0x19u, 0x04u},
			{0x1Bu, 0x11u},
			{0x20u, 0x50u},
			{0x22u, 0x10u},
			{0x23u, 0x02u},
			{0x30u, 0x28u},
			{0x38u, 0x50u},
			{0x40u, 0x04u},
			{0x44u, 0x40u},
			{0x47u, 0x02u},
			{0x48u, 0x20u},
			{0x49u, 0x01u},
			{0x4Au, 0x02u},
			{0x4Eu, 0x02u},
			{0x4Fu, 0x20u},
			{0x50u, 0x08u},
			{0x52u, 0x02u},
			{0x58u, 0x40u},
			{0x59u, 0x05u},
			{0x5Au, 0x20u},
			{0x72u, 0x40u},
			{0x8Bu, 0x01u},
			{0x8Cu, 0x01u},
			{0xC0u, 0x05u},
			{0xC2u, 0x08u},
			{0xC4u, 0x06u},
			{0xCCu, 0x06u},
			{0xCEu, 0x0Cu},
			{0xD0u, 0x04u},
			{0xD2u, 0x38u},
			{0xD6u, 0x0Fu},
			{0xE2u, 0x80u},
			{0xE6u, 0x10u},
			{0x83u, 0x04u},
			{0x87u, 0x10u},
			{0xE6u, 0x60u},
			{0x53u, 0x80u},
			{0x57u, 0x04u},
			{0x5Fu, 0x20u},
			{0x67u, 0x20u},
			{0x8Bu, 0x80u},
			{0x8Fu, 0x20u},
			{0x93u, 0x20u},
			{0xA3u, 0x04u},
			{0xD4u, 0x60u},
			{0xD6u, 0x20u},
			{0xD8u, 0x80u},
			{0x10u, 0x01u},
			{0x11u, 0x01u},
		};



		CYPACKED typedef struct {
			void CYFAR *address;
			uint16 size;
		} CYPACKED_ATTR cfg_memset_t;

		static const cfg_memset_t CYCODE cfg_memset_list [] = {
			/* address, size */
			{(void CYFAR *)(CYDEV_UDB_P0_U0_BASE), 1024u},
			{(void CYFAR *)(CYDEV_UDB_DSI0_BASE), 1024u},
		};

		uint8 CYDATA i;

		/* Zero out critical memory blocks before beginning configuration */
		for (i = 0u; i < (sizeof(cfg_memset_list)/sizeof(cfg_memset_list[0])); i++)
		{
			const cfg_memset_t CYCODE * CYDATA ms = &cfg_memset_list[i];
			CYMEMZERO(ms->address, (size_t)(uint32)(ms->size));
		}

		cfg_write_bytes32(cy_cfg_addr_table, cy_cfg_data_table);

		/* Perform normal device configuration. Order is not critical for these items. */
		CY_SET_XTND_REG16((void CYFAR *)(CYREG_UDB_BCTL0_DRV), 0x0100u);
		CY_SET_XTND_REG8((void CYFAR *)(CYREG_UDB_BCTL0_DRV + 0x2u), 0x01u);

		/* HSIOM Starting address: CYDEV_HSIOM_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_HSIOM_BASE), 0xFFFF0090u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_HSIOM_PORT_SEL1), 0x00030333u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_HSIOM_PORT_SEL3), 0x00090008u);

		/* UDB_PA_0 Starting address: CYDEV_UDB_PA0_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA0_BASE), 0x00990000u);

		/* UDB_PA_1 Starting address: CYDEV_UDB_PA1_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA1_BASE), 0x00990000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA1_CFG8), 0x030D0000u);

		/* UDB_PA_2 Starting address: CYDEV_UDB_PA2_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA2_BASE), 0x00990000u);

		/* UDB_PA_3 Starting address: CYDEV_UDB_PA3_BASE */
		CY_SET_XTND_REG32((void CYFAR *)(CYDEV_UDB_PA3_BASE), 0x00990000u);
		CY_SET_XTND_REG32((void CYFAR *)(CYREG_UDB_PA3_CFG4), 0x08000000u);

		/* Enable digital routing */
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL, CY_GET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL) | 0x02u);

		/* Enable UDB array */
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDBIF_WAIT_CFG, (uint8)((CY_GET_XTND_REG8((void *)CYREG_UDB_UDBIF_WAIT_CFG) & 0xC3u) | 0x14u));
		CY_SET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL, (uint8)(CY_GET_XTND_REG8((void *)CYREG_UDB_UDBIF_BANK_CTL) | 0x16u));
	}

	/* Perform second pass device configuration. These items must be configured in specific order after the regular configuration is done. */
	/* IOPINS0_0 Starting address: CYDEV_GPIO_PRT0_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT0_BASE), 0x000000D2u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT0_PC), 0x00D8E030u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT0_PC2), 0x000000D2u);

	/* IOPINS0_1 Starting address: CYDEV_GPIO_PRT1_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT1_BASE), 0x00000017u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT1_PC), 0x000061B6u);

	/* IOPINS0_2 Starting address: CYDEV_GPIO_PRT2_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT2_BASE), 0x00000040u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT2_PC), 0x00180000u);

	/* IOPINS0_3 Starting address: CYDEV_GPIO_PRT3_BASE */
	CY_SET_XTND_REG32((void CYFAR *)(CYDEV_GPIO_PRT3_BASE), 0x000000E1u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT3_PC), 0x00D91006u);
	CY_SET_XTND_REG32((void CYFAR *)(CYREG_GPIO_PRT3_INTR_CFG), 0x00000C00u);

	/* Setup clocks based on selections from Clock DWR */
	ClockSetup();

	/* Perform basic analog initialization to defaults */
	AnalogSetDefault();

}
