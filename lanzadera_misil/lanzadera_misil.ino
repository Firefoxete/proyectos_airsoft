#include <Wire.h>
//libreria teclado
#include <Keypad.h>
//libreria pantalla
#include <LiquidCrystal_I2C.h>
//libreria dfplayer
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

LiquidCrystal_I2C lcd1(0x27,16,2); // Dirección de la pantalla uno

SoftwareSerial mySoftwareSerial(A0,A1); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Definir los pines del teclado
const byte FILAS = 4; // Número de filas
const byte COLUMNAS = 4; // Número de columnas

// Definir el mapa de teclas (ejemplo)
const byte teclado[FILAS][COLUMNAS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

byte pinFilas[FILAS] = {9, 8, 7, 6}; // Pines de las filas (salidas)
byte pinColumnas[COLUMNAS] = {5, 4, 3}; // Pines de las columnas (entradas)

Keypad teclado1 = Keypad( makeKeymap(teclado), pinFilas, pinColumnas, FILAS, COLUMNAS); 

byte A[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000											
};
byte B[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000							
};

byte C[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000							
};

byte D[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100									
};

byte I[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110						
};

byte J[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111							
};

//varibles de conexiones
int misil = 10;
int comb = 11;
int chip = 12;
int led_motor = 13;
int fuego = A3;
int lanzadera = A2;
int boton = 2;
//variables de tiempo
unsigned long tiempo = 0;
unsigned long T_max = 2000;
//variables estado
boolean m = true; //misil
boolean d = true; //deposito
boolean c = true; //chip
boolean l = true; // lanzadera
//variables tevlado
char contrasena[5];  //codigo correcto
char codigo[5];
char valor[]="#"; //si
char valor1[]="*"; //no
int cont=0;
int n=0;
bool activo=true;


void setup()
{
  lcd1.init();
  lcd1.backlight();

  lcd1.createChar(0, A);
  lcd1.createChar(1, B);
  lcd1.createChar(2, C);
  lcd1.createChar(3, D);
  lcd1.createChar(4, I);
  lcd1.createChar(5, J);
  
  pinMode(misil, INPUT);
  pinMode(chip, INPUT);
  pinMode(comb, INPUT);
  pinMode(lanzadera, INPUT);
  pinMode(fuego, OUTPUT);
  pinMode(led_motor, OUTPUT);
  pinMode(boton,INPUT_PULLUP);
  
  Serial.begin(9600);
  
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  //comprueba si el dfplayer está conectado
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  if (!myDFPlayer.begin(mySoftwareSerial)) { 
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }

  myDFPlayer.volume(20);
 
  //establece código de activación
    myDFPlayer.play(9);
    delay(1000);
    establecer_clave();//establece la clave activación de cada partida.
 
  lcd1.setCursor(3,0);
  lcd1.print("TERCIO GRT");
  lcd1.setCursor(4,1);
  lcd1.print("AIRSOFT");

  delay(1500);
  lcd1.clear();

  lcd1.setCursor(2,0);
  lcd1.print("...SISTEMA...");
  lcd1.setCursor(0,1);
  lcd1.print("...DE MISILES...");

  delay(1000);
  
  for(int i=0;i<=38;i++){
    lcd1.scrollDisplayRight();
    delay(150);
  }
  delay(1500);
  lcd1.clear();
}

void loop()
{
  //código para misil
  if(digitalRead(misil)==1 && m==true) //si está en hight y es true
  { 
    myDFPlayer.play(1);
    delay(500);
    
    tiempo=millis();
    lcd1.clear();
    
    while(digitalRead(misil)==1 && m==true) //bucle de carga
    {
      lcd1.setCursor(3,0);
      lcd1.print("DETECTANDO");
      lcd1.setCursor(5,1);
      lcd1.print("MISIL");
      if(millis()-tiempo>=T_max) //condición tiempo
      {
      	lcd1.clear();
        for(int i=0; i <= 100; i++) //se ejecuta la carga con un bucle "for"
        {
          if(digitalRead(misil)==1)
          {  
            lcd1.setCursor(0,0);
            lcd1.print(i);
            lcd1.print("%");
            lcd1.setCursor(5,0);
            lcd1.print("CARGANDO...");
            delay(50);
            BarraDeCarga(i, 100, 1);
              if(i==100) //si tiene exito la carga
              {
               delay(1000); 
               lcd1.clear();
               lcd1.setCursor(5,0);
               lcd1.print("MISIL");
               lcd1.setCursor(3,1);
               lcd1.print("CONECTADO");
               myDFPlayer.play(2);
               delay(2000);
               lcd1.clear(); 
              }
          }else // si no tiene exito la carga
          {
            lcd1.clear();
            lcd1.setCursor(5,0);
            lcd1.print("MISIL");
    		    lcd1.setCursor(2,1);
    		    lcd1.print("DESCONECTADO");
            myDFPlayer.play(3);
            delay(1000);
            lcd1.clear();
            break;
          }
      	}
      }
    }   
      
  }else if (digitalRead(misil)==0 && m==true) //esperando la conexión
  {
    myDFPlayer.play(4);
    delay(500);
  	while(digitalRead(misil)==0 && m==true)
    {
      lcd1.setCursor(3,0);
      lcd1.print("ESPERANDO");
      lcd1.setCursor(5,1);
      lcd1.print("MISIL");
    }
  }

  //Combustible
  if(digitalRead(comb)==1 && digitalRead(misil)==1 && d==true && m==false) //si está en hight y es true
  { 
    myDFPlayer.play(11);
    delay(500);
    
    tiempo=millis();
    lcd1.clear();
    
    while(digitalRead(comb)==1 && digitalRead(misil)==1 && d==true && m==false) //bucle de carga
    {
      lcd1.setCursor(3,0);
      lcd1.print("DETECTANDO");
      lcd1.setCursor(2,1);
      lcd1.print("COMBUSTIBLE");  
      if(millis()-tiempo>=T_max) //condición tiempo
      {
      	lcd1.clear();
        
        for(int i=0; i <= 100; i++) //se ejecuta la carga con un bucle "for"
        {
          if(digitalRead(comb)==1)
          {  
            lcd1.setCursor(0,0);
            lcd1.print(i);
            lcd1.print("%");
            lcd1.setCursor(5,0);
            lcd1.print("CARGANDO...");
            delay(50);
            BarraDeCarga(i, 100, 1);
              if(i==100) //si tiene exito la carga
              {
               delay(1000); 
               lcd1.clear();
               lcd1.setCursor(3,0);
               lcd1.print("COMBUSTIBLE");
               lcd1.setCursor(4,1);
               lcd1.print("CONECTADO");
               myDFPlayer.play(12);
               delay(2000);
               lcd1.clear();
              }
          }else // si no tiene exito la carga
          {
            lcd1.clear();
            lcd1.setCursor(3,0);
            lcd1.print("COMBUSTIBLE");
    		    lcd1.setCursor(2,1);
    		    lcd1.print("DESCONECTADO");
            myDFPlayer.play(13);
            delay(1000);
      		  lcd1.clear();	
            break;
          }
      	}
      }
    }   
  }else if (digitalRead(comb)==0 && digitalRead(misil)==1 && d==true && m==false) //esperando la conexión
  {
    myDFPlayer.play(14);
    delay(500);
  	while(digitalRead(comb)==0 && digitalRead(misil)==1 && d==true && m==false)
    {
      lcd1.setCursor(4,0);
      lcd1.print("ESPERANDO");
      lcd1.setCursor(3,1);
      lcd1.print("COMBUSTIBLE");
    }
  }

  //chip
  if(digitalRead(chip)==1 && digitalRead(comb)==1 && digitalRead(misil)==1 && c==true && d==false && m==false) //si está en hight y es true
  { 
    myDFPlayer.play(15);
    delay(500);
    
    tiempo=millis();
    lcd1.clear();
    
    while(digitalRead(chip)==1 && digitalRead(comb)==1 && digitalRead(misil)==1 && c==true && d==false && m==false) //bucle de carga
    {
      lcd1.setCursor(4,0);
      lcd1.print("DETECTADO");
      lcd1.setCursor(6,1);
      lcd1.print("CHIP");  
      if(millis()-tiempo>=T_max) //condición tiempo
      {
      	lcd1.clear();
        
        for(int i=0; i <= 100; i++) //se ejecuta la carga con un bucle "for"
        {
          if(digitalRead(chip)==1)
          {  
            lcd1.setCursor(0,0);
            lcd1.print(i);
            lcd1.print("%");
            lcd1.setCursor(5,0);
            lcd1.print("CARGANDO...");
            delay(50);
            BarraDeCarga(i, 100, 1);
              if(i==100) //si tiene exito la carga
              {
               delay(1000);  
               lcd1.clear();
               lcd1.setCursor(6,0);
               lcd1.print("CHIP");
               lcd1.setCursor(4,1);
               lcd1.print("CONECTADO");
               myDFPlayer.play(16);
               delay(2000);
               lcd1.clear(); 
              }
          }else // si no tiene exito la carga
          {
            lcd1.clear();
            lcd1.setCursor(6,0);
            lcd1.print("CHIP");
    		    lcd1.setCursor(2,1);
    		    lcd1.print("DESCONECTADO");
            myDFPlayer.play(17);
            delay(1000);
      		  lcd1.clear();	
            break;
          }
      	}
      }
    }   
  }else if (digitalRead(chip)==0 && digitalRead(comb)==1 && digitalRead(misil)==1 && c==true && d==false && m==false) //esperando la conexión
  {
    myDFPlayer.play(18);
  	delay(500);
    while(digitalRead(chip)==0 && digitalRead(comb)==1 && digitalRead(misil)==1 && c==true && d==false && m==false)
    {
      lcd1.setCursor(4,0);
      lcd1.print("ESPERANDO");
      lcd1.setCursor(6,1);
      lcd1.print("CHIP");
    }
  }

   //lanzadera
  if(digitalRead(misil)==1 && digitalRead(comb)==1 && digitalRead(chip)==1 && m==false && d==false && c==false )
  {
    motor(); //da corriente a la lanzadera - se enciende motor

    if(digitalRead(lanzadera)==1 && l==true) //tope lanzadera alcanzado
    {
      lcd1.clear();
      lcd1.setCursor(4,0);
      lcd1.print("LANZADERA");
      lcd1.setCursor(3,1);
      lcd1.print("EN POSICION");
      myDFPlayer.play(19);
      delay(2000);

      l=false;
      motor();// motor se apaga
      lcd1.clear();
      
      code(); //debes introducir condigo activación
    }
    else if(digitalRead(lanzadera)==0 && l==true) //tope no alcanzado
    {
      myDFPlayer.play(20);
      delay(500);
      
      while(digitalRead(lanzadera)==0 && digitalRead(chip)==1 && digitalRead(comb)==1 && digitalRead(misil)==1 && l==true)
      {
        lcd1.setCursor(5,0);
        lcd1.print("ELEVAR");
        lcd1.setCursor(4,1);
        lcd1.print("LANZADERA");
      }
    } 
    
  }else if(digitalRead(misil)==0 && m==false)//si misil se apaga
  {
    lcd1.clear();
    lcd1.setCursor(5,0);
    lcd1.print("MISIL");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");
    myDFPlayer.play(3);
    delay(2000);
    lcd1.clear();

    m=true; 
    motor(); //apaga motor
  }else if(digitalRead(comb)==0 && d==false)//si combustible se apaga
  {
    lcd1.clear();
    lcd1.setCursor(3,0);
    lcd1.print("COMBUSTIBLE");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");
    myDFPlayer.play(13);
    delay(2000);
    lcd1.clear();

    d=true;
    motor(); //apaga motor
  }else if(digitalRead(chip)==0 && c==false)//si chip se apaga
  { 
    lcd1.clear();
    lcd1.setCursor(6,0);
    lcd1.print("CHIP");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");
    myDFPlayer.play(17);
    delay(2000);
    lcd1.clear();

    c=true;
    motor(); //apaga motor
  }

  while(activo==false)
  {
    tiempo=millis();
    while(digitalRead(boton)==0)
    {
      if(millis()-tiempo>=100) //tiempo de pulsado de disparador
      {
        F_regresiva();
      }  
    }
  }
}

//Funciones
void F_regresiva()
{
  lcd1.clear();
  myDFPlayer.play(27);
  delay(500);
  lcd1.setCursor(3,0);
  lcd1.print("CUENTA ATRAS");
  delay(1000);

  for(int i=0; i <= 10; i++)
  {
    if(i==0)
    {
      lcd1.setCursor(7,1);
      lcd1.print("10");
    }
    else
    {
      lcd1.setCursor(7,1);
      lcd1.print("0");
      lcd1.setCursor(8,1);
      lcd1.print(10-i);
    }
    myDFPlayer.play(31);
    delay(1000);

    if(i==10)
    {
      delay(500);
      if(digitalRead(misil)==1 && digitalRead(comb)==1 && digitalRead(chip)==1 && digitalRead(lanzadera)==1)
      {
        lcd1.clear();
        digitalWrite(fuego,HIGH);
        if(i==10)
        {
          myDFPlayer.play(28);
          delay(500);
          motor();
          while(i==10)
          {
            lcd1.setCursor(5,0);
            lcd1.print("MISIL");
            lcd1.setCursor(4,1);
            lcd1.print("LANZADO");
            digitalWrite(led_motor,HIGH);
          }
        }
      }
      else
      {
        lcd1.clear();
        stop();
      }
    }
  }
}

void motor()
{
  if(m,d,c == false && l==true)//corriente lanzadera 
  {
    digitalWrite(led_motor,HIGH);
  }
  else //no corriente lanzadera
  {
    digitalWrite(led_motor,LOW);
  }     
}

void code()
{
  lcd1.setCursor(0,0);
  lcd1.print("INTRODUCE CODIGO");
  lcd1.setCursor(2,1);
  lcd1.print("DE ACTIVACION");
  myDFPlayer.play(21);
  delay(2000);
  lcd1.clear();
  
  while(activo==true)
  {
    if(digitalRead(misil)==1 && digitalRead(comb)==1 && digitalRead(chip)==1 && digitalRead(lanzadera)==1) //para que se desconecte si falla algun interuptor
    {
      lcd1.setCursor(0,0);
      lcd1.print("CODIGO:");
        
      char tecla_presionada = teclado1.getKey();
        
      if (tecla_presionada!= NO_KEY) 
      {
        myDFPlayer.play(10);
        delay(500);
        codigo[cont]=tecla_presionada;
        cont=cont+1;
          
        if(cont<5)
        {
          lcd1.setCursor(cont-1,1);
          lcd1.print("*");
        }
          
        if(cont==4)
        { 
          if(codigo[0]==contrasena[0]&&codigo[1]==contrasena[1]&&codigo[2]==contrasena[2]&&codigo[3]==contrasena[3])  
          {
            lcd1.clear();
            lcd1.setCursor(0,0);
            lcd1.print("CODIGO:");
            lcd1.setCursor(0,1);
            lcd1.print("CORRECTO");
            myDFPlayer.play(22);
            delay(1500);
            
            lcd1.clear();
            lcd1.setCursor(4,0);
            lcd1.print("SISTEMAS");
            lcd1.setCursor(3,1);
            lcd1.print("ACTIVADOS");
            myDFPlayer.play(23);
            delay(1500);

            lcd1.clear();
            lcd1.setCursor(4,0);
            lcd1.print("ESPERANDO");
            lcd1.setCursor(3,1);
            lcd1.print("LANZAMIENTO");
            myDFPlayer.play(24);
            delay(1500);
            activo=false;
          }
          else
          {
            lcd1.clear();
            lcd1.setCursor(0,0);
            lcd1.print("CODIGO:");
            lcd1.setCursor(0,1);
            lcd1.print("INCORRECTO");
            myDFPlayer.play(25);
            delay(2000);
            lcd1.clear();
            cont=0;
            n++;
              
            if(n==3)
            {
              myDFPlayer.play(26);
              delay(500);
              digitalWrite(led_motor,HIGH);
              lcd1.clear();
              while(n==3)
              {
                lcd1.setCursor(4,0);
                lcd1.print("SISTEMA");
                lcd1.setCursor(3,1);
                lcd1.print("BLOQUEADO");
              }
            }  
          }
        }   
      }
    }
    else //si durante el proceso se desconecta alguno de los 4 interuptores
    {
      l=true;
      lcd1.clear();
      break;
    }
  }  
}

//establecer codigo 
void establecer_clave()
{
  lcd1.setCursor(4,0);
  lcd1.print("INTRODUCE");
  lcd1.setCursor(2,1);
  lcd1.print("NUEVO CODIGO:");
  myDFPlayer.play(5);
  delay(2000);
  lcd1.clear();

  while(activo==true)
  {
    if(cont==0)
    {
      lcd1.setCursor(0,0);
      lcd1.print("PIN:");
    }  
       
    char tecla_presionada = teclado1.getKey();
    
    if (tecla_presionada!= NO_KEY)
    {
      myDFPlayer.play(10);
      delay(500);
      contrasena[cont]=tecla_presionada;
      
      if(cont<4)
      {
        
        lcd1.setCursor(cont,1);
        lcd1.print("*");
        
        if(cont==3)
        {
          myDFPlayer.play(6);
          delay(500);
          lcd1.setCursor(0,0);
          lcd1.print("CONFIRMAR CODIGO");
          lcd1.setCursor(0,1);
          lcd1.print("si # / no *");
        }
        
        cont=cont+1;
      }
      else if(cont==4)
      {
        if(contrasena[4]==valor[0])
        {
          lcd1.clear();
          lcd1.setCursor(5,0);
          lcd1.print("CODIGO");
          lcd1.setCursor(3,1);
          lcd1.print("CONFIRMADO");
          myDFPlayer.play(7);
          delay(2000);
          lcd1.clear();
          cont=0;
          break;
        }
        else if(contrasena[4]==valor1[0])
        {
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("PRUEBA");
          lcd1.setCursor(0,1);
          lcd1.print("OTRA VEZ");
          myDFPlayer.play(8);
          cont=0;
          delay(1500);
          lcd1.clear();
        }
        else if(contrasena[5]!=valor[0] && contrasena[5]!=valor1[0])
        {
          cont=5;
        }  
      }
    } 
  }
}

//bara de carga
void BarraDeCarga(unsigned long count, unsigned long totalCount, int lineToPrintOn)
 {
    if(count<100)
    {
      double factor = totalCount/80.0;  
      int percent = (count+1)/factor;
      int number = percent/5;
      int remainder = percent%5;

        if(number > 0)
      	{
       		lcd1.setCursor(number-1,lineToPrintOn);
       		lcd1.write(5);
      	}
       		lcd1.setCursor(number,lineToPrintOn);
       		lcd1.write(remainder); 
    }else
    {
      if(digitalRead(misil)==1 && m==true)
      {
        m=false;
      }else if (digitalRead(comb)==1 && d==true)
      {
        d=false;
      }else if (digitalRead(chip)==1 && c==true)
      {
        c=false;
      }      
    } 
 }
 void stop()
 {
  lcd1.setCursor(6,0);
  lcd1.print("ERROR");
  lcd1.setCursor(3,1);
  lcd1.print("LANZAMIENTO");
  myDFPlayer.play(30);
  delay(2000);
  lcd1.clear();
  digitalWrite(led_motor,HIGH); //enciende el motor para bajar lanzader

  if(digitalRead(misil)==0)
  {
    myDFPlayer.play(3);
    delay(500);
    lcd1.setCursor(5,0);
    lcd1.print("MISIL");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");  
  }
  else if (digitalRead(comb)==0)
  {
    myDFPlayer.play(13);
    delay(500);
    lcd1.setCursor(3,0);
    lcd1.print("COMBUSTIBLE");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");  
  }
  else if (digitalRead(chip)==0)
  {
    myDFPlayer.play(17);
    delay(500);
    lcd1.setCursor(6,0);
    lcd1.print("CHIP");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");   
  }
  else if(digitalRead(lanzadera)==0)
  {
    myDFPlayer.play(29);
    delay(500);
    lcd1.setCursor(4,0);
    lcd1.print("LANZADERA");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADA");
  }

  while(true);
}