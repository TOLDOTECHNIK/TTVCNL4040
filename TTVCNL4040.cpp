/*
* TTVCNL4040
* Version 1.0 June, 2017
* Copyright 2017 TOLDO TECHNIK
* For more details, see https://github.com/TOLDOTECHNIK/TTVCNL4040
*/

#include "TTVCNL4040.h"

TTVCNL4040::TTVCNL4040(){
   Wire.begin();
   _aLSSoftwareInterruptEnabled = false;
   _aLSEnabled = false;
   _pSEnabled = false;
   _luxMultiplier = 0.12;
}

void TTVCNL4040::update(){
  if(_aLSIntCallbackFunction || _pSIntCallbackFunction){
    if(millis() - _timestamp > 100){
      uint8_t flag = highByte(readCommandRegister(TTVCNL4040_REG_INT_Flag_H));

      if(bitRead(flag, TTVCNL4040_BIT_INT_Flag_ALS_IF_H) || bitRead(flag, TTVCNL4040_BIT_INT_Flag_ALS_IF_L)){
        if (_aLSIntCallbackFunction) _aLSIntCallbackFunction();
      }

      if(bitRead(flag, TTVCNL4040_BIT_INT_Flag_PS_IF_AWAY) || bitRead(flag, TTVCNL4040_BIT_INT_Flag_PS_IF_AWAY)){
        if (_pSIntCallbackFunction) _pSIntCallbackFunction();
      }
      
      _timestamp = millis();
    }
  }
}

boolean TTVCNL4040::isConnected(){
  return readCommandRegister(TTVCNL4040_REG_ID_L) == TTVCNL4040_ID;
}

void TTVCNL4040::enableALS(){
  setRegisterBits(TTVCNL4040_REG_ALS_CONF, false, TTVCNL4040_BIT_CONF_ALS_SD, 1, 0);
  _aLSEnabled = true;
}

boolean TTVCNL4040::isALSEnabled(){
  return _aLSEnabled;
}

unsigned int TTVCNL4040::readALS(){
  if(isALSEnabled()){
    return readCommandRegister(TTVCNL4040_REG_ALS_Data_L);
  }
  else{
    return 0;
  }
}

unsigned int TTVCNL4040::readWhite(){
  if(isALSEnabled()){
    return readCommandRegister(TTVCNL4040_REG_White_Data_L);
  }
  else{
    return 0;
  }
}

float TTVCNL4040::readLux(){
  return readALS() * _luxMultiplier;
}

void TTVCNL4040::disableALS(){
  setRegisterBits(TTVCNL4040_REG_ALS_CONF, false, TTVCNL4040_BIT_CONF_ALS_SD, 1, 1);
  _aLSEnabled = false;
}

void TTVCNL4040::setALSThreshold(unsigned int lowerThreshold, unsigned int upperThreshold){
  writeCommandRegister(TTVCNL4040_REG_ALS_THDH_L, lowByte(upperThreshold), highByte(upperThreshold));
  writeCommandRegister(TTVCNL4040_REG_ALS_THDL_L, lowByte(lowerThreshold), highByte(lowerThreshold));
}

void TTVCNL4040::attachALSSoftwareInterrupt(callbackFunction function){
  setRegisterBits(TTVCNL4040_REG_ALS_CONF, false, TTVCNL4040_BIT_CONF_ALS_INT_EN, 1, 1);
  _aLSIntCallbackFunction = function;
}

void TTVCNL4040::disableALSSoftwareInterrupt(){
  _aLSIntCallbackFunction = NULL;
}

void TTVCNL4040::attachALSHardwareInterrupt(uint8_t intNo, callbackFunction function){

}

void TTVCNL4040::enablePS(uint8_t irPower){
  setRegisterBits(TTVCNL4040_REG_PS_CONF3, true, TTVCNL4040_BIT_CONF3_LED_I, 3, irPower);
  setRegisterBits(TTVCNL4040_REG_PS_CONF1, false, TTVCNL4040_BIT_CONF1_PS_SD, 1, 0);
  _pSEnabled = true;
}

boolean TTVCNL4040::isPSEnabled(){
  return _pSEnabled;
}

void TTVCNL4040::setALSIntegrationTime(uint8_t time){
  switch(time){
    case ALS_IT_160MS:
      setRegisterBits(TTVCNL4040_REG_ALS_CONF, false, TTVCNL4040_BIT_CONF_ALS_IT, 2, ALS_IT_160MS);
      _luxMultiplier = 0.06;
    break;
    case ALS_IT_320MS:
      setRegisterBits(TTVCNL4040_REG_ALS_CONF, false, TTVCNL4040_BIT_CONF_ALS_IT, 2, ALS_IT_320MS);
      _luxMultiplier = 0.03;
    break;
    case ALS_IT_640MS:
      setRegisterBits(TTVCNL4040_REG_ALS_CONF, false, TTVCNL4040_BIT_CONF_ALS_IT, 2, ALS_IT_640MS);
      _luxMultiplier = 0.015;
    break;
    default:
      setRegisterBits(TTVCNL4040_REG_ALS_CONF, false, TTVCNL4040_BIT_CONF_ALS_IT, 2, ALS_IT_80MS);
      _luxMultiplier = 0.12;
  }
}

unsigned int TTVCNL4040::readPS(){
  if(isPSEnabled()){
    return readCommandRegister(TTVCNL4040_REG_PS_Data_L);
  }
  else{
    return 0;
  }
}

void TTVCNL4040::disablePS(){
  setRegisterBits(TTVCNL4040_REG_PS_CONF1, false, TTVCNL4040_BIT_CONF1_PS_SD, 1, 1);
  _pSEnabled = false;
}

unsigned int TTVCNL4040::readCommandRegister(uint8_t registerAddress){
  byte lowValue = 0;
  byte highValue = 0;

  Wire.beginTransmission(TTVCNL4040_I2C_ADDRESS);
  Wire.write(registerAddress);
  Wire.endTransmission(false);
  Wire.requestFrom(TTVCNL4040_I2C_ADDRESS, 2);

  if(Wire.available() > 1){
    lowByte = Wire.read();
    highValue = Wire.read();
  }

  return ((unsigned int)(highValue) << 8 | lowValue);
}

void TTVCNL4040::writeCommandRegister(uint8_t registerAddress, byte lowByte, byte highByte){
  Wire.beginTransmission(TTVCNL4040_I2C_ADDRESS);
  Wire.write(registerAddress);
  Wire.write(lowByte);
  Wire.write(highByte);
  Wire.endTransmission();
}

void TTVCNL4040::setRegisterBits(uint8_t registerAddress, boolean manipulateHighByte, uint8_t startBit, uint8_t bitCount, uint8_t bits){
  unsigned int registerValue = readCommandRegister(registerAddress);
  uint8_t value = manipulateHighByte ? registerValue >> 8 : registerValue & 0xFF;
  for(uint8_t index = 0; index < bitCount; index++){
    if((bits >> index) & 0x01){
      bitSet(value, index + startBit);
    }
    else{
      bitClear(value, index + startBit);
    }
  }
  writeCommandRegister(registerAddress, manipulateHighByte ? registerValue & 0xFF : value, manipulateHighByte ? value : registerValue >> 8);
}