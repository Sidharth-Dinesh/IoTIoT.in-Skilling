/** 
 * @file  thingsboard.cxx
 * @brief File containing functions for telemetry,
 *        RPC, device and attribute management on a IoT Gateway device
 *        and Thingsboard dashboard
 * 
 * @author Athang Gupte
 *
 *  NOTE: For Thingsboard API goto 
 *        https://thingsboard.io/docs/reference/gateway-mqtt-api
 */

/*
 *#####################################################################
 *  Initialization block
 *  ---------------------
 *#####################################################################
 */

/* --- Standard Includes --- */
#include <string.h>
#include <shunyaInterfaces.h>

/* --- Project Includes --- */
#include <mqtt.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"

#include "thingsboard.h"

/* --- Static Definitions --- */
static Attributes_Callback userAttribsCallback;
static RPC_Callback userRPCCallback;

/*
 *#####################################################################
 *  Process block
 *  -------------
 *#####################################################################
 */

/**
 * @brief internal callback for received attribute messages from Thingsboard
 * 
 * @param topic_len length of the topic name in bytes
 * @param topic_name name of the topic receiving messages from
 * @param payload_len length of the payload string
 * @param payload the main payload message string
 *
 */
static void attribs_callback(int topic_len, char* topic_name, int payload_len, char* payload)
{	
	/* Parse the payload as a JSON */
	rapidjson::Document d;
	d.Parse(payload);

	/* Create a C-string from the 'data' JSON object */
	rapidjson::StringBuffer attribsBuf;
	rapidjson::Writer<StringBuffer> writer(attribsBuf);
	rapidjson::Value& attribsObj = d["data"];
	paramsObj.Accept(writer);

	/* Store the stringified attributes object */
	char* attribs = new char[attribsBuf.GetLength()];
	strcpy(params, attribsBuf.GetString());

	/* call the user-defined attributes callback to handle attributes update */
	userAttribsCallback()
}

/**
 * @brief internal callback for received RPC from Thingsboard
 * 
 * @param topic_len length of the topic name in bytes
 * @param topic_name name of the topic receiving messages from
 * @param payload_len length of the payload string
 * @param payload the main payload message string
 *
 */
static void rpc_callback(int topic_len, char* topic_name, int payload_len, char* payload,mqttObj broker1)
{
	/* Parse the payload as a JSON */
	rapidjson::Document d;
	d.Parse(payload);

	/* Retrieve the name of the device on which RPC has been requested */
	char* device = new char[d["device"].GetStringLength()];
	strcpy(device, d["device"].GetString());

	/* Retrieve the name of the method called */
	char* method = new char[d["data"]["method"].GetStringLength()];
	strcpy(method, d["data"]["method"].GetString());

	/* Create a C-string from the 'params' JSON object */
	rapidjson::StringBuffer paramsBuf;
	rapidjson::Writer<StringBuffer> writer(paramsBuf);
	rapidjson::Value& paramsObj = d["data"]["params"];
	paramsObj.Accept(writer);

	/* Store the stringified params */
	char* params = new char[paramsBuf.GetLength()];
	strcpy(params, paramsBuf.GetString());

	/* Call the user-defined RPC callback to handle the RPC request */
	bool output = userRPCCallback(device, method, params);

	/* Send response back to the RPC server to update the RPC success status */
	char response[256];
	sprintf(response, "{\"device\":\"%s\", \"id\":%s, \"data\":{\"success\":%s}}", device, d["data"]["id"].GetString(), (outcome == 0) ? "true" : "false");
	mqttPublish(&broker1,"v1/gateway/rpc", "%s", response);
}

/*
 *#####################################################################
 *  API exposed block
 *  -----------------
 *#####################################################################
 */

 /**
 * @brief Connect to Thingsboard
 * 
 */
void connectToThingsboard(mqttObj broker1)
{
	/* Connect to broker */
	mqttConnect(&broker1);

}

/**
 * @brief Disconnect from Thingsboard
 *
 */
void disconnectFromThingsboard(mqttObj broker1)
{
	mqttDisconnect(&broker1);
}

/**
 * @brief Publish telemetry to Thingsboard
 * 
 */
int publishTelemetryToThingsboard(mqttObj broker1);
{   
    char buffer[]="{\"temperature\":47.6,\"humidity\":86,}";
    
    /* Publish to topic */
    mqttPublish(&broker1, "v1/gateway/telemetry","%s",buffer);
    
}

/**
 * @brief Publish device attribute to Thingsboard
 * 
 */
int publishAttributeToThingsboard(mqttObj broker1)
{   
    char buffer[]="{\"device\": \"Device A\", \"data\": {\"attribute1\": \"value1\", \"attribute2\": 42}}";
    
    /* Publish to topic */
    mqttPublish(&broker1,"v1/gateway/attributes","%s",buffer);
}

/**
 * @brief Subscribe to device attribute updates from Thingsboard
 * 
 */
void subscribeToAttributesFromThingsboard(mqttObj broker1, Attributes_Callback userCallback)
{
	// Make a callback function which parses the received 
	// JSON to retrieve the device name and attributes data object
	// and then calls the userCallback with the retrieved params
	userAttribsCallback = userCallback;
	
	/* Subscribe to topic */
    mqttSubscribe(&broker1, "v1/gateway/attributes",attribs_callback) ;
    
    mqttSetSubCallback(&broker1, attribs_callback);
}

/**
 * @brief Subscribe to Remote Procedure Calls from Thingsboard
 * 
 */
void subscribeToRPCFromThingsboard(mqttObj broker1, RPC_Callback userCallback);
{
	// Make a callback function which parses the received 
	// JSON to retrieve the device name, called method name and 
	// parameters to the method, then calls the userCallback 
	// and finally publishes the success message back to server
	userRPCCallback = userCallback;
	
	/* Subscribe to topic */
    mqttSubscribe(&broker1, "v1/gateway/rpc",rpc_callback) ;
    
    mqttSetSubCallback(&broker1,rpc_callback);
}

/**
 * @brief Notify Thingsboard that device has connected
 * 
 */
void notifyDeviceConnectToThingsboard(mqttObj broker1, const char* deviceName)
{
	mqttPublish(&broker1,"v1/gateway/connect", "{\"device\":\"%s\"}", deviceName);
}

/**
 * @brief Notify Thingsboard that device has disconnected
 * 
 */
void notifyDeviceDisonnectToThingsboard(mqttObj broker1, const char* deviceName)
{
	mqttPublish(&broker1,"v1/gateway/disconnect", "{\"device\":\"%s\"}", deviceName);
}
