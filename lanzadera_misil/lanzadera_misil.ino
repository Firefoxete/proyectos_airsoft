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
byte pinFilas[FILAS] = {8, 7, 6, 5}; // Pines de las filas (salidas)
byte pinColumnas[COLUMNAS] = {4, 3, 2}; // Pines de las columnas (entradas)
Keypad teclado1 = Keypad( makeKeymap(teclado), pinFilas, pinColumnas, FILAS, COLUMNAS); 

//Imagen Barra de carga
#define FILAS(x) {x,x,x,x,x,x,x,x}
byte A[8] = FILAS(B00000);
byte B[8] = FILAS(B10000);
byte C[8] = FILAS(B11000);
byte D[8] = FILAS(B11100);
byte I[8] = FILAS(B11110);
byte J[8] = FILAS(B11111);

//varibles de conexiones
int misil = 9, comb = 10, chip = 11, fuego = 12, boton = 13;
//variables de tiempo
unsigned long tiempo = 0, T_max = 2000;
//variables estado
bool m = true, d = true, c = true, activo=true;
//variables teclado
char contrasena[5], codigo[5];
int cont = 0, n = 0;

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
  pinMode(fuego, OUTPUT);
  pinMode(boton,INPUT_PULLUP);
  
  Serial.begin(9600);
  mySoftwareSerial.begin(9600);

  //comprueba si el dfplayer está conectado
  if (!myDFPlayer.begin(mySoftwareSerial)) {
  Serial.println("Error al iniciar DFPlayer");
  while(true);
  }

  //delay(1000);          
  myDFPlayer.reset();  // reset interno del módulo
  delay(1000);     

  myDFPlayer.volume(20);
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
 
  //Bienvenida
  myDFPlayer.playMp3Folder(9);
  lcd1.setCursor(3,0);
  lcd1.print("TERCIO GRT");
  lcd1.setCursor(4,1);
  lcd1.print("AIRSOFT");
  delay(3000);
  lcd1.clear();
  establecer_Codigo();//establece la clave activación de cada partida.
 
  myDFPlayer.playMp3Folder(35);
  lcd1.setCursor(2,0);
  lcd1.print("CARGANDO...");
  lcd1.setCursor(1,1);
  lcd1.print("SISTEMA MISILES");
  delay(3000);
  lcd1.clear();
}

