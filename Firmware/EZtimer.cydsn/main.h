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

#if !defined(MAIN_H)
#define MAIN_H
#include <project.h>
#include "BLEProcess.h"
#include "servos.h"
#include "WriteUserSFlash.h"
	
/************************************* Macros *********************************************/
/* General Macros */
#define TRUE							0x01
#define FALSE							0x00
    
/* DataSet Macros */
#define DATA_SET_LEN                    0x34
/******************************************************************************************/
    
/*********************************** Variables ********************************************/
/* Global Variables */
uint8 DataSet[DATA_SET_LEN];
/******************************************************************************************/

/******************************* Function Declaration *************************************/
void InitializeSystem(void);
/******************************************************************************************/

#endif

/* [] END OF FILE */
