/*******************************************************************************
* File Name: PWMbeep.c
* Version 3.30
*
* Description:
*  The PWM User Module consist of an 8 or 16-bit counter with two 8 or 16-bit
*  comparitors. Each instance of this user module is capable of generating
*  two PWM outputs with the same period. The pulse width is selectable between
*  1 and 255/65535. The period is selectable between 2 and 255/65536 clocks.
*  The compare value output may be configured to be active when the present
*  counter is less than or less than/equal to the compare value.
*  A terminal count output is also provided. It generates a pulse one clock
*  width wide when the counter is equal to zero.
*
* Note:
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "PWMbeep.h"

/* Error message for removed <resource> through optimization */
#ifdef PWMbeep_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* PWMbeep_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 PWMbeep_initVar = 0u;


/*******************************************************************************
* Function Name: PWMbeep_Start
********************************************************************************
*
* Summary:
*  The start function initializes the pwm with the default values, the
*  enables the counter to begin counting.  It does not enable interrupts,
*  the EnableInt command should be called if interrupt generation is required.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  PWMbeep_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void PWMbeep_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PWMbeep_initVar == 0u)
    {
        PWMbeep_Init();
        PWMbeep_initVar = 1u;
    }
    PWMbeep_Enable();

}


/*******************************************************************************
* Function Name: PWMbeep_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  PWMbeep_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWMbeep_Init(void) 
{
    #if (PWMbeep_UsingFixedFunction || PWMbeep_UseControl)
        uint8 ctrl;
    #endif /* (PWMbeep_UsingFixedFunction || PWMbeep_UseControl) */

    #if(!PWMbeep_UsingFixedFunction)
        #if(PWMbeep_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 PWMbeep_interruptState;
        #endif /* (PWMbeep_UseStatus) */
    #endif /* (!PWMbeep_UsingFixedFunction) */

    #if (PWMbeep_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        PWMbeep_CONTROL |= PWMbeep_CFG0_MODE;
        #if (PWMbeep_DeadBand2_4)
            PWMbeep_CONTROL |= PWMbeep_CFG0_DB;
        #endif /* (PWMbeep_DeadBand2_4) */

        ctrl = PWMbeep_CONTROL3 & ((uint8 )(~PWMbeep_CTRL_CMPMODE1_MASK));
        PWMbeep_CONTROL3 = ctrl | PWMbeep_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        PWMbeep_RT1 &= ((uint8)(~PWMbeep_RT1_MASK));
        PWMbeep_RT1 |= PWMbeep_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        PWMbeep_RT1 &= ((uint8)(~PWMbeep_SYNCDSI_MASK));
        PWMbeep_RT1 |= PWMbeep_SYNCDSI_EN;

    #elif (PWMbeep_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = PWMbeep_CONTROL & ((uint8)(~PWMbeep_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~PWMbeep_CTRL_CMPMODE1_MASK));
        PWMbeep_CONTROL = ctrl | PWMbeep_DEFAULT_COMPARE2_MODE |
                                   PWMbeep_DEFAULT_COMPARE1_MODE;
    #endif /* (PWMbeep_UsingFixedFunction) */

    #if (!PWMbeep_UsingFixedFunction)
        #if (PWMbeep_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            PWMbeep_AUX_CONTROLDP0 |= (PWMbeep_AUX_CTRL_FIFO0_CLR);
        #else /* (PWMbeep_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            PWMbeep_AUX_CONTROLDP0 |= (PWMbeep_AUX_CTRL_FIFO0_CLR);
            PWMbeep_AUX_CONTROLDP1 |= (PWMbeep_AUX_CTRL_FIFO0_CLR);
        #endif /* (PWMbeep_Resolution == 8) */

        PWMbeep_WriteCounter(PWMbeep_INIT_PERIOD_VALUE);
    #endif /* (!PWMbeep_UsingFixedFunction) */

    PWMbeep_WritePeriod(PWMbeep_INIT_PERIOD_VALUE);

        #if (PWMbeep_UseOneCompareMode)
            PWMbeep_WriteCompare(PWMbeep_INIT_COMPARE_VALUE1);
        #else
            PWMbeep_WriteCompare1(PWMbeep_INIT_COMPARE_VALUE1);
            PWMbeep_WriteCompare2(PWMbeep_INIT_COMPARE_VALUE2);
        #endif /* (PWMbeep_UseOneCompareMode) */

        #if (PWMbeep_KillModeMinTime)
            PWMbeep_WriteKillTime(PWMbeep_MinimumKillTime);
        #endif /* (PWMbeep_KillModeMinTime) */

        #if (PWMbeep_DeadBandUsed)
            PWMbeep_WriteDeadTime(PWMbeep_INIT_DEAD_TIME);
        #endif /* (PWMbeep_DeadBandUsed) */

    #if (PWMbeep_UseStatus || PWMbeep_UsingFixedFunction)
        PWMbeep_SetInterruptMode(PWMbeep_INIT_INTERRUPTS_MODE);
    #endif /* (PWMbeep_UseStatus || PWMbeep_UsingFixedFunction) */

    #if (PWMbeep_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        PWMbeep_GLOBAL_ENABLE |= PWMbeep_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        PWMbeep_CONTROL2 |= PWMbeep_CTRL2_IRQ_SEL;
    #else
        #if(PWMbeep_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            PWMbeep_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            PWMbeep_STATUS_AUX_CTRL |= PWMbeep_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(PWMbeep_interruptState);

            /* Clear the FIFO to enable the PWMbeep_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            PWMbeep_ClearFIFO();
        #endif /* (PWMbeep_UseStatus) */
    #endif /* (PWMbeep_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMbeep_Enable
