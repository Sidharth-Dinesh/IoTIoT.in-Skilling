/*
 * @file        problem2.cxx
 * @brief       This is a program to parse a JSON configuration file and configure Wifi and Ethernet parameters in Linux.
 * @author      Sidharth Dinesh
 * @bugs        No bugs found.
 */

/*


/*
 *#####################################################################
 *  Initialization block
 *  ---------------------
 *  This block contains initialization code for this particular file.
 *  It typically contains Includes, constants or global variables used
 *  throughout the file.
 *#####################################################################
 */

/* Standard Includes */
#include <cstdio>
#include <iostream>
#include <bits/stdc++.h> 
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

/* RapidJSON Includes */
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

/* --- Project Includes --- */

using namespace rapidjson;

/*
 *#####################################################################
 *  Process block
 *  -------------
 *  Solve all your problems here 
 *#####################################################################
 */
 
Document d;                     /* Declare a JSON document globally. JSON document parsed will be stored in this variable */

/** 
 *  @brief Description on function_1
 *  
 *  Full description of the function
 *
 *  @return List all Function returns 
 */
 
void json_parse(void)   /* 1. Parse a JSON string into DOM. */
{
    

    /* Open the example.json file in read mode */
    FILE* fp = fopen("example.json", "rb"); 

    /* Declare read buffer */
    char readBuffer[65536];

    /* Declare stream for reading the example stream */
    FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    
    /* Parse example.json and store it in `d` */
    d.ParseStream(is);

    /* Close the example.json file*/
    fclose(fp);
}

void wire_less()
{
    /* Declare an object to store the value 
    * and assign the document "wireless" "status" value to eStatus */
    
    Value& eStatus = d["wireless"]["status"];

    /* Print the string value */
    std::cout<<eStatus.GetString()<<std::endl;

    /* Modify the string value */
    eStatus.SetString("off");
    
    Value& eSsid = d["wireless"]["ssid"];
    Value& ePassword = d["wireless"]["password"];
    
    char cmd[128];
    sprintf(cmd,"wpa_passphrase %s %s | sudo tee /etc/wpa_supplicant.conf",eSsid.GetString(),eStatus.GetString());
	system(cmd);
    system("sudo wpa_supplicant -c /etc/wpa_supplicant.conf -i wlan0");
    system("wpa_cli -i wlan0 reconfigure");
    system("sudo dhclient wlan0");
}

void ethernet_static()
{
    /* Declare an object to store the value 
    * and assign the document "ethernet" "status" value to eStatus */
    
    Value& eStatus = d["ethernet"][0]["status"];

    /* Print the string value */
    std::cout<<eStatus.GetString()<<std::endl;

    /* Modify the string value */
    eStatus.SetString("off");
    
    Value& eAllocation = d["ethernet"][0]["allocation"];
    Value& eipa = d["ethernet"][0]["ipAddress"];
    Value& emask = d["ethernet"][0]["subnetMask"];
    Value& erouter = d["ethernet"][0]["routerAddress"];
    
    char cmd[128];
	//network interface
    char nwkInf[5]="eth0";

	//link down command in Linux
	sprintf(cmd,"ip link set %s down",nwkInf);
	system(cmd); 
	
	
	memset(cmd,0x00,64);
	//command to set ip address, netmask
	sprintf(cmd,"ifconfig %s %s netmask %s",nwkInf,eipa.GetString(),emask.GetString());
	system(cmd);	 
	printf("\ncmd : %s",cmd); fflush(stdout);
	memset(cmd,0X00,64);

	//command to set gateway
	sprintf(cmd,"route add default gw %s %s",erouter.GetString(),nwkInf);
	system(cmd); 

	memset(cmd,0X00,64);
	//link up command
	sprintf(cmd,"ip link set %s up",nwkInf);
	system(cmd); 
}

void ethernet_dhcp()
{
    system("echo “iface eth1 inet dhcp” >>/etc/network/interfaces");    //The first command updates /etc/network/interfaces file with eth1 interface to use DHCP.
    system("ifup eth1");                                                //The next command brings up the interface.
}

void host_name()
{   
    Value& ehost = d["hostname"];
    char cmd[128];
    sprintf(cmd,"sudo hostnamectl set-hostname %s",ehost.GetString());
    system(cmd);
}
void time_zone()
{   
    Value& etime = d["timezone"];
    char cmd[128];
    sprintf(cmd,"sudo timedatectl set-timezone %s",etime.GetString());
    system(cmd);
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
        json_parse();   //Calling parsing function
 
        wire_less();     //Configure wifi
        
        ethernet_static();     //Configure Ethernet with allocation=static
        
        ethernet_dhcp();        //Configure Ethernet with allocation=dhcp
        
        host_name();         //Configure Hostname
        
        time_zone();         //Configure Timezone
        
        return 0;
}

