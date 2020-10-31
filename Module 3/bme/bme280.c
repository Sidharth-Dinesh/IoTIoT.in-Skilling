/** @file <bme280>.c
 *  @brief This is a program to read values from the BME280 Sensor  
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
#include "bme280.h"

#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>
#include <math.h>

#include <shunyaInterfaces.h>
#include <simple-mode.h>
#include <functions.h>

bool begin(uint8_t addr) {
    
    bool status = false;
    
    wireBegin(1);
    _i2caddr = addr;
    
    // if chip is still reading calibration, delay
    while (isReadingCalibration())
        delay(10);

    readCoefficients(); // read trimming parameters, see DS 4.2.2

    delay(100);
    
    return true;
}

/*!
 *   @brief  Writes an 8 bit value over I2C 
 *   @param reg the register address to write to
 *   @param value the value to write to the register
 */
void write8(byte reg, byte value) {

    wireBeginTransmission((uint8_t)_i2caddr);
    wireWrite((uint8_t)reg);
    wireWrite((uint8_t)value);
    wireEndTransmission();
}

/*!
 *   @brief  Reads an 8 bit value over I2C 
 *   @param reg the register address to read from
 *   @returns the data byte read from the device
 */
uint8_t read8(byte reg) {
    uint8_t value;
    
    wireBeginTransmission((uint8_t)_i2caddr);
    wireWrite((uint8_t)reg);
    wireEndTransmission();
    
    wireRequestFrom((uint8_t)_i2caddr,(byte)1);
    value = wireRead();
    
    return value;
}

/*!
 *   @brief  Reads a 16 bit value over I2C 
 *   @param reg the register address to read from
 *   @returns the 16 bit data value read from the device
 */
uint16_t read16(byte reg) {
    uint16_t value;
    
    wireBeginTransmission((uint8_t)_i2caddr);
    wireWrite((uint8_t)reg);
    wireEndTransmission();
    
    wireRequestFrom((uint8_t)_i2caddr,(byte)2);
    value = (wireRead() << 8) | wireRead();

    return value;
}

/*!
 *   @brief  Reads a signed 16 bit little endian value over I2C 
 *   @param reg the register address to read from
 *   @returns the 16 bit data value read from the device
 */
uint16_t read16_LE(byte reg) {
  uint16_t temp = read16(reg);
  return (temp >> 8) | (temp << 8);
}

/*!
 *   @brief  Reads a signed 16 bit value over I2C 
 *   @param reg the register address to read from
 *   @returns the 16 bit data value read from the device
 */
int16_t readS16(byte reg) { return (int16_t)read16(reg); }

/*!
 *   @brief  Reads a signed little endian 16 bit value over I2C 
 *   @param reg the register address to read from
 *   @returns the 16 bit data value read from the device
 */
int16_t readS16_LE(byte reg) {
  return (int16_t)read16_LE(reg);
}

/*!
 *   @brief  Reads a 24 bit value over I2C
 *   @param reg the register address to read from
 *   @returns the 24 bit data value read from the device
 */
uint32_t read24(byte reg) {
    uint32_t value;
    
    wireBeginTransmission((uint8_t)_i2caddr);
    wireWrite((uint8_t)reg);
    wireEndTransmission();
    
    wireRequestFrom((uint8_t)_i2caddr,(byte)3);

    value = wireRead();
    value <<= 8;
    value |= wireRead();
    value <<= 8;
    value |= wireRead();
    
    return value;
}

void readCoefficients(void) {
  _bme280_calib.dig_T1 = read16_LE(BME280_REGISTER_DIG_T1);
  _bme280_calib.dig_T2 = readS16_LE(BME280_REGISTER_DIG_T2);
  _bme280_calib.dig_T3 = readS16_LE(BME280_REGISTER_DIG_T3);

  _bme280_calib.dig_P1 = read16_LE(BME280_REGISTER_DIG_P1);
  _bme280_calib.dig_P2 = readS16_LE(BME280_REGISTER_DIG_P2);
  _bme280_calib.dig_P3 = readS16_LE(BME280_REGISTER_DIG_P3);
  _bme280_calib.dig_P4 = readS16_LE(BME280_REGISTER_DIG_P4);
  _bme280_calib.dig_P5 = readS16_LE(BME280_REGISTER_DIG_P5);
  _bme280_calib.dig_P6 = readS16_LE(BME280_REGISTER_DIG_P6);
  _bme280_calib.dig_P7 = readS16_LE(BME280_REGISTER_DIG_P7);
  _bme280_calib.dig_P8 = readS16_LE(BME280_REGISTER_DIG_P8);
  _bme280_calib.dig_P9 = readS16_LE(BME280_REGISTER_DIG_P9);

  _bme280_calib.dig_H1 = read8(BME280_REGISTER_DIG_H1);
  _bme280_calib.dig_H2 = readS16_LE(BME280_REGISTER_DIG_H2);
  _bme280_calib.dig_H3 = read8(BME280_REGISTER_DIG_H3);
  _bme280_calib.dig_H4 = ((int8_t)read8(BME280_REGISTER_DIG_H4) << 4) |
                         (read8(BME280_REGISTER_DIG_H4 + 1) & 0xF);
  _bme280_calib.dig_H5 = ((int8_t)read8(BME280_REGISTER_DIG_H5 + 1) << 4) |
                         (read8(BME280_REGISTER_DIG_H5) >> 4);
  _bme280_calib.dig_H6 = (int8_t)read8(BME280_REGISTER_DIG_H6);
}

