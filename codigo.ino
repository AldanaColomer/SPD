//Aldana Colomer - 1F

#include <IRremote.h>
#include  <Servo.h>
#include <LiquidCrystal.h>

#define LEDR 12
#define LEDA 13
#define LEDV 10

String estacion = "Configurar: 1-4";
int temperaturaMaxima = 600;
int flag= 1;
bool prendido = false;

LiquidCrystal display(2,3,4,5,6,7);
IRrecv control(11);
Servo sistemaContraIncendio;

void setup()
{
	display.begin(16,2);
    control.enableIRIn();
    sistemaContraIncendio.attach(9);
    pinMode(LEDR , OUTPUT);
    pinMode(LEDA , OUTPUT);
    pinMode(LEDV , OUTPUT);
    Serial.begin(9600);
}
void sistema()
{
  display.setCursor(0,1);
  display.print("ALERTA INCENDIO");
  sistemaContraIncendio.write(90);
  delay(1000);
  sistemaContraIncendio.write(180);
  delay(1000);
 
}

void ledPrendido(int led)
{
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDA, LOW);
  digitalWrite(LEDV, LOW);
  
  if(led == 1)
  {
  	digitalWrite(LEDV , HIGH);
  }
  else if(led == 2)
  {
    digitalWrite(LEDA , HIGH);
    digitalWrite(LEDV , HIGH);
  }
  else if(led == 3)
  {
    digitalWrite(LEDV , HIGH);
    digitalWrite(LEDR , HIGH);
    digitalWrite(LEDA , HIGH);
  }
}

void temperatura()
{
  char stringTemperatura[16];
  int leer, mostrar;
  
  leer = analogRead(A0);
  mostrar = map(leer, 20, 358, -40, 125);
  display.setCursor(0,0);
  display.print("Temperatura: ");
  display.print(mostrar);
  display.print("  ");
  Serial.print("Valor tmp: ");
  Serial.println(mostrar);
  
  if(mostrar > temperaturaMaxima && mostrar < temperaturaMaxima )
  {
    ledPrendido(2);
  }
  else if(mostrar > temperaturaMaxima )
  {
	ledPrendido(3);
    sistema();
  }
  else
  {
  	ledPrendido(1);
    display.setCursor(0,1);
    display.print(estacion);
  }
}

void evaluarEstacion()
{
	long botonApretado;
    if(control.decode())
    {
      botonApretado = control.decodedIRData.decodedRawData;
      Serial.print("valor del boton:   ");
      Serial.println(botonApretado);
      
      switch(botonApretado)
      {
      	case -284115200:
        	estacion = "VERANO             ";
        	temperaturaMaxima = 45;
        break;
        
        case -300826880:
        	estacion = "INVIERNO           ";
            temperaturaMaxima = 30;
        break;
        
        case -317538560:
        	estacion = "PRIMAVERA         ";
            temperaturaMaxima = 37;
        break;
        
        case -350961920:
        	estacion = "OTONO              ";
        	temperaturaMaxima = 25;
        break;
        default:
         break;
        
      }
      display.setCursor(0,1);
      display.print(estacion);
      control.resume();
      flag += 1;
      
    }
}
 bool prendidoApagado()
 {
 	long botonInicio;
    if(control.decode())
    {
    	botonInicio = control.decodedIRData.decodedRawData;
        if(botonInicio == -16728320)
        {
        	prendido = !prendido;
            control.resume();
        }
    }
 }
  
 void loop()
 {
 	if(prendido)
    {
    	evaluarEstacion();
        temperatura();
    }
    else
    {
      display.clear();
      ledPrendido(4);
      flag = 1;
      estacion = "Configurar 1-4";
      temperaturaMaxima = 600;
    
    }
   prendidoApagado();
   		
 }

  
