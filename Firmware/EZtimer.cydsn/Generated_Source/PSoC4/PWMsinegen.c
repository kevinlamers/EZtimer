/*******************************************************************************
* File Name: PWMsinegen.c
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

#include "PWMsinegen.h"

/* Error message for removed <resource> through optimization */
#ifdef PWMsinegen_PWMUDB_genblk1_ctrlreg__REMOVED
    #error PWM_v3_30 detected with a constant 0 for the enable or \
         constant 1 for reset. This will prevent the component from operating.
#endif /* PWMsinegen_PWMUDB_genblk1_ctrlreg__REMOVED */

uint8 PWMsinegen_initVar = 0u;


/*******************************************************************************
* Function Name: PWMsinegen_Start
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
*  PWMsinegen_initVar: Is modified when this function is called for the
*   first time. Is used to ensure that initialization happens only once.
*
*******************************************************************************/
void PWMsinegen_Start(void) 
{
    /* If not Initialized then initialize all required hardware and software */
    if(PWMsinegen_initVar == 0u)
    {
        PWMsinegen_Init();
        PWMsinegen_initVar = 1u;
    }
    PWMsinegen_Enable();

}


/*******************************************************************************
* Function Name: PWMsinegen_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the
*  customizer of the component placed onto schematic. Usually called in
*  PWMsinegen_Start().
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void PWMsinegen_Init(void) 
{
    #if (PWMsinegen_UsingFixedFunction || PWMsinegen_UseControl)
        uint8 ctrl;
    #endif /* (PWMsinegen_UsingFixedFunction || PWMsinegen_UseControl) */

    #if(!PWMsinegen_UsingFixedFunction)
        #if(PWMsinegen_UseStatus)
            /* Interrupt State Backup for Critical Region*/
            uint8 PWMsinegen_interruptState;
        #endif /* (PWMsinegen_UseStatus) */
    #endif /* (!PWMsinegen_UsingFixedFunction) */

    #if (PWMsinegen_UsingFixedFunction)
        /* You are allowed to write the compare value (FF only) */
        PWMsinegen_CONTROL |= PWMsinegen_CFG0_MODE;
        #if (PWMsinegen_DeadBand2_4)
            PWMsinegen_CONTROL |= PWMsinegen_CFG0_DB;
        #endif /* (PWMsinegen_DeadBand2_4) */

        ctrl = PWMsinegen_CONTROL3 & ((uint8 )(~PWMsinegen_CTRL_CMPMODE1_MASK));
        PWMsinegen_CONTROL3 = ctrl | PWMsinegen_DEFAULT_COMPARE1_MODE;

         /* Clear and Set SYNCTC and SYNCCMP bits of RT1 register */
        PWMsinegen_RT1 &= ((uint8)(~PWMsinegen_RT1_MASK));
        PWMsinegen_RT1 |= PWMsinegen_SYNC;

        /*Enable DSI Sync all all inputs of the PWM*/
        PWMsinegen_RT1 &= ((uint8)(~PWMsinegen_SYNCDSI_MASK));
        PWMsinegen_RT1 |= PWMsinegen_SYNCDSI_EN;

    #elif (PWMsinegen_UseControl)
        /* Set the default compare mode defined in the parameter */
        ctrl = PWMsinegen_CONTROL & ((uint8)(~PWMsinegen_CTRL_CMPMODE2_MASK)) &
                ((uint8)(~PWMsinegen_CTRL_CMPMODE1_MASK));
        PWMsinegen_CONTROL = ctrl | PWMsinegen_DEFAULT_COMPARE2_MODE |
                                   PWMsinegen_DEFAULT_COMPARE1_MODE;
    #endif /* (PWMsinegen_UsingFixedFunction) */

    #if (!PWMsinegen_UsingFixedFunction)
        #if (PWMsinegen_Resolution == 8)
            /* Set FIFO 0 to 1 byte register for period*/
            PWMsinegen_AUX_CONTROLDP0 |= (PWMsinegen_AUX_CTRL_FIFO0_CLR);
        #else /* (PWMsinegen_Resolution == 16)*/
            /* Set FIFO 0 to 1 byte register for period */
            PWMsinegen_AUX_CONTROLDP0 |= (PWMsinegen_AUX_CTRL_FIFO0_CLR);
            PWMsinegen_AUX_CONTROLDP1 |= (PWMsinegen_AUX_CTRL_FIFO0_CLR);
        #endif /* (PWMsinegen_Resolution == 8) */

        PWMsinegen_WriteCounter(PWMsinegen_INIT_PERIOD_VALUE);
    #endif /* (!PWMsinegen_UsingFixedFunction) */

    PWMsinegen_WritePeriod(PWMsinegen_INIT_PERIOD_VALUE);

        #if (PWMsinegen_UseOneCompareMode)
            PWMsinegen_WriteCompare(PWMsinegen_INIT_COMPARE_VALUE1);
        #else
            PWMsinegen_WriteCompare1(PWMsinegen_INIT_COMPARE_VALUE1);
            PWMsinegen_WriteCompare2(PWMsinegen_INIT_COMPARE_VALUE2);
        #endif /* (PWMsinegen_UseOneCompareMode) */

        #if (PWMsinegen_KillModeMinTime)
            PWMsinegen_WriteKillTime(PWMsinegen_MinimumKillTime);
        #endif /* (PWMsinegen_KillModeMinTime) */

        #if (PWMsinegen_DeadBandUsed)
            PWMsinegen_WriteDeadTime(PWMsinegen_INIT_DEAD_TIME);
        #endif /* (PWMsinegen_DeadBandUsed) */

    #if (PWMsinegen_UseStatus || PWMsinegen_UsingFixedFunction)
        PWMsinegen_SetInterruptMode(PWMsinegen_INIT_INTERRUPTS_MODE);
    #endif /* (PWMsinegen_UseStatus || PWMsinegen_UsingFixedFunction) */

    #if (PWMsinegen_UsingFixedFunction)
        /* Globally Enable the Fixed Function Block chosen */
        PWMsinegen_GLOBAL_ENABLE |= PWMsinegen_BLOCK_EN_MASK;
        /* Set the Interrupt source to come from the status register */
        PWMsinegen_CONTROL2 |= PWMsinegen_CTRL2_IRQ_SEL;
    #else
        #if(PWMsinegen_UseStatus)

            /* CyEnterCriticalRegion and CyExitCriticalRegion are used to mark following region critical*/
            /* Enter Critical Region*/
            PWMsinegen_interruptState = CyEnterCriticalSection();
            /* Use the interrupt output of the status register for IRQ output */
            PWMsinegen_STATUS_AUX_CTRL |= PWMsinegen_STATUS_ACTL_INT_EN_MASK;

             /* Exit Critical Region*/
            CyExitCriticalSection(PWMsinegen_interruptState);

            /* Clear the FIFO to enable the PWMsinegen_STATUS_FIFOFULL
                   bit to be set on FIFO full. */
            PWMsinegen_ClearFIFO();
        #endif /* (PWMsinegen_UseStatus) */
    #endif /* (PWMsinegen_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMsinegen_Enable
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
void PWMsinegen_Enable(void) 
{
    /* Globally Enable the Fixed Function Block chosen */
    #if (PWMsinegen_UsingFixedFunction)
        PWMsinegen_GLOBAL_ENABLE |= PWMsinegen_BLOCK_EN_MASK;
        PWMsinegen_GLOBAL_STBY_ENABLE |= PWMsinegen_BLOCK_STBY_EN_MASK;
    #endif /* (PWMsinegen_UsingFixedFunction) */

    /* Enable the PWM from the control register  */
    #if (PWMsinegen_UseControl || PWMsinegen_UsingFixedFunction)
        PWMsinegen_CONTROL |= PWMsinegen_CTRL_ENABLE;
    #endif /* (PWMsinegen_UseControl || PWMsinegen_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: PWMsinegen_Stop
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
void PWMsinegen_Stop(void) 
{
    #if (PWMsinegen_UseControl || PWMsinegen_UsingFixedFunction)
        PWMsinegen_CONTROL &= ((uint8)(~PWMsinegen_CTRL_ENABLE));
    #endif /* (PWMsinegen_UseControl || PWMsinegen_UsingFixedFunction) */

    /* Globally disable the Fixed Function Block chosen */
    #if (PWMsinegen_UsingFixedFunction)
        PWMsinegen_GLOBAL_ENABLE &= ((uint8)(~PWMsinegen_BLOCK_EN_MASK));
        PWMsinegen_GLOBAL_STBY_ENABLE &= ((uint8)(~PWMsinegen_BLOCK_STBY_EN_MASK));
    #endif /* (PWMsinegen_UsingFixedFunction) */
}

#if (PWMsinegen_UseOneCompareMode)
    #if (PWMsinegen_CompareMode1SW)


        /*******************************************************************************
        * Function Name: PWMsinegen_SetCompareMode
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
        void PWMsinegen_SetCompareMode(uint8 comparemode) 
        {
            #if(PWMsinegen_UsingFixedFunction)

                #if(0 != PWMsinegen_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMsinegen_CTRL_CMPMODE1_SHIFT));
                #else
                    uint8 comparemodemasked = comparemode;
                #endif /* (0 != PWMsinegen_CTRL_CMPMODE1_SHIFT) */

                PWMsinegen_CONTROL3 &= ((uint8)(~PWMsinegen_CTRL_CMPMODE1_MASK)); /*Clear Existing Data */
                PWMsinegen_CONTROL3 |= comparemodemasked;

            #elif (PWMsinegen_UseControl)

                #if(0 != PWMsinegen_CTRL_CMPMODE1_SHIFT)
                    uint8 comparemode1masked = ((uint8)((uint8)comparemode << PWMsinegen_CTRL_CMPMODE1_SHIFT)) &
                                                PWMsinegen_CTRL_CMPMODE1_MASK;
                #else
                    uint8 comparemode1masked = comparemode & PWMsinegen_CTRL_CMPMODE1_MASK;
                #endif /* (0 != PWMsinegen_CTRL_CMPMODE1_SHIFT) */

                #if(0 != PWMsinegen_CTRL_CMPMODE2_SHIFT)
                    uint8 comparemode2masked = ((uint8)((uint8)comparemode << PWMsinegen_CTRL_CMPMODE2_SHIFT)) &
                                               PWMsinegen_CTRL_CMPMODE2_MASK;
                #else
                    uint8 comparemode2masked = comparemode & PWMsinegen_CTRL_CMPMODE2_MASK;
                #endif /* (0 != PWMsinegen_CTRL_CMPMODE2_SHIFT) */

                /*Clear existing mode */
                PWMsinegen_CONTROL &= ((uint8)(~(PWMsinegen_CTRL_CMPMODE1_MASK |
                                            PWMsinegen_CTRL_CMPMODE2_MASK)));
                PWMsinegen_CONTROL |= (comparemode1masked | comparemode2masked);

            #else
                uint8 temp = comparemode;
            #endif /* (PWMsinegen_UsingFixedFunction) */
        }
    #endif /* PWMsinegen_CompareMode1SW */

