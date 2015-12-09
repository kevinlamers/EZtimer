/*******************************************************************************
* File Name: PWMsinegen.h
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

#if !defined(CY_PWM_PWMsinegen_H)
#define CY_PWM_PWMsinegen_H

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h" /* For CyEnterCriticalSection() and CyExitCriticalSection() functions */

extern uint8 PWMsinegen_initVar;


/***************************************
* Conditional Compilation Parameters
***************************************/
#define PWMsinegen_Resolution                     (8u)
#define PWMsinegen_UsingFixedFunction             (0u)
#define PWMsinegen_DeadBandMode                   (0u)
#define PWMsinegen_KillModeMinTime                (0u)
#define PWMsinegen_KillMode                       (0u)
#define PWMsinegen_PWMMode                        (0u)
#define PWMsinegen_PWMModeIsCenterAligned         (0u)
#define PWMsinegen_DeadBandUsed                   (0u)
#define PWMsinegen_DeadBand2_4                    (0u)

#if !defined(PWMsinegen_PWMUDB_genblk8_stsreg__REMOVED)
    #define PWMsinegen_UseStatus                  (1u)
#else
    #define PWMsinegen_UseStatus                  (0u)
#endif /* !defined(PWMsinegen_PWMUDB_genblk8_stsreg__REMOVED) */

#if !defined(PWMsinegen_PWMUDB_genblk1_ctrlreg__REMOVED)
    #define PWMsinegen_UseControl                 (1u)
#else
    #define PWMsinegen_UseControl                 (0u)
#endif /* !defined(PWMsinegen_PWMUDB_genblk1_ctrlreg__REMOVED) */

#define PWMsinegen_UseOneCompareMode              (1u)
#define PWMsinegen_MinimumKillTime                (1u)
#define PWMsinegen_EnableMode                     (0u)

#define PWMsinegen_CompareMode1SW                 (0u)
#define PWMsinegen_CompareMode2SW                 (0u)

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component PWM_v3_30 requires cy_boot v3.0 or later
#endif /* (CY_ PSOC5LP) */

/* Use Kill Mode Enumerated Types */
#define PWMsinegen__B_PWM__DISABLED 0
#define PWMsinegen__B_PWM__ASYNCHRONOUS 1
#define PWMsinegen__B_PWM__SINGLECYCLE 2
#define PWMsinegen__B_PWM__LATCHED 3
#define PWMsinegen__B_PWM__MINTIME 4


/* Use Dead Band Mode Enumerated Types */
#define PWMsinegen__B_PWM__DBMDISABLED 0
#define PWMsinegen__B_PWM__DBM_2_4_CLOCKS 1
#define PWMsinegen__B_PWM__DBM_256_CLOCKS 2


/* Used PWM Mode Enumerated Types */
#define PWMsinegen__B_PWM__ONE_OUTPUT 0
#define PWMsinegen__B_PWM__TWO_OUTPUTS 1
#define PWMsinegen__B_PWM__DUAL_EDGE 2
#define PWMsinegen__B_PWM__CENTER_ALIGN 3
#define PWMsinegen__B_PWM__DITHER 5
#define PWMsinegen__B_PWM__HARDWARESELECT 4


/* Used PWM Compare Mode Enumerated Types */
#define PWMsinegen__B_PWM__LESS_THAN 1
#define PWMsinegen__B_PWM__LESS_THAN_OR_EQUAL 2
#define PWMsinegen__B_PWM__GREATER_THAN 3
#define PWMsinegen__B_PWM__GREATER_THAN_OR_EQUAL_TO 4
#define PWMsinegen__B_PWM__EQUAL 0
#define PWMsinegen__B_PWM__FIRMWARE 5



/***************************************
* Data Struct Definition
***************************************/


/**************************************************************************
 * Sleep Wakeup Backup structure for PWM Component
 *************************************************************************/