void loop()
{
  //código para misil
  if(digitalRead(misil)==1 && m==true) //si está en hight y es true
  { 
    reproducirSonido(23, 2000);
    reproducirSonido(1, 500);
  
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
            Barra_Carga(i, 100, 1);
              if(i==100) //si tiene exito la carga
              {
               delay(1000); 
               lcd1.clear();
               lcd1.setCursor(5,0);
               lcd1.print("MISIL");
               lcd1.setCursor(3,1);
               lcd1.print("CONECTADO");
               reproducirSonido(2, 2000);
               lcd1.clear(); 
              }
          }else // si no tiene exito la carga
          {
            lcd1.clear();
            lcd1.setCursor(5,0);
            lcd1.print("MISIL");
    		    lcd1.setCursor(2,1);
    		    lcd1.print("DESCONECTADO");
            reproducirSonido(29, 2000);
            reproducirSonido(3, 2000);
            lcd1.clear();
            break;
          }
      	}
      }
    }   
      
  }else if (digitalRead(misil)==0 && m==true) //esperando la conexión
  {
    reproducirSonido(4, 500);

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
    reproducirSonido(23, 2000);
    reproducirSonido(11, 500);

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
          if(digitalRead(comb)==1 && digitalRead(misil)==1)
          {  
            lcd1.setCursor(0,0);
            lcd1.print(i);
            lcd1.print("%");
            lcd1.setCursor(5,0);
            lcd1.print("CARGANDO...");
            delay(50);
            Barra_Carga(i, 100, 1);
              if(i==100) //si tiene exito la carga
              {
               delay(1000); 
               lcd1.clear();
               lcd1.setCursor(3,0);
               lcd1.print("COMBUSTIBLE");
               lcd1.setCursor(4,1);
               lcd1.print("CONECTADO");
               reproducirSonido(12, 2500);
               lcd1.clear();
              }
          }else // si no tiene exito la carga
          {
            lcd1.clear();
            lcd1.setCursor(3,0);
            lcd1.print("COMBUSTIBLE");
    		    lcd1.setCursor(2,1);
    		    lcd1.print("DESCONECTADO");
            reproducirSonido(29, 2000);
            reproducirSonido(13, 2000);
      		  lcd1.clear();	
            break;
          }
      	}
      }
    }   
  }else if (digitalRead(comb)==0 && digitalRead(misil)==1 && d==true && m==false) //esperando la conexión
  {
    reproducirSonido(14, 500);

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
    reproducirSonido(23, 2000);
    reproducirSonido(15, 500);

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
          if(digitalRead(chip)==1 && digitalRead(comb)==1 && digitalRead(misil)==1)
          {  
            lcd1.setCursor(0,0);
            lcd1.print(i);
            lcd1.print("%");
            lcd1.setCursor(5,0);
            lcd1.print("CARGANDO...");
            delay(50);
            Barra_Carga(i, 100, 1);
              if(i==100) //si tiene exito la carga
              {
               delay(1000);  
               lcd1.clear();
               lcd1.setCursor(6,0);
               lcd1.print("CHIP");
               lcd1.setCursor(4,1);
               lcd1.print("CONECTADO");
               reproducirSonido(16, 2000);
               lcd1.clear(); 
              }
          }else // si no tiene exito la carga
          {
            lcd1.clear();
            lcd1.setCursor(6,0);
            lcd1.print("CHIP");
    		    lcd1.setCursor(2,1);
    		    lcd1.print("DESCONECTADO");
            reproducirSonido(29, 2000);
            reproducirSonido(17, 2000);
      		  lcd1.clear();	
            break;
          }
      	}
      }
    }   
  }else if (digitalRead(chip)==0 && digitalRead(comb)==1 && digitalRead(misil)==1 && c==true && d==false && m==false) //esperando la conexión
  {
    reproducirSonido(18, 500);

    while(digitalRead(chip)==0 && digitalRead(comb)==1 && digitalRead(misil)==1 && c==true && d==false && m==false)
    {
      lcd1.setCursor(4,0);
      lcd1.print("ESPERANDO");
      lcd1.setCursor(6,1);
      lcd1.print("CHIP");
    }
  }

  // Desconexion de alguno de los 3 elemento
  if(digitalRead(misil)==0 && !m)//si misil se apaga
  {
    lcd1.clear();
    lcd1.setCursor(5,0);
    lcd1.print("MISIL");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");
    reproducirSonido(29, 2000);
    reproducirSonido(3, 2000);
    lcd1.clear();

    m=true;
    activo=true; 
  }
  else if(digitalRead(comb)==0 && !d)//si combustible se apaga
  {
    lcd1.clear();
    lcd1.setCursor(3,0);
    lcd1.print("COMBUSTIBLE");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");
    reproducirSonido(29, 2000);
    reproducirSonido(13, 2000);
    lcd1.clear();

    d=true;
    activo=true;
  }
  else if(digitalRead(chip)==0 && !c)//si chip se apaga
  { 
    lcd1.clear();
    lcd1.setCursor(6,0);
    lcd1.print("CHIP");
    lcd1.setCursor(2,1);
    lcd1.print("DESCONECTADO");
    reproducirSonido(29, 2000);
    reproducirSonido(17, 2000);
    lcd1.clear();

    c=true;
    activo=true;
  }
  else if(!m && !d && !c && digitalRead(misil)==1 && digitalRead(comb)==1 && digitalRead(chip)==1)
  {
    static unsigned long tiempoPulsado = 0;

    if(activo == true) 
    {
      
      lcd1.clear();
      lcd1.setCursor(4,0);
      lcd1.print("SISTEMAS");
      lcd1.setCursor(3,1);
      lcd1.print("CONECTADOS");
      reproducirSonido(30, 5000);
      reproducirSonido(32, 2500);
      lcd1.clear();
      activar_Codigo(); //debes introducir condigo activación
    }
    else
    {
      if (digitalRead(boton) == LOW)
      {
        if (tiempoPulsado == 0)
        {
          tiempoPulsado = millis();
        }

        if (millis() - tiempoPulsado >= 100)
        {
          tiempoPulsado = 0;   // reset obligatorio
          cuenta_Regresiva();
        }
      }
      else
      {
        tiempoPulsado = 0;
      }
    }
  }
}
// ---Funciones---

//Función regresiva
void cuenta_Regresiva()
{
    lcd1.clear();
    lcd1.setCursor(3,0);
    lcd1.print("CUENTA ATRAS");
    reproducirSonido(27, 2000);

    for (int cuenta = 10; cuenta >= 0; cuenta--)
    {
        if (digitalRead(misil) == 0 || 
            digitalRead(comb) == 0 || 
            digitalRead(chip) == 0)
        {
            lcd1.clear();
            return;
        }

        lcd1.setCursor(7,1);
        lcd1.print("  ");
        lcd1.setCursor(7,1);
        lcd1.print(cuenta);

        myDFPlayer.playMp3Folder(31);

        delay(1000);  
    }

    lcd1.clear();
    lcd1.setCursor(5,0);
    lcd1.print("MISIL");
    lcd1.setCursor(4,1);
    lcd1.print("LANZADO");
    digitalWrite(fuego, HIGH);
    reproducirSonido(34, 4500);
    reproducirSonido(28, 2000);
    
    while (true)
    {
        // bloqueo del sistema
    }
}

