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

char str[50]; // Make large enough, otherwise undefined behaviour.

CY_ISR(SWPin_Control)
{
    if(InputPin_Read() == 0u)
    {
        PWM_Beep_Start();
        PWM_Beep_WriteCompare(4u);
        flight_fase = fase_Ready;
    }
    else
    {
        PWM_Beep_WriteCompare(0u);
        //UpdateServoPWM(DataSet);
        flight_fase = fase_Flight;
        PWM_Beep_Stop();
        time_10ms = 0;
    }
    InputPin_ClearInterrupt();
}

CY_ISR(TimerStep)
{
    time_10ms++;
    uint32 source = Timer_10ms_GetInterruptSourceMasked();
    Timer_10ms_ClearInterrupt(source);
    
}

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    InitializeSystem();
   
    InputInterrupt_StartEx(SWPin_Control);
    
    TimerInterrupt_StartEx(TimerStep);
    
    CyGlobalIntEnable;
    
    UART_Start();
    UART_UartPutString("\r\n");
    UART_UartPutString("\r\n");
    UART_UartPutString("UART Initialised\r\n");
    
   
                
    flight_fase = fase_Standby;
    
    /* Read DataSet from SFlash */
    ReadArraySFlash(DataSet, DATA_SET_LEN);
    
   
    
    UpdateServocharacteristic(DataSet, sizeof(DataSet), CYBLE_SERVOS_SERVO_CONTROL_CHAR_HANDLE);

    ProcessDataSet();

   
    for(;;)
    {
        /* Place your application code here. */
        CyBle_ProcessEvents();
        
        
        //sprintf(str, "Time_ms: %lu\r\n", time_10ms);
        //UART_UartPutString(str);
        
        switch(flight_fase)
        {    
            case fase_Standby:
                /* This event is generated when timer is standby */
            break;
            
            case fase_Ready:
                /* This event is generated when airplane is ready and waiting for launch */
                timeStep = 0;
                ServoStartPosition();
            break;
            
            case fase_Flight:
                /* This event is generated when airplane is flying */
                    
                        
                    Timer_10ms_Start();
                    //UART_UartPutString("Timer Initialised\r\n");

                    /*sprintf(str, "servotimes[0]: %d\r\n", servoTimes[0]);
                    UART_UartPutString(str);
               
                    sprintf(str, "servotimes[0]: %d\r\n", servoTimes[0]);
                    UART_UartPutString(str);
                    sprintf(str, "servotimes[0]: %d\r\n", servoTimes[0]);
                    UART_UartPutString(str);
                    sprintf(str, "Dataset[i]: %d\r\n", (DataSet[TIME1_INDEX] | (DataSet[TIME1_INDEX+1]<<8)));
                    UART_UartPutString(str);*/
                    
//                  
                   // UART_UartPutString("bla \r\n");
                    if (time_10ms > servoTimes[timeStep])
                    {
                        UART_UartPutString("Next Servo Position!\r\n");
                        sprintf(str, "timeStep: %d\r\n", timeStep);
                        UART_UartPutString(str);
                        uint16 postemp = servo1Positions[timeStep];
                        sprintf(str, "servo1position[timeStep]: %u\r\n", postemp);
                        UART_UartPutString(str);

                        ServoNextPosition();
                    }
                    
                    if (timeStep>10)
                    {
                        flight_fase = fase_DT;
                    }
            break;
            
            case fase_DT:
                /* This event is generated when airplane has DT'ed */
                CyDelay(1000u);
                PWM_12_Stop();
                
            break;
                        
            case fase_Sleep:
                /* This event is generated when airplane is sleeping */
                
            break;
                       
        break;
 
        default:
        
        break;
        }
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
