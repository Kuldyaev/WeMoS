#include <Wire.h>             
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#define SERIAL_BAUD 115200

#define I2C_SDA 4
#define I2C_SCL 5
Adafruit_BMP085 bmp;
void setup(){
  pinMode(2, OUTPUT);
  Serial.begin(SERIAL_BAUD);
  while(!Serial) {} // Wait
  Wire.begin(I2C_SDA, I2C_SCL);
  delay(500);
  Serial.println("\nI2C Scanner");
} 

void loop(){
    byte error, address;
    int nDevices;

    digitalWrite(2, HIGH);
    delay(500);
    Serial.println("Scanning...");
 
    nDevices = 0;
    for(address = 8; address < 127; address++ ){
        Wire.beginTransmission(address);
        error = Wire.endTransmission();
 
        if (error == 0){
            Serial.print("I2C device found at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.print(address,HEX);
            Serial.println(" !");
 
            nDevices++;
        }
        else if (error==4) {
            Serial.print("Unknow error at address 0x");
            if (address<16)
                Serial.print("0");
            Serial.println(address,HEX);
        } 
    }
    if (nDevices == 0)
        Serial.println("No I2C devices found\n");
    else
        Serial.println("done\n");
  delay(1000);
    if (!bmp.begin(0x77)) {
      Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
      delay(2000);
    }
    Serial.print("Temperature = ");
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");
    
    Serial.print("Pressure = ");
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");
  delay(15000);
}