typedef struct
{

    uint8 PWMEnableState;

    #if(!PWMsinegen_UsingFixedFunction)
        uint8 PWMUdb;               /* PWM Current Counter value  */
        #if(!PWMsinegen_PWMModeIsCenterAligned)
            uint8 PWMPeriod;
        #endif /* (!PWMsinegen_PWMModeIsCenterAligned) */
        #if (PWMsinegen_UseStatus)
            uint8 InterruptMaskValue;   /* PWM Current Interrupt Mask */
        #endif /* (PWMsinegen_UseStatus) */

        /* Backup for Deadband parameters */
        #if(PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_256_CLOCKS || \
            PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_2_4_CLOCKS)
            uint8 PWMdeadBandValue; /* Dead Band Counter Current Value */
        #endif /* deadband count is either 2-4 clocks or 256 clocks */

        /* Backup Kill Mode Counter*/
        #if(PWMsinegen_KillModeMinTime)
            uint8 PWMKillCounterPeriod; /* Kill Mode period value */
        #endif /* (PWMsinegen_KillModeMinTime) */

        /* Backup control register */
        #if(PWMsinegen_UseControl)
            uint8 PWMControlRegister; /* PWM Control Register value */
        #endif /* (PWMsinegen_UseControl) */

    #endif /* (!PWMsinegen_UsingFixedFunction) */

}PWMsinegen_backupStruct;


/***************************************
*        Function Prototypes
 **************************************/

void    PWMsinegen_Start(void) ;
void    PWMsinegen_Stop(void) ;

#if (PWMsinegen_UseStatus || PWMsinegen_UsingFixedFunction)
    void  PWMsinegen_SetInterruptMode(uint8 interruptMode) ;
    uint8 PWMsinegen_ReadStatusRegister(void) ;
#endif /* (PWMsinegen_UseStatus || PWMsinegen_UsingFixedFunction) */

#define PWMsinegen_GetInterruptSource() PWMsinegen_ReadStatusRegister()

#if (PWMsinegen_UseControl)
    uint8 PWMsinegen_ReadControlRegister(void) ;
    void  PWMsinegen_WriteControlRegister(uint8 control)
          ;
#endif /* (PWMsinegen_UseControl) */

#if (PWMsinegen_UseOneCompareMode)
   #if (PWMsinegen_CompareMode1SW)
       void    PWMsinegen_SetCompareMode(uint8 comparemode)
               ;
   #endif /* (PWMsinegen_CompareMode1SW) */
#else
    #if (PWMsinegen_CompareMode1SW)
        void    PWMsinegen_SetCompareMode1(uint8 comparemode)
                ;
    #endif /* (PWMsinegen_CompareMode1SW) */
    #if (PWMsinegen_CompareMode2SW)
        void    PWMsinegen_SetCompareMode2(uint8 comparemode)
                ;
    #endif /* (PWMsinegen_CompareMode2SW) */
#endif /* (PWMsinegen_UseOneCompareMode) */

#if (!PWMsinegen_UsingFixedFunction)
    uint8   PWMsinegen_ReadCounter(void) ;
    uint8 PWMsinegen_ReadCapture(void) ;

    #if (PWMsinegen_UseStatus)
            void PWMsinegen_ClearFIFO(void) ;
    #endif /* (PWMsinegen_UseStatus) */

    void    PWMsinegen_WriteCounter(uint8 counter)
            ;
#endif /* (!PWMsinegen_UsingFixedFunction) */

void    PWMsinegen_WritePeriod(uint8 period)
        ;
uint8 PWMsinegen_ReadPeriod(void) ;

#if (PWMsinegen_UseOneCompareMode)
    void    PWMsinegen_WriteCompare(uint8 compare)
            ;
    uint8 PWMsinegen_ReadCompare(void) ;
#else
    void    PWMsinegen_WriteCompare1(uint8 compare)
            ;
    uint8 PWMsinegen_ReadCompare1(void) ;
    void    PWMsinegen_WriteCompare2(uint8 compare)
            ;
    uint8 PWMsinegen_ReadCompare2(void) ;
