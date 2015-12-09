/*******************************************************************************
* File Name: PWM_34_PM.c
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

#include "PWM_34.h"

static PWM_34_backupStruct PWM_34_backup;


/*******************************************************************************
* Function Name: PWM_34_SaveConfig
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
*  PWM_34_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void PWM_34_SaveConfig(void) 
{

    #if(!PWM_34_UsingFixedFunction)
        #if(!PWM_34_PWMModeIsCenterAligned)
            PWM_34_backup.PWMPeriod = PWM_34_ReadPeriod();
        #endif /* (!PWM_34_PWMModeIsCenterAligned) */
        PWM_34_backup.PWMUdb = PWM_34_ReadCounter();
        #if (PWM_34_UseStatus)
            PWM_34_backup.InterruptMaskValue = PWM_34_STATUS_MASK;
        #endif /* (PWM_34_UseStatus) */

        #if(PWM_34_DeadBandMode == PWM_34__B_PWM__DBM_256_CLOCKS || \
            PWM_34_DeadBandMode == PWM_34__B_PWM__DBM_2_4_CLOCKS)
            PWM_34_backup.PWMdeadBandValue = PWM_34_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(PWM_34_KillModeMinTime)
             PWM_34_backup.PWMKillCounterPeriod = PWM_34_ReadKillTime();
        #endif /* (PWM_34_KillModeMinTime) */

        #if(PWM_34_UseControl)
            PWM_34_backup.PWMControlRegister = PWM_34_ReadControlRegister();
        #endif /* (PWM_34_UseControl) */
    #endif  /* (!PWM_34_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWM_34_RestoreConfig
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
*  PWM_34_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_34_RestoreConfig(void) 
{
        #if(!PWM_34_UsingFixedFunction)
            #if(!PWM_34_PWMModeIsCenterAligned)
                PWM_34_WritePeriod(PWM_34_backup.PWMPeriod);
            #endif /* (!PWM_34_PWMModeIsCenterAligned) */

            PWM_34_WriteCounter(PWM_34_backup.PWMUdb);

            #if (PWM_34_UseStatus)
                PWM_34_STATUS_MASK = PWM_34_backup.InterruptMaskValue;
            #endif /* (PWM_34_UseStatus) */

            #if(PWM_34_DeadBandMode == PWM_34__B_PWM__DBM_256_CLOCKS || \
                PWM_34_DeadBandMode == PWM_34__B_PWM__DBM_2_4_CLOCKS)
                PWM_34_WriteDeadTime(PWM_34_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(PWM_34_KillModeMinTime)
                PWM_34_WriteKillTime(PWM_34_backup.PWMKillCounterPeriod);
            #endif /* (PWM_34_KillModeMinTime) */

            #if(PWM_34_UseControl)
                PWM_34_WriteControlRegister(PWM_34_backup.PWMControlRegister);
            #endif /* (PWM_34_UseControl) */
        #endif  /* (!PWM_34_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: PWM_34_Sleep
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
*  PWM_34_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void PWM_34_Sleep(void) 
{
    #if(PWM_34_UseControl)
        if(PWM_34_CTRL_ENABLE == (PWM_34_CONTROL & PWM_34_CTRL_ENABLE))
        {
            /*Component is enabled */
            PWM_34_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            PWM_34_backup.PWMEnableState = 0u;
        }
    #endif /* (PWM_34_UseControl) */

    /* Stop component */
    PWM_34_Stop();

    /* Save registers configuration */
    PWM_34_SaveConfig();
}


/*******************************************************************************
* Function Name: PWM_34_Wakeup
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
*  PWM_34_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void PWM_34_Wakeup(void) 
{
     /* Restore registers values */
    PWM_34_RestoreConfig();

    if(PWM_34_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        PWM_34_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