#else /* UseOneCompareMode */

    #if (PWMsinegen_CompareMode1SW)


        /*******************************************************************************
        * Function Name: PWMsinegen_SetCompareMode1
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
        void PWMsinegen_SetCompareMode1(uint8 comparemode) 
        {
            #if(0 != PWMsinegen_CTRL_CMPMODE1_SHIFT)
                uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMsinegen_CTRL_CMPMODE1_SHIFT)) &
                                           PWMsinegen_CTRL_CMPMODE1_MASK;
            #else
                uint8 comparemodemasked = comparemode & PWMsinegen_CTRL_CMPMODE1_MASK;
            #endif /* (0 != PWMsinegen_CTRL_CMPMODE1_SHIFT) */

            #if (PWMsinegen_UseControl)
                PWMsinegen_CONTROL &= ((uint8)(~PWMsinegen_CTRL_CMPMODE1_MASK)); /*Clear existing mode */
                PWMsinegen_CONTROL |= comparemodemasked;
            #endif /* (PWMsinegen_UseControl) */
        }
    #endif /* PWMsinegen_CompareMode1SW */

#if (PWMsinegen_CompareMode2SW)


    /*******************************************************************************
    * Function Name: PWMsinegen_SetCompareMode2
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
    void PWMsinegen_SetCompareMode2(uint8 comparemode) 
    {

        #if(0 != PWMsinegen_CTRL_CMPMODE2_SHIFT)
            uint8 comparemodemasked = ((uint8)((uint8)comparemode << PWMsinegen_CTRL_CMPMODE2_SHIFT)) &
                                                 PWMsinegen_CTRL_CMPMODE2_MASK;
        #else
            uint8 comparemodemasked = comparemode & PWMsinegen_CTRL_CMPMODE2_MASK;
        #endif /* (0 != PWMsinegen_CTRL_CMPMODE2_SHIFT) */

        #if (PWMsinegen_UseControl)
            PWMsinegen_CONTROL &= ((uint8)(~PWMsinegen_CTRL_CMPMODE2_MASK)); /*Clear existing mode */
            PWMsinegen_CONTROL |= comparemodemasked;
        #endif /* (PWMsinegen_UseControl) */
    }
    #endif /*PWMsinegen_CompareMode2SW */

