//PINOUT DEFINITONS
//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||  SENSORES  ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// Temperatura D7 TIENE QUE LLEVAR UNA RESISTENCIA DE 4.7K CONECTADO A 5V
// Humedad D6
#define TempBusPin 25  //Pin del bus de sensores de temperatura, trabaja con pines digitales.
#define HumPin  24 //Pin del sensor de humedad
#define DHTTYPE DHT11 //tipo de sensor DHT, nosotros usamos el DHT11

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||  DATA LOGGER  |||||||||||||||||||||||||||||||||||||||||||||||||||||||
// CS pin 28 D10 (vale cualquier digital)
// hay que usar estos pines 
// SCK pin 1 D13
// MOSI pin 29 D11
// MISO pin 30 D12
// VCC 5V
#define CSPin 28 //Pin de comunicacion con el data logger

//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||  GPS  |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
// TX D9
// RX D8  TIENE QUE LLEVAR UN RESISTENCIA DE 1K
#define GPS_txPin 27
#define GPS_rxPin 26

//AUX DEFINITIONS
#define writeTime 30 //iteraciones a esperar para volver a escribir un valor, una tasa muy baja haria que el gps no llegara a actualizarse a tiempo.
