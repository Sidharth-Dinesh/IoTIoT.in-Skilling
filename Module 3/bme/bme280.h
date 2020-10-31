/** @file <bme280>.h
 *  @brief Header file contains the function prototypes of all the functions in the bme280.c file
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
 
#ifndef BME280_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define BME280_H

/* --- Standard Includes --- */
#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>
#include <math.h>

#include <shunyaInterfaces.h>
#include <simple-mode.h>
#include <functions.h>

/*!
 *  @brief  default I2C address
 */
#define BME280_ADDRESS (0x77)           // Primary I2C Address
                                        /*!
                                         *  @brief  alternate I2C address
                                         */
#define BME280_ADDRESS_ALTERNATE (0x76) // Alternate Address

/*!
 *  @brief Register addresses
 */
enum {
  BME280_REGISTER_DIG_T1 = 0x88,
  BME280_REGISTER_DIG_T2 = 0x8A,
  BME280_REGISTER_DIG_T3 = 0x8C,

  BME280_REGISTER_DIG_P1 = 0x8E,
  BME280_REGISTER_DIG_P2 = 0x90,
  BME280_REGISTER_DIG_P3 = 0x92,
  BME280_REGISTER_DIG_P4 = 0x94,
  BME280_REGISTER_DIG_P5 = 0x96,
  BME280_REGISTER_DIG_P6 = 0x98,
  BME280_REGISTER_DIG_P7 = 0x9A,
  BME280_REGISTER_DIG_P8 = 0x9C,
  BME280_REGISTER_DIG_P9 = 0x9E,

  BME280_REGISTER_DIG_H1 = 0xA1,
  BME280_REGISTER_DIG_H2 = 0xE1,
  BME280_REGISTER_DIG_H3 = 0xE3,
  BME280_REGISTER_DIG_H4 = 0xE4,
  BME280_REGISTER_DIG_H5 = 0xE5,
  BME280_REGISTER_DIG_H6 = 0xE7,

  BME280_REGISTER_CHIPID = 0xD0,
  BME280_REGISTER_VERSION = 0xD1,
  BME280_REGISTER_SOFTRESET = 0xE0,

  BME280_REGISTER_CAL26 = 0xE1, // R calibration stored in 0xE1-0xF0

  BME280_REGISTER_CONTROLHUMID = 0xF2,
  BME280_REGISTER_STATUS = 0XF3,
  BME280_REGISTER_CONTROL = 0xF4,
  BME280_REGISTER_CONFIG = 0xF5,
  BME280_REGISTER_PRESSUREDATA = 0xF7,
  BME280_REGISTER_TEMPDATA = 0xFA,
  BME280_REGISTER_HUMIDDATA = 0xFD
};

/**************************************************************************/
/*!
    @brief  calibration data
*/
/**************************************************************************/
typedef struct {
  uint16_t dig_T1; ///< temperature compensation value
  int16_t dig_T2;  ///< temperature compensation value
  int16_t dig_T3;  ///< temperature compensation value

  uint16_t dig_P1; ///< pressure compensation value
  int16_t dig_P2;  ///< pressure compensation value
  int16_t dig_P3;  ///< pressure compensation value
  int16_t dig_P4;  ///< pressure compensation value
  int16_t dig_P5;  ///< pressure compensation value
  int16_t dig_P6;  ///< pressure compensation value
  int16_t dig_P7;  ///< pressure compensation value
  int16_t dig_P8;  ///< pressure compensation value
  int16_t dig_P9;  ///< pressure compensation value

  uint8_t dig_H1; ///< humidity compensation value
  int16_t dig_H2; ///< humidity compensation value
  uint8_t dig_H3; ///< humidity compensation value
  int16_t dig_H4; ///< humidity compensation value
  int16_t dig_H5; ///< humidity compensation value
  int8_t dig_H6;  ///< humidity compensation value
} bme280_calib_data;
/*=========================================================================*/

void readCoefficients(void);
bool isReadingCalibration(void);

void write8(byte reg, byte value);
uint8_t read8(byte reg);
uint16_t read16(byte reg);
uint32_t read24(byte reg);
int16_t readS16(byte reg);
uint16_t read16_LE(byte reg); // little endian
int16_t readS16_LE(byte reg); // little endian

uint8_t _i2caddr;  //!< I2C addr for the TwoWire interface
int32_t _sensorID; //!< ID of the BME Sensor
int32_t t_fine; //!< temperature with high resolution, stored as an attribute
                  //!< as this is used for temperature compensation reading
                  //!< humidity and pressure

int32_t t_fine_adjust = 0; //!< add to compensate temp readings and in turn
                             //!< to pressure and humidity readings

bme280_calib_data _bme280_calib; //!< here calibration data is stored

bool begin(uint8_t addr = BME280_ADDRESS);

float getTemperature(void);
float getPressure(void);
int8_t getHumidity(void);
float getAltitude(void);

#endif
