/*******************************************************************************
* File Name: PWMbeep.h
* Version 3.30
*
* Description:
*  Contains the prototypes and constants for the functions available to the
*  PWM user module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(CY_PWM_PWMbeep_H)
#define CY_PWM_PWMbeep_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 PWMbeep_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define PWMbeep_Resolution                     (8u)
#define PWMbeep_UsingFixedFunction             (0u)
#define PWMbeep_DeadBandMode                   (0u)
#define PWMbeep_KillModeMinTime                (0u)
#define PWMbeep_KillMode                       (0u)
#define PWMbeep_PWMMode                        (0u)
#define PWMbeep_PWMModeIsCenterAligned         (0u)
#define PWMbeep_DeadBandUsed                   (0u)
#define PWMbeep_DeadBand2_4                    (0u)

#if !defined(PWMbeep_PWMUDB_genblk8_stsreg__REMOVED)
    #define PWMbeep_UseStatus                  (1u)
#else
    #define PWMbeep_UseStatus                  (0u)
#endif /* !defined(PWMbeep_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(PWMbeep_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define PWMbeep_UseControl                 (1u)
#else
    #define PWMbeep_UseControl                 (0u)
#endif /* !defined(PWMbeep_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define PWMbeep_UseOneCompareMode              (1u)
#define PWMbeep_MinimumKillTime                (1u)
#define PWMbeep_EnableMode                     (0u)

#define PWMbeep_CompareMode1SW                 (0u)
#define PWMbeep_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define PWMbeep__B_PWM__DISABLED 0
#define PWMbeep__B_PWM__ASYNCHRONOUS 1
#define PWMbeep__B_PWM__SINGLECYCLE 2
#define PWMbeep__B_PWM__LATCHED 3
#define PWMbeep__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define PWMbeep__B_PWM__DBMDISABLED 0
#define PWMbeep__B_PWM__DBM_2_4_CLOCKS 1
#define PWMbeep__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define PWMbeep__B_PWM__ONE_OUTPUT 0
#define PWMbeep__B_PWM__TWO_OUTPUTS 1
#define PWMbeep__B_PWM__DUAL_EDGE 2
#define PWMbeep__B_PWM__CENTER_ALIGN 3
#define PWMbeep__B_PWM__DITHER 5
#define PWMbeep__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define PWMbeep__B_PWM__LESS_THAN 1
#define PWMbeep__B_PWM__LESS_THAN_OR_EQUAL 2
#define PWMbeep__B_PWM__GREATER_THAN 3
#define PWMbeep__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define PWMbeep__B_PWM__EQUAL 0
#define PWMbeep__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!PWMbeep_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!PWMbeep_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!PWMbeep_PWMModeIsCenterAligned) */
        #if (PWMbeep_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (PWMbeep_UseStatus) */

        /* Backup for Deadband parameters */
        #if(PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_256_CLOCKS || \
            PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(PWMbeep_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (PWMbeep_KillModeMinTime) */

        /* Backup control register */
        #if(PWMbeep_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (PWMbeep_UseControl) */

    #endif /* (!PWMbeep_UsingFixedFunction) */

}PWMbeep_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    PWMbeep_Start(void) ;
void    PWMbeep_Stop(void) ;

#if (PWMbeep_UseStatus || PWMbeep_UsingFixedFunction)
    void  PWMbeep_SetInterruptMode(uint8 interruptMode) ;
    uint8 PWMbeep_ReadStatusRegister(void) ;
#endif /* (PWMbeep_UseStatus || PWMbeep_UsingFixedFunction) */

#define PWMbeep_GetInterruptSource() PWMbeep_ReadStatusRegister()

#if (PWMbeep_UseControl)
    uint8 PWMbeep_ReadControlRegister(void) ;
    void  PWMbeep_WriteControlRegister(uint8 control)
          ;
#endif /* (PWMbeep_UseControl) */

#if (PWMbeep_UseOneCompareMode)
   #if (PWMbeep_CompareMode1SW)
       void    PWMbeep_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (PWMbeep_CompareMode1SW) */
