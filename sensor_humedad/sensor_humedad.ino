#include <DHT.h>

#define DHTPin 2
#define DHTTYPE DHT11

DHT dht(DHTPin, DHTTYPE);

float humedad;

void setup() {
  Serial.begin(9600);
  dht.begin();

}

void loop() {
  humedad = dht.readHumidity();
  Serial.println(humedad);

}
