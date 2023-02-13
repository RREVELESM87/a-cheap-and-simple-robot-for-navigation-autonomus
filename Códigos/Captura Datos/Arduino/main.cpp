/*Corrern con 6-Autonomos_V2.py*/

#include <Arduino.h>
#include <PS4BT.h>
#include <usbhub.h>
#include <Servo.h>
#include <AFMotor.h>



// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

int mapeo;
int sensorValue;
int retardo_Frame=1;
int angulo;

int velocidad   = 0;
int velocidad0 = 73;//raspDuino V1 //Se está trabajando con 12.2V
//int velocidad0 = 65;//raspDuino V2
int velocidad1 = 76;
int velocidad2 = 78;
int velocidad3 = 80;
int pos;
int dato_read;
String entradaSerial = "";
bool entradaCompleta=false;



int pulsoMin =544;
int pulseMedio = 1472;
int pulsoMax= 2400;
//int angulo=0;
String punto_pdi;
//int sensorValue;
int pdi_derecha=8;
int pdi_izquierda=-8;
int entradaSerial_int;
float val;
int str_lef=90-10;
int str_rigth=90+10;


#define pinServo A0
// /* Parámetros de los motores raspDuino V1*/
AF_DCMotor motor1(3);//DERECHO
AF_DCMotor motor2(4);//IZQUIERDO
Servo myservo;
void asignarCategoria_7();
int vel();

USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
PS4BT PS4(&Btd, PAIR);
bool printAngle, printTouch;
uint8_t oldL2Value, oldR2Value;

int vel(){
  if (PS4.getButtonClick(SQUARE)) {
  velocidad = velocidad0;}
  if (PS4.getButtonClick(TRIANGLE)) {
  velocidad = velocidad1;}
  if (PS4.getButtonClick(CIRCLE)) {
  velocidad = velocidad2;}
  if (PS4.getButtonClick(CROSS)) {
  velocidad = velocidad3;}
  return velocidad;
}



void motores(){

    /*******************************Navegación***************************/
    
  if (PS4.getButtonClick(PS)) {PS4.setRumbleOn(RumbleHigh);PS4.disconnect();}

  if (PS4.getButtonClick(SHARE)) {
    
    motor1.setSpeed(velocidad);
    motor2.setSpeed(velocidad);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    }

  if (PS4.getButtonClick(TOUCHPAD)) {
    
    motor1.setSpeed(velocidad);
    motor2.setSpeed(velocidad);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    }

  if (PS4.getButtonClick(R2)) {
    motor1.setSpeed(velocidad);
    motor2.setSpeed(velocidad);
    motor1.run(FORWARD);
    motor2.run(FORWARD); 
    }
   /*******************************Navegación***************************/
}


void leer_serial(){
    while (Serial.available() > 0) {
    dato_read=Serial.read();


    }
}

void serialEvent() {
  while (Serial.available()) {
    // Obtener bytes de entrada:
    char inChar = (char)Serial.read();
    // Agregar al String de entrada:
    entradaSerial += inChar;
    // Para saber si el string está completo, se detendrá al recibir
    // el caracter de retorno de línea ENTER \n
    if (inChar == '\n') {//En esta parte arduino se da cuena cuando termina el comando
                          //Desde raspberry luego lo envía al loop
      entradaCompleta = true;
    }
  }
}



  
void navegacionAutonoma(){
if (entradaCompleta) {
  val=entradaSerial.toInt();
    if (val<str_lef)
    {
      angulo=map(val,val,str_lef,str_lef,str_rigth);
      myservo.write(angulo);
    }
    else if (val>str_rigth)
    {
      angulo=map(val,str_rigth,val,str_lef,str_rigth);
      myservo.write(angulo);
      
    }
    else 
    {
      angulo=map(val,str_lef,str_rigth,str_lef,str_rigth);
      myservo.write(angulo+5); 
    }

    
    
    
  
  
  entradaSerial = "";
  entradaCompleta = false;
}
}




void setup() {
  myservo.attach(pinServo);
  myservo.write(90+5);
  delay(15);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  Serial.begin(115200);
  Serial.setTimeout(10);
  #if !defined(__MIPSEL__)
    while (!Serial); 
    #endif
  if (Usb.Init() == -1) {
    while (1); //halt
  }
}

void loop() {
  //leer_serial();
  /*Función para modo Navegación Autónomo, salida de la red
  Neuornal*/
  //sinControl();
  navegacionAutonoma();

  Usb.Task();
  vel();// Esta función modifica la velocidad del robo
  if (PS4.connected()) {


    motores();// Esta función sirve para controlar los motores  
 
  }
}

/*Comdntarios*/