//------------------------------------------------------------   LIBRERIAS   ------------------------------------------------------------
//librerias para el GPS
#include <NMEAGPS.h>
#include <GPSport.h>

//librerias para el data logger
#include <SPI.h>
#include <SD.h>

//librerias para el sensor de temperatura
#include <OneWire.h>
#include <DallasTemperature.h>

//libreria para el sensor de humedad
#include <DHT.h>

//librerias propias
#include "Biblioteca.h"
//#include <SoftwareSerial.h>

//-------------------------------------------------------------  OBJETOS Y VARIABLES   ------------------------------------------------  
// GPS
int i = 0;  //entero que se itera para comparar con la velocidad de escritura del data logger
NMEAGPS gps; 
gps_fix fix;
byte gps_set_sucess = 0;

//data logger
File ValoresSonda;

//Sensores de temperatura
OneWire oneWire(TempBusPin);
DallasTemperature sensors(&oneWire);
//float temp;
int NumSensTemp = 0;

//Sensor de humedad
DHT dht(HumPin, DHTTYPE);
//float humedad;

//-------------------------------------------------------------   FUNCIONES   ------------------------------------------------------------------

//---------------------------------------------------------------------------- MOVIDA --------------------------------------------------------------------------------------------
void sendUBX(uint8_t *MSG, uint8_t len) {
  for(int i=0; i<len; i++) {
    gpsPort.write(MSG[i]);
  }
  gpsPort.println();
}

boolean getUBX_ACK(uint8_t *MSG) {
  uint8_t b;
  uint8_t ackByteID = 0;
  uint8_t ackPacket[10];
  unsigned long startTime = millis();
  ValoresSonda.print(" * Reading ACK response: ");
 
  // Construct the expected ACK packet    
  ackPacket[0] = 0xB5;  // header
  ackPacket[1] = 0x62;  // header
  ackPacket[2] = 0x05;  // class
  ackPacket[3] = 0x01;  // id
  ackPacket[4] = 0x02;  // length
  ackPacket[5] = 0x00;
  ackPacket[6] = MSG[2];  // ACK class
  ackPacket[7] = MSG[3];  // ACK id
  ackPacket[8] = 0;   // CK_A
  ackPacket[9] = 0;   // CK_B
 
  // Calculate the checksums
  for (uint8_t i=2; i<8; i++) {
    ackPacket[8] = ackPacket[8] + ackPacket[i];
    ackPacket[9] = ackPacket[9] + ackPacket[8];
  }
 
  while (1) {
 
    // Test for success
    if (ackByteID > 9) {
      // All packets in order!
      ValoresSonda.println(" (SUCCESS!)");
      return true;
    }
 
    // Timeout if no valid response in 3 seconds
    if (millis() - startTime > 3000) { 
      ValoresSonda.println(" (FAILED!)");
      return false;
    }
 
    // Make sure data is available to read
    if (gpsPort.available()) {
      b = gpsPort.read();
 
      // Check that bytes arrive in sequence as per expected ACK packet
      if (b == ackPacket[ackByteID]) { 
        ackByteID++;
        ValoresSonda.print(b, HEX);
      } 
      else {
        ackByteID = 0;  // Reset and look again, invalid order
      }
    }
  }
}
//------------------------------------------------------------------------------ FIN MOVIDA -------------------------------------------------------------------------------------

void GPSloop();
void GPSloop(){
  while (gps.available(gpsPort)){
    fix = gps.read();
    Write_DataLogger(fix);
  }
}

static void Write_DataLogger();
static void Write_DataLogger(const gps_fix & fix){
  if(i == writeTime){

    if (fix.valid.time) {
    // Set these values to the offset of your timezone from GMT
    static const int32_t zone_hours   = +2L; // CEST
    static const int32_t zone_minutes =  0L; // usually zero
    static const NeoGPS::clock_t zone_offset  = zone_hours * NeoGPS::SECONDS_PER_HOUR + zone_minutes * NeoGPS::SECONDS_PER_MINUTE;
    NeoGPS::time_t localTime( fix.dateTime + zone_offset );
    ValoresSonda << localTime;
  }

  ValoresSonda = SD.open("ValoresSonda.txt", FILE_WRITE);

  if(ValoresSonda){
  if (fix.valid.location){
    
    if ( fix.dateTime.seconds < 10 ) ValoresSonda.print( '0' );
    ValoresSonda.print( fix.dateTime.seconds );
    ValoresSonda.print(",");
    
    if (fix.valid.satellites) ValoresSonda.print(fix.satellites);
    ValoresSonda.print(",");

    ValoresSonda.print(fix.alt.whole);
    ValoresSonda.print(",");

    ValoresSonda.print(fix.latitudeL());
    ValoresSonda.print(",");

    ValoresSonda.print(fix.longitudeL());
    ValoresSonda.print(",");

    ValoresSonda.print(fix.speed_kph());
    ValoresSonda.print(",");
      
  }

    for(int i = 0; i < NumSensTemp; i++){
    //temperatura = sensors.getTempCByIndex(i);
    ValoresSonda.print(sensors.getTempCByIndex(i));
    ValoresSonda.print(",");
    }
    //humedad = dht.readHumidity();
    ValoresSonda.print(dht.readHumidity());
    ValoresSonda.print(",");
    
    ValoresSonda.println(); //salto de linea para una nueva tanda de valores a escribir en el data logger.
    i = 0;                //reinicio las iteraciones.
    
  } else{
    Serial.print("No es posible escribir en el data logger");
  }
  } else{
    i++;
  }
}



void CabeceraDataLoger();
void CabeceraDataLoger(){
  ValoresSonda = SD.open("ValoresSonda.txt", FILE_WRITE);
  ValoresSonda.println(" Fecha/hora | Satelites | Altura | Latitud | Longitud | Velocidad | Temperatura | Humedad");
  ValoresSonda.println("-----------------------------------------------------------------------------------------");
}

//------------------------------------------------------------------------------- SETUP ---------------------------------------------------------------------------------------------
void setup() {
  //Serial.begin(9600);
  gpsPort.begin(9600);
  sensors.begin();

  NumSensTemp = sensors.getDeviceCount();

  pinMode(HumPin, INPUT);

  //establezco el gps en flight mode para que funcione hasta casi 50Km.
  uint8_t setNav[] = {
    0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00,
    0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16, 0xDC };
  while(!gps_set_sucess)
  {
    sendUBX(setNav, sizeof(setNav)/sizeof(uint8_t));
    gps_set_sucess=getUBX_ACK(setNav);
  }
  gps_set_sucess=0;

  CabeceraDataLoger();

}

//------------------------------------------------------------------------------ LOOP ------------------------------------------------------------------------------------------------
void loop() {

  GPSloop();

}
