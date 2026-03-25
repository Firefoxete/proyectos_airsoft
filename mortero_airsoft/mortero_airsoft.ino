//LIBRERIAS//
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
//LCD//
LiquidCrystal_I2C lcd(32,16,2);
//vARIABLES//
int subir=2;
int bajar=3;
int aceptar=4;
int impacto=5;
int LedVerde=6;
int LedRojo=7;
int Altavoz=8;
int BLANCO=0;
int CONT=0;
int a;

unsigned long tiempo;
unsigned long tiempo_max=5000;

//PROGRAMA
void setup()
{
  pinMode(subir, INPUT_PULLUP);
  pinMode(bajar, INPUT_PULLUP);
  pinMode(aceptar, INPUT_PULLUP);
  pinMode(impacto, INPUT_PULLUP);
  pinMode(LedVerde, OUTPUT);
  pinMode(LedRojo, OUTPUT);
  pinMode(Altavoz, OUTPUT);

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  tono();
  lcd.setCursor(0,0);
  lcd.print(" INTRO ACIERTOS");
  lcd.setCursor(0,1);
  lcd.print(" PULSA +/-");
}

void loop()
{
 if(digitalRead(subir)==0 && CONT==0){//blancos + 
    tono();
 	BLANCO++;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ACIERTOS:");
    lcd.setCursor(10,0);
    lcd.print(BLANCO);
    delay(1000);
  }
 if(digitalRead(bajar)==0 && CONT==0 && BLANCO>0){//blancos -
    tono();
 	BLANCO--;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ACIERTOS:");
    lcd.setCursor(10,0);
    lcd.print(BLANCO);
    delay(1000);
  }
  if(digitalRead(aceptar)==0 && CONT==0){//aceptar blancos
    if(BLANCO>0){
      tono();
      CONT=1;
      lcd.clear();
      delay(100);
      lcd.setCursor(0,0);
      lcd.print("BLANCOS");
      lcd.setCursor(8,0);
      lcd.print(BLANCO);
      lcd.setCursor(0,1);
      lcd.print("SI/NO");
    }
    delay(1000);
  }
  if(digitalRead(subir)==0 && CONT==1){//confirmar blancos
      tono();
      CONT=2;
      delay(100);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ACEPTADO");
      delay(1000);	
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ESPERANDO");
      lcd.setCursor(0,1);
      lcd.print("IMPACTO");
      verde();
  }else if(digitalRead(bajar)==0 && CONT==1){//denegar blancos
      tono();
      CONT=0;
      BLANCO=0;
      delay(100);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CANCELADO");
      delay(1000);	
      lcd.clear();
      lcd.setCursor(0,0);
  	  lcd.print(" INTRO ACIERTOS");
  	  lcd.setCursor(0,1);
  	  lcd.print(" PULSA +/");
      rojo();
  }
  if(digitalRead(impacto)==0 && CONT==2){//impacto
    while(digitalRead(impacto)==0){ 
      // para que el boton no se quede enganchado.
    }
    tono();
    lcd.clear();
    BLANCO--;
    lcd.setCursor(0,0);
    lcd.print("ALERTA IMPACTO");
    rojo();
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("QUEDAN");
    lcd.setCursor(8,1);
    lcd.print(BLANCO);
    delay(500);
    if(BLANCO==0){
      tono();
      CONT=3;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("BLANCO ELIMINADO");
    }else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("ESPERANDO");
      lcd.setCursor(0,1);
      lcd.print("IMPACTO");
    }
  }
  while(digitalRead(aceptar)==0 && CONT==3){//reiniciar
    if(millis()-tiempo==tiempo_max){
      tono();
      CONT=0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("JUEGO REINICIADO");
      delay(1000);
      lcd.setCursor(0,0);
      lcd.print(" INTRO ACIERTOS");
      lcd.setCursor(0,1);
      lcd.print(" PULSA +/");
    }
  }
   while(digitalRead(bajar)==0 && CONT==2)
   {
     if(millis()-tiempo==2000){
     	tono();
        lcd.clear();
        BLANCO--;
        lcd.setCursor(0,0);
        lcd.print("ALERTA IMPACTO");
        rojo();
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("QUEDAN");
        lcd.setCursor(8,1);
        lcd.print(BLANCO);
        delay(500);
        if(BLANCO==0){
          tono();
          CONT=3;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("BLANCO ELIMINADO");
        }else{
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("ESPERANDO");
          lcd.setCursor(0,1);
          lcd.print("IMPACTO");
        }
     }
  }
  tiempo=millis();
}
void tono()//tono
{
  tone(Altavoz,2000);
  delay(50);
  noTone(Altavoz);
  delay(100);
}
void verde()//led verde
{
  digitalWrite(LedVerde,HIGH);
  delay(500);
  digitalWrite(LedVerde,LOW);
  delay(100);
}
void rojo()//led rojo
{
  digitalWrite(LedRojo,HIGH);
  delay(500);
  digitalWrite(LedRojo,LOW);
  delay(100);
}