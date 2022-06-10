#include <Wire.h>                                  // Стандартная библиотека шины I2C
#include <AHT10.h>                                 // Библиотека датчика
#define I2C_SDA 4
#define I2C_SCL 5

AHT10Class AHT10;                                  // Создаем объект AHT10
AHT10Class AHT10_2;                                  // Создаем второй объект AHT10

void setup() {
   Serial.begin(9600); 
     pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);
   // Инициализируем монитор порта на скорости 9600
   Wire.begin(I2C_SDA, I2C_SCL);                                  // Инициализируем шину I2C
  if(AHT10.begin(0x38))                            // Инициализируем датчик с адресом 0x38
    Serial.println("AHT10 подключен.");            // Если удачно печатаем "AHT10 подключен."
  else{
    Serial.println("AHT10 не подключен.");         // Если не удачно печатаем "AHT10 не подключен."
    while(1);                                      // Заканчиваем выполнение
  }
 if(AHT10_2.begin(0x39))                            // Инициализируем датчик с адресом 0x38
    Serial.println("AHT10 второй подключен.");            // Если удачно печатаем "AHT10 подключен."
  else{
    Serial.println("AHT10 второй не подключен.");         // Если не удачно печатаем "AHT10 не подключен."
    while(1);                                      // Заканчиваем выполнение
  }
  
  Serial.println();                                // Печатаем пустую строку
  delay(2000);                                     // Пауза в 2 секунды
}

void loop() {
  float T = AHT10.GetTemperature();                // Считываем показание температуры
  float H = AHT10.GetHumidity();                   // Считываем показание влажности
  Serial.print("Температура: ");                   // Печатаем "Температура: "
  Serial.print(T);                                 // Печатаем показание температуры
  Serial.println(" *C");                           // Печатаем " *C"

  Serial.print("Влажность: ");                     // Печатаем "Влажность: "
  Serial.print(H);                                 // Печатаем показание влажности
  Serial.println(" %");                            // Печатаем " %"

  float T2 = AHT10_2.GetTemperature();                // Считываем показание температуры
  float H2 = AHT10_2.GetHumidity();                   // Считываем показание влажности
   Serial.print("Температура второго: ");                   // Печатаем "Температура: "
  Serial.print(T2);                                 // Печатаем показание температуры
  Serial.println(" *C");                           // Печатаем " *C"

  Serial.print("Влажность второго: ");                     // Печатаем "Влажность: "
  Serial.print(H2);                                 // Печатаем показание влажности
  Serial.println(" %");                            // Печатаем " %"
  
  
  Serial.println();
  delay(2000);
}
