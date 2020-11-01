/** @file <iftt>.h
 *  @brief Header file contains the function prototypes of all the functions in the iftt.cxx file
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
 
#ifndef IFTT_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define IFFT_H

/* --- Standard Includes --- */
#include <stdio.h>
#include <stdlib.h>
#include <shunyaInterfaces.h>
#include <curl/curl.h>
#include <iostream>
#include <string>

/* --- Project Includes --- */


/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here 
 *#####################################################################
 */
 
 void http_post(char *Url,char *data);
 
 
 #endif
