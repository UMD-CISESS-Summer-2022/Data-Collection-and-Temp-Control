#include <RH_ASK.h>
#include <SPI.h>

RH_ASK rf_driver;
String str_out;

void setup() {
  rf_driver.init();
  Serial.begin(115200);
}

void loop() {
  uint8_t buf[8];
  uint8_t buflen = sizeof(buf);
  if (rf_driver.recv(buf, &buflen)) {
    str_out = String((char*)buf);
    Serial.println(str_out);
  }
}
