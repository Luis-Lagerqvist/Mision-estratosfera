#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

float temp;
int numSensores = 0;

void setup() {
  sensors.begin();
  Serial.begin(9600);

  Serial.print("Numero de sensores: ");
  numSensores = sensors.getDeviceCount();
  Serial.println(numSensores);
  
}

void loop() {
  for(int i = 0; i < numSensores; i++){
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.print(" : ");
    temp = sensors.getTempCByIndex(i);
    Serial.print(temp);
    Serial.print((char)176);//shows degrees character
    Serial.println("C");
  }

  delay(2000);

}
