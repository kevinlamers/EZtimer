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
#include <main.h>

CY_ISR(SWPin_Control)
{
    if(InputPin_Read() == 0u)
    {
        PWMbeep_Start();
        PWMbeep_WriteCompare(4u);
    }
    else
    {
        PWMbeep_WriteCompare(0u);
        UpdateServoPWM(DataSet);
    }
    InputPin_ClearInterrupt();
}

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    InitializeSystem();
    
    InputInterrupt_StartEx(SWPin_Control);
    
    CyGlobalIntEnable;
                
    /* Read DataSet from SFlash */
    ReadArraySFlash(DataSet, DATA_SET_LEN);
    
    UpdateServocharacteristic(DataSet, sizeof(DataSet), CYBLE_SERVOS_SERVO_CONTROL_CHAR_HANDLE);
   
    for(;;)
    {
        /* Place your application code here. */
        CyBle_ProcessEvents();
        
        
//        switch(flight_fase)
//        {    
//            case fase_Ready:
//                /* This event is generated when airplane is ready and waiting for launch */
//                
//            break;
//            
//            case fase_Flight:
//                /* This event is generated when airplane is flying */
//                
//            break;
//            
//            case fase_DT:
//                /* This event is generated when airplane has DT'ed */
//                
//            break;
//                        
//            case fase_sleep:
//                /* This event is generated when airplane is sleeping */
//                
//            break;
//        }
    }
}

void InitializeSystem(void)
{
    /* Enable Global Interrupt Mask */
    CyGlobalIntEnable;

    /* Start BLE stack and register the event callback function.*/
    CyBle_Start(GeneralEventHandler);
}

/* [] END OF FILE */