********************************************************************************
*
* Summary:
*  Enables the PWM block operation
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  This works only if software enable mode is chosen
*
*******************************************************************************/
void PWMbeep_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (PWMbeep_UsingFixedFunction)
        PWMbeep_GLOBAL_ENABLE |= PWMbeep_BLOCK_EN_MASK;
        PWMbeep_GLOBAL_STBY_ENABLE |= PWMbeep_BLOCK_STBY_EN_MASK;
    #endif /* (PWMbeep_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (PWMbeep_UseControl || PWMbeep_UsingFixedFunction)
        PWMbeep_CONTROL |= PWMbeep_CTRL_ENABLE;
    #endif /* (PWMbeep_UseControl || PWMbeep_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMbeep_Stop
********************************************************************************
*
* Summary:
*  The stop function halts the PWM, but does not change any modes or disable
*  interrupts.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Side Effects:
*  If the Enable mode is set to Hardware only then this function
*  has no effect on the operation of the PWM
*
*******************************************************************************/
void PWMbeep_Stop(void) 
{
    #if (PWMbeep_UseControl || PWMbeep_UsingFixedFunction)
        PWMbeep_CONTROL &= ((uint8)(~PWMbeep_CTRL_ENABLE));
    #endif /* (PWMbeep_UseControl || PWMbeep_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (PWMbeep_UsingFixedFunction)
        PWMbeep_GLOBAL_ENABLE &= ((uint8)(~PWMbeep_BLOCK_EN_MASK));
        PWMbeep_GLOBAL_STBY_ENABLE &= ((uint8)(~PWMbeep_BLOCK_STBY_EN_MASK));
    #endif /* (PWMbeep_UsingFixedFunction) */
}

