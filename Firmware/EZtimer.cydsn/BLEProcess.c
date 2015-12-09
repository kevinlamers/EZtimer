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

void GeneralEventHandler(uint32 event, void * eventParam)
{
    /* Structure to store data written by Client */
    CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;   

    switch(event)
    {
        case CYBLE_EVT_STACK_ON:
            /* This event is generated when BLE stack is ON */
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
        break;

        case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
            /* This event is generated whenever Advertisement starts or stops */
            if(CyBle_GetState() == CYBLE_STATE_DISCONNECTED)
            {
                CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            }
        break;

        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            /* This event is generated at GAP disconnection. */
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
        break;
            
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            /* Initiate an MTU exchange request */
            CyBle_GattcExchangeMtuReq(cyBle_connHandle, CYBLE_GATT_MTU);
        break;
 
        case CYBLE_EVT_GATTS_WRITE_REQ:
            /* This event is generated when the connected Central */
            /* device sends a Write request. */
            /* The parameter ‘eventParam’ contains the data written */
            
            /* Extract the Write data sent by Client */
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;

            /* If the attribute handle of the characteristic written to
             * is equal to that of RGB_LED characteristic, then extract
             * the RGB LED data */
            if(CYBLE_SERVOS_SERVO_CONTROL_CHAR_HANDLE ==
                                    wrReqParam->handleValPair.attrHandle)
            {
                /* Store RGB LED data in local array */
                int i;
                for (i=0;i<DATA_SET_LEN;i++)
                {
                    DataSet[i] = wrReqParam->handleValPair.value.val[i];
                }

                Pin_1_Write(0u); //Blue on
                //CyDelay(300u);
                
                //CyDelay(300u);
                
                /* Write new DataSet to SFlash */
                WriteArraySFlash(DataSet, DATA_SET_LEN);
   

                /* Update the GATT DB for DataSet characteristics*/
                UpdateServocharacteristic(DataSet, sizeof(DataSet),CYBLE_SERVOS_SERVO_CONTROL_CHAR_HANDLE);
                Pin_1_Write(1u); //Blue off
            }

            /* Send the response to the write request rreceived. */
            CyBle_GattsWriteRsp(cyBle_connHandle);          
        break;

        case CYBLE_EVT_GATT_DISCONNECT_IND:
            /* This event is generated at GATT disconnection. */
     
            /* Register the new color in GATT DB*/
            UpdateServocharacteristic(DataSet, sizeof(DataSet), CYBLE_SERVOS_SERVO_CONTROL_CHAR_HANDLE);
        break;

        break;
 
        default:
        
        break;
    }
}

void UpdateServocharacteristic(uint8* DataSet, uint8 len, uint16 attrHandle)
{
    Pin_3_Write(0u); //Red on
    //CyDelay(300u);
    
    //CyDelay(300u);
    
	/* 'rgbHandle' stores RGB control data parameters */
	CYBLE_GATT_HANDLE_VALUE_PAIR_T		dataHandle;

	/* Update RGB control handle with new values */
	dataHandle.attrHandle = attrHandle;
	dataHandle.value.val = DataSet;
	dataHandle.value.len = len;
	
	/* Update the RGB LED attribute value. This will allow 
	* Client device to read the existing color values over 
	* RGB LED characteristic */
	CyBle_GattsWriteAttributeValue(&dataHandle, 
									FALSE, 
									&cyBle_connHandle, 
									CYBLE_GATT_DB_LOCALLY_INITIATED);
    Pin_3_Write(1u); //Red off
}

/* [] END OF FILE */
