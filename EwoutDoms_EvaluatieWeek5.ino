#define BLYNK_TEMPLATE_ID "user2"
#define BLYNK_TEMPLATE_NAME "user2@server.wyns.it"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

int led = 21;
int pt = 32;
const int DHTPin = 4;
#define DHTTYPE DHT11
DHT dht(DHTPin, DHTTYPE);

char auth[] = "X8GS2yKNHz8-SjBw9qx8NFr_hovEsmbF";
char ssid[] = "embed";
char pass[] = "weareincontrol";

BlynkTimer timer;

void setup() {
  Serial.begin(115200);

  dht.begin();
  pinMode(led, OUTPUT);
  pinMode(pt, OUTPUT);
  analogWriteResolution(12);

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);
  timer.setInterval(1000L, sendPotValue);
}

void loop() {
  timer.run();
  Blynk.run();
  delay(1000);
}

void sendPotValue() {
  float temperature = dht.readTemperature();
  String temp = "Temperatuur: " + String(temperature) + "°C";
  Serial.println(temp);
  Blynk.virtualWrite(V2, temperature);

  int temppotentio = map(analogRead(pt), 0, 4095, 0, 30);
  Serial.println("pt 0-30: " + String(temppotentio));
  Blynk.virtualWrite(V1, temppotentio);

  if (temperature < temppotentio) {
    digitalWrite(led, HIGH);
    Blynk.virtualWrite(V3, 255);
  } else {
    digitalWrite(led, LOW);
    Blynk.virtualWrite(V3, 0);
  }
}



BLYNK_WRITE(V4) {
  float temperature = dht.readTemperature();
  String temp = "Temperatuur: " + String(temperature) + "°C";
  Serial.println(temp);
  Blynk.virtualWrite(V2, temperature);

  // Function to handle the value changes of a slider widget in the Blynk app
  int newPotValue = param.asInt();      // Get the new value from the app
  Blynk.virtualWrite(V1, newPotValue);  // Update the value of virtual pin V1

  if (newPotValue > temperature) {
    digitalWrite(led, HIGH);
    Blynk.virtualWrite(V3, 255);
  } else {
    digitalWrite(led, LOW);
    Blynk.virtualWrite(V3, 0);
  }
  delay(1000);
}