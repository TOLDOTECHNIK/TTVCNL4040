/*
  TTVCNL4040 example

  This example for the VCNL4040 shows how 
  to use the TTVCNL4040 library to configure
  and read the chip's sensor values.

  For further documentation please visit

  https://github.com/TOLDOTECHNIK/TTVCNL4040

*/

#include <TTVCNL4040.h>

TTVCNL4040 ttvcnl4040 = TTVCNL4040();

void setup(){
  Serial.begin(9600);
  ttvcnl4040.enablePS(IR_PWR_200MA);
  ttvcnl4040.enableALS();
  ttvcnl4040.setALSIntegrationTime(ALS_IT_640MS);
}


void loop(){
  Serial.print("PS: ");
  Serial.print(ttvcnl4040.readPS(), DEC);
  Serial.print(" LUX: ");
  Serial.print(ttvcnl4040.readLux(), DEC);
  Serial.print(" ALS: ");
  Serial.print(ttvcnl4040.readALS(), DEC);
  Serial.print(" White: ");
  Serial.println(ttvcnl4040.readWhite(), DEC);
  delay(200);
}
