/** @file <thingsboard_example>.cxx
 *  @brief Contains the main function which calls the functions from the thingsboard.cxx file.
 *  @author Sidharth Dinesh 
 *  @bug No bugs found in the file  
 */

/*
 *#####################################################################
 *  Initialization block
 *  ---------------------
 *  This block contains initialization code for this particular file.
 *  It typically contains Includes, constants or global variables used
 *  throughout the file.
 *#####################################################################
 */
 
/* --- Standard Includes --- */
#include <string.h>
#include <stdio.h>
#include <shunyaInterfaces.h>
#include <iostream.h>

/* --- Project Includes --- */
#include <mqtt.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

#include "thingsboard.h"


/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here 
 *#####################################################################
 */

/* Declare an object to store the JSON document in  */

/** 
 *  @brief Description on main
 *  
 *  Full description of the function
 *
 *  @return List all Function returns 
 */

int main (void)
{
    char brokerUrl[20];
    printf("Enter brokerUrl: ");
    scanf("%s", brokerUrl);
    
    char clientID[20];
    printf("Enter clientID: ");
    scanf("%s", clientID);
    
    char buffer[100];
    sprintf(buffer, "{\"brokerUrl\":\"%s\",\"clientID\":%s,}", brokerUrl, clientID);
    
    /* Create New Instance */
    mqttObj broker1 = newMqtt(buffer);
    
    connectToThingsboard(broker1);
    
    publishTelemetryToThingsboard(broker1);
    
    publishAttributeToThingsboard(broker1);
    
    subscribeToAttributesFromThingsboard(mqttObj broker1, Attributes_Callback userCallback);
    
    subscribeToRPCFromThingsboard(mqttObj broker1, RPC_Callback userCallback);
    
    disconnectFromThingsboard(broker1);
    
    return 0;
}
