#include <SPI.h>        // Librería para comunicación SPI
#include <nRF24L01.h>   // Definiciones del módulo NRF24L01
#include <RF24.h>       // Librería para controlar el NRF24L01

// Pines usados para el módulo NRF24
#define CE_PIN  16
#define CSN_PIN 10

// Pines del bus SPI
#define SPI_SCK  12
#define SPI_MISO 13
#define SPI_MOSI 11

// Pin donde está conectado el LED
#define LED_PIN 2

// Se crea el objeto radio
RF24 radio(CE_PIN, CSN_PIN);

// Dirección de comunicación (debe ser la misma que el transmisor)
const byte address[6] = "00001";

void setup() {

  Serial.begin(115200);   // Inicializa la comunicación serial
  delay(300);             // Pequeña pausa de inicio

  pinMode(LED_PIN, OUTPUT); // Configura el LED como salida

  // Inicializa comunicación SPI con los pines definidos
  SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);

  // Verifica que el módulo NRF24 funcione
  if (!radio.begin()) {
    Serial.println("Error NRF24");
    while (1); // Detiene el programa si hay error
  }

  // Configuración del módulo
  radio.setPALevel(RF24_PA_MIN);     // Potencia baja
  radio.setDataRate(RF24_250KBPS);   // Velocidad de transmisión
  radio.setChannel(76);              // Canal de comunicación

  // Abre el canal de lectura
  radio.openReadingPipe(1, address);

  // Se configura como receptor
  radio.startListening();

  Serial.println("Receptor listo");
}

void loop() {

  // Verifica si hay datos disponibles
  if (radio.available()) {

    int sensorValue;

    // Lee el dato enviado por el transmisor
    radio.read(&sensorValue, sizeof(sensorValue));

    Serial.print("Valor recibido: ");
    Serial.println(sensorValue);

    // Convierte el rango del ESP32 (0-4095) a un rango PWM (0-255)
    int brillo = map(sensorValue, 0, 4095, 0, 255);

    // Ajusta el brillo del LED según el valor recibido
    analogWrite(LED_PIN, brillo);

  }
}