#endif /* UseOneCompareMode */


#if (!PWMsinegen_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMsinegen_WriteCounter
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
    void PWMsinegen_WriteCounter(uint8 counter) \
                                       
    {
        CY_SET_REG8(PWMsinegen_COUNTER_LSB_PTR, counter);
    }


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadCounter
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
    uint8 PWMsinegen_ReadCounter(void) 
    {
        /* Force capture by reading Accumulator */
        /* Must first do a software capture to be able to read the counter */
        /* It is up to the user code to make sure there isn't already captured data in the FIFO */
          (void)CY_GET_REG8(PWMsinegen_COUNTERCAP_LSB_PTR_8BIT);

        /* Read the data from the FIFO */
        return (CY_GET_REG8(PWMsinegen_CAPTURE_LSB_PTR));
    }

    #if (PWMsinegen_UseStatus)


        /*******************************************************************************
        * Function Name: PWMsinegen_ClearFIFO
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
        void PWMsinegen_ClearFIFO(void) 
        {
            while(0u != (PWMsinegen_ReadStatusRegister() & PWMsinegen_STATUS_FIFONEMPTY))
            {
                (void)PWMsinegen_ReadCapture();
            }
        }

    #endif /* PWMsinegen_UseStatus */

#endif /* !PWMsinegen_UsingFixedFunction */


