/** 
 * @file  thingsboard.h
 * @brief Header file containing functions prototypes for telemetry,
 *        RPC, device and attribute management on a IoT Gateway device
 *        and Thingsboard dashboard
 * 
 * @author Athang Gupte
 *
 *  NOTE: For Thingsboard demo goto 
 *        https://thingsboard.io/docs/getting-started-guides/helloworld
 */

/*! Include Guard */
#ifndef __THINGSBOARD_H__
#define __THINGSBOARD_H__

/*! CPP guard */
#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>
#include <shunyaInterfaces.h>

#define MAX_HOSTNAME_SIZE 1024
#define ACCESS_TOKEN_SIZE 512

/**
 * @brief Thingsboard settings
 * 
 */
struct ThingsboardSettings { 
        char hostname[MAX_HOSTNAME_SIZE]; /* Dashboard Hostname */
        char accessToken[ACCESS_TOKEN_SIZE]; /* Dashboard  */
};

/**
 * @brief subscribe attribute callback
 * 
 * @param device_name name of the device whose attributes were changed
 * @param attributes JSON object containing changed attributes and their values
 *
 * @return void
 */
typedef void (*Attributes_Callback)(const char* device_name, const char* attributes);

/**
 * @brief subscribe RPC callback
 * 
 * @param device_name device on which RPC method is to be executed
 * @param method the method to execute
 * @param params JSON object comtaining parameters to pass to the called method
 *
 * @return 0 for SUCCESS 
 */
typedef int (*RPC_Callback)(const char* device_name, const char* method, const char* params);


#ifdef USE_NAMESPACE
typedef struct {
        static void connect(struct ThingsboardSettings set);
        static void publishTelemetry(const char* jsonObject);
        static void publishAttributes(const char* jsonObject);
        static void subscribeToAttributes();
        static void subscribeToRPC();
        static void notifyDeviceConnect(const char* deviceName);
        static void notifyDeviceDisconnect(const char* deviceName);
} Thingsboard;
#endif


/*
 *#####################################################################
 *  Dashboard API's
 *  ------------------------
 *#####################################################################
 */

/**
 * @brief Connect to Thingsboard
 * 
 * @param set the Thingsboard settings 
 *
 * @return void
 */
void connectToThingsboard(mqttObj broker1)

/**
 * @brief Disconnect from Thingsboard
 *
 * @return void
 */
void disconnectFromThingsboard(mqttObj broker1);

/**
 * @brief Publish to Thingsboard
 * 
 * @param jsonObject the telemetry payload in JSON format
 *
 * @return 0 on Success
 */
int publishTelemetryToThingsboard(mqttObj broker1);

/**
 * @brief Publish device attribute to Thingsboard
 * 
 * @param jsonObject the telemetry payload in JSON format
 *
 * @return 0 on Success
 */
int publishAttributeToThingsboard(mqttObj broker1);

/**
 * @brief Subscribe to device attribute updates from Thingsboard
 * 
 * @param userCallback callback function to handle attribute updates
 *        from Thingsboard
 * 
 * @return void
 */
void subscribeToAttributesFromThingsboard(mqttObj broker1, Attributes_Callback userCallback);

/**
 * @brief Subscribe to Remote Procedure Calls from Thingsboard
 * 
 * @param userCallback callback function to handle Remote Procedure
 *        call request from Thingsboard
 * 
 * @return void
 */
void subscribeToRPCFromThingsboard(mqttObj broker1, RPC_Callback userCallback);

/**
 * @brief Notify Thingsboard that device has connected
 * 
 * @param deviceName the name of the device which has connected
 * 
 * @return void
 */
void notifyDeviceConnectToThingsboard(mqttObj broker1, const char* deviceName);

/**
 * @brief Notify Thingsboard that device has disconnected
 * 
 * @param deviceName the name of the device which has disconnected
 * 
 * @return void
 */
void notifyDeviceDisonnectToThingsboard(mqttObj broker1, const char* deviceName);


#ifdef __cplusplus
}
#endif /* End of CPP guard */

#endif /* __THINGSBOARD_H__ */  /* End of Include Guard */
