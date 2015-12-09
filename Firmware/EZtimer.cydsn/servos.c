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
#include "main.h"

void UpdateServoPWM(uint8* DataSet)
{
    // all data is Little Endian: least significant byte first
    uint16 time1 = DataSet[TIME1_INDEX] | (DataSet[TIME1_INDEX+1]<<8);
    uint16 time2 = DataSet[TIME2_INDEX] | (DataSet[TIME2_INDEX+1]<<8);
    uint16 time3 = DataSet[TIME3_INDEX] | (DataSet[TIME3_INDEX+1]<<8);
    uint16 time4 = DataSet[TIME4_INDEX] | (DataSet[TIME4_INDEX+1]<<8);
    uint16 time5 = DataSet[TIME5_INDEX] | (DataSet[TIME5_INDEX+1]<<8);
    uint16 time6 = DataSet[TIME6_INDEX] | (DataSet[TIME6_INDEX+1]<<8);
    uint16 time7 = DataSet[TIME7_INDEX] | (DataSet[TIME7_INDEX+1]<<8);
    uint16 time8 = DataSet[TIME8_INDEX] | (DataSet[TIME8_INDEX+1]<<8);
    uint16 time9 = DataSet[TIME9_INDEX] | (DataSet[TIME9_INDEX+1]<<8);
    uint16 time10 = DataSet[TIME10_INDEX] | (DataSet[TIME10_INDEX+1]<<8);
    uint16 timeDT = DataSet[TIMEDT_INDEX] | (DataSet[TIMEDT_INDEX+1]<<8);
    
    uint16 servo1_posStart = DataSet[SERVO1_POSSTART_INDEX] | (DataSet[SERVO1_POSSTART_INDEX+1]<<8);
    uint16 servo1_pos1 = DataSet[SERVO1_POS1_INDEX] | (DataSet[SERVO1_POS1_INDEX+1]<<8);
    uint16 servo1_pos2 = DataSet[SERVO1_POS2_INDEX] | (DataSet[SERVO1_POS2_INDEX+1]<<8);
    uint16 servo1_pos3 = DataSet[SERVO1_POS3_INDEX] | (DataSet[SERVO1_POS3_INDEX+1]<<8);
    uint16 servo1_pos4 = DataSet[SERVO1_POS4_INDEX] | (DataSet[SERVO1_POS4_INDEX+1]<<8);
    uint16 servo1_pos5 = DataSet[SERVO1_POS5_INDEX] | (DataSet[SERVO1_POS5_INDEX+1]<<8);
    uint16 servo1_pos6 = DataSet[SERVO1_POS6_INDEX] | (DataSet[SERVO1_POS6_INDEX+1]<<8);
    uint16 servo1_pos7 = DataSet[SERVO1_POS7_INDEX] | (DataSet[SERVO1_POS7_INDEX+1]<<8);
    uint16 servo1_pos8 = DataSet[SERVO1_POS8_INDEX] | (DataSet[SERVO1_POS8_INDEX+1]<<8);
    uint16 servo1_pos9 = DataSet[SERVO1_POS9_INDEX] | (DataSet[SERVO1_POS9_INDEX+1]<<8);
    uint16 servo1_pos10 = DataSet[SERVO1_POS10_INDEX] | (DataSet[SERVO1_POS10_INDEX+1]<<8);
    uint16 servo1_posDT = DataSet[SERVO1_POSDT_INDEX] | (DataSet[SERVO1_POSDT_INDEX+1]<<8);

    uint16 timecum = time1+time2+time3+time4+time5+time6+time7+time8+time9+time10;
    
//    Pin_2_Write(0u); //Green
//    CyDelay(300u);
//    Pin_2_Write(1u);
//    CyDelay(300u);
                
    PWM_1_Start();
    PWM_1_WriteCompare(servo1_posStart);
    CyDelay(time1*10u);
    PWM_1_WriteCompare(servo1_pos1);
    CyDelay(time2*10u);
    PWM_1_WriteCompare(servo1_pos2);
    CyDelay(time3*10u);
    PWM_1_WriteCompare(servo1_pos3);
    CyDelay(time4*10u);
    PWM_1_WriteCompare(servo1_pos4);
    CyDelay(time5*10u);
    PWM_1_WriteCompare(servo1_pos5);
    CyDelay(time6*10u);
    PWM_1_WriteCompare(servo1_pos6);
    CyDelay(time7*10u);
    PWM_1_WriteCompare(servo1_pos7);
    CyDelay(time8*10u);
    PWM_1_WriteCompare(servo1_pos8);
    CyDelay(time9*10u);
    PWM_1_WriteCompare(servo1_pos9);
    CyDelay(time10*10u);
    PWM_1_WriteCompare(servo1_pos10);
    if (timecum<timeDT){
        CyDelay((timeDT-timecum)*10u);
    }
    PWM_1_WriteCompare(servo1_posDT);
    CyDelay(1000u);
    PWM_1_WriteCompare(servo1_posStart);
    CyDelay(1000u);
    PWM_1_Stop();
}

/* [] END OF FILE */
