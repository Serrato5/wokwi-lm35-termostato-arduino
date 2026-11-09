/*
  Simulación Wokwi: LED, botón, potenciómetro y LM35 con Arduino UNO
  Autor: Sebastián Serrato Ramírez (20241220023) - Universidad Surcolombiana

  Funcionamiento (termostato con alarma):
  - El LM35 mide la temperatura (10 mV/°C) en A1.
  - El potenciómetro (A0) fija el umbral de alarma entre 0 y 100 °C.
  - Si la temperatura supera el umbral, el LED (pin 13) se enciende.
  - El botón (pin 2) activa o desactiva la alarma.
  - El monitor serial muestra todas las lecturas.
*/

const int PIN_LED   = 13;
const int PIN_BOTON = 2;
const int PIN_POT   = A0;
const int PIN_LM35  = A1;

bool alarmaActiva = true;          // la alarma arranca activada
int  estadoAnterior = HIGH;        // con INPUT_PULLUP, suelto = HIGH
unsigned long ultimoCambio  = 0;   // para el antirrebote
unsigned long ultimoReporte = 0;   // para imprimir cada 500 ms

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BOTON, INPUT_PULLUP);   // resistencia pull-up interna: no hace falta resistencia externa
  Serial.begin(9600);
  Serial.println("Termostato con LM35 listo");
}

void loop() {
  // 1. Botón: cambia el estado de la alarma en cada pulsación (antirrebote de 50 ms)
  int lectura = digitalRead(PIN_BOTON);
  if (lectura != estadoAnterior && millis() - ultimoCambio > 50) {
    ultimoCambio = millis();
    estadoAnterior = lectura;
    if (lectura == LOW) {               // LOW = botón presionado
      alarmaActiva = !alarmaActiva;
      Serial.println(alarmaActiva ? ">> Alarma ACTIVADA" : ">> Alarma DESACTIVADA");
    }
  }

  // 2. LM35: ADC de 10 bits con referencia de 5 V -> voltaje -> °C
  int   adcLM35     = analogRead(PIN_LM35);
  float voltaje     = adcLM35 * 5.0 / 1023.0;   // V
  float temperatura = voltaje * 100.0;          // 10 mV/°C  ->  T = V / 0,01

  // 3. Potenciómetro: umbral de 0 a 100 °C
  int   adcPot = analogRead(PIN_POT);
  float umbral = adcPot * 100.0 / 1023.0;

  // 4. Decisión: LED encendido si la alarma está activa y T > umbral
  bool alarma = alarmaActiva && (temperatura > umbral);
  digitalWrite(PIN_LED, alarma ? HIGH : LOW);

  // 5. Reporte por el monitor serial cada 500 ms
  if (millis() - ultimoReporte >= 500) {
    ultimoReporte = millis();
    Serial.print("T = ");       Serial.print(temperatura, 1);
    Serial.print(" C | Umbral = "); Serial.print(umbral, 1);
    Serial.print(" C | Alarma: ");  Serial.print(alarmaActiva ? "ON" : "OFF");
    Serial.print(" | LED: ");       Serial.println(alarma ? "ENCENDIDO" : "apagado");
  }
}