#else
    #if (PWMbeep_CompareMode1SW)
        void    PWMbeep_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (PWMbeep_CompareMode1SW) */
    #if (PWMbeep_CompareMode2SW)
        void    PWMbeep_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (PWMbeep_CompareMode2SW) */
#endif /* (PWMbeep_UseOneCompareMode) */

#if (!PWMbeep_UsingFixedFunction)
    uint8   PWMbeep_ReadCounter(void) ;
    uint8 PWMbeep_ReadCapture(void) ;

    #if (PWMbeep_UseStatus)
            void PWMbeep_ClearFIFO(void) ;
    #endif /* (PWMbeep_UseStatus) */

    void    PWMbeep_WriteCounter(uint8 counter)
            ;
#endif /* (!PWMbeep_UsingFixedFunction) */

void    PWMbeep_WritePeriod(uint8 period)
        ;
uint8 PWMbeep_ReadPeriod(void) ;

#if (PWMbeep_UseOneCompareMode)
    void    PWMbeep_WriteCompare(uint8 compare)
            ;
    uint8 PWMbeep_ReadCompare(void) ;
#else
    void    PWMbeep_WriteCompare1(uint8 compare)
            ;
    uint8 PWMbeep_ReadCompare1(void) ;
    void    PWMbeep_WriteCompare2(uint8 compare)
            ;
    uint8 PWMbeep_ReadCompare2(void) ;
#endif /* (PWMbeep_UseOneCompareMode) */


#if (PWMbeep_DeadBandUsed)
    void    PWMbeep_WriteDeadTime(uint8 deadtime) ;
    uint8   PWMbeep_ReadDeadTime(void) ;
#endif /* (PWMbeep_DeadBandUsed) */

#if ( PWMbeep_KillModeMinTime)
    void PWMbeep_WriteKillTime(uint8 killtime) ;
    uint8 PWMbeep_ReadKillTime(void) ;
#endif /* ( PWMbeep_KillModeMinTime) */

void PWMbeep_Init(void) ;
void PWMbeep_Enable(void) ;
void PWMbeep_Sleep(void) ;
void PWMbeep_Wakeup(void) ;
void PWMbeep_SaveConfig(void) ;
void PWMbeep_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define PWMbeep_INIT_PERIOD_VALUE          (8u)
#define PWMbeep_INIT_COMPARE_VALUE1        (4u)
#define PWMbeep_INIT_COMPARE_VALUE2        (2u)
#define PWMbeep_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    PWMbeep_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMbeep_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMbeep_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMbeep_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define PWMbeep_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  PWMbeep_CTRL_CMPMODE2_SHIFT)
#define PWMbeep_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  PWMbeep_CTRL_CMPMODE1_SHIFT)
#define PWMbeep_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (PWMbeep_UsingFixedFunction)
   #define PWMbeep_PERIOD_LSB              (*(reg16 *) PWMbeep_PWMHW__PER0)
   #define PWMbeep_PERIOD_LSB_PTR          ( (reg16 *) PWMbeep_PWMHW__PER0)
   #define PWMbeep_COMPARE1_LSB            (*(reg16 *) PWMbeep_PWMHW__CNT_CMP0)
   #define PWMbeep_COMPARE1_LSB_PTR        ( (reg16 *) PWMbeep_PWMHW__CNT_CMP0)
   #define PWMbeep_COMPARE2_LSB            (0x00u)
   #define PWMbeep_COMPARE2_LSB_PTR        (0x00u)
   #define PWMbeep_COUNTER_LSB             (*(reg16 *) PWMbeep_PWMHW__CNT_CMP0)
   #define PWMbeep_COUNTER_LSB_PTR         ( (reg16 *) PWMbeep_PWMHW__CNT_CMP0)
   #define PWMbeep_CAPTURE_LSB             (*(reg16 *) PWMbeep_PWMHW__CAP0)
   #define PWMbeep_CAPTURE_LSB_PTR         ( (reg16 *) PWMbeep_PWMHW__CAP0)
   #define PWMbeep_RT1                     (*(reg8 *)  PWMbeep_PWMHW__RT1)
   #define PWMbeep_RT1_PTR                 ( (reg8 *)  PWMbeep_PWMHW__RT1)

