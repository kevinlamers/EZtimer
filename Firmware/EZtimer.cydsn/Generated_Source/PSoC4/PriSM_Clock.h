/*******************************************************************************
* File Name: PriSM_Clock.h
* Version 2.20
*
*  Description:
*   Provides the function and constant definitions for the clock component.
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_CLOCK_PriSM_Clock_H)
#define CY_CLOCK_PriSM_Clock_H

#include <cytypes.h>
#include <cyfitter.h>


/***************************************
*        Function Prototypes
***************************************/
#if defined CYREG_PERI_DIV_CMD

void PriSM_Clock_StartEx(uint32 alignClkDiv);
#define PriSM_Clock_Start() \
    PriSM_Clock_StartEx(PriSM_Clock__PA_DIV_ID)

#else

void PriSM_Clock_Start(void);

#endif/* CYREG_PERI_DIV_CMD */

void PriSM_Clock_Stop(void);

void PriSM_Clock_SetFractionalDividerRegister(uint16 clkDivider, uint8 clkFractional);

uint16 PriSM_Clock_GetDividerRegister(void);
uint8  PriSM_Clock_GetFractionalDividerRegister(void);

#define PriSM_Clock_Enable()                         PriSM_Clock_Start()
#define PriSM_Clock_Disable()                        PriSM_Clock_Stop()
#define PriSM_Clock_SetDividerRegister(clkDivider, reset)  \
    PriSM_Clock_SetFractionalDividerRegister((clkDivider), 0u)
#define PriSM_Clock_SetDivider(clkDivider)           PriSM_Clock_SetDividerRegister((clkDivider), 1u)
#define PriSM_Clock_SetDividerValue(clkDivider)      PriSM_Clock_SetDividerRegister((clkDivider) - 1u, 1u)


/***************************************
*             Registers
***************************************/
#if defined CYREG_PERI_DIV_CMD

#define PriSM_Clock_DIV_ID     PriSM_Clock__DIV_ID

#define PriSM_Clock_CMD_REG    (*(reg32 *)CYREG_PERI_DIV_CMD)
#define PriSM_Clock_CTRL_REG   (*(reg32 *)PriSM_Clock__CTRL_REGISTER)
#define PriSM_Clock_DIV_REG    (*(reg32 *)PriSM_Clock__DIV_REGISTER)

#define PriSM_Clock_CMD_DIV_SHIFT          (0u)
#define PriSM_Clock_CMD_PA_DIV_SHIFT       (8u)
#define PriSM_Clock_CMD_DISABLE_SHIFT      (30u)
#define PriSM_Clock_CMD_ENABLE_SHIFT       (31u)

#define PriSM_Clock_CMD_DISABLE_MASK       ((uint32)((uint32)1u << PriSM_Clock_CMD_DISABLE_SHIFT))
#define PriSM_Clock_CMD_ENABLE_MASK        ((uint32)((uint32)1u << PriSM_Clock_CMD_ENABLE_SHIFT))

#define PriSM_Clock_DIV_FRAC_MASK  (0x000000F8u)
#define PriSM_Clock_DIV_FRAC_SHIFT (3u)
#define PriSM_Clock_DIV_INT_MASK   (0xFFFFFF00u)
#define PriSM_Clock_DIV_INT_SHIFT  (8u)

#else 

#define PriSM_Clock_DIV_REG        (*(reg32 *)PriSM_Clock__REGISTER)
#define PriSM_Clock_ENABLE_REG     PriSM_Clock_DIV_REG
#define PriSM_Clock_DIV_FRAC_MASK  PriSM_Clock__FRAC_MASK
#define PriSM_Clock_DIV_FRAC_SHIFT (16u)
#define PriSM_Clock_DIV_INT_MASK   PriSM_Clock__DIVIDER_MASK
#define PriSM_Clock_DIV_INT_SHIFT  (0u)

#endif/* CYREG_PERI_DIV_CMD */

#endif /* !defined(CY_CLOCK_PriSM_Clock_H) */

/* [] END OF FILE */
