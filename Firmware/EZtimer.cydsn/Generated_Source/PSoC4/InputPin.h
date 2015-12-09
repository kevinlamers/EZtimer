/*******************************************************************************
* File Name: InputPin.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_InputPin_H) /* Pins InputPin_H */
#define CY_PINS_InputPin_H

#include "cytypes.h"
#include "cyfitter.h"
#include "InputPin_aliases.h"


/***************************************
*        Function Prototypes             
***************************************/    

void    InputPin_Write(uint8 value) ;
void    InputPin_SetDriveMode(uint8 mode) ;
uint8   InputPin_ReadDataReg(void) ;
uint8   InputPin_Read(void) ;
uint8   InputPin_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define InputPin_DRIVE_MODE_BITS        (3)
#define InputPin_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - InputPin_DRIVE_MODE_BITS))

#define InputPin_DM_ALG_HIZ         (0x00u)
#define InputPin_DM_DIG_HIZ         (0x01u)
#define InputPin_DM_RES_UP          (0x02u)
#define InputPin_DM_RES_DWN         (0x03u)
#define InputPin_DM_OD_LO           (0x04u)
#define InputPin_DM_OD_HI           (0x05u)
#define InputPin_DM_STRONG          (0x06u)
#define InputPin_DM_RES_UPDWN       (0x07u)

/* Digital Port Constants */
#define InputPin_MASK               InputPin__MASK
#define InputPin_SHIFT              InputPin__SHIFT
#define InputPin_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define InputPin_PS                     (* (reg32 *) InputPin__PS)
/* Port Configuration */
#define InputPin_PC                     (* (reg32 *) InputPin__PC)
/* Data Register */
#define InputPin_DR                     (* (reg32 *) InputPin__DR)
/* Input Buffer Disable Override */
#define InputPin_INP_DIS                (* (reg32 *) InputPin__PC2)


#if defined(InputPin__INTSTAT)  /* Interrupt Registers */

    #define InputPin_INTSTAT                (* (reg32 *) InputPin__INTSTAT)

#endif /* Interrupt Registers */


/***************************************
* The following code is DEPRECATED and 
* must not be used.
***************************************/

#define InputPin_DRIVE_MODE_SHIFT       (0x00u)
#define InputPin_DRIVE_MODE_MASK        (0x07u << InputPin_DRIVE_MODE_SHIFT)


#endif /* End Pins InputPin_H */


/* [] END OF FILE */
