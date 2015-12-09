/*******************************************************************************
* File Name: PWM_12_PM.c
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

#include "PWM_12.h"

static PWM_12_backupStruct PWM_12_backup;


/*******************************************************************************
* Function Name: PWM_12_SaveConfig
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
*  PWM_12_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_12_SaveConfig(void) 
{

    #if(!PWM_12_UsingFixedFunction)
        #if(!PWM_12_PWMModeIsCenterAligned)
            PWM_12_backup.PWMPeriod = PWM_12_ReadPeriod();
        #endif /* (!PWM_12_PWMModeIsCenterAligned) */
        PWM_12_backup.PWMUdb = PWM_12_ReadCounter();
        #if (PWM_12_UseStatus)
            PWM_12_backup.InterruptMaskValue = PWM_12_STATUS_MASK;
        #endif /* (PWM_12_UseStatus) */

        #if(PWM_12_DeadBandMode == PWM_12__B_PWM__DBM_256_CLOCKS || \
            PWM_12_DeadBandMode == PWM_12__B_PWM__DBM_2_4_CLOCKS)
            PWM_12_backup.PWMdeadBandValue = PWM_12_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_12_KillModeMinTime)
             PWM_12_backup.PWMKillCounterPeriod = PWM_12_ReadKillTime();
        #endif /* (PWM_12_KillModeMinTime) */

        #if(PWM_12_UseControl)
            PWM_12_backup.PWMControlRegister = PWM_12_ReadControlRegister();
        #endif /* (PWM_12_UseControl) */
    #endif  /* (!PWM_12_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_12_RestoreConfig
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
*  PWM_12_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_12_RestoreConfig(void) 
{
        #if(!PWM_12_UsingFixedFunction)
            #if(!PWM_12_PWMModeIsCenterAligned)
                PWM_12_WritePeriod(PWM_12_backup.PWMPeriod);
            #endif /* (!PWM_12_PWMModeIsCenterAligned) */

            PWM_12_WriteCounter(PWM_12_backup.PWMUdb);

            #if (PWM_12_UseStatus)
                PWM_12_STATUS_MASK = PWM_12_backup.InterruptMaskValue;
            #endif /* (PWM_12_UseStatus) */

            #if(PWM_12_DeadBandMode == PWM_12__B_PWM__DBM_256_CLOCKS || \
                PWM_12_DeadBandMode == PWM_12__B_PWM__DBM_2_4_CLOCKS)
                PWM_12_WriteDeadTime(PWM_12_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_12_KillModeMinTime)
                PWM_12_WriteKillTime(PWM_12_backup.PWMKillCounterPeriod);
            #endif /* (PWM_12_KillModeMinTime) */

            #if(PWM_12_UseControl)
                PWM_12_WriteControlRegister(PWM_12_backup.PWMControlRegister);
            #endif /* (PWM_12_UseControl) */
        #endif  /* (!PWM_12_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_12_Sleep
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
*  PWM_12_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_12_Sleep(void) 
{
    #if(PWM_12_UseControl)
        if(PWM_12_CTRL_ENABLE == (PWM_12_CONTROL & PWM_12_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_12_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_12_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_12_UseControl) */

    /* Stop component */
    PWM_12_Stop();

    /* Save registers configuration */
    PWM_12_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_12_Wakeup
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
*  PWM_12_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_12_Wakeup(void) 
{
     /* Restore registers values */
    PWM_12_RestoreConfig();

    if(PWM_12_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_12_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
