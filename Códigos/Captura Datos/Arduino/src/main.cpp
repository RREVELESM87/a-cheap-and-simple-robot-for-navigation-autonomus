/*
Este código de Arduino tiene como objetivo controlar un robot móvil a través de un mando inalámbrico PS4 Bluetooth
mediante la librería PS4BT. En primer lugar, se importan las librerías necesarias como Servo.h para controlar el 
servo motor, AFMotor.h para controlar los motores, y PS4BT.h para establecer la comunicación con el controlador PS4. 
También se define el pin del servo motor y el pin de interrupción.
Luego se definen varias variables globales, como la velocidad del robot, 
los ángulos de los servos, los valores de pulso mínimo, medio y máximo, y algunas variables de control de entrada serial. 
También se crea una instancia de la clase USB y de la clase BTD para conectar el controlador PS4 con el Arduino.
La función "vel()" se encarga de leer los botones del controlador PS4 para asignar la velocidad del robot. 
Si se presiona el botón SQUARE, se asigna la velocidad0, si se presiona el botón TRIANGLE, se asigna la velocidad1, y así sucesivamente. 
La función devuelve la velocidad asignada.
La función "motores()" se encarga de controlar el movimiento del robot en función de los botones del controlador PS4. 
Si se presiona el botón SHARE, el robot se detiene. Si se presiona el botón TOUCHPAD, el robot se mueve hacia atrás. 
Si se presiona el botón R2, el robot se mueve hacia adelante.
La función "capturarFrames_7()" se encarga de controlar el servo motor para la detección de objetos en diferentes posiciones. 
Si el joystick izquierdo se mueve a la izquierda, el servo se mueve a la posición correspondiente y se envía una señal de categoría a 
través del puerto serie. Si el joystick se mueve a la derecha, se hace lo mismo, pero para la posición derecha. Si el joystick está 
en el centro, el servo se mueve a la posición central y se envía la categoría correspondiente.
La función "capturarFrames_7_bandas()" es similar a "capturarFrames_7()", pero tiene algunas diferencias en la 
detección de objetos en diferentes posiciones.
*/

#include <Arduino.h>
#include <PS4BT.h>
#include <usbhub.h>
#include <Servo.h>
#include <AFMotor.h>



#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

int mapeo;
int sensorValue;
int retardo_Frame=1;
int angulo;

int velocidad   = 0;
int velocidad0 = 73;//raspDuino V1 //Se está trabajando con 11.1V
//int velocidad0 = 65;//raspDuino V2
int velocidad1 = 68;
int velocidad2 = 70;
int velocidad3 = 72;
int pos;
int dato_read;
String entradaSerial = "";
bool entradaCompleta=false;



int pulsoMin =544;
int pulseMedio = 1472;
int pulsoMax= 2400;

String punto_pdi;
//int sensorValue;
int pdi_derecha=8;
int pdi_izquierda=-8;
int entradaSerial_int;
float val;
int str_lef=90-10;
int str_rigth=90+10;


#define pinServo A0
#define pinInterrupcion A1
// /* Parámetros de los motores raspDuino V1*/
AF_DCMotor motor1(3);//DERECHO
AF_DCMotor motor2(4);//IZQUIERDO
Servo myservo;
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


void direccion(){
  if (PS4.getAnalogHat(LeftHatX) > 137) {
    sensorValue = map(PS4.getAnalogHat(LeftHatX), 137, 255, 90, 180);
    angulo = sensorValue;
    myservo.write(angulo);

    
    if (angulo > 110 && angulo <= 180) {
      // Serial.print("Python Almacena: ");
      // Serial.print("\tCategoría Derecha Angulo: ");
      // Serial.println(angulo);
      Serial.write('d');
    }
  }
  else if (PS4.getAnalogHat(LeftHatX) < 117) {
    sensorValue = map(PS4.getAnalogHat(LeftHatX), 0, 117, 0, 90);
    angulo = sensorValue;
    myservo.write(angulo);
    
    if (angulo < 70 && angulo >= 0) {
      // Serial.print("Python Almacena: ");
      // Serial.print("\tCategoría Izquierda Angulo: ");;
      // Serial.println(angulo);
      Serial.write('a');
    }
  }
  else {
    myservo.write(90);
    // Serial.print("Python Almacena: ");
    // Serial.print("\tCategoría Centro Angulo: ");
    // Serial.println("90");
    Serial.write('w');
  }
}





void setup() {
  myservo.attach(pinServo);
  pinMode(pinInterrupcion,LOW);
  //attachInterrupt(digitalPinToInterrupt(2), giro_servo, RISING);
  myservo.write(90);
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

  // Serial.print(F("\r\nPS4 Bluetooth Library Started\n"));
  // Serial.print(F("\r\nRobot en Modo\n"));
  // Serial.print(F("\r\nTeleoperado\n"));
  Serial.print(F("\r\nConecte Control\n"));
}


void loop() {
  Usb.Task();
  vel();// Esta función modifica la velocidad del robot
  if (PS4.connected()) {
    direccion();
    motores();// Esta función sirve para controlar los motores  
   }
}