#endif /* (PWMsinegen_UseOneCompareMode) */


#if (PWMsinegen_DeadBandUsed)
    void    PWMsinegen_WriteDeadTime(uint8 deadtime) ;
    uint8   PWMsinegen_ReadDeadTime(void) ;
#endif /* (PWMsinegen_DeadBandUsed) */

#if ( PWMsinegen_KillModeMinTime)
    void PWMsinegen_WriteKillTime(uint8 killtime) ;
    uint8 PWMsinegen_ReadKillTime(void) ;
#endif /* ( PWMsinegen_KillModeMinTime) */

void PWMsinegen_Init(void) ;
void PWMsinegen_Enable(void) ;
void PWMsinegen_Sleep(void) ;
void PWMsinegen_Wakeup(void) ;
void PWMsinegen_SaveConfig(void) ;
void PWMsinegen_RestoreConfig(void) ;


/***************************************
*         Initialization Values
**************************************/
#define PWMsinegen_INIT_PERIOD_VALUE          (3u)
#define PWMsinegen_INIT_COMPARE_VALUE1        (1u)
#define PWMsinegen_INIT_COMPARE_VALUE2        (63u)
#define PWMsinegen_INIT_INTERRUPTS_MODE       (uint8)(((uint8)(0u <<   \
                                                    PWMsinegen_STATUS_TC_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMsinegen_STATUS_CMP2_INT_EN_MASK_SHIFT)) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMsinegen_STATUS_CMP1_INT_EN_MASK_SHIFT )) | \
                                                    (uint8)((uint8)(0u <<  \
                                                    PWMsinegen_STATUS_KILL_INT_EN_MASK_SHIFT )))
#define PWMsinegen_DEFAULT_COMPARE2_MODE      (uint8)((uint8)1u <<  PWMsinegen_CTRL_CMPMODE2_SHIFT)
#define PWMsinegen_DEFAULT_COMPARE1_MODE      (uint8)((uint8)1u <<  PWMsinegen_CTRL_CMPMODE1_SHIFT)
#define PWMsinegen_INIT_DEAD_TIME             (1u)


/********************************
*         Registers
******************************** */

#if (PWMsinegen_UsingFixedFunction)
   #define PWMsinegen_PERIOD_LSB              (*(reg16 *) PWMsinegen_PWMHW__PER0)
   #define PWMsinegen_PERIOD_LSB_PTR          ( (reg16 *) PWMsinegen_PWMHW__PER0)
   #define PWMsinegen_COMPARE1_LSB            (*(reg16 *) PWMsinegen_PWMHW__CNT_CMP0)
   #define PWMsinegen_COMPARE1_LSB_PTR        ( (reg16 *) PWMsinegen_PWMHW__CNT_CMP0)
   #define PWMsinegen_COMPARE2_LSB            (0x00u)
   #define PWMsinegen_COMPARE2_LSB_PTR        (0x00u)
   #define PWMsinegen_COUNTER_LSB             (*(reg16 *) PWMsinegen_PWMHW__CNT_CMP0)
   #define PWMsinegen_COUNTER_LSB_PTR         ( (reg16 *) PWMsinegen_PWMHW__CNT_CMP0)
   #define PWMsinegen_CAPTURE_LSB             (*(reg16 *) PWMsinegen_PWMHW__CAP0)
   #define PWMsinegen_CAPTURE_LSB_PTR         ( (reg16 *) PWMsinegen_PWMHW__CAP0)
   #define PWMsinegen_RT1                     (*(reg8 *)  PWMsinegen_PWMHW__RT1)
   #define PWMsinegen_RT1_PTR                 ( (reg8 *)  PWMsinegen_PWMHW__RT1)

