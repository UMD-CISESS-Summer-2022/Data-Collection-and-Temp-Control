#include <Wire.h>
char buffer[8];
String stringOne;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(115200);
}

void loop()
{
  float data = -510.05;
  for(int i=0; i<60; i++) {
    Wire.beginTransmission(8); // transmit to device #8
    dtostrf(data, 7, 2, buffer);
    Wire.write(buffer);
    Serial.println(buffer);
    Wire.endTransmission();    // stop transmitting
    data += 0.2;
    delay(1);
  }
}
