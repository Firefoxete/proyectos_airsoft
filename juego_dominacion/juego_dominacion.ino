//librerias
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//pantalla
LiquidCrystal_I2C lcd(0x27,16,2);

//pines
int menos_A = 11; 
int mas_B = 10;
int aceptar = 9;
int Altavoz = A1;

//variables
int Tiempo_Partida=0;
int cont=0;
boolean equipo_a = false;
boolean equipo_b = false;

//variables de tiempo
unsigned long time = 0;
unsigned long tiempo = 0;
unsigned long tiempo_A = 0;
unsigned long tiempo_B = 0;
int segundo = 0;
int minuto = 0;
int segundo_a = 0;
int minuto_a = 0;
int segundo_b = 0;
int minuto_b = 0;
  
void setup() 
{
  pinMode(aceptar, INPUT_PULLUP);
  pinMode(menos_A, INPUT_PULLUP);
  pinMode(mas_B, INPUT_PULLUP);
  pinMode(Altavoz, OUTPUT);

  Serial.begin(9600);


  lcd.init();
  lcd.backlight();
  sonido();

  lcd.setCursor(3,0);
  lcd.print("TERCIO GRT");
  lcd.setCursor(4,1);
  lcd.print("AIRSOFT");

  delay(1500);

  lcd.clear();
  lcd.setCursor(6,0);
  lcd.print("JUEGO");
  lcd.setCursor(2,1);
  lcd.print("DE DOMINACION");

  delay(1500);

  for(int i=0;i<=38;i++){
    lcd.scrollDisplayRight();
    delay(100);
}
    delay(1000);
    lcd.clear();  
  
}

