#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment time = Adafruit_7segment();

int sg0=0, sg1=0, mn=0, hr=0; 

// Pines
int verde=6;
int rojo=5;
int boton=4;
int Altavoz=3;

// Variables de control
int cont=0;
unsigned long tmp;
unsigned long tiempo_pulsacion;
unsigned long tiempo_max_pulso = 1000;

void setup() {
  pinMode(verde, OUTPUT);
  pinMode(rojo, OUTPUT);
  pinMode(Altavoz, OUTPUT);
  pinMode(boton, INPUT_PULLUP);
  
  digitalWrite(verde, LOW);
  digitalWrite(rojo, LOW);

  time.begin(0x70);
  sonido_inicio(); 
  resetearVariables();
  actualizarPantalla();
}

void loop() {
  // --- ESTADO 0: ESPERA ---
  if(cont == 0) {
    if(digitalRead(boton) == LOW) {
      if (tiempo_pulsacion == 0) tiempo_pulsacion = millis(); 
      if (millis() - tiempo_pulsacion >= tiempo_max_pulso) {
        sonido_boton();    
        digitalWrite(rojo, HIGH); 
        cont = 1;
        tiempo_pulsacion = 0;
        tmp = millis(); 
      }
    } else {
      tiempo_pulsacion = 0; 
    }
  } 

  // --- ESTADO 1: CUENTA REGRESIVA ---
  if(cont == 1) {
    if(millis() - tmp >= 1000) {
      tmp = millis();
      sg0--; 
      
      if(sg0 < 0) { sg0 = 9; sg1--; }
      if(sg1 < 0) { sg1 = 5; mn--; }
      
      actualizarPantalla(); // Actualizamos PRIMERO para que se vea el 00:00

      // Si aún no ha llegado a cero, pero faltan menos de 10 seg
      if(mn == 0 && sg1 == 0 && sg0 > 0) {
        tone(Altavoz, 2500, 70); // Pitido de aviso corto
      }

      // Si el tiempo se ha agotado
      if(mn == 0 && sg1 == 0 && sg0 == 0) {
        cont = 2; 
      }
    }
  }
  
  // --- ESTADO 2: FINALIZADO (Sonido largo y transición) ---
  if(cont == 2) {
    digitalWrite(rojo, LOW);
    digitalWrite(verde, HIGH);
    
    sonido_respawn(); // Este sonido ahora es más largo y complejo
    
    delay(4000); // Mantenemos el verde un poco más (el sonido ya consume tiempo)
    
    digitalWrite(verde, LOW);
    delay(1000); // Silencio de cortesía
    
    cont = 0; 
    resetearVariables();
    actualizarPantalla();
    sonido_inicio(); // Aviso de "Listo"
  }
}

// --- MEJORA DE SONIDOS ---

void sonido_inicio() {
  // Beep Beep rápido
  tone(Altavoz, 2000); delay(80); noTone(Altavoz);
  delay(80);
  tone(Altavoz, 2000); delay(80); noTone(Altavoz);
}

void sonido_boton() {
  tone(Altavoz, 2800, 200);
}

void sonido_respawn() {
  // Secuencia ascendente para dar sensación de finalización
  tone(Altavoz, 800, 200);  delay(200);
  tone(Altavoz, 1000, 200); delay(200);
  tone(Altavoz, 1200, 600); delay(600); // Tono final largo
}

void resetearVariables() {
  mn = 1; sg1 = 0; sg0 = 0; hr = 0;
}

void actualizarPantalla() {
  time.writeDigitNum(4, sg0);
  time.writeDigitNum(3, sg1);
  time.drawColon(true); 
  time.writeDigitNum(1, mn);
  time.writeDigitNum(0, hr);
  time.writeDisplay();
}