#else
   #if (PWMsinegen_Resolution == 8u) /* 8bit - PWM */

       #if(PWMsinegen_PWMModeIsCenterAligned)
           #define PWMsinegen_PERIOD_LSB      (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__D1_REG)
           #define PWMsinegen_PERIOD_LSB_PTR  ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #else
           #define PWMsinegen_PERIOD_LSB      (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__F0_REG)
           #define PWMsinegen_PERIOD_LSB_PTR  ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u0__F0_REG)
       #endif /* (PWMsinegen_PWMModeIsCenterAligned) */

       #define PWMsinegen_COMPARE1_LSB        (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define PWMsinegen_COMPARE1_LSB_PTR    ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u0__D0_REG)
       #define PWMsinegen_COMPARE2_LSB        (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define PWMsinegen_COMPARE2_LSB_PTR    ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u0__D1_REG)
       #define PWMsinegen_COUNTERCAP_LSB      (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define PWMsinegen_COUNTERCAP_LSB_PTR  ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u0__A1_REG)
       #define PWMsinegen_COUNTER_LSB         (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define PWMsinegen_COUNTER_LSB_PTR     ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u0__A0_REG)
       #define PWMsinegen_CAPTURE_LSB         (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__F1_REG)
       #define PWMsinegen_CAPTURE_LSB_PTR     ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u0__F1_REG)

   #else
        #if(CY_PSOC3) /* 8-bit address space */
            #if(PWMsinegen_PWMModeIsCenterAligned)
               #define PWMsinegen_PERIOD_LSB      (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__D1_REG)
               #define PWMsinegen_PERIOD_LSB_PTR  ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #else
               #define PWMsinegen_PERIOD_LSB      (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__F0_REG)
               #define PWMsinegen_PERIOD_LSB_PTR  ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__F0_REG)
            #endif /* (PWMsinegen_PWMModeIsCenterAligned) */

            #define PWMsinegen_COMPARE1_LSB       (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define PWMsinegen_COMPARE1_LSB_PTR   ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__D0_REG)
            #define PWMsinegen_COMPARE2_LSB       (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define PWMsinegen_COMPARE2_LSB_PTR   ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__D1_REG)
            #define PWMsinegen_COUNTERCAP_LSB     (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define PWMsinegen_COUNTERCAP_LSB_PTR ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__A1_REG)
            #define PWMsinegen_COUNTER_LSB        (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define PWMsinegen_COUNTER_LSB_PTR    ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__A0_REG)
            #define PWMsinegen_CAPTURE_LSB        (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__F1_REG)
            #define PWMsinegen_CAPTURE_LSB_PTR    ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__F1_REG)
        #else
            #if(PWMsinegen_PWMModeIsCenterAligned)
               #define PWMsinegen_PERIOD_LSB      (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
               #define PWMsinegen_PERIOD_LSB_PTR  ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #else
               #define PWMsinegen_PERIOD_LSB      (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
               #define PWMsinegen_PERIOD_LSB_PTR  ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_F0_REG)
            #endif /* (PWMsinegen_PWMModeIsCenterAligned) */

            #define PWMsinegen_COMPARE1_LSB       (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define PWMsinegen_COMPARE1_LSB_PTR   ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_D0_REG)
            #define PWMsinegen_COMPARE2_LSB       (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define PWMsinegen_COMPARE2_LSB_PTR   ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_D1_REG)
            #define PWMsinegen_COUNTERCAP_LSB     (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define PWMsinegen_COUNTERCAP_LSB_PTR ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_A1_REG)
            #define PWMsinegen_COUNTER_LSB        (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define PWMsinegen_COUNTER_LSB_PTR    ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_A0_REG)
            #define PWMsinegen_CAPTURE_LSB        (*(reg16 *) PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
            #define PWMsinegen_CAPTURE_LSB_PTR    ((reg16 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__16BIT_F1_REG)
        #endif /* (CY_PSOC3) */

       #define PWMsinegen_AUX_CONTROLDP1          (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)
       #define PWMsinegen_AUX_CONTROLDP1_PTR      ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u1__DP_AUX_CTL_REG)

   #endif /* (PWMsinegen_Resolution == 8) */

   #define PWMsinegen_COUNTERCAP_LSB_PTR_8BIT ( (reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__A1_REG)
   #define PWMsinegen_AUX_CONTROLDP0          (*(reg8 *)  PWMsinegen_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)
   #define PWMsinegen_AUX_CONTROLDP0_PTR      ((reg8 *)   PWMsinegen_PWMUDB_sP8_pwmdp_u0__DP_AUX_CTL_REG)

