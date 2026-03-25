#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pines de componentes
int condensador = 4;
int aceptar = 5;
int mas = 6;
int menos = 7;
int Altavoz = 8;

int rojo, verde, amarillo, azul; // Pines aleatorios para cables

// Variables de tiempo
unsigned long tiempoAnterior = 0;
long tiempoRestante = 0; 
long intervalo = 1000; // Velocidad del reloj (ms)

// Función para leer qué cable se ha activado
int cableActivo() {
  if (digitalRead(rojo) == LOW) return 1;
  if (digitalRead(verde) == LOW) return 2;
  if (digitalRead(amarillo) == LOW) return 3;
  if (digitalRead(azul) == LOW) return 4;
  return 0; 
}

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0)); // Aleatoriedad real

  // Mezclar funciones de los cables (Pines 9 a 12)
  int opciones[] = {9, 10, 11, 12}; 
  int n = 4;
  for (int i = 0; i < n; i++) {
    int r = random(i, n);
    int temp = opciones[i];
    opciones[i] = opciones[r];
    opciones[r] = temp;
  }

  rojo = opciones[0]; verde = opciones[1]; amarillo = opciones[2]; azul = opciones[3];

  pinMode(rojo, INPUT_PULLUP); pinMode(verde, INPUT_PULLUP);
  pinMode(amarillo, INPUT_PULLUP); pinMode(azul, INPUT_PULLUP);
  pinMode(mas, INPUT_PULLUP); pinMode(menos, INPUT_PULLUP);
  pinMode(aceptar, INPUT_PULLUP);
  pinMode(Altavoz, OUTPUT);
  pinMode(condensador, OUTPUT);

  lcd.init();
  lcd.backlight();
  sonido();
  
  // Inicio de LCD y bienvenida
  lcd.setCursor(3, 0);
  lcd.print("TERCIO GRT");
  lcd.setCursor(4, 1);
  lcd.print("AIRSOFT");
  delay(1500);

  tiempo_Juego(); 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("TIEMPO RESTANTE:");
}

void loop() {
  unsigned long tiempoActual = millis();

  // Lógica del cronómetro de cuenta atrás
  if (tiempoActual - tiempoAnterior >= intervalo && tiempoRestante > 0) {
    tiempoAnterior = tiempoActual;
    tiempoRestante--;

    int minutos = tiempoRestante / 60;
    int segundos = tiempoRestante % 60;

    // Mostrar MM:SS
    lcd.setCursor(5, 1); 
    if (minutos < 10) lcd.print("0");
    lcd.print(minutos);
    lcd.print(":");
    if (segundos < 10) lcd.print("0");
    lcd.print(segundos);

    tone(Altavoz, 2000, 50); // Beep de segundo

    if (tiempoRestante <= 0) {
      detonar();
    }
  }

  // Comprobar estado de cables
  int estado = cableActivo();
  if (estado != 0) {
    switch (estado) {
      case 1: detonar(); break; // Cable explosivo 
      case 2: desactiva(); break; // Cable correcto
      case 3: intervalo = 500; break; // Cable acelera tiempo
      case 4: break; // Cable neutro
    }
  }
}

// --- FUNCIONES ---
void tiempo_Juego() { //se establece el tiempo de juego
  int minutos_config = 5;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ajustar Tiempo:");
  
  while (true) {
    lcd.setCursor(6, 1);
    if (minutos_config < 10) lcd.print("0");
    lcd.print(minutos_config);
    lcd.print(" min ");

    if (digitalRead(mas) == LOW) { // Incrementa en uno el tiempo
      minutos_config++;
      if (minutos_config > 60) minutos_config = 60;
      delay(100); // rebote pulsador
    }
    if (digitalRead(menos) == LOW) { // Disminuye en uno el tiempo
      minutos_config--;
      if (minutos_config < 1) minutos_config = 1;
      delay(100); // rebote de pulsador
    }
    if (digitalRead(aceptar) == LOW) { // Aceptar tiempo
      tiempoRestante = (long)minutos_config * 60; 
      lcd.clear();
      lcd.setCursor(2, 0);
      lcd.print("BOMBA ARMADA");
      sonido();
      delay(1500);
      return;
    }
  }
}

void sonido() { //Sonido inicio
  tone(Altavoz, 2400, 100);
  delay(200);
  tone(Altavoz, 2400, 100);
}

void TNT() { // Detona condensador opcional
  digitalWrite(condensador, HIGH);
  delay(1000);
  digitalWrite(condensador, LOW);
}

void detonar() { 
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("BOOOOOM!");
  TNT();
  while(true) { // Alarma infinita
    tone(Altavoz, 500, 500);
    delay(500);
    tone(Altavoz, 300, 500);
    delay(500);
  }
}

void desactiva() {
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("Bomba");
  lcd.setCursor(2,1);
  lcd.print("Desactivada");
  sonido();
  while(true); // Bloqueo de fin de juego
}