#else
   #if (PWMbeep_Resolution == 8u) /* 8bit - PWM */

       #if(PWMbeep_PWMModeIsCenterAligned)
           #define PWMbeep_PERIOD_LSB      (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define PWMbeep_PERIOD_LSB_PTR  ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define PWMbeep_PERIOD_LSB      (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define PWMbeep_PERIOD_LSB_PTR  ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (PWMbeep_PWMModeIsCenterAligned) */

       #define PWMbeep_COMPARE1_LSB        (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define PWMbeep_COMPARE1_LSB_PTR    ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define PWMbeep_COMPARE2_LSB        (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define PWMbeep_COMPARE2_LSB_PTR    ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define PWMbeep_COUNTERCAP_LSB      (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define PWMbeep_COUNTERCAP_LSB_PTR  ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define PWMbeep_COUNTER_LSB         (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define PWMbeep_COUNTER_LSB_PTR     ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define PWMbeep_CAPTURE_LSB         (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define PWMbeep_CAPTURE_LSB_PTR     ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(PWMbeep_PWMModeIsCenterAligned)
               #define PWMbeep_PERIOD_LSB      (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define PWMbeep_PERIOD_LSB_PTR  ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define PWMbeep_PERIOD_LSB      (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define PWMbeep_PERIOD_LSB_PTR  ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (PWMbeep_PWMModeIsCenterAligned) */

            #define PWMbeep_COMPARE1_LSB       (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define PWMbeep_COMPARE1_LSB_PTR   ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define PWMbeep_COMPARE2_LSB       (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define PWMbeep_COMPARE2_LSB_PTR   ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define PWMbeep_COUNTERCAP_LSB     (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define PWMbeep_COUNTERCAP_LSB_PTR ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define PWMbeep_COUNTER_LSB        (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define PWMbeep_COUNTER_LSB_PTR    ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define PWMbeep_CAPTURE_LSB        (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define PWMbeep_CAPTURE_LSB_PTR    ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(PWMbeep_PWMModeIsCenterAligned)
               #define PWMbeep_PERIOD_LSB      (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define PWMbeep_PERIOD_LSB_PTR  ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define PWMbeep_PERIOD_LSB      (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define PWMbeep_PERIOD_LSB_PTR  ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (PWMbeep_PWMModeIsCenterAligned) */

            #define PWMbeep_COMPARE1_LSB       (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define PWMbeep_COMPARE1_LSB_PTR   ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define PWMbeep_COMPARE2_LSB       (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define PWMbeep_COMPARE2_LSB_PTR   ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define PWMbeep_COUNTERCAP_LSB     (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define PWMbeep_COUNTERCAP_LSB_PTR ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define PWMbeep_COUNTER_LSB        (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define PWMbeep_COUNTER_LSB_PTR    ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define PWMbeep_CAPTURE_LSB        (*(reg16 *) PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define PWMbeep_CAPTURE_LSB_PTR    ((reg16 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define PWMbeep_AUX_CONTROLDP1          (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define PWMbeep_AUX_CONTROLDP1_PTR      ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (PWMbeep_Resolution == 8) */

   #define PWMbeep_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define PWMbeep_AUX_CONTROLDP0          (*(reg8 *)  PWMbeep_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define PWMbeep_AUX_CONTROLDP0_PTR      ((reg8 *)   PWMbeep_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (PWMbeep_UsingFixedFunction) */

#if(PWMbeep_KillModeMinTime )
    #define PWMbeep_KILLMODEMINTIME        (*(reg8 *)  PWMbeep_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define PWMbeep_KILLMODEMINTIME_PTR    ((reg8 *)   PWMbeep_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (PWMbeep_KillModeMinTime ) */

