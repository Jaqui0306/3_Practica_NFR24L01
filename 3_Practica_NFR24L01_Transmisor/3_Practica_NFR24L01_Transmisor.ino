#include <SPI.h>        // Librería para comunicación SPI
#include <nRF24L01.h>   // Definiciones del módulo NRF24L01
#include <RF24.h>       // Librería para controlar el NRF24L01

// Definición de pines usados por el módulo NRF24
#define CE_PIN   17
#define CSN_PIN  5
#define SPI_SCK  18
#define SPI_MOSI 23
#define SPI_MISO 19

// Pin donde está conectado el sensor (potenciómetro)
#define SENSOR_PIN 34

// Se crea el objeto radio indicando los pines CE y CSN
RF24 radio(CE_PIN, CSN_PIN);

// Dirección de comunicación entre transmisor y receptor
const byte address[6] = "00001";

void setup() {

  Serial.begin(115200);   // Inicializa la comunicación serial
  delay(300);             // Pequeña pausa para estabilizar

  // Inicializa el bus SPI con los pines definidos
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  // Verifica si el módulo NRF24 se inicializó correctamente
  if (!radio.begin()) {
    Serial.println("Error NRF24");
    while (1); // Detiene el programa si hay error
  }

  // Configuración del módulo de radio
  radio.setPALevel(RF24_PA_MIN);     // Nivel de potencia bajo
  radio.setDataRate(RF24_250KBPS);   // Velocidad de transmisión
  radio.setChannel(76);              // Canal de comunicación

  // Se abre el canal de envío
  radio.openWritingPipe(address);

  // Se configura como transmisor
  radio.stopListening();

  Serial.println("Transmisor listo");
}

void loop() {

  // Lectura del valor analógico del sensor
  int sensorValue = analogRead(SENSOR_PIN);

  // Envía el valor del sensor al receptor
  radio.write(&sensorValue, sizeof(sensorValue));

  // Muestra el valor enviado en el monitor serial
  Serial.print("Valor enviado: ");
  Serial.println(sensorValue);

  delay(100); // Espera 100 ms antes de volver a enviar
}
