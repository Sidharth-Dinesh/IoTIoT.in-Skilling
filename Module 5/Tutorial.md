# Temperature and CPU RAM Usage monitor for IFTT platform

This is a Demo for IFTT platform in which we monitor:
  1. Temperature using BME280 sensor
  2. CPU RAM usage of the docker.  

And sends the webhook to IFTT platform, when temperature reaches higher than 30 deg Celsius, or, CPU RAM usage of the docker reaches  
higher than 100 MB. IFTT then sends  an email that the 'Temperature is high' or 'the RAM usage is high'.

## Step 1: Components Required:
  1. Raspberry Pi 3/4 (or any variant of these).
  2. micro-SD card and micro-SD card Reader/Adapter.
  3. Temperature sensor - BME280
  4. Analog to Digital Converter - PCF8591 module.
  5. OLED Display (128x64) (optional).
  6. Female to Female Du-point cables.

## Step 2: Pre-Requisite Knowledge:
  1. **Install Shunya OS**: Shunya Interfaces is built on top of Shunya OS and comes pre-installed with Shunya Interfaces.   
    Shunya OS is an linux-based OS that runs on your hardware, it light-weight and configurable.
  2. **Booting Up with Shunya OS**:
     1. Insert the micro-SD card into the board.
     2. Connect peripherals like Keyboard, Mouse and HDMI monitor, and connect Power Supply.
     3. The board should boot up with Shunya OS. Login to Shunya with these credentials: Username : shunya, Password : shunya
  3.  **We have interfaced BME280 sensor with RPI 4 over I2C.** 
      * **Inter-Integrated Circuit(I2C)** combines the features of UART & SPI. It can have multiple slave, multi-master multi slave   
       or multi-master single slave configuration. It uses two lines for communication:
        * **SDA (Serial Data)** – The line for the master and slave to send and receive data.
        * **SCL (Serial Clock)** – The line that carries the clock signal.
      * **Advantages**:
        * Only uses two wires.
        * Supports multiple masters and multiple slaves.
        * ACK/NACK bit gives confirmation that each frame is transferred successfully.
        * Hardware is less complicated than with UARTs.
        * Well known and widely used protocol.
      * **Disadvantages**:
        * Slower data transfer rate than SPI.
        * The size of the data frame is limited to 8 bits.
        * More complicated hardware needed to implement than SPI.
  4. **Connections/building the circuit:**
      * We have connected 2 devices to the Raspberry Pi (BME280 and PCF8591) to pin 3 (SDA) & pin 5 (SCL) respectively.
  5. **Introduction to Shunya Interfaces APIs:**
     * Shunya Interfaces depends on the core library to provide basic API's to interact with the Microprocessor Interfaces like GPIO, SPI, UART, I2C.

### GPIO API

| API|Description      |
|----------|:-------------:|
| pinmode()|Sets the direction of the GPIO pin to INPUT or OUTPUT|
| digitalWrite()|Sets the digital value of the GPIO pin| 
| digitalRead()|Read digital value of the GPIO pin|  

### Interrupt API

| API|Description|
|----------|:-------------:|
| attachInterrupt()|Sets the digital value of the GPIO pin|

### Delay API

| API|Description|
|----------|:-------------:|
| delay()|Code sleeps for given milliseconds|
| delayMicroseconds()|Code sleeps for given microseconds| 

### I2C API

| API|Description|
|----------|:-------------:|
| wireBegin()|Initializes the I2C device|
| wireBeginTransmission()|Starts I2C communication| 
| wireWrite()|Send data to the I2C device|
| wireRequestFrom()|Request Data from the I2C device|
| wireAvailable()|Check number of bytes available to read from the device| 
| wireRead()|Read from the I2C device|
| wireEndTransmission()|End communication to the I2C device|
| wireEnd()|De-initialize the I2C device| 

  6. **Compiling instructions**: steps to compile and run example code in a ShunyOS Docker Container
     * Pull the shunyaos/shunya:armv7 image: **docker pull shunyaos/shunya:armv7**
     * Then, make sure your code is at a folder. Then, open a shell prompt in the docker container by bind mounting   
       the before-mentioned directory to a directory on docker: **docker run -v /home/../<your-folder>/max30100:/src/max30100 -ti shunyaos/shunya:armv7 /bin/bash**
     * Edit the files in your host system with your favourite text editor and compile them in the ShunyaOS container as follows:  
         **cd /src/max30100**  
         **g++ MAX30100_example.cpp MAX30100.cpp -lshunyaInterfaces_core**  
         **./a.out**  

## Step 3: Circuit Diagram:
<img src="extras/farm_bb.png" width=700px height=500px>

## Step 4: Source Code and Step-by-Step Code Explanation through comments:
```
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
 *   @brief  HTTPosting
 */
void http_post(char *Url,char *data)
{
    CURL *curl;
    CURLcode res;
 
    /* In windows, this will init the winsock stuff */ 
    curl_global_init(CURL_GLOBAL_ALL);
 
    /* get a curl handle */ 
    curl = curl_easy_init();
    if(curl) 
    {
        /* First set the URL that is about to receive our POST. This URL can
        just as well be a https:// URL if that is what should receive the
        data. */ 
        curl_easy_setopt(curl, CURLOPT_URL, Url);
        
        // Setting headers in libcurl requests are done in a generic way, by building a list of our own headers and then passing that list to libcurl.
        struct curl_slist *headers=NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");

        /* Now specify the POST data */ 
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        
        /* pass our list of custom made headers */
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        
        /* Perform the request, res will get the return code */ 
        res = curl_easy_perform(curl);
        
        /* Check for errors */ 
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
 
        /* always cleanup */ 
        curl_easy_cleanup(curl);
        
        curl_slist_free_all(headers); /* free the header list */
    }
    curl_global_cleanup();   
	
}

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