#if(PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_256_CLOCKS)
    #define PWMbeep_DEADBAND_COUNT         (*(reg8 *)  PWMbeep_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMbeep_DEADBAND_COUNT_PTR     ((reg8 *)   PWMbeep_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMbeep_DEADBAND_LSB_PTR       ((reg8 *)   PWMbeep_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define PWMbeep_DEADBAND_LSB           (*(reg8 *)  PWMbeep_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (PWMbeep_UsingFixedFunction)
        #define PWMbeep_DEADBAND_COUNT         (*(reg8 *)  PWMbeep_PWMHW__CFG0)
        #define PWMbeep_DEADBAND_COUNT_PTR     ((reg8 *)   PWMbeep_PWMHW__CFG0)
        #define PWMbeep_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMbeep_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define PWMbeep_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define PWMbeep_DEADBAND_COUNT         (*(reg8 *)  PWMbeep_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMbeep_DEADBAND_COUNT_PTR     ((reg8 *)   PWMbeep_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMbeep_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMbeep_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define PWMbeep_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (PWMbeep_UsingFixedFunction) */
#endif /* (PWMbeep_DeadBandMode == PWMbeep__B_PWM__DBM_256_CLOCKS) */



#if (PWMbeep_UsingFixedFunction)
    #define PWMbeep_STATUS                 (*(reg8 *) PWMbeep_PWMHW__SR0)
    #define PWMbeep_STATUS_PTR             ((reg8 *) PWMbeep_PWMHW__SR0)
    #define PWMbeep_STATUS_MASK            (*(reg8 *) PWMbeep_PWMHW__SR0)
    #define PWMbeep_STATUS_MASK_PTR        ((reg8 *) PWMbeep_PWMHW__SR0)
    #define PWMbeep_CONTROL                (*(reg8 *) PWMbeep_PWMHW__CFG0)
    #define PWMbeep_CONTROL_PTR            ((reg8 *) PWMbeep_PWMHW__CFG0)
    #define PWMbeep_CONTROL2               (*(reg8 *) PWMbeep_PWMHW__CFG1)
    #define PWMbeep_CONTROL3               (*(reg8 *) PWMbeep_PWMHW__CFG2)
    #define PWMbeep_GLOBAL_ENABLE          (*(reg8 *) PWMbeep_PWMHW__PM_ACT_CFG)
    #define PWMbeep_GLOBAL_ENABLE_PTR      ( (reg8 *) PWMbeep_PWMHW__PM_ACT_CFG)
    #define PWMbeep_GLOBAL_STBY_ENABLE     (*(reg8 *) PWMbeep_PWMHW__PM_STBY_CFG)
    #define PWMbeep_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) PWMbeep_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define PWMbeep_BLOCK_EN_MASK          (PWMbeep_PWMHW__PM_ACT_MSK)
    #define PWMbeep_BLOCK_STBY_EN_MASK     (PWMbeep_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define PWMbeep_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define PWMbeep_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define PWMbeep_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define PWMbeep_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define PWMbeep_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define PWMbeep_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define PWMbeep_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMbeep_CTRL_ENABLE_SHIFT)
    #define PWMbeep_CTRL_RESET             (uint8)((uint8)0x01u << PWMbeep_CTRL_RESET_SHIFT)
    #define PWMbeep_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMbeep_CTRL_CMPMODE2_SHIFT)
    #define PWMbeep_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMbeep_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define PWMbeep_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define PWMbeep_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << PWMbeep_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define PWMbeep_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define PWMbeep_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define PWMbeep_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMbeep_STATUS_TC_INT_EN_MASK_SHIFT            (PWMbeep_STATUS_TC_SHIFT - 4u)
    #define PWMbeep_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMbeep_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMbeep_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define PWMbeep_STATUS_TC              (uint8)((uint8)0x01u << PWMbeep_STATUS_TC_SHIFT)
    #define PWMbeep_STATUS_CMP1            (uint8)((uint8)0x01u << PWMbeep_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define PWMbeep_STATUS_TC_INT_EN_MASK              (uint8)((uint8)PWMbeep_STATUS_TC >> 4u)
    #define PWMbeep_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)PWMbeep_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define PWMbeep_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define PWMbeep_RT1_MASK              (uint8)((uint8)0x03u << PWMbeep_RT1_SHIFT)
    #define PWMbeep_SYNC                  (uint8)((uint8)0x03u << PWMbeep_RT1_SHIFT)
    #define PWMbeep_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define PWMbeep_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << PWMbeep_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define PWMbeep_SYNCDSI_EN            (uint8)((uint8)0x0Fu << PWMbeep_SYNCDSI_SHIFT)