#endif /* (PWMsinegen_UsingFixedFunction) */

#if(PWMsinegen_KillModeMinTime )
    #define PWMsinegen_KILLMODEMINTIME        (*(reg8 *)  PWMsinegen_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    #define PWMsinegen_KILLMODEMINTIME_PTR    ((reg8 *)   PWMsinegen_PWMUDB_sKM_killmodecounterdp_u0__D0_REG)
    /* Fixed Function Block has no Kill Mode parameters because it is Asynchronous only */
#endif /* (PWMsinegen_KillModeMinTime ) */

#if(PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_256_CLOCKS)
    #define PWMsinegen_DEADBAND_COUNT         (*(reg8 *)  PWMsinegen_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMsinegen_DEADBAND_COUNT_PTR     ((reg8 *)   PWMsinegen_PWMUDB_sDB255_deadbandcounterdp_u0__D0_REG)
    #define PWMsinegen_DEADBAND_LSB_PTR       ((reg8 *)   PWMsinegen_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
    #define PWMsinegen_DEADBAND_LSB           (*(reg8 *)  PWMsinegen_PWMUDB_sDB255_deadbandcounterdp_u0__A0_REG)
#elif(PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_2_4_CLOCKS)
    
    /* In Fixed Function Block these bits are in the control blocks control register */
    #if (PWMsinegen_UsingFixedFunction)
        #define PWMsinegen_DEADBAND_COUNT         (*(reg8 *)  PWMsinegen_PWMHW__CFG0)
        #define PWMsinegen_DEADBAND_COUNT_PTR     ((reg8 *)   PWMsinegen_PWMHW__CFG0)
        #define PWMsinegen_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMsinegen_DEADBAND_COUNT_SHIFT)

        /* As defined by the Register Map as DEADBAND_PERIOD[1:0] in CFG0 */
        #define PWMsinegen_DEADBAND_COUNT_SHIFT   (0x06u)
    #else
        /* Lower two bits of the added control register define the count 1-3 */
        #define PWMsinegen_DEADBAND_COUNT         (*(reg8 *)  PWMsinegen_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMsinegen_DEADBAND_COUNT_PTR     ((reg8 *)   PWMsinegen_PWMUDB_genblk7_dbctrlreg__CONTROL_REG)
        #define PWMsinegen_DEADBAND_COUNT_MASK    (uint8)((uint8)0x03u << PWMsinegen_DEADBAND_COUNT_SHIFT)

        /* As defined by the verilog implementation of the Control Register */
        #define PWMsinegen_DEADBAND_COUNT_SHIFT   (0x00u)
    #endif /* (PWMsinegen_UsingFixedFunction) */
#endif /* (PWMsinegen_DeadBandMode == PWMsinegen__B_PWM__DBM_256_CLOCKS) */



