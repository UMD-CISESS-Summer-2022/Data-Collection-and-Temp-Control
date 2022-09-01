#include <RH_ASK.h>                //433MHz wireless
#include<SPI.h>                    //433MHz wireless (compile)
#include <Wire.h>                  //I2C serial communication
#include <LiquidCrystal_I2C.h>     //LCD

LiquidCrystal_I2C lcd(0x27, 16, 2);
RH_ASK rf_driver;

#define gatePin 10 //Transistor base
int thermistorPin = 0;

int Vo;
float R1 = 10000;
float logR2, R2, T, Tc, Tf;
float c1=0.001129148, c2=0.000234125, c3=8.76741e-8;

String dataString;
String dataRecord;
int counter = 0;
int loopCounter;
//char dataString[8];

void setup() {
  //Fan control setup
  pinMode(gatePin, OUTPUT);
  digitalWrite(gatePin, LOW);
//  analogWrite(gatePin, 0);
  
  //433MHz wireless setup
  rf_driver.init();

  //I2C serial communication setup
  Wire.begin(8);                           //Set as device #8
  Wire.onReceive(receiveEvent);

   //LCD setup
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Env Temp: ");
  lcd.print(getTemp());
  lcd.setCursor(0, 1);
  lcd.print("RM Data: ");

//  //Serial monitor
  Serial.begin(115200);
}

//Function for temperature calculation (convert voltage to celsius)
float getTemp() {
  Vo = analogRead(thermistorPin);
  R2 = R1 * (1023.0 / (float)Vo - 1.0);
  logR2 = log(R2);
  T = (1.0 / (c1 + c2*logR2 + c3*logR2*logR2*logR2)); //Kelvin
  Tc = T - 273.15;                                    //Celsius
  Tf = (Tc * 9.0)/ 5.0 + 32.0;                        //Fahrenheit
  return Tc;
}

//Function for analog value calculation for __% fan speed
float getFanVal(int percent) {
  float i = (percent/100)*(255);                   //Fan starts functioning at 50
  //i += 45;
  return i;
}

void loop()
{
  char* msg = dataRecord.c_str();
  rf_driver.send((uint8_t *)msg, 8);
  rf_driver.waitPacketSent();

  Serial.println(dataRecord);
  
  float envTemp = getTemp();
  //Serial.println(envTemp);
  
//  if (envTemp > 30) {                                 //30% speed when >30C
//    Serial.println("Fan at 30% speed");
//    analogWrite(gatePin, getFanVal(30));
//    if(envTemp > 50) {                                //60% speed when >50C
//      analogWrite(gatePin, getFanVal(60));
//      if(envTemp > 90) {                              //100% speed when >90C
//        analogWrite(gatePin, 255);
//      }
//    }
//  }

  loopCounter += 1;
  if (loopCounter == 50) {
    Serial.print("loopCounter = 100");
    lcd.setCursor(10, 0);
    lcd.print(envTemp);
    lcd.print("C");

    lcd.setCursor(9, 1);
    lcd.print(dataRecord);

    loopCounter = 0;
  }
  delay(10);
}

//I2C serial communication: peripheral receiver
void receiveEvent(char vn, float val) {
  while (Wire.available()) {
    char c = Wire.read();
    dataString += c;
  }
  //Serial.println(dataString);
  //strcpy(dataArray, datastring);
  dataRecord = dataString;
  dataString = "";
}


//433MHz wireless
//int rf433_send(char vn, float val) {
  //char* msg[8];
  //dataString.toCharArray(msg, 8);
  //rf_driver.send((uint8_t *)msg, strlen(msg));
  //rf_driver.waitPacketSent();
  
  //dataString = "";
//}