//Activar codigo lanzamiento
void activar_Codigo()
{
  lcd1.setCursor(0,0);
  lcd1.print("INTRODUCE CODIGO");
  lcd1.setCursor(2,1);
  lcd1.print("DE ACTIVACION");
  reproducirSonido(21, 2000);
  lcd1.clear();

  int cont = 0;

  while (true)
  {
    // Si falla un interruptor, salir
    if (digitalRead(misil) == 0 || digitalRead(comb) == 0 || digitalRead(chip) == 0)
    {
      lcd1.clear();
      return;
    }

    lcd1.setCursor(0,0);
    lcd1.print("CODIGO:");

    char tecla = teclado1.getKey();

    if (tecla != NO_KEY)
    {
      reproducirSonido(10, 350);

      if (cont < 4)
      {
        codigo[cont] = tecla;
        lcd1.setCursor(cont,1);
        lcd1.print("*");
        cont++;
      }

      // Cuando ya hay 4 digitos
      if (cont == 4)
      {
        bool correcto = true;

        for (int i = 0; i < 4; i++)
        {
          if (codigo[i] != contrasena[i])
          {
            correcto = false;
            break;
          }
        }

        if (correcto)
        {
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("CODIGO:");
          lcd1.setCursor(0,1);
          lcd1.print("CORRECTO");
          reproducirSonido(22, 1500);

          lcd1.clear();
          lcd1.setCursor(4,0);
          lcd1.print("ESPERANDO");
          lcd1.setCursor(3,1);
          lcd1.print("LANZAMIENTO");
          reproducirSonido(33, 8000);
          reproducirSonido(24, 2500);

          activo = false;
          return;
        }
        else
        {
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("CODIGO:");
          lcd1.setCursor(0,1);
          lcd1.print("INCORRECTO");
          reproducirSonido(29, 2000);
          reproducirSonido(25, 2000);
          lcd1.clear();

          cont = 0;
          n++;

          if (n == 3)
          {
            reproducirSonido(2, 500);
            lcd1.clear();

            while (true)
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
}

//establecer codigo inicio
void establecer_Codigo()
{
  lcd1.setCursor(4,0);
  lcd1.print("INTRODUCE");
  lcd1.setCursor(2,1);
  lcd1.print("NUEVO CODIGO:");
  reproducirSonido(5, 2500);
  lcd1.clear();

  lcd1.setCursor(0,0);
  lcd1.print("PIN:");

  int cont = 0;

  while (true)
  {
    char tecla = teclado1.getKey();

    if (tecla != NO_KEY)
    {
      // Guardar solo los 4 digitos
      if (cont < 4)
      {
        reproducirSonido(10, 350);

        contrasena[cont] = tecla;
        lcd1.setCursor(cont,1);
        lcd1.print("*");
        cont++;

        if (cont == 4)
        {
          reproducirSonido(6, 500);
          lcd1.clear();
          lcd1.setCursor(0,0);
          lcd1.print("CONFIRMAR CODIGO");
          lcd1.setCursor(0,1);
          lcd1.print("si # / no *");
        }
      }
      else
      {
        // Confirmacion
        if (tecla == '#')
        {
          reproducirSonido(10, 1000);
          myDFPlayer.playMp3Folder(7);
          lcd1.clear();
          lcd1.setCursor(5,0);
          lcd1.print("CODIGO");
          lcd1.setCursor(3,1);
          lcd1.print("CONFIRMADO");
          delay(2000);
          lcd1.clear();
          return; // salir de la funcion
        }
        else if (tecla == '*')
        {
          reproducirSonido(10, 1000);
          myDFPlayer.playMp3Folder(8);
          lcd1.clear();
          lcd1.setCursor(5,0);
          lcd1.print("PRUEBA");
          lcd1.setCursor(4,1);
          lcd1.print("OTRA VEZ");
          delay(1500);
          lcd1.clear();

          cont = 0;
          lcd1.setCursor(0,0);
          lcd1.print("PIN:");
        }
      }
    }
  }
}

//bara de carga
void Barra_Carga(unsigned long count, unsigned long totalCount, int lineToPrintOn)
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
    }
    else
    {
      if(m==true)
      {
        m=false;
      }else if (d==true)
      {
        d=false;
      }else if (c==true)
      {
        c=false;
      }      
    } 
 }

void reproducirSonido(uint8_t numeroSonido, unsigned long tiempoEspera)
{
  myDFPlayer.playMp3Folder(numeroSonido);
  delay(tiempoEspera);
}





 