#else
    #define PWMbeep_STATUS                (*(reg8 *)   PWMbeep_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMbeep_STATUS_PTR            ((reg8 *)    PWMbeep_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMbeep_STATUS_MASK           (*(reg8 *)   PWMbeep_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMbeep_STATUS_MASK_PTR       ((reg8 *)    PWMbeep_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMbeep_STATUS_AUX_CTRL       (*(reg8 *)   PWMbeep_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define PWMbeep_CONTROL               (*(reg8 *)   PWMbeep_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define PWMbeep_CONTROL_PTR           ((reg8 *)    PWMbeep_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define PWMbeep_CTRL_ENABLE_SHIFT      (0x07u)
    #define PWMbeep_CTRL_RESET_SHIFT       (0x06u)
    #define PWMbeep_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define PWMbeep_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define PWMbeep_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define PWMbeep_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMbeep_CTRL_ENABLE_SHIFT)
    #define PWMbeep_CTRL_RESET             (uint8)((uint8)0x01u << PWMbeep_CTRL_RESET_SHIFT)
    #define PWMbeep_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMbeep_CTRL_CMPMODE2_SHIFT)
    #define PWMbeep_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMbeep_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define PWMbeep_STATUS_KILL_SHIFT          (0x05u)
    #define PWMbeep_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define PWMbeep_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define PWMbeep_STATUS_TC_SHIFT            (0x02u)
    #define PWMbeep_STATUS_CMP2_SHIFT          (0x01u)
    #define PWMbeep_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define PWMbeep_STATUS_KILL_INT_EN_MASK_SHIFT          (PWMbeep_STATUS_KILL_SHIFT)
    #define PWMbeep_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (PWMbeep_STATUS_FIFONEMPTY_SHIFT)
    #define PWMbeep_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (PWMbeep_STATUS_FIFOFULL_SHIFT)
    #define PWMbeep_STATUS_TC_INT_EN_MASK_SHIFT            (PWMbeep_STATUS_TC_SHIFT)
    #define PWMbeep_STATUS_CMP2_INT_EN_MASK_SHIFT          (PWMbeep_STATUS_CMP2_SHIFT)
    #define PWMbeep_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMbeep_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define PWMbeep_STATUS_KILL            (uint8)((uint8)0x00u << PWMbeep_STATUS_KILL_SHIFT )
    #define PWMbeep_STATUS_FIFOFULL        (uint8)((uint8)0x01u << PWMbeep_STATUS_FIFOFULL_SHIFT)
    #define PWMbeep_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << PWMbeep_STATUS_FIFONEMPTY_SHIFT)
    #define PWMbeep_STATUS_TC              (uint8)((uint8)0x01u << PWMbeep_STATUS_TC_SHIFT)
    #define PWMbeep_STATUS_CMP2            (uint8)((uint8)0x01u << PWMbeep_STATUS_CMP2_SHIFT)
    #define PWMbeep_STATUS_CMP1            (uint8)((uint8)0x01u << PWMbeep_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define PWMbeep_STATUS_KILL_INT_EN_MASK            (PWMbeep_STATUS_KILL)
    #define PWMbeep_STATUS_FIFOFULL_INT_EN_MASK        (PWMbeep_STATUS_FIFOFULL)
    #define PWMbeep_STATUS_FIFONEMPTY_INT_EN_MASK      (PWMbeep_STATUS_FIFONEMPTY)
    #define PWMbeep_STATUS_TC_INT_EN_MASK              (PWMbeep_STATUS_TC)
    #define PWMbeep_STATUS_CMP2_INT_EN_MASK            (PWMbeep_STATUS_CMP2)
    #define PWMbeep_STATUS_CMP1_INT_EN_MASK            (PWMbeep_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define PWMbeep_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define PWMbeep_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define PWMbeep_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define PWMbeep_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define PWMbeep_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* PWMbeep_UsingFixedFunction */

#endif  /* CY_PWM_PWMbeep_H */


/* [] END OF FILE */
