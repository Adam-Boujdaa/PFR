/*
 CONTROLE MOTEURS - J. DAVY
 */
#include <AFMotor.h>

// CAPTEURS ULTRASONS
#define TRIG_AV 22 
#define ECHO_AV 24
#define TRIG_DR 44
#define ECHO_DR 46
#define TRIG_GA 34
#define ECHO_GA 36

// MOTEURS
AF_DCMotor m1(1), m2(2), m3(3), m4(4);
int vitesse = 200;  

// CAPTEURS 
long calculDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duree = pulseIn(echoPin, HIGH, 30000);
  if (duree == 0) {
    return -1 ;
  }
  return duree * 0.034 / 2;
}

void envoyerDistances() {
  long av = calculDistance(TRIG_AV, ECHO_AV);
  long dr = calculDistance(TRIG_DR, ECHO_DR);
  long ga = calculDistance(TRIG_GA, ECHO_GA);
  Serial.print("DIST:");
  Serial.print(av);
  Serial.print(":");
  Serial.print(dr);
  Serial.print(":");
  Serial.println(ga);
}

// MOTEURS: commandes simples
void stop() {
  m1.run(RELEASE); m2.run(RELEASE);
  m3.run(RELEASE); m4.run(RELEASE);
}

void avancer() {
  m1.setSpeed(vitesse); m2.setSpeed(vitesse);
  m3.setSpeed(vitesse); m4.setSpeed(vitesse);
  m1.run(FORWARD); m2.run(FORWARD);
  m3.run(FORWARD); m4.run(FORWARD);
}

void reculer() {
  m1.setSpeed(vitesse); m2.setSpeed(vitesse);
  m3.setSpeed(vitesse); m4.setSpeed(vitesse);
  m1.run(BACKWARD); m2.run(BACKWARD);
  m3.run(BACKWARD); m4.run(BACKWARD);
}

void tournerGauche() {
  m1.setSpeed(vitesse); m2.setSpeed(vitesse);
  m3.setSpeed(vitesse); m4.setSpeed(vitesse);
  m1.run(FORWARD);  m3.run(FORWARD);
  m2.run(BACKWARD); m4.run(BACKWARD);
}

void tournerDroite() {
  m1.setSpeed(vitesse); m2.setSpeed(vitesse);
  m3.setSpeed(vitesse); m4.setSpeed(vitesse);
  m1.run(BACKWARD); m3.run(BACKWARD);
  m2.run(FORWARD);  m4.run(FORWARD);
}


// SETUP  
void setup() {
  Serial.begin(9600);
  pinMode(TRIG_AV, OUTPUT);
  pinMode(ECHO_AV, INPUT);
  pinMode(TRIG_DR, OUTPUT);
  pinMode(ECHO_DR,  INPUT);
  pinMode(TRIG_GA, OUTPUT);
  pinMode(ECHO_GA, INPUT);
}

//LOOP
void loop() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    if (cmd.length() == 0) return;
    
    // Commandes simples (port_serie.py)
    if      (cmd == "AVANCER")   avancer();
    else if (cmd == "RECULER")   reculer();
    else if (cmd == "TOURNER_G") tournerGauche();
    else if (cmd == "TOURNER_D") tournerDroite();
    else if (cmd == "STOP")      stop();
    else if (cmd == "DISTANCE")  envoyerDistances();
  }
}