#if (PWMsinegen_UsingFixedFunction)
    #define PWMsinegen_STATUS                 (*(reg8 *) PWMsinegen_PWMHW__SR0)
    #define PWMsinegen_STATUS_PTR             ((reg8 *) PWMsinegen_PWMHW__SR0)
    #define PWMsinegen_STATUS_MASK            (*(reg8 *) PWMsinegen_PWMHW__SR0)
    #define PWMsinegen_STATUS_MASK_PTR        ((reg8 *) PWMsinegen_PWMHW__SR0)
    #define PWMsinegen_CONTROL                (*(reg8 *) PWMsinegen_PWMHW__CFG0)
    #define PWMsinegen_CONTROL_PTR            ((reg8 *) PWMsinegen_PWMHW__CFG0)
    #define PWMsinegen_CONTROL2               (*(reg8 *) PWMsinegen_PWMHW__CFG1)
    #define PWMsinegen_CONTROL3               (*(reg8 *) PWMsinegen_PWMHW__CFG2)
    #define PWMsinegen_GLOBAL_ENABLE          (*(reg8 *) PWMsinegen_PWMHW__PM_ACT_CFG)
    #define PWMsinegen_GLOBAL_ENABLE_PTR      ( (reg8 *) PWMsinegen_PWMHW__PM_ACT_CFG)
    #define PWMsinegen_GLOBAL_STBY_ENABLE     (*(reg8 *) PWMsinegen_PWMHW__PM_STBY_CFG)
    #define PWMsinegen_GLOBAL_STBY_ENABLE_PTR ( (reg8 *) PWMsinegen_PWMHW__PM_STBY_CFG)


    /***********************************
    *          Constants
    ***********************************/

    /* Fixed Function Block Chosen */
    #define PWMsinegen_BLOCK_EN_MASK          (PWMsinegen_PWMHW__PM_ACT_MSK)
    #define PWMsinegen_BLOCK_STBY_EN_MASK     (PWMsinegen_PWMHW__PM_STBY_MSK)
    
    /* Control Register definitions */
    #define PWMsinegen_CTRL_ENABLE_SHIFT      (0x00u)

    /* As defined by Register map as MODE_CFG bits in CFG2*/
    #define PWMsinegen_CTRL_CMPMODE1_SHIFT    (0x04u)

    /* As defined by Register map */
    #define PWMsinegen_CTRL_DEAD_TIME_SHIFT   (0x06u)  

    /* Fixed Function Block Only CFG register bit definitions */
    /*  Set to compare mode */
    #define PWMsinegen_CFG0_MODE              (0x02u)   

    /* Enable the block to run */
    #define PWMsinegen_CFG0_ENABLE            (0x01u)   
    
    /* As defined by Register map as DB bit in CFG0 */
    #define PWMsinegen_CFG0_DB                (0x20u)   

    /* Control Register Bit Masks */
    #define PWMsinegen_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMsinegen_CTRL_ENABLE_SHIFT)
    #define PWMsinegen_CTRL_RESET             (uint8)((uint8)0x01u << PWMsinegen_CTRL_RESET_SHIFT)
    #define PWMsinegen_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMsinegen_CTRL_CMPMODE2_SHIFT)
    #define PWMsinegen_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMsinegen_CTRL_CMPMODE1_SHIFT)

    /* Control2 Register Bit Masks */
    /* As defined in Register Map, Part of the TMRX_CFG1 register */
    #define PWMsinegen_CTRL2_IRQ_SEL_SHIFT    (0x00u)
    #define PWMsinegen_CTRL2_IRQ_SEL          (uint8)((uint8)0x01u << PWMsinegen_CTRL2_IRQ_SEL_SHIFT)

    /* Status Register Bit Locations */
    /* As defined by Register map as TC in SR0 */
    #define PWMsinegen_STATUS_TC_SHIFT        (0x07u)   
    
    /* As defined by the Register map as CAP_CMP in SR0 */
    #define PWMsinegen_STATUS_CMP1_SHIFT      (0x06u)   

    /* Status Register Interrupt Enable Bit Locations */
    #define PWMsinegen_STATUS_KILL_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMsinegen_STATUS_TC_INT_EN_MASK_SHIFT            (PWMsinegen_STATUS_TC_SHIFT - 4u)
    #define PWMsinegen_STATUS_CMP2_INT_EN_MASK_SHIFT          (0x00u)
    #define PWMsinegen_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMsinegen_STATUS_CMP1_SHIFT - 4u)

    /* Status Register Bit Masks */
    #define PWMsinegen_STATUS_TC              (uint8)((uint8)0x01u << PWMsinegen_STATUS_TC_SHIFT)
    #define PWMsinegen_STATUS_CMP1            (uint8)((uint8)0x01u << PWMsinegen_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks */
    #define PWMsinegen_STATUS_TC_INT_EN_MASK              (uint8)((uint8)PWMsinegen_STATUS_TC >> 4u)
    #define PWMsinegen_STATUS_CMP1_INT_EN_MASK            (uint8)((uint8)PWMsinegen_STATUS_CMP1 >> 4u)

    /*RT1 Synch Constants */
    #define PWMsinegen_RT1_SHIFT             (0x04u)

    /* Sync TC and CMP bit masks */
    #define PWMsinegen_RT1_MASK              (uint8)((uint8)0x03u << PWMsinegen_RT1_SHIFT)
    #define PWMsinegen_SYNC                  (uint8)((uint8)0x03u << PWMsinegen_RT1_SHIFT)
    #define PWMsinegen_SYNCDSI_SHIFT         (0x00u)

    /* Sync all DSI inputs */
    #define PWMsinegen_SYNCDSI_MASK          (uint8)((uint8)0x0Fu << PWMsinegen_SYNCDSI_SHIFT)

    /* Sync all DSI inputs */
    #define PWMsinegen_SYNCDSI_EN            (uint8)((uint8)0x0Fu << PWMsinegen_SYNCDSI_SHIFT)