/*!
 *   @brief return true if chip is busy reading cal data
 *   @returns true if reading calibration, false otherwise
 */
bool isReadingCalibration(void) {
  uint8_t const rStatus = read8(BME280_REGISTER_STATUS);

  return (rStatus & (1 << 0)) != 0;
}

/** 
 *  @brief Returns the temperature from the sensor
 *  @return the temperature read from the device 
 */

float getTemperature(void)
{
    int32_t var1, var2;

    int32_t adc_T = read24(BME280_REGISTER_TEMPDATA);
    
    if (adc_T == 0x800000) // value in case temp measurement was disabled
        return NAN;
        
    adc_T >>= 4;

    var1 = ((((adc_T >> 3) - ((int32_t)_bme280_calib.dig_T1 << 1))) * ((int32_t)_bme280_calib.dig_T2)) >> 11;

    var2 = (((((adc_T >> 4) - ((int32_t)_bme280_calib.dig_T1)) *((adc_T >> 4) - ((int32_t)_bme280_calib.dig_T1))) >>12) *((int32_t)_bme280_calib.dig_T3)) >> 14;

    t_fine = var1 + var2 + t_fine_adjust;
    
    float T = (t_fine * 5 + 128) >> 8;
    
    return T / 100;
}

/** 
 *   @brief  Returns the pressure from the sensor
 * 
 *   @returns the pressure value (in Pascal) read from the device
 */

float getPressure(void)
{
    int64_t var1, var2, p;

    getTemperature(); // must be done first to get t_fine

    int32_t adc_P = read24(BME280_REGISTER_PRESSUREDATA);
    
    if (adc_P == 0x800000) // value in case pressure measurement was disabled
        return NAN;
        
    adc_P >>= 4;

    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)_bme280_calib.dig_P6;
    var2 = var2 + ((var1 * (int64_t)_bme280_calib.dig_P5) << 17);
    var2 = var2 + (((int64_t)_bme280_calib.dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)_bme280_calib.dig_P3) >> 8) +((var1 * (int64_t)_bme280_calib.dig_P2) << 12);
    var1 =(((((int64_t)1) << 47) + var1)) * ((int64_t)_bme280_calib.dig_P1) >> 33;

    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)_bme280_calib.dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)_bme280_calib.dig_P8) * p) >> 19;

    p = ((p + var1 + var2) >> 8) + (((int64_t)_bme280_calib.dig_P7) << 4);
    
    return (float)p / 256;
}

/** 
 *  @brief  Returns the humidity from the sensor
 * 
 *  @return the humidity value read from the device
 */

int8_t getHumidity(void)
{
    getTemperature(); // must be done first to get t_fine

    int32_t adc_H = read16(BME280_REGISTER_HUMIDDATA);
    
    if (adc_H == 0x8000) // value in case humidity measurement was disabled
        return NAN;

    int32_t v_x1_u32r;

    v_x1_u32r = (t_fine - ((int32_t)76800));

    v_x1_u32r = (((((adc_H << 14) - (((int32_t)_bme280_calib.dig_H4) << 20) -(((int32_t)_bme280_calib.dig_H5) * v_x1_u32r)) +((int32_t)16384)) >>15) *(((((((v_x1_u32r * ((int32_t)_bme280_calib.dig_H6)) >> 10) *(((v_x1_u32r * ((int32_t)_bme280_calib.dig_H3)) >> 11) +((int32_t)32768))) >>10) +((int32_t)2097152)) *((int32_t)_bme280_calib.dig_H2) +8192) >>14));

    v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) *((int32_t)_bme280_calib.dig_H1)) >>4));

    v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
    v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;
    
    float h = (v_x1_u32r >> 12);
    
    return (int8_t)(h/1024.0);
}

/** 
 *  @brief Calculates the altitude (in meters) from the specified atmospheric
 *         pressure (in hPa), and sea-level pressure (in hPa).
 *  @return the altitude value read from the device 
 */

float getAltitude(void)
{
    float seaLevel=1013.25;
    float atmospheric = getPressure();
    
    return 44330.0 * (1.0 - pow(atmospheric / seaLevel, 0.1903));
}
