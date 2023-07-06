//PIN_OUT del data logger
// CS pin 4 (vale cualquier digital)
// SCK pin 13
// MOSI pin 11
// MISO pin 12
// VCC 5V

//PIN_OUT del sensor de temperatura
//Pin 2

//||||||||||||||| CODIGO ESCRITO PARA HACER PRUEBAS CON SOLO 1 SENSOR DE TEMPERATURA, NO FUNCIONA SI SE USA OTRO SENSOR Y SI ES UN BUS SOLO LEERA EL 1 |||||||||||||||||

//librerias para el data logger
#include <SPI.h>
#include <SD.h>

//librerias para el sensor de temperatura
#include <OneWire.h>
#include <DallasTemperature.h>

//DEFINES
#define ONE_WIRE_BUS 2 
#define CSPin 4

//Objeto de la clase File al que acceder para escribir en el datalogger
File auxFile;

//objetos y variables para leer los sensores de temperatura ocupando solo 1 pin.
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
float temp;
int numSensores = 0;


void setup() {
  Serial.begin(9600);
  sensors.begin();

  Serial.print("Numero de sensores: ");
  numSensores = sensors.getDeviceCount();
  Serial.println(numSensores);

  while(!SD.begin(CSPin)){
    Serial.println("fallo en la inicializacion del data logger");
  }
  Serial.println("inicializacion del data logger correcta");

}

void loop() {
  
  while(!SD.begin(CSPin)){
    Serial.println("No se puede escribir en la SD");
  }
  
  auxFile = SD.open("Pruebas.txt", FILE_WRITE);

  if(auxFile){
    auxFile.print(millis());
    auxFile.print(", ");
    auxFile.print(lecturaTemperatura(ONE_WIRE_BUS));
  }

    Serial.print(millis());
    Serial.print(", ");
    Serial.print(lecturaTemperatura(ONE_WIRE_BUS));

    delay(2000);

}

float lecturaTemperatura(int pin){
    temp = sensors.getTempCByIndex(0);
    return temp;
}