#else
    #define PWMsinegen_STATUS                (*(reg8 *)   PWMsinegen_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMsinegen_STATUS_PTR            ((reg8 *)    PWMsinegen_PWMUDB_genblk8_stsreg__STATUS_REG )
    #define PWMsinegen_STATUS_MASK           (*(reg8 *)   PWMsinegen_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMsinegen_STATUS_MASK_PTR       ((reg8 *)    PWMsinegen_PWMUDB_genblk8_stsreg__MASK_REG)
    #define PWMsinegen_STATUS_AUX_CTRL       (*(reg8 *)   PWMsinegen_PWMUDB_genblk8_stsreg__STATUS_AUX_CTL_REG)
    #define PWMsinegen_CONTROL               (*(reg8 *)   PWMsinegen_PWMUDB_genblk1_ctrlreg__CONTROL_REG)
    #define PWMsinegen_CONTROL_PTR           ((reg8 *)    PWMsinegen_PWMUDB_genblk1_ctrlreg__CONTROL_REG)


    /***********************************
    *          Constants
    ***********************************/

    /* Control Register bit definitions */
    #define PWMsinegen_CTRL_ENABLE_SHIFT      (0x07u)
    #define PWMsinegen_CTRL_RESET_SHIFT       (0x06u)
    #define PWMsinegen_CTRL_CMPMODE2_SHIFT    (0x03u)
    #define PWMsinegen_CTRL_CMPMODE1_SHIFT    (0x00u)
    #define PWMsinegen_CTRL_DEAD_TIME_SHIFT   (0x00u)   /* No Shift Needed for UDB block */
    
    /* Control Register Bit Masks */
    #define PWMsinegen_CTRL_ENABLE            (uint8)((uint8)0x01u << PWMsinegen_CTRL_ENABLE_SHIFT)
    #define PWMsinegen_CTRL_RESET             (uint8)((uint8)0x01u << PWMsinegen_CTRL_RESET_SHIFT)
    #define PWMsinegen_CTRL_CMPMODE2_MASK     (uint8)((uint8)0x07u << PWMsinegen_CTRL_CMPMODE2_SHIFT)
    #define PWMsinegen_CTRL_CMPMODE1_MASK     (uint8)((uint8)0x07u << PWMsinegen_CTRL_CMPMODE1_SHIFT)

    /* Status Register Bit Locations */
    #define PWMsinegen_STATUS_KILL_SHIFT          (0x05u)
    #define PWMsinegen_STATUS_FIFONEMPTY_SHIFT    (0x04u)
    #define PWMsinegen_STATUS_FIFOFULL_SHIFT      (0x03u)
    #define PWMsinegen_STATUS_TC_SHIFT            (0x02u)
    #define PWMsinegen_STATUS_CMP2_SHIFT          (0x01u)
    #define PWMsinegen_STATUS_CMP1_SHIFT          (0x00u)

    /* Status Register Interrupt Enable Bit Locations - UDB Status Interrupt Mask match Status Bit Locations*/
    #define PWMsinegen_STATUS_KILL_INT_EN_MASK_SHIFT          (PWMsinegen_STATUS_KILL_SHIFT)
    #define PWMsinegen_STATUS_FIFONEMPTY_INT_EN_MASK_SHIFT    (PWMsinegen_STATUS_FIFONEMPTY_SHIFT)
    #define PWMsinegen_STATUS_FIFOFULL_INT_EN_MASK_SHIFT      (PWMsinegen_STATUS_FIFOFULL_SHIFT)
    #define PWMsinegen_STATUS_TC_INT_EN_MASK_SHIFT            (PWMsinegen_STATUS_TC_SHIFT)
    #define PWMsinegen_STATUS_CMP2_INT_EN_MASK_SHIFT          (PWMsinegen_STATUS_CMP2_SHIFT)
    #define PWMsinegen_STATUS_CMP1_INT_EN_MASK_SHIFT          (PWMsinegen_STATUS_CMP1_SHIFT)

    /* Status Register Bit Masks */
    #define PWMsinegen_STATUS_KILL            (uint8)((uint8)0x00u << PWMsinegen_STATUS_KILL_SHIFT )
    #define PWMsinegen_STATUS_FIFOFULL        (uint8)((uint8)0x01u << PWMsinegen_STATUS_FIFOFULL_SHIFT)
    #define PWMsinegen_STATUS_FIFONEMPTY      (uint8)((uint8)0x01u << PWMsinegen_STATUS_FIFONEMPTY_SHIFT)
    #define PWMsinegen_STATUS_TC              (uint8)((uint8)0x01u << PWMsinegen_STATUS_TC_SHIFT)
    #define PWMsinegen_STATUS_CMP2            (uint8)((uint8)0x01u << PWMsinegen_STATUS_CMP2_SHIFT)
    #define PWMsinegen_STATUS_CMP1            (uint8)((uint8)0x01u << PWMsinegen_STATUS_CMP1_SHIFT)

    /* Status Register Interrupt Bit Masks  - UDB Status Interrupt Mask match Status Bit Locations */
    #define PWMsinegen_STATUS_KILL_INT_EN_MASK            (PWMsinegen_STATUS_KILL)
    #define PWMsinegen_STATUS_FIFOFULL_INT_EN_MASK        (PWMsinegen_STATUS_FIFOFULL)
    #define PWMsinegen_STATUS_FIFONEMPTY_INT_EN_MASK      (PWMsinegen_STATUS_FIFONEMPTY)
    #define PWMsinegen_STATUS_TC_INT_EN_MASK              (PWMsinegen_STATUS_TC)
    #define PWMsinegen_STATUS_CMP2_INT_EN_MASK            (PWMsinegen_STATUS_CMP2)
    #define PWMsinegen_STATUS_CMP1_INT_EN_MASK            (PWMsinegen_STATUS_CMP1)

    /* Datapath Auxillary Control Register bit definitions */
    #define PWMsinegen_AUX_CTRL_FIFO0_CLR         (0x01u)
    #define PWMsinegen_AUX_CTRL_FIFO1_CLR         (0x02u)
    #define PWMsinegen_AUX_CTRL_FIFO0_LVL         (0x04u)
    #define PWMsinegen_AUX_CTRL_FIFO1_LVL         (0x08u)
    #define PWMsinegen_STATUS_ACTL_INT_EN_MASK    (0x10u) /* As defined for the ACTL Register */
#endif /* PWMsinegen_UsingFixedFunction */

#endif  /* CY_PWM_PWMsinegen_H */


/* [] END OF FILE */