/*******************************************************************************
* Function Name: PWMsinegen_WritePeriod
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
void PWMsinegen_WritePeriod(uint8 period) 
{
    #if(PWMsinegen_UsingFixedFunction)
        CY_SET_REG16(PWMsinegen_PERIOD_LSB_PTR, (uint16)period);
    #else
        CY_SET_REG8(PWMsinegen_PERIOD_LSB_PTR, period);
    #endif /* (PWMsinegen_UsingFixedFunction) */
}

#if (PWMsinegen_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWMsinegen_WriteCompare
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
    void PWMsinegen_WriteCompare(uint8 compare) \
                                       
    {
        #if(PWMsinegen_UsingFixedFunction)
            CY_SET_REG16(PWMsinegen_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(PWMsinegen_COMPARE1_LSB_PTR, compare);
        #endif /* (PWMsinegen_UsingFixedFunction) */

        #if (PWMsinegen_PWMMode == PWMsinegen__B_PWM__DITHER)
            #if(PWMsinegen_UsingFixedFunction)
                CY_SET_REG16(PWMsinegen_COMPARE2_LSB_PTR, (uint16)(compare + 1u));
            #else
                CY_SET_REG8(PWMsinegen_COMPARE2_LSB_PTR, (compare + 1u));
            #endif /* (PWMsinegen_UsingFixedFunction) */
        #endif /* (PWMsinegen_PWMMode == PWMsinegen__B_PWM__DITHER) */
    }


#else


    /*******************************************************************************
    * Function Name: PWMsinegen_WriteCompare1
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
    void PWMsinegen_WriteCompare1(uint8 compare) \
                                        
    {
        #if(PWMsinegen_UsingFixedFunction)
            CY_SET_REG16(PWMsinegen_COMPARE1_LSB_PTR, (uint16)compare);
        #else
            CY_SET_REG8(PWMsinegen_COMPARE1_LSB_PTR, compare);
        #endif /* (PWMsinegen_UsingFixedFunction) */
    }


    /*******************************************************************************
    * Function Name: PWMsinegen_WriteCompare2
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
    void PWMsinegen_WriteCompare2(uint8 compare) \
                                        
    {
        #if(PWMsinegen_UsingFixedFunction)
            CY_SET_REG16(PWMsinegen_COMPARE2_LSB_PTR, compare);
        #else
            CY_SET_REG8(PWMsinegen_COMPARE2_LSB_PTR, compare);
        #endif /* (PWMsinegen_UsingFixedFunction) */
    }
