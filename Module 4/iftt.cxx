/** @file <iftt>.cxx
 *  @brief This is a program which contains functions that can be used to send data to the IFTT platform.
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
 *   @brief  Returns the pressure from the sensor
 * 
 *   @returns the pressure value (in Pascal) read from the device
 */
void http_post(char *Url,char *data);
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
