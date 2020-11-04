/** @file <iftt_demo>.cxx
 *  @brief Contains the main function which calls the functions from the iftt.cxx and bme280.c file.
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
#include "bme280.h"

/* --- Project Includes --- */


/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here 
 *#####################################################################
 */

/** 
 *  @brief Sending webhooks to iftt
 *  @return void
 */

void iftt_start (float temp,float cpu)
{
    if(temp>303.0) //30deg C=303K
    {
        char eventname[20];
        printf("Enter event name: ");
        scanf("%s", eventname);
    
        char key[20];
        printf("Enter key: ");
        scanf("%s", key);
                
        char Url[100];
        sprintf(Url,"https://maker.ifttt.com/trigger/%s/with/key/%s", eventname, key);     //IFTT URL
        
        char data[100];
        sprintf(data, "{\"Temperature\":\"%f\",\"Status\":\"HIGH\",}", temp);
        
        // Your IFTT URL with event name, key and json parameters (values)
        http_post(Url,data);
    }
    
    if(cpu>100.0) //100MB
    {
        char eventname[20];
        printf("Enter event name: ");
        scanf("%s", eventname);
    
        char key[20];
        printf("Enter key: ");
        scanf("%s", key);
                
        char Url[100];
        sprintf(Url,"https://maker.ifttt.com/trigger/%s/with/key/%s", eventname, key);     //IFTT URL

        char data[100];
        sprintf(data, "{\"CPU RAM Usage\":\"%f\",\"Status\":\"HIGH\",}", cpu);
        
        // Your IFTT URL with event name, key and json parameters (values)
        http_post(Url,data);
    }
}

/** 
 *  @brief Read temperature data from the sensor 
 *  @return temperature 
 */
float bme_start (void)
{
    shunyaInterfacesSetup();
    
    bool status;
    
    // default settings
    status = begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    /* Read data from the sensor */
    float temperature = getTemperature();
    
    return temperature;
}
/** 
 *  @brief Description on main
 *  
 *  Full description of the function
 *
 *  @return List all Function returns 
 */

int main (void)
{   
    float temp=bme_start();
    
    char cmd[128];
    sprintf(cmd,"docker stats --format \"table {{.Container}}\t{{.CPUPerc}}\t{{.MemUsage}}\"");
	system(cmd);
    //To list all containers statistics with their name, CPU percentage and memory usage in a table format
    
    float cpu;
    cout<<"Enter CPU Ram Usage from above table in float type:";
    cin>>cpu;
    
    iftt_start(temp,cpu);
    
    return 0;
}