#endif /* UseOneCompareMode */

#if (PWMsinegen_DeadBandUsed)


    /*******************************************************************************
    * Function Name: PWMsinegen_WriteDeadTime
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
    void PWMsinegen_WriteDeadTime(uint8 deadtime) 
    {
        /* If using the Dead Band 1-255 mode then just write the register */
        #if(!PWMsinegen_DeadBand2_4)
            CY_SET_REG8(PWMsinegen_DEADBAND_COUNT_PTR, deadtime);
        #else
            /* Otherwise the data has to be masked and offset */
            /* Clear existing data */
            PWMsinegen_DEADBAND_COUNT &= ((uint8)(~PWMsinegen_DEADBAND_COUNT_MASK));

            /* Set new dead time */
            #if(PWMsinegen_DEADBAND_COUNT_SHIFT)
                PWMsinegen_DEADBAND_COUNT |= ((uint8)((uint8)deadtime << PWMsinegen_DEADBAND_COUNT_SHIFT)) &
                                                    PWMsinegen_DEADBAND_COUNT_MASK;
            #else
                PWMsinegen_DEADBAND_COUNT |= deadtime & PWMsinegen_DEADBAND_COUNT_MASK;
            #endif /* (PWMsinegen_DEADBAND_COUNT_SHIFT) */

        #endif /* (!PWMsinegen_DeadBand2_4) */
    }


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadDeadTime
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
    uint8 PWMsinegen_ReadDeadTime(void) 
    {
        /* If using the Dead Band 1-255 mode then just read the register */
        #if(!PWMsinegen_DeadBand2_4)
            return (CY_GET_REG8(PWMsinegen_DEADBAND_COUNT_PTR));
        #else

            /* Otherwise the data has to be masked and offset */
            #if(PWMsinegen_DEADBAND_COUNT_SHIFT)
                return ((uint8)(((uint8)(PWMsinegen_DEADBAND_COUNT & PWMsinegen_DEADBAND_COUNT_MASK)) >>
                                                                           PWMsinegen_DEADBAND_COUNT_SHIFT));
            #else
                return (PWMsinegen_DEADBAND_COUNT & PWMsinegen_DEADBAND_COUNT_MASK);
            #endif /* (PWMsinegen_DEADBAND_COUNT_SHIFT) */
        #endif /* (!PWMsinegen_DeadBand2_4) */
    }
