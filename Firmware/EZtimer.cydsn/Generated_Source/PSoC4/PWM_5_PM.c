/*******************************************************************************
* File Name: PWM_5_PM.c
* Version 2.0
*
* Description:
*  This file contains the setup, control, and status commands to support
*  the component operations in the low power mode.
*
* Note:
*  None
*
********************************************************************************
* Copyright 2013-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWM_5.h"

static PWM_5_BACKUP_STRUCT PWM_5_backup;


/*******************************************************************************
* Function Name: PWM_5_SaveConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to save here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_5_SaveConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_5_Sleep
********************************************************************************
*
* Summary:
*  Stops the component operation and saves the user configuration.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_5_Sleep(void)
{
    if(0u != (PWM_5_BLOCK_CONTROL_REG & PWM_5_MASK))
    {
        PWM_5_backup.enableState = 1u;
    }
    else
    {
        PWM_5_backup.enableState = 0u;
    }

    PWM_5_Stop();
    PWM_5_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_5_RestoreConfig
********************************************************************************
*
* Summary:
*  All configuration registers are retention. Nothing to restore here.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_5_RestoreConfig(void)
{

}


/*******************************************************************************
* Function Name: PWM_5_Wakeup
********************************************************************************
*
* Summary:
*  Restores the user configuration and restores the enable state.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWM_5_Wakeup(void)
{
    PWM_5_RestoreConfig();

    if(0u != PWM_5_backup.enableState)
    {
        PWM_5_Enable();
    }
}


/* [] END OF FILE */
