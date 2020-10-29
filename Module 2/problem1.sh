## @file <file_name>.sh

 #@file        problem1.sh
 #@brief       This is a shell script to connect to a bluetooth device in Linux.
 #@author      Sidharth Dinesh
 #@bugs        No bugs found.
 
#!/bin/bash

#
 ######################################################################
 #  Main callback
 #  -------------
 #  All your function callbacks and shell commands go below
 ######################################################################
 #
 
 hcitool dev                    #Identify the Bluetooth device we want to scan from with hcitool dev.
 
 hcitool -i hci0 scan           #Scan available devices and view MAC address of the device u want to connect to
 
 read -p "Enter MAC address : " mac  
 echo "The MAC address: $mac"
 
 echo "connect $mac" | bluetoothctl         #Connect it from the bluetoothctl console