#endif /* DeadBandUsed */

#if (PWMsinegen_UseStatus || PWMsinegen_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMsinegen_SetInterruptMode
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
    void PWMsinegen_SetInterruptMode(uint8 interruptMode) 
    {
        CY_SET_REG8(PWMsinegen_STATUS_MASK_PTR, interruptMode);
    }


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadStatusRegister
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
    uint8 PWMsinegen_ReadStatusRegister(void) 
    {
        return (CY_GET_REG8(PWMsinegen_STATUS_PTR));
    }

#endif /* (PWMsinegen_UseStatus || PWMsinegen_UsingFixedFunction) */


#if (PWMsinegen_UseControl)


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadControlRegister
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
    uint8 PWMsinegen_ReadControlRegister(void) 
    {
        uint8 result;

        result = CY_GET_REG8(PWMsinegen_CONTROL_PTR);
        return (result);
    }


    /*******************************************************************************
    * Function Name: PWMsinegen_WriteControlRegister
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
    void PWMsinegen_WriteControlRegister(uint8 control) 
    {
        CY_SET_REG8(PWMsinegen_CONTROL_PTR, control);
    }

#endif /* (PWMsinegen_UseControl) */


#if (!PWMsinegen_UsingFixedFunction)


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadCapture
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
    uint8 PWMsinegen_ReadCapture(void) 
    {
        return (CY_GET_REG8(PWMsinegen_CAPTURE_LSB_PTR));
    }

#endif /* (!PWMsinegen_UsingFixedFunction) */


#if (PWMsinegen_UseOneCompareMode)


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadCompare
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
    uint8 PWMsinegen_ReadCompare(void) 
    {
        #if(PWMsinegen_UsingFixedFunction)
            return ((uint8)CY_GET_REG16(PWMsinegen_COMPARE1_LSB_PTR));
        #else
            return (CY_GET_REG8(PWMsinegen_COMPARE1_LSB_PTR));
        #endif /* (PWMsinegen_UsingFixedFunction) */
    }

#else


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadCompare1
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
    uint8 PWMsinegen_ReadCompare1(void) 
    {
        return (CY_GET_REG8(PWMsinegen_COMPARE1_LSB_PTR));
    }


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadCompare2
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
    uint8 PWMsinegen_ReadCompare2(void) 
    {
        return (CY_GET_REG8(PWMsinegen_COMPARE2_LSB_PTR));
    }

#endif /* (PWMsinegen_UseOneCompareMode) */


/*******************************************************************************
* Function Name: PWMsinegen_ReadPeriod
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
uint8 PWMsinegen_ReadPeriod(void) 
{
    #if(PWMsinegen_UsingFixedFunction)
        return ((uint8)CY_GET_REG16(PWMsinegen_PERIOD_LSB_PTR));
    #else
        return (CY_GET_REG8(PWMsinegen_PERIOD_LSB_PTR));
    #endif /* (PWMsinegen_UsingFixedFunction) */
}

#if ( PWMsinegen_KillModeMinTime)


    /*******************************************************************************
    * Function Name: PWMsinegen_WriteKillTime
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
    void PWMsinegen_WriteKillTime(uint8 killtime) 
    {
        CY_SET_REG8(PWMsinegen_KILLMODEMINTIME_PTR, killtime);
    }


    /*******************************************************************************
    * Function Name: PWMsinegen_ReadKillTime
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
    uint8 PWMsinegen_ReadKillTime(void) 
    {
        return (CY_GET_REG8(PWMsinegen_KILLMODEMINTIME_PTR));
    }

#endif /* ( PWMsinegen_KillModeMinTime) */

/* [] END OF FILE */
