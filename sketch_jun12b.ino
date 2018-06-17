#include <Servo.h>

#include <DHT.h>
#include <DHT_U.h>

#include <SPI.h>
#include <Phpoc.h>

#define DHTPIN 2 // data pin to DHT22

#define DHTTYPE DHT22 // using DHT 22

DHT dht(DHTPIN, DHTTYPE);
Servo servo1;
PhpocServer server(80);
int cnt = 0;
bool manual = false;

void setup() {
  Serial.begin(9600);

  Phpoc.begin(PF_LOG_SPI | PF_LOG_NET);

  server.beginWebSocket("remote_push");
  servo1.attach(A1);

  dht.begin();
}

void loop() {
  float hum = dht.readHumidity();

  float tmp = dht.readTemperature();

  int cds = analogRead(A0);

  PhpocClient client = server.available();


  if (isnan(tmp) || isnan(hum)) {

    Serial.println("Reading error..");

  }
  else {
    if (client) {
      if (client.available() > 0) {
        // read the bytes incoming from the client:
        char thisChar = client.read();

        if (thisChar == 'A')
          manual = true;
        if (thisChar == 'B')
          manual = false;
      }
    }
    cnt++;
    if (cnt == 1000)
    {
      Serial.print("Temperature: ");

      Serial.print(tmp);

      Serial.print(" C\t");

      Serial.print("Humidity: ");

      Serial.print(hum);

      Serial.println(" %");

      Serial.print("Lightness : ");
      Serial.println(1024 - cds);
      cnt = 0;
    }
    if (hum >= 75 && cds >= 900 || cds < 500 || manual) // 비오는 날 혹은 너무 밝을 때
    {
      servo1.write(90);
    }
    else servo1.write(0);
  }
}
