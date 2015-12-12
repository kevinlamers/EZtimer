/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

//#if !defined(MAIN_H)
//#define MAIN_H
    
#include <project.h>
#include <stdio.h>
    
#include "BLEProcess.h"
#include "servos.h"
#include "WriteUserSFlash.h"
#include "UART.h"
#include "DataSet.h"    

	
/************************************* Macros *********************************************/
/* General Macros */
#define TRUE							0x01
#define FALSE							0x00
 
/* Flight Fases */
#define fase_Standby                    0x00    
#define fase_Ready                      0x01
#define fase_Flight                     0x02
#define fase_DT                         0x03
#define fase_Sleep                      0x04
    
/* DataSet Macros */
#define DATA_SET_LEN                    0x34
/******************************************************************************************/
    
/*********************************** Variables ********************************************/
/* Global Variables */
uint8 DataSet[DATA_SET_LEN];

uint32 time_10ms;                   // Current time in 10ms interval
uint8 flight_fase;
int timeStep;
/******************************************************************************************/

/******************************* Function Declaration *************************************/
void InitializeSystem(void);
/******************************************************************************************/

//#endif

/* [] END OF FILE */
