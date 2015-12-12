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
#include <servos.h>

void ProcessDataSet(void)
{
    servoTimes[0] = DataSet[TIME1_INDEX] | (DataSet[TIME1_INDEX+1]<<8);
    servoTimes[1] = DataSet[TIME2_INDEX] | (DataSet[TIME2_INDEX+1]<<8);
    servoTimes[2] = DataSet[TIME3_INDEX] | (DataSet[TIME3_INDEX+1]<<8);
    servoTimes[3] = DataSet[TIME4_INDEX] | (DataSet[TIME4_INDEX+1]<<8);
    servoTimes[4] = DataSet[TIME5_INDEX] | (DataSet[TIME5_INDEX+1]<<8);
    servoTimes[5] = DataSet[TIME6_INDEX] | (DataSet[TIME6_INDEX+1]<<8);
    servoTimes[6] = DataSet[TIME7_INDEX] | (DataSet[TIME7_INDEX+1]<<8);
    servoTimes[7] = DataSet[TIME8_INDEX] | (DataSet[TIME8_INDEX+1]<<8);
    servoTimes[8] = DataSet[TIME9_INDEX] | (DataSet[TIME9_INDEX+1]<<8);
    servoTimes[9] = DataSet[TIME10_INDEX] | (DataSet[TIME10_INDEX+1]<<8);
    servoTimes[10] = DataSet[TIMEDT_INDEX] | (DataSet[TIMEDT_INDEX+1]<<8);

    servo1Positions[0] = DataSet[SERVO1_POS1_INDEX] | (DataSet[SERVO1_POS1_INDEX+1]<<8);
    servo1Positions[1] = DataSet[SERVO1_POS2_INDEX] | (DataSet[SERVO1_POS2_INDEX+1]<<8);
    servo1Positions[2] = DataSet[SERVO1_POS3_INDEX] | (DataSet[SERVO1_POS3_INDEX+1]<<8);
    servo1Positions[3] = DataSet[SERVO1_POS4_INDEX] | (DataSet[SERVO1_POS4_INDEX+1]<<8);
    servo1Positions[4] = DataSet[SERVO1_POS5_INDEX] | (DataSet[SERVO1_POS5_INDEX+1]<<8);
    servo1Positions[5] = DataSet[SERVO1_POS6_INDEX] | (DataSet[SERVO1_POS6_INDEX+1]<<8);
    servo1Positions[6] = DataSet[SERVO1_POS7_INDEX] | (DataSet[SERVO1_POS7_INDEX+1]<<8);
    servo1Positions[7] = DataSet[SERVO1_POS8_INDEX] | (DataSet[SERVO1_POS8_INDEX+1]<<8);
    servo1Positions[8] = DataSet[SERVO1_POS9_INDEX] | (DataSet[SERVO1_POS9_INDEX+1]<<8);
    servo1Positions[9] = DataSet[SERVO1_POS10_INDEX] | (DataSet[SERVO1_POS10_INDEX+1]<<8);
    servo1Positions[10] = DataSet[SERVO1_POSDT_INDEX] | (DataSet[SERVO1_POSDT_INDEX+1]<<8);
    
    int i, sum=0;
    for (i=0; i<10; i++)
    {
	    sum = sum + servoTimes[i];
    }
    if (servoTimes[10] > sum)
    {
        servoTimes[10] = servoTimes[10] - sum;
    }
    else
    {
        servoTimes[10] = 0; // Imidiate DT
    }
}
/* [] END OF FILE */
