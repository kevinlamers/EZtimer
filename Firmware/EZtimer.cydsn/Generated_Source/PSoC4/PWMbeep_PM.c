/*******************************************************************************
* File Name: PWMbeep_PM.c
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

#include "PWMbeep.h"

static PWMbeep_backupStruct PWMbeep_backup;


/*******************************************************************************
* Function Name: PWMbeep_SaveConfig
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
*  PWMbeep_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWMbeep_SaveConfig(void) 
{

    #if(!PWMbeep_UsingFixedFunction)
        #if(!PWMbeep_PWMModeIsCenterAligned)
            PWMbeep_backup.PWMPeriod = PWMbeep_ReadPeriod();
        #endif /* (!PWMbeep_PWMModeIsCenterAligned) */
        PWMbeep_backup.PWMUdb = PWMbeep_ReadCounter();
        #if (PWMbeep_UseStatus)
            PWMbeep_backup.InterruptMaskValue = PWMbeep_STATUS_MASK;
        #endif /* (PWMbeep_UseStatus) */

        #if(PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_256_CLOCKS || \
            PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_2_4_CLOCKS)
            PWMbeep_backup.PWMdeadBandValue = PWMbeep_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWMbeep_KillModeMinTime)
             PWMbeep_backup.PWMKillCounterPeriod = PWMbeep_ReadKillTime();
        #endif /* (PWMbeep_KillModeMinTime) */

        #if(PWMbeep_UseControl)
            PWMbeep_backup.PWMControlRegister = PWMbeep_ReadControlRegister();
        #endif /* (PWMbeep_UseControl) */
    #endif  /* (!PWMbeep_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMbeep_RestoreConfig
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
*  PWMbeep_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWMbeep_RestoreConfig(void) 
{
        #if(!PWMbeep_UsingFixedFunction)
            #if(!PWMbeep_PWMModeIsCenterAligned)
                PWMbeep_WritePeriod(PWMbeep_backup.PWMPeriod);
            #endif /* (!PWMbeep_PWMModeIsCenterAligned) */

            PWMbeep_WriteCounter(PWMbeep_backup.PWMUdb);

            #if (PWMbeep_UseStatus)
                PWMbeep_STATUS_MASK = PWMbeep_backup.InterruptMaskValue;
            #endif /* (PWMbeep_UseStatus) */

            #if(PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_256_CLOCKS || \
                PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_2_4_CLOCKS)
                PWMbeep_WriteDeadTime(PWMbeep_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWMbeep_KillModeMinTime)
                PWMbeep_WriteKillTime(PWMbeep_backup.PWMKillCounterPeriod);
            #endif /* (PWMbeep_KillModeMinTime) */

            #if(PWMbeep_UseControl)
                PWMbeep_WriteControlRegister(PWMbeep_backup.PWMControlRegister);
            #endif /* (PWMbeep_UseControl) */
        #endif  /* (!PWMbeep_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWMbeep_Sleep
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
*  PWMbeep_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWMbeep_Sleep(void) 
{
    #if(PWMbeep_UseControl)
        if(PWMbeep_CTRL_ENABLE == (PWMbeep_CONTROL & PWMbeep_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWMbeep_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWMbeep_backup.PWMEnableState = 0u;
        }
    #endif /* (PWMbeep_UseControl) */

    /* Stop component */
    PWMbeep_Stop();

    /* Save registers configuration */
    PWMbeep_SaveConfig();
}


/*******************************************************************************
* Function Name: PWMbeep_Wakeup
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
*  PWMbeep_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWMbeep_Wakeup(void) 
{
     /* Restore registers values */
    PWMbeep_RestoreConfig();

    if(PWMbeep_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWMbeep_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
