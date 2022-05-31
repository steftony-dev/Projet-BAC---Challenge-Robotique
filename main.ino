///// Initialisation des librairie /////
#include <Servo.h>

#include <PS4USB.h>
#include <PS4BT.h>
#include <usbhub.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

USB Usb;
PS4USB PS4_USB(&Usb);
BTD Btd(&Usb);

PS4BT PS4_BT(&Btd, PAIR);
////////////////////////////



///// Initialisation des variables/moteurs /////
int vitesse = 0;

//Carte Shield 1 | Broche Moteur A | Moteur Gauche M1
int M1direction = 5; //Broche 12
int M1vitesse = 3;
int M1frein = 9;

//Carte Shield 1 | Broche Moteur B | Moteur Droit M2
int M2direction = 4; //Broche 13
int M2vitesse = 6; //Broche 11
int M2frein = 8;

//Carte Shield 2 | Broche Moteur B | Moteur de rotation du bras M3
int M3direction = 22; //Broche 13
int M3vitesse = 25; //Broche 11
int M3frein = 23; //Broche 8

//Carte Shield 1 | Moteur de la pince M4
int M4 = 24;
Servo servoM4;
int posM4 = 0; //Position initial du moteur
/////////////////////////////////



void setup() {
  Serial.begin(9600);
#if !defined(__MIPSEL__)
  while (!Serial);
#endif
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC has not started"));
    while (1);
  }
  Serial.print(F("\r\nLibrary USB PS4 started"));
  Serial.print(F("\r\nLibrary Bluetooth PS4 started"));

///// Initialisations des moteurs //////////
  //Carte Shield 1 | Broche Moteur A | Moteur Gauche M1
  pinMode(M1direction, OUTPUT);
  pinMode(M1vitesse, OUTPUT);
  pinMode(M1break, OUTPUT);
  
  digitalWrite(M1direction,HIGH);
  analogWrite(M1vitesse,0);
  digitalWrite(M1frein, HIGH);
  
  //Carte Shield 1 | Broche Moteur B | Moteur droit M2
  pinMode(M2direction, OUTPUT);
  pinMode(M2vitesse, OUTPUT);
  pinMode(M2break, OUTPUT);

  digitalWrite(M2direction,HIGH);
  analogWrite(M2vitesse,0);
  digitalWrite(M2frein, HIGH);

  //Carte Shield 2 | Broche Moteur B | Moteur de rotation du bras M3
  pinMode(M3direction, OUTPUT);
  pinMode(M3vitesse, OUTPUT);
  pinMode(M3break, OUTPUT);
  
  digitalWrite(M3direction,HIGH);
  analogWrite(M3vitesse,0);
  digitalWrite(M3frein, HIGH);

  //Carte Shield 1 | Moteur de la pince M4
  servoM4.attach(M4);
  servoM4.write(posM4);
////////////////////////////////////////////
}



void loop() {
  Usb.Task();
  
            ///////////////////////////////////////////////////////////////////
           ///////////////////////////// PS4 USB /////////////////////////////
          ///////////////////////////////////////////////////////////////////

  if (PS4_USB.connected()) {
////////////////////// L2 (M1 Gauche) //////////////////////
    if (PS4_USB.getAnalogButton(L2)) {
      vitesse = PS4_USB.getAnalogButton(L2);
      
      analogWrite(M1vitesse,map(vitesse, 0, 255, 0, 100));
      
      analogWrite(M2vitesse,0);
      digitalWrite(M2frein, HIGH);
      
      delay(100);
    } else
////////////////////// R2 (M2 Droite) //////////////////////
    if (PS4_USB.getAnalogButton(R2)) {
      vitesse = PS4_USB.getAnalogButton(R2);
      
      analogWrite(M2vitesse,map(vitesse, 0, 255, 0, 100));
      
      analogWrite(M1vitesse,0);
      digitalWrite(M1frein, HIGH);
      
      delay(100);
    } else
////////////////////// L (M1 et M2 | Avancer/Reculer) //////////////////////
    if (PS4_USB.getAnalogHat(LeftHatY)) {
      if (PS4_USB.getAnalogHat(LeftHatY) < 120) { //Avancer
        vitesse = PS4_USB.getAnalogHat(LeftHatY);
        
        digitalWrite(M1direction,HIGH);
        analogWrite(M1vitesse,map(vitesse, 0, 120, 0, 100));
        digitalWrite(M1frein, LOW);
        
        digitalWrite(M2direction,HIGH);
        analogWrite(M2vitesse,map(vitesse, 0, 120, 0, 100));
        digitalWrite(M2frein, LOW);
        
        delay(100);
      } else if (PS4_USB.getAnalogHat(LeftHatY) > 135) { //Reculer
        vitesse = PS4_USB.getAnalogHat(LeftHatY);
        
        digitalWrite(M1direction,LOW);
        analogWrite(M1vitesse,map(vitesse, 135, 255, 0, 100));
        digitalWrite(M1frein, LOW);
        
        digitalWrite(M2direction,LOW);
        analogWrite(M2vitesse,map(vitesse, 135, 255, 0, 100));
        digitalWrite(M2frein, LOW);
        
        delay(100);
      } else {
        digitalWrite(M1frein, HIGH);
        analogWrite(M1vitesse,0);
        
        digitalWrite(M2frein, HIGH);
        analogWrite(M2vitesse,0);
     }
    }
////////////////////// R (M3 Rotation du bras de la pince) //////////////////////
    if (PS4_USB.getAnalogHat(RightHatY)) {
      if (PS4_USB.getAnalogHat(RightHatY) < 120) { //Direction Avant
        vitesse = PS4_USB.getAnalogHat(RightHatY);
        
        digitalWrite(M3direction,HIGH);
        analogWrite(M3vitesse,map(vitesse, 0, 120, 0, 100));
        digitalWrite(M3frein, LOW);
        
        delay(100);
      } else if (PS4_USB.getAnalogHat(RightHatY) > 135) { //Direction Arriére
        vitesse = PS4_USB.getAnalogHat(RightHatY);
        
        digitalWrite(M3direction,LOW);
        analogWrite(M3vitesse,map(vitesse, 135, 255, 0, 100));
        digitalWrite(M3frein, LOW);
        
        delay(100);
      } else {
        analogWrite(M3vitesse,0);
        digitalWrite(M3frein, HIGH);
     }
    }
////////////////////// R1 (M4 Ouvrir/Fermer de la pince) //////////////////////
    if (PS4_USB.getButtonClick(R1)) {
      if(posM4<90) {
        posM4=180;
      } else {
        posM4=0;
      }
    }
  }
  servoM4.write(posM4); 
}