void loop()
{ 
  if(Tiempo_Partida==0 && cont==0)//mensaje de inicio
  {
    lcd.setCursor(3,0);
    lcd.print("Introduce");
    lcd.setCursor(1,1);
    lcd.print("tiempo partida");
    cont=1;
  }
  
  if(digitalRead(mas_B)==0 && cont==1)//sumar tiempo
  {
    Tiempo_Partida=Tiempo_Partida+1;
    
    if(Tiempo_Partida>60)
    {
      Tiempo_Partida=60;
    }
      lcd.clear();
      lcd.setCursor(1,0);
	    lcd.print("Tiempo partida");
      lcd.setCursor(6,1);
	    lcd.print(Tiempo_Partida);
	  if(Tiempo_Partida<=9)
      {
        lcd.setCursor(8,1);
        lcd.print("min");
      }
      else
      {
        lcd.setCursor(9,1);
        lcd.print("min");
      }
    
    delay(100);
  }
  
  if(digitalRead(menos_A)==0 && cont==1)//restar tiempo
  {
    Tiempo_Partida=Tiempo_Partida-1;
    
    if(Tiempo_Partida<0)
    {
      Tiempo_Partida=0;
    }
      lcd.clear();
      lcd.setCursor(1,0);
	    lcd.print("Tiempo partida");
      lcd.setCursor(6,1);
	    lcd.print(Tiempo_Partida);
	  if(Tiempo_Partida<=9)
      {
        lcd.setCursor(8,1);
        lcd.print("min");
      }
      else
      {
        lcd.setCursor(9,1);
        lcd.print("min");
      }  
    
    delay(100);  
  }
  //boton aceptar tiempo partida
  if(digitalRead(aceptar)==0 && Tiempo_Partida>0 && cont==1)
  {
   	cont=2;
      
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Tiempo partida");
      lcd.setCursor(2,1);
      lcd.print("Establecido");
   
    delay(1500);
    lcd.clear();
    
  }
  //inicia tiempo de juego
  if(cont==2)
  {
    if(Tiempo_Partida>minuto)//inicio de partida
    {
      segundos();
      contador();
      escribe_hora();
      
      time=millis(); //contador para cambio de timepo de equipo A a equipo B
      
      if(digitalRead(menos_A)==0 && equipo_a==false)//contador tiempo A
      {
        while(millis()-time<=200)//espera 200 milis antes de activar tiempo A
        {
          segundos();
          contador();
          escribe_hora();
        }
          equipo_a=true;
          equipo_b=false;
      }
      
      if(digitalRead(mas_B)==0 && equipo_b==false)//contador tiempo B
      {
        while(millis()-time<=200)//espera 200 milis antes de activar tiempo B
        {
          segundos();
          contador();
          escribe_hora();
        }
          equipo_b=true;
          equipo_a=false;
      }
      
      if(equipo_a==true)//ejecuta tiempo equipo A
      {
        segundos_A();
      	contador_A();
      }
      if(equipo_b==true)//ejecuta tiempo equipo B
      {
        segundos_B();  
      	contador_B(); 
      }
      
      escribe_tiempo_A_B();//ejecuta escritura de tiempo de equipos
      
    }
    else //fin de partida
    {
      while(cont==2)
      {
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print("Fin de partida");
        lcd.setCursor(1,1);
        
        if(minuto_a>minuto_b)
        {
          lcd.print("Ganador Eq. A");
        }
        else if(minuto_b>minuto_a)
        {
          lcd.print("Ganador Eq. B");
        }
        else
        {
          if(segundo_a>segundo_b)
          {
            lcd.print("Ganador Eq. A");
          }
          else if(segundo_b>segundo_a)
          {
            lcd.print("Ganador Eq. B");
          }
          else
          {
            lcd.print("Empate.");
          }  
        }  
        
        cont=3;
      } 
    }
  }
}
//////////////////////
/////////////////////
//funciones ....   
void sonido()//tono
{
  tone(Altavoz,2400,100);
  delay(250);
  tone(Altavoz,2400,100);
  delay(250);
}
//segundos de la partida
void segundos() 
{ 
  if(millis()-tiempo>=100)  
  {
    tiempo=millis();
    segundo++;
  }
}
// segundos de A
void segundos_A() 
{ 
   if(millis()-tiempo_A>=100)  
   {
     tiempo_A=millis();
     segundo_a++;
   }
}  
//segundos de B
void segundos_B() 
{
   if(millis()-tiempo_B>=100)  
   {
     tiempo_B=millis();
     segundo_b++;
   } 
}
// contador tiempo partida
void contador()
{ 
  if(segundo == 60)
  {
      segundo =0;
      minuto++;
  } 
}
//contador tiempo A
void contador_A()
{ 
  if(segundo_a == 60)
  {
      segundo_a =0;
      minuto_a++;
  } 
}
// contador tiempo B
void contador_B()
{ 
  if(segundo_b == 60)
  {
      segundo_b =0;
      minuto_b++;
  } 
}
//funcion que escribe tiempo de partida
void escribe_hora()
{   
  lcd.setCursor(0,0);  
  lcd.print("T. partida"); 
  lcd.setCursor(11,0);
  if(minuto<10)
  {
    lcd.print("0");
    lcd.print(minuto); 
  }
  else
  {
    lcd.print(minuto);
  }
  lcd.print(":");
  if(segundo<10)
  {
    lcd.print("0");
    lcd.print(segundo);
  }
  else
  {
    lcd.print(segundo);
  }     
}
// funcion que escribe tiempo equipo A y B
void escribe_tiempo_A_B()
{
  //equipo A
  lcd.setCursor(0,1);  
  lcd.print("A"); 
  lcd.setCursor(2,1);
  if(minuto_a<10)
  {
    lcd.print("0");
    lcd.print(minuto_a); 
  }
  else
  {
    lcd.print(minuto_a);
  }
  lcd.print(":");
  if(segundo_a<10)
  {
    lcd.print("0");
    lcd.print(segundo_a);
  }
  else
  {
    lcd.print(segundo_a);
  }
  //equipo B
  lcd.setCursor(9,1);  
  lcd.print("B"); 
  lcd.setCursor(11,1);
  if(minuto_b<10)
  {
    lcd.print("0");
    lcd.print(minuto_b); 
  }
  else
  {
    lcd.print(minuto_b);
  }
  lcd.print(":");
  if(segundo_b<10)
  {
    lcd.print("0");
    lcd.print(segundo_b);
  }
  else
  {
    lcd.print(segundo_b);
  }  
}



