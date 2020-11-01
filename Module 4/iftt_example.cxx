/** @file <iftt_example>.cxx
 *  @brief Contains the main function which calls the functions from the iftt.cxx file.
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
#include <stdio.h>
#include <stdlib.h>
#include <shunyaInterfaces.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

#include "iftt.h"


/* --- Project Includes --- */


/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here 
 *#####################################################################
 */

/** 
 *  @brief Description on main
 *  
 *  Full description of the function
 *
 *  @return List all Function returns 
 */

int main (void)
{
    // Set a variable to hold the GPIO Pin identity
    int pinpir = 17;
    
    // Set GPIO pin as input
    pinmode(pinpir, INPUT);
    
    // Variables to hold the current and last states
    int currentstate = 0;
    int previousstate = 0;
    
    bool status=true;
    
    while(status)
    {
        printf("Waiting for PIR to settle ...");

        // Loop until PIR output is 0
        while (digitalRead(pinpir)== 1)
            currentstate = 0;
        
        printf("Ready");
        
        while(true)
        {   
            // Read PIR state
            currentstate = digitalRead(pinpir);
            
            // If the PIR is triggered
            if (currentstate == 1 && previousstate == 0)
            {
                printf("Motion detected!");
                
                char eventname[20];
                printf("Enter event name: ");
                scanf("%s", eventname);
    
                char key[20];
                printf("Enter key: ");
                scanf("%s", key);
                
                char Url[100];
                sprintf(Url,"https://maker.ifttt.com/trigger/%s/with/key/%s", eventname, key);     //IFTT URL

                char data[]="{\"property1\":\"value1\",\"property2\":\"value2\"}"; //sample data
                
                // Your IFTT URL with event name, key and json parameters (values)
                http_post(Url,data);

                // Record new previous state
                previousstate = 1

                // Wait 120 seconds before looping again
                printf("Waiting 120 seconds");
                delay(1200000);
            }
            else if(currentstate == 0 && previousstate == 1) // If the PIR has returned to ready state
            {
                printf("Ready");
                previousstate = 0

                // Wait for 10 milliseconds
                delay(10);
            }
        }
    }
    /*Make sure you comment every line */
    return 0;
        
}

