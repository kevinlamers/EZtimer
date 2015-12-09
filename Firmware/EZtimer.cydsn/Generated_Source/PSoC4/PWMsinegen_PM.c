/*******************************************************************************
* File Name: PWMsinegen_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "PWMsinegen.h"

static PWMsinegen_backupStruct PWMsinegen_backup;


/*******************************************************************************
* Function Name: PWMsinegen_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMsinegen_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWMsinegen_SaveConfig(void) 
{

    #if(!PWMsinegen_UsingFixedFunction)
        #if(!PWMsinegen_PWMModeIsCenterAligned)
            PWMsinegen_backup.PWMPeriod = PWMsinegen_ReadPeriod();
        #endif /* (!PWMsinegen_PWMModeIsCenterAligned) */
        PWMsinegen_backup.PWMUdb = PWMsinegen_ReadCounter();
        #if (PWMsinegen_UseStatus)
            PWMsinegen_backup.InterruptMaskValue = PWMsinegen_STATUS_MASK;
        #endif /* (PWMsinegen_UseStatus) */

        #if(PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_256_CLOCKS || \
            PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_2_4_CLOCKS)
            PWMsinegen_backup.PWMdeadBandValue = PWMsinegen_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWMsinegen_KillModeMinTime)
             PWMsinegen_backup.PWMKillCounterPeriod = PWMsinegen_ReadKillTime();
        #endif /* (PWMsinegen_KillModeMinTime) */

        #if(PWMsinegen_UseControl)
            PWMsinegen_backup.PWMControlRegister = PWMsinegen_ReadControlRegister();
        #endif /* (PWMsinegen_UseControl) */
    #endif  /* (!PWMsinegen_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMsinegen_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMsinegen_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWMsinegen_RestoreConfig(void) 
{
        #if(!PWMsinegen_UsingFixedFunction)
            #if(!PWMsinegen_PWMModeIsCenterAligned)
                PWMsinegen_WritePeriod(PWMsinegen_backup.PWMPeriod);
            #endif /* (!PWMsinegen_PWMModeIsCenterAligned) */

            PWMsinegen_WriteCounter(PWMsinegen_backup.PWMUdb);

            #if (PWMsinegen_UseStatus)
                PWMsinegen_STATUS_MASK = PWMsinegen_backup.InterruptMaskValue;
            #endif /* (PWMsinegen_UseStatus) */

            #if(PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_256_CLOCKS || \
                PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_2_4_CLOCKS)
                PWMsinegen_WriteDeadTime(PWMsinegen_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWMsinegen_KillModeMinTime)
                PWMsinegen_WriteKillTime(PWMsinegen_backup.PWMKillCounterPeriod);
            #endif /* (PWMsinegen_KillModeMinTime) */

            #if(PWMsinegen_UseControl)
                PWMsinegen_WriteControlRegister(PWMsinegen_backup.PWMControlRegister);
            #endif /* (PWMsinegen_UseControl) */
        #endif  /* (!PWMsinegen_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWMsinegen_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMsinegen_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWMsinegen_Sleep(void) 
{
    #if(PWMsinegen_UseControl)
        if(PWMsinegen_CTRL_ENABLE == (PWMsinegen_CONTROL & PWMsinegen_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWMsinegen_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWMsinegen_backup.PWMEnableState = 0u;
        }
    #endif /* (PWMsinegen_UseControl) */

    /* Stop component */
    PWMsinegen_Stop();

    /* Save registers configuration */
    PWMsinegen_SaveConfig();
}


/*******************************************************************************
* Function Name: PWMsinegen_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMsinegen_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWMsinegen_Wakeup(void) 
{
     /* Restore registers values */
    PWMsinegen_RestoreConfig();

    if(PWMsinegen_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWMsinegen_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
