/*******************************************************************************
* File Name: InputPin.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "InputPin.h"

#define SetP4PinDriveMode(shift, mode)  \
    do { \
        InputPin_PC =   (InputPin_PC & \
                                (uint32)(~(uint32)(InputPin_DRIVE_MODE_IND_MASK << (InputPin_DRIVE_MODE_BITS * (shift))))) | \
                                (uint32)((uint32)(mode) << (InputPin_DRIVE_MODE_BITS * (shift))); \
    } while (0)


/*******************************************************************************
* Function Name: InputPin_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None 
*  
*******************************************************************************/
void InputPin_Write(uint8 value) 
{
    uint8 drVal = (uint8)(InputPin_DR & (uint8)(~InputPin_MASK));
    drVal = (drVal | ((uint8)(value << InputPin_SHIFT) & InputPin_MASK));
    InputPin_DR = (uint32)drVal;
}


/*******************************************************************************
* Function Name: InputPin_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  InputPin_DM_STRONG     Strong Drive 
*  InputPin_DM_OD_HI      Open Drain, Drives High 
*  InputPin_DM_OD_LO      Open Drain, Drives Low 
*  InputPin_DM_RES_UP     Resistive Pull Up 
*  InputPin_DM_RES_DWN    Resistive Pull Down 
*  InputPin_DM_RES_UPDWN  Resistive Pull Up/Down 
*  InputPin_DM_DIG_HIZ    High Impedance Digital 
*  InputPin_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void InputPin_SetDriveMode(uint8 mode) 
{
	SetP4PinDriveMode(InputPin__0__SHIFT, mode);
}


/*******************************************************************************
* Function Name: InputPin_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro InputPin_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 InputPin_Read(void) 
{
    return (uint8)((InputPin_PS & InputPin_MASK) >> InputPin_SHIFT);
}


/*******************************************************************************
* Function Name: InputPin_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 InputPin_ReadDataReg(void) 
{
    return (uint8)((InputPin_DR & InputPin_MASK) >> InputPin_SHIFT);
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(InputPin_INTSTAT) 

    /*******************************************************************************
    * Function Name: InputPin_ClearInterrupt
    ********************************************************************************
    *
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 InputPin_ClearInterrupt(void) 
    {
		uint8 maskedStatus = (uint8)(InputPin_INTSTAT & InputPin_MASK);
		InputPin_INTSTAT = maskedStatus;
        return maskedStatus >> InputPin_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 


/* [] END OF FILE */
