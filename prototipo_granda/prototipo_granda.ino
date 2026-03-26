// Granada de sonido Airsoft - Versión ATtiny85
// Pin físico 6 (PB1 / Pin 1 en Arduino IDE)

const int buzzer = 1; // Usamos 'const' para ahorrar memoria RAM

void setup() {
  pinMode(buzzer, OUTPUT);

  // --- 1. SEÑAL DE ARMADO ---
  // Un pitido rápido para confirmar que tiene batería y está activa
  tone(buzzer, 3000, 100); 
  delay(150);

  // --- 2. RETARDO DE LANZAMIENTO ---
  delay(3000); // 3 segundos para lanzarla

  // --- 3. DETONACIÓN (Alarma) ---
  for(int x = 0; x < 5; x++) {
    tone(buzzer, 2000);
    delay(60);
    noTone(buzzer); // Usamos noTone para limpiar el sonido
    delay(100);
  }
}

void loop() {
  // --- 4. MODO LOCALIZADOR ---
  // Un pitido cada 10 segundos para encontrarla en la maleza
  delay(10000); 
  
  tone(buzzer, 2500, 100); // Pitido agudo de 0.1 segundos
}