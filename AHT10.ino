#include <Wire.h>                                  // Стандартная библиотека шины I2C
#include <AHT10.h>                                 // Библиотека датчика AHT10

#define I2C_SDA D2                                 // Пин для I2C
#define I2C_SCL D1                                //  Пин для I2C
#define LOAD_PIN1 D5                              //  Пин управления нагрузкой 1
#define LOAD_PIN2 D6                             //   Пин управления нагрузкой 2
#define POWER_PIN D8                            //    Пин включения питания датчиков I2C
 
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "https://e419-46-73-129-192.eu.ngrok.io/"  // адрес сервера 
#ifndef STASSID
#define STASSID "***********"                                   // имя роутера
#define STAPSK  "************"                                  // пароль роутера
#endif

AHT10Class AHT10;                                           // Создаем объект AHT10

void setup() {
  pinMode(POWER_PIN, OUTPUT);                               // определяем пин питания датчиков
  pinMode(LOAD_PIN1, OUTPUT);                              // определяем пин питания потребителей
  pinMode(LOAD_PIN2, OUTPUT);                              // определяем пин питания потребителей
  Serial.begin(9600);
  
}

void loop() {
   operation();
   delay(1000);
}

void operation(){

  digitalWrite(POWER_PIN, HIGH);                           // подаем питание на датчики
  delay(500);
  Wire.begin(I2C_SDA, I2C_SCL);                           // Инициализируем шину I2C
  delay(500);
  if(AHT10.begin(0x38))                            // Инициализируем датчик с адресом 0x38
    Serial.println("AHT10 первый подключен.");     // Если удачно печатаем "AHT10 подключен."
  else{
    Serial.println("AHT10 первый не подключен.");  // Если не удачно печатаем "AHT10 не подключен."
    while(1);                                      // Заканчиваем выполнение
  }
  Serial.println("*");                             // Печатаем пустую строку
  delay(2000);                                     // Пауза в 2 секунды
  float T = AHT10.GetTemperature();                // Считываем показание температуры первого 
  float H = AHT10.GetHumidity();                   // Считываем показание влажности первого
  Serial.print("Температура: ");                   // Печатаем "Температура: "
  Serial.print(T);                                 // Печатаем показание температуры
  Serial.println(" *C");                           // Печатаем " *C"
  Serial.print("Влажность: ");                     // Печатаем "Влажность: "
  Serial.print(H);                                 // Печатаем показание влажности
  Serial.println(" %");                            // Печатаем " %"
  Serial.println();
  digitalWrite(POWER_PIN, LOW);                    // убираем питание с датчиков
  delay(5500);
}


}
