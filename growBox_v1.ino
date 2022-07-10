#include <Wire.h>                                  // Стандартная библиотека шины I2C
#include <AHT10.h>                                 // Библиотека датчика AHT10

#define I2C_SDA D2                                 // Пин для I2C
#define I2C_SCL D1                                //  Пин для I2C
#define LOAD_PIN1 D5                              //  Пин управления нагрузкой 1
#define LOAD_PIN2 D6                             //   Пин управления нагрузкой 2
#define POWER_PIN D8                            //    Пин включения питания датчиков I2C
 
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiClient.h>

#define SERVER_IP "192.168.0.91:8833"                        // адрес сервера  192.168.0.91:8833 
#ifndef STASSID
#define STASSID "************************"                   // имя роутера
#define STAPSK  "********"                                   // пароль роутера
#endif

AHT10Class AHT10;                                           // Создаем объект AHT10
ESP8266WiFiMulti WiFiMulti;                                 // Создаем объект для подключения WiFi
DynamicJsonDocument doc(1024);

String TempA, HumA;                                        // Переменные для значений сенсоров
int lightA;                                                // Переменные для значений сенсоров
bool Load1=false, Load2=false;                               // переменные для включения нагрузки

void setup() {
  pinMode(POWER_PIN, OUTPUT);                              // определяем пин питания датчиков
  pinMode(LOAD_PIN1, OUTPUT);                              // определяем пин питания потребителей
  pinMode(LOAD_PIN2, OUTPUT);                              // определяем пин питания потребителей
  pinMode(A0, INPUT);                                      // включаем  аналоговый пин на чтение
  Serial.begin(9600);
  TempA = "empty";
  HumA = "empty";
  
}

void loop() {
  checksensors();
  showdata();
  wifisession();
  if (Load1){digitalWrite(LOAD_PIN1, HIGH);} else {  digitalWrite(LOAD_PIN1, LOW);  }
  if (Load2){digitalWrite(LOAD_PIN2, HIGH);} else {  digitalWrite(LOAD_PIN2, LOW);  }
  // Если включена нагрузка, включаем delay, если нет спящий энергосберегающий режим
  if (Load1 || Load2) { delay(600000); } else { ESP.deepSleep(600e6);}
}

void checksensors(){
  digitalWrite(POWER_PIN, HIGH);                           // подаем питание на датчики
  delay(500);
  Wire.begin(I2C_SDA, I2C_SCL);                            // Инициализируем шину I2C
  delay(2000);                                             // Пауза в 2 секунды прогрев датчиков
  if(AHT10.begin(0x38)){                                   // Инициализируем датчик с адресом 0x38
    TempA = String(AHT10.GetTemperature());                // Считываем показание температуры первого 
    HumA = String(AHT10.GetHumidity());                    // Считываем показание влажности первого
  }
  else{while(1);    }                                      // Заканчиваем выполнение
  digitalWrite(POWER_PIN, LOW);                            // убираем питание с датчиков

  lightA = analogRead(A0);                                  // считать напряжение с пина A0
  
}

void showdata(){
  Serial.print("Температура: ");                           // Печатаем "Температура: "
  Serial.print(TempA);                                    // Печатаем показание температуры
  Serial.println(" *C");                                 // Печатаем " *C"
  Serial.print("Влажность: ");                              // Печатаем "Влажность: "
  Serial.print(HumA);                                      // Печатаем показание влажности
  Serial.println(" %");                                   // Печатаем " %"
  Serial.print("Свет: ");                                  // Печатаем "Свет: "
  Serial.print(lightA);                                   // Печатаем показание освещенности
  Serial.println(" ед");
  Serial.println();
}

void wifisession(){
   WiFi.mode(WIFI_STA);                                  // Определяем режим для WiFi сети
   WiFiMulti.addAP(STASSID, STAPSK);                     // Варианты для логирования в сети 
   while (WiFiMulti.run() != WL_CONNECTED) {             // Пока не подключились выполняем печать точки....
     delay(500);
     Serial.print(".");
   }
   if ((WiFiMulti.run() == WL_CONNECTED)) {               // Если подключились к сети WiFi 
     Serial.println("connected");                         // печатаем сообщение
     getrequest();                                        // выполняем отправку GET запроса 
     Serial.println("GetRequest finished");
     delay(1000);
     postrequest();                                        // выполняем отправку POST запроса 
     Serial.println("PostRequest finished");
  }
}

void postrequest(){                                       // функция отправки запроса 
  WiFiClient client;
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, "https://" SERVER_IP "/test")) {
    http.addHeader("Content-Type", "application/json");
    String sendText = "{\"bot\":\"testBot\",\"data\":{\"temp\":\"" + String(TempA) + "\",\"hum\":\"" + String(HumA) +"\",\"switch1\":\"" + String(Load1) +"\", \"switch2\":\"" + String(Load2) +"\", \"light\":\"" + String(lightA) +"\"}}";
    int httpCode = http.POST(sendText);
    if (httpCode > 0) {
      //Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("AnsCode:" + String(httpCode) + ";Payload:" + String(payload));
      }
      delay(500);
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      Serial.println();
    }
    http.end();
  }else {
   Serial.printf("[HTTP} Unable to connect\n");
  }
}

void getrequest(){                                       // функция отправки запроса 
  WiFiClient client;
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  if (http.begin(client, "https://" SERVER_IP "/test")) {
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.GET();
    if (httpCode > 0) {
      //Serial.printf("[HTTP] POST... code: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("AnsCode:" + String(httpCode) + ";Payload:" + String(payload));
        deserializeJson(doc, payload);
          if(String(doc["switch1"]) != "false"){Load1=true;}
          else {Load1=false;}
          if(String(doc["switch2"]) != "false"){Load2=true;}
          else {Load2=false;}
        }
      delay(500);
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      Serial.println();
    }
    http.end();
  }else {
   Serial.printf("[HTTP} Unable to connect\n");
  }
}