#if (PWMbeep_UseOneCompareMode)
    #if (PWMbeep_CompareMode1SW)


        /*******************************************************************************
        * Function Name: PWMbeep_SetCompareMode
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm output when in Dither mode,
        *  Center Align Mode or One Output Mode.
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void PWMbeep_SetCompareMode(uint8 comparemode) 
        {
            #if(PWMbeep_UsingFixedFunction)

                #if(0 != PWMbeep_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMbeep_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != PWMbeep_CTRL_CMPMODE1_SHIFT) */

                PWMbeep_CONTROL3 &= ((uint8)(~PWMbeep_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                PWMbeep_CONTROL3 |= comparemodemasked;

            #elif (PWMbeep_UseControl)

                #if(0 != PWMbeep_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << PWMbeep_CTRL_CMPMODE1_SHIFT)) &
                                                PWMbeep_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & PWMbeep_CTRL_CMPMODE1_MASK;
                #endif /* (0 != PWMbeep_CTRL_CMPMODE1_SHIFT) */

                #if(0 != PWMbeep_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << PWMbeep_CTRL_CMPMODE2_SHIFT)) &
                                               PWMbeep_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & PWMbeep_CTRL_CMPMODE2_MASK;
                #endif /* (0 != PWMbeep_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                PWMbeep_CONTROL &= ((uint8)(~(PWMbeep_CTRL_CMPMODE1_MASK |
                                            PWMbeep_CTRL_CMPMODE2_MASK)));
                PWMbeep_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (PWMbeep_UsingFixedFunction) */
        }
    #endif /* PWMbeep_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (PWMbeep_CompareMode1SW)


        /*******************************************************************************
        * Function Name: PWMbeep_SetCompareMode1
        ********************************************************************************
        *
        * Summary:
        *  This function writes the Compare Mode for the pwm or pwm1 output
        *
        * Parameters:
        *  comparemode:  The new compare mode for the PWM output. Use the compare types
        *                defined in the H file as input arguments.
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void PWMbeep_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != PWMbeep_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMbeep_CTRL_CMPMODE1_SHIFT)) &
                                           PWMbeep_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & PWMbeep_CTRL_CMPMODE1_MASK;
            #endif /* (0 != PWMbeep_CTRL_CMPMODE1_SHIFT) */

            #if (PWMbeep_UseControl)
                PWMbeep_CONTROL &= ((uint8)(~PWMbeep_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                PWMbeep_CONTROL |= comparemodemasked;
            #endif /* (PWMbeep_UseControl) */
        }
    #endif /* PWMbeep_CompareMode1SW */

#if (PWMbeep_CompareMode2SW)


    /*******************************************************************************
    * Function Name: PWMbeep_SetCompareMode2
    ********************************************************************************
    *
    * Summary:
    *  This function writes the Compare Mode for the pwm or pwm2 output
    *
    * Parameters:
    *  comparemode:  The new compare mode for the PWM output. Use the compare types
    *                defined in the H file as input arguments.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMbeep_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != PWMbeep_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMbeep_CTRL_CMPMODE2_SHIFT)) &
                                                 PWMbeep_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & PWMbeep_CTRL_CMPMODE2_MASK;
        #endif /* (0 != PWMbeep_CTRL_CMPMODE2_SHIFT) */

        #if (PWMbeep_UseControl)
            PWMbeep_CONTROL &= ((uint8)(~PWMbeep_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            PWMbeep_CONTROL |= comparemodemasked;
        #endif /* (PWMbeep_UseControl) */
    }
    #endif /*PWMbeep_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!PWMbeep_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMbeep_WriteCounter
    ********************************************************************************
    *
    * Summary:
    *  Writes a new counter value directly to the counter register. This will be
    *  implemented for that currently running period and only that period. This API
    *  is valid only for UDB implementation and not available for fixed function
    *  PWM implementation.
    *
    * Parameters:
    *  counter:  The period new period counter value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  The PWM Period will be reloaded when a counter value will be a zero
    *
    *******************************************************************************/
    void PWMbeep_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(PWMbeep_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: PWMbeep_ReadCounter
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current value of the counter.  It doesn't matter
    *  if the counter is enabled or running.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  The current value of the counter.
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(PWMbeep_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(PWMbeep_CAPTURE_LSB_PTR));
    }

    #if (PWMbeep_UseStatus)


        /*******************************************************************************
        * Function Name: PWMbeep_ClearFIFO
        ********************************************************************************
        *
        * Summary:
        *  This function clears all capture data from the capture FIFO
        *
        * Parameters:
        *  None
        *
        * Return:
        *  None
        *
        *******************************************************************************/
        void PWMbeep_ClearFIFO(void) 
        {
            while(0u != (PWMbeep_ReadStatusRegister() & PWMbeep_STATUS_FIFONEMPTY))
            {
                (void)PWMbeep_ReadCapture();
            }
        }

    #endif /* PWMbeep_UseStatus */

#endif /* !PWMbeep_UsingFixedFunction */


/*******************************************************************************
* Function Name: PWMbeep_WritePeriod
********************************************************************************
*
* Summary:
*  This function is used to change the period of the counter.  The new period
*  will be loaded the next time terminal count is detected.
*
* Parameters:
*  period:  Period value. May be between 1 and (2^Resolution)-1.  A value of 0
*           will result in the counter remaining at zero.
*
* Return:
*  None
*
*******************************************************************************/
void PWMbeep_WritePeriod(uint8 period) 
{
    #if(PWMbeep_UsingFixedFunction)
        CY_SET_REG16(PWMbeep_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(PWMbeep_PERIOD_LSB_PTR, period);
    #endif /* (PWMbeep_UsingFixedFunction) */
}

#if (PWMbeep_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWMbeep_WriteCompare
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value when the PWM is in Dither
    *  mode. The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  compared to the compare value based on the compare mode defined in
    *  Dither Mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    * Side Effects:
    *  This function is only available if the PWM mode parameter is set to
    *  Dither Mode, Center Aligned Mode or One Output Mode
    *
    *******************************************************************************/
    void PWMbeep_WriteCompare(uint8 compare) \
                                       
    {
        #if(PWMbeep_UsingFixedFunction)
            CY_SET_REG16(PWMbeep_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(PWMbeep_COMPARE1_LSB_PTR, compare);
        #endif /* (PWMbeep_UsingFixedFunction) */

        #if (PWMbeep_PWMMode == PWMbeep__B_PWM__DITHER)
            #if(PWMbeep_UsingFixedFunction)
                CY_SET_REG16(PWMbeep_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(PWMbeep_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (PWMbeep_UsingFixedFunction) */
        #endif /* (PWMbeep_PWMMode == PWMbeep__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: PWMbeep_WriteCompare1
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare1 value.  The compare output will
    *  reflect the new value on the next UDB clock.  The compare output will be
    *  driven high when the present counter value is less than or less than or
    *  equal to the compare register, depending on the mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMbeep_WriteCompare1(uint8 compare) \
                                        
    {
        #if(PWMbeep_UsingFixedFunction)
            CY_SET_REG16(PWMbeep_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(PWMbeep_COMPARE1_LSB_PTR, compare);
        #endif /* (PWMbeep_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: PWMbeep_WriteCompare2
    ********************************************************************************
    *
    * Summary:
    *  This funtion is used to change the compare value, for compare1 output.
    *  The compare output will reflect the new value on the next UDB clock.
    *  The compare output will be driven high when the present counter value is
    *  less than or less than or equal to the compare register, depending on the
    *  mode.
    *
    * Parameters:
    *  compare:  New compare value.
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMbeep_WriteCompare2(uint8 compare) \
                                        
    {
        #if(PWMbeep_UsingFixedFunction)
            CY_SET_REG16(PWMbeep_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(PWMbeep_COMPARE2_LSB_PTR, compare);
        #endif /* (PWMbeep_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (PWMbeep_DeadBandUsed)


    /*******************************************************************************
    * Function Name: PWMbeep_WriteDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function writes the dead-band counts to the corresponding register
    *
    * Parameters:
    *  deadtime:  Number of counts for dead time
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMbeep_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!PWMbeep_DeadBand2_4)
            CY_SET_REG8(PWMbeep_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            PWMbeep_DEADBAND_COUNT &= ((uint8)(~PWMbeep_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(PWMbeep_DEADBAND_COUNT_SHIFT)
                PWMbeep_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << PWMbeep_DEADBAND_COUNT_SHIFT)) &
                                                    PWMbeep_DEADBAND_COUNT_MASK;
            #else
                PWMbeep_DEADBAND_COUNT |= deadtime & PWMbeep_DEADBAND_COUNT_MASK;
            #endif /* (PWMbeep_DEADBAND_COUNT_SHIFT) */

        #endif /* (!PWMbeep_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: PWMbeep_ReadDeadTime
    ********************************************************************************
    *
    * Summary:
    *  This function reads the dead-band counts from the corresponding register
    *
    * Parameters:
    *  None
    *
    * Return:
    *  Dead Band Counts
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!PWMbeep_DeadBand2_4)
            return (CY_GET_REG8(PWMbeep_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(PWMbeep_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(PWMbeep_DEADBAND_COUNT & PWMbeep_DEADBAND_COUNT_MASK)) >>
                                                                           PWMbeep_DEADBAND_COUNT_SHIFT));
            #else
                return (PWMbeep_DEADBAND_COUNT & PWMbeep_DEADBAND_COUNT_MASK);
            #endif /* (PWMbeep_DEADBAND_COUNT_SHIFT) */
        #endif /* (!PWMbeep_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (PWMbeep_UseStatus || PWMbeep_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMbeep_SetInterruptMode
    ********************************************************************************
    *
    * Summary:
    *  This function configures the interrupts mask control of theinterrupt
    *  source status register.
    *
    * Parameters:
    *  uint8 interruptMode: Bit field containing the interrupt sources enabled
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMbeep_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(PWMbeep_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: PWMbeep_ReadStatusRegister
    ********************************************************************************
    *
    * Summary:
    *  This function returns the current state of the status register.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current status register value. The status register bits are:
    *  [7:6] : Unused(0)
    *  [5]   : Kill event output
    *  [4]   : FIFO not empty
    *  [3]   : FIFO full
    *  [2]   : Terminal count
    *  [1]   : Compare output 2
    *  [0]   : Compare output 1
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(PWMbeep_STATUS_PTR));
    }

#endif /* (PWMbeep_UseStatus || PWMbeep_UsingFixedFunction) */


#if (PWMbeep_UseControl)


    /*******************************************************************************
    * Function Name: PWMbeep_ReadControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Returns the current state of the control register. This API is available
    *  only if the control register is not removed.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8 : Current control register value
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(PWMbeep_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: PWMbeep_WriteControlRegister
    ********************************************************************************
    *
    * Summary:
    *  Sets the bit field of the control register. This API is available only if
    *  the control register is not removed.
    *
    * Parameters:
    *  uint8 control: Control register bit field, The status register bits are:
    *  [7]   : PWM Enable
    *  [6]   : Reset
    *  [5:3] : Compare Mode2
    *  [2:0] : Compare Mode2
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMbeep_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(PWMbeep_CONTROL_PTR, control);
    }

#endif /* (PWMbeep_UseControl) */


#if (!PWMbeep_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMbeep_ReadCapture
    ********************************************************************************
    *
    * Summary:
    *  Reads the capture value from the capture FIFO.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: The current capture value
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadCapture(void) 
    {
        return (CY_GET_REG8(PWMbeep_CAPTURE_LSB_PTR));
    }

#endif /* (!PWMbeep_UsingFixedFunction) */


#if (PWMbeep_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWMbeep_ReadCompare
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare output when the PWM Mode parameter is
    *  set to Dither mode, Center Aligned mode, or One Output mode.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadCompare(void) 
    {
        #if(PWMbeep_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(PWMbeep_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(PWMbeep_COMPARE1_LSB_PTR));
        #endif /* (PWMbeep_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: PWMbeep_ReadCompare1
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare1 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadCompare1(void) 
    {
        return (CY_GET_REG8(PWMbeep_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: PWMbeep_ReadCompare2
    ********************************************************************************
    *
    * Summary:
    *  Reads the compare value for the compare2 output.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8/uint16: Current compare value.
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadCompare2(void) 
    {
        return (CY_GET_REG8(PWMbeep_COMPARE2_LSB_PTR));
    }

#endif /* (PWMbeep_UseOneCompareMode) */


/*******************************************************************************
* Function Name: PWMbeep_ReadPeriod
********************************************************************************
*
* Summary:
*  Reads the period value used by the PWM hardware.
*
* Parameters:
*  None
*
* Return:
*  uint8/16: Period value
*
*******************************************************************************/
uint8 PWMbeep_ReadPeriod(void) 
{
    #if(PWMbeep_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(PWMbeep_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(PWMbeep_PERIOD_LSB_PTR));
    #endif /* (PWMbeep_UsingFixedFunction) */
}

#if ( PWMbeep_KillModeMinTime)


    /*******************************************************************************
    * Function Name: PWMbeep_WriteKillTime
    ********************************************************************************
    *
    * Summary:
    *  Writes the kill time value used by the hardware when the Kill Mode
    *  is set to Minimum Time.
    *
    * Parameters:
    *  uint8: Minimum Time kill counts
    *
    * Return:
    *  None
    *
    *******************************************************************************/
    void PWMbeep_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(PWMbeep_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: PWMbeep_ReadKillTime
    ********************************************************************************
    *
    * Summary:
    *  Reads the kill time value used by the hardware when the Kill Mode is set
    *  to Minimum Time.
    *
    * Parameters:
    *  None
    *
    * Return:
    *  uint8: The current Minimum Time kill counts
    *
    *******************************************************************************/
    uint8 PWMbeep_ReadKillTime(void) 
    {
        return (CY_GET_REG8(PWMbeep_KILLMODEMINTIME_PTR));
    }

#endif /* ( PWMbeep_KillModeMinTime) */

/* [] END OF FILE */
