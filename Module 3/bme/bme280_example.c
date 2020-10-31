/** @file <bme280_example>.c
 *  @brief Contains the main function which calls the functions from the bme280.c file.
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
#include <Wire.h>

#include <shunyaInterfaces.h>
#include <simple-mode.h>
#include <functions.h>


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
 *  @brief Description on main
 *  
 *  Full description of the function
 *
 *  @return List all Function returns 
 */

int main (void)
{
    shunyaInterfacesSetup();
    
    bool status;
    
    // default settings
    status = begin();  
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    while (1)
    {       
        /* Read data from the sensor */
        float temperature = getTemperature();
        float pressure = getPressure();
        int8_t humidity = getHumidity();
        float altitude = getAltitude();

        delay(10*60*1000);
    }
        /*Make sure you comment every line */
        return 0;
        
}
