/*
* TTVCNL4040
* Version 1.0 June, 2017
* Copyright 2017 TOLDO TECHNIK
* For more details, see https://github.com/TOLDOTECHNIK/TTVCNL4040
*/

#ifndef TTVCNL4040_H
#define TTVCNL4040_H

#include <Wire.h>
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <Wprogram.h>
#endif

//CONSTANTS
#define TTVCNL4040_I2C_ADDRESS              0x60
#define TTVCNL4040_ID                       0x186

#define TTVCNL4040_REG_ALS_CONF             0x00
#define TTVCNL4040_REG_ALS_THDH_L           0x01
#define TTVCNL4040_REG_ALS_THDL_L           0x02
#define TTVCNL4040_REG_PS_CONF1             0x03
#define TTVCNL4040_REG_PS_CONF3             0x04
#define TTVCNL4040_REG_PS_CANC_L            0x05
#define TTVCNL4040_REG_PS_THDL_L            0x06
#define TTVCNL4040_REG_PS_THDH_L            0x07
#define TTVCNL4040_REG_PS_Data_L            0x08
#define TTVCNL4040_REG_ALS_Data_L           0x09
#define TTVCNL4040_REG_White_Data_L         0x0A
#define TTVCNL4040_REG_INT_Flag_H           0x0B
#define TTVCNL4040_REG_ID_L                 0x0C

#define TTVCNL4040_BIT_CONF_ALS_SD          0x00
#define TTVCNL4040_BIT_CONF_ALS_INT_EN      0x01
#define TTVCNL4040_BIT_CONF_ALS_PERS        0x02
#define TTVCNL4040_BIT_CONF_ALS_IT          0x06

#define TTVCNL4040_BIT_CONF1_PS_SD          0x00
#define TTVCNL4040_BIT_CONF1_PS_IT_1        0x01
#define TTVCNL4040_BIT_CONF1_PS_IT_2        0x02
#define TTVCNL4040_BIT_CONF1_PS_IT_3        0x03
#define TTVCNL4040_BIT_CONF1_PS_PERS_1      0x04
#define TTVCNL4040_BIT_CONF1_PS_PERS_2      0x05
#define TTVCNL4040_BIT_CONF1_PS_DUTY_1      0x06
#define TTVCNL4040_BIT_CONF1_PS_DUTY_2      0x07

#define TTVCNL4040_BIT_CONF2_PS_INT_1       0x00
#define TTVCNL4040_BIT_CONF2_PS_INT_2       0x01
#define TTVCNL4040_BIT_CONF2_PS_HD          0x03

#define TTVCNL4040_BIT_CONF3_LED_I          0x00
#define TTVCNL4040_BIT_CONF3_PS_TRIG        0x02
#define TTVCNL4040_BIT_CONF3_PS_AF          0x03
#define TTVCNL4040_BIT_CONF3_PS_SMART_PERS  0x04

#define TTVCNL4040_BIT_INT_Flag_PS_IF_AWAY  0x00
#define TTVCNL4040_BIT_INT_Flag_PS_IF_CLOSE 0x01
#define TTVCNL4040_BIT_INT_Flag_ALS_IF_H    0x04
#define TTVCNL4040_BIT_INT_Flag_ALS_IF_L    0x05

#define IR_PWR_50MA                         0x00
#define IR_PWR_75MA                         0x01
#define IR_PWR_100MA                        0x02
#define IR_PWR_120MA                        0x03
#define IR_PWR_140MA                        0x04
#define IR_PWR_160MA                        0x05
#define IR_PWR_180MA                        0x06
#define IR_PWR_200MA                        0x07

#define ALS_IT_80MS                         0x00
#define ALS_IT_160MS                        0x01
#define ALS_IT_320MS                        0x02
#define ALS_IT_640MS                        0x03

extern "C" {
  typedef void (*callbackFunction)(void);
}

class TTVCNL4040{
  public:
    TTVCNL4040();
    void update();

    boolean isConnected();
    boolean isALSEnabled();
    boolean isPSEnabled();
    
    void enableALS();
    unsigned int readALS();
    unsigned int readWhite();
    float readLux(); 
    void disableALS();
    void setALSThreshold(unsigned int lowerThreshold, unsigned int upperThreshold);
    void setALSIntegrationTime(uint8_t time);
    void attachALSSoftwareInterrupt(callbackFunction function);
    void attachALSHardwareInterrupt(uint8_t intNo, callbackFunction function);
    void disableALSSoftwareInterrupt();

    void enablePS(uint8_t irPower);
    unsigned int readPS();
    void disablePS();

    unsigned int readCommandRegister(uint8_t registerAddress);
    void writeCommandRegister(uint8_t registerAddress, byte lowByte, byte highByte);

  private:
    void setRegisterBits(uint8_t registerAddress, boolean manipulateHighByte, uint8_t startBit, uint8_t bitCount, uint8_t bits);
    unsigned long _timestamp;
    float _luxMultiplier;
    boolean _aLSEnabled;
    boolean _pSEnabled;
    boolean _aLSSoftwareInterruptEnabled;
    callbackFunction _aLSIntCallbackFunction;
    callbackFunction _pSIntCallbackFunction;
};

#endif