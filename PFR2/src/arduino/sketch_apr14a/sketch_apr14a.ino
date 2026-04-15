#include <AFMotor.h>

// --- CAPTEURS ULTRASONS ---
// C-AVANT
#define TRIG_AV 22 
#define ECHO_AV 24

// C-DROITE
#define TRIG_DR 44
#define ECHO_DR 346

// C-GAUCHE
#define TRIG_GA 34
#define ECHO_GA 36

// Calcul de la distance (documentation Arduino)
long calculDistance(int trigPin, int echoPin) {
  // Émission d'un signal de durée 10 microsecondes
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Écoute de l'écho
  long duree = pulseIn(echoPin, HIGH); // éventuellement rajouter un timeout

  // Calcul de la distance
  return duree*0.034/2; // cm
}

// Envoie données capteurs ultrasons (AV, DR, GA)
void envoyerDistances() {
  long distAvant = calculDistance(TRIG_AV, ECHO_AV);
  long distDroite = calculDistance(TRIG_DR, ECHO_DR);
  long distGauche = calculDistance(TRIG_GA, ECHO_GA);

  Serial.print("DIST:");
  Serial.print(distAvant);
  Serial.print(":");
  Serial.print(distDroite);
  Serial.print(":");
  Serial.println(distGauche);
}

// --- MOTEURS ---
AF_DCMotor m1(1), m2(2), m3(3), m4(4);
int vitesse = 200;

void stop() {
  m1.run(RELEASE);
  m2.run(RELEASE);
  m3.run(RELEASE);
  m4.run(RELEASE);
}

void avancer(int speed) {
  m1.setSpeed(speed); m2.setSpeed(speed); m3.setSpeed(speed); m4.setSpeed(speed);

  m1.run(FORWARD);
  m2.run(FORWARD);
  m3.run(FORWARD);
  m4.run(FORWARD);
}

void reculer(int speed) {
  m1.setSpeed(speed); m2.setSpeed(speed); m3.setSpeed(speed); m4.setSpeed(speed);

  m1.run(BACKWARD);
  m2.run(BACKWARD);
  m3.run(BACKWARD);
  m4.run(BACKWARD);
}

void tournerGauche(int speed) {
  m1.setSpeed(speed); m2.setSpeed(speed); m3.setSpeed(speed); m4.setSpeed(speed);
  
  m1.run(FORWARD);
  m2.run(BACKWARD); 
  m3.run(FORWARD); 
  m4.run(BACKWARD);
}

void tournerDroite(int speed) {
  m1.setSpeed(speed); m2.setSpeed(speed); m3.setSpeed(speed); m4.setSpeed(speed);
  
  m1.run(BACKWARD);
  m2.run(FORWARD);
  m3.run(BACKWARD);
  m4.run(FORWARD);
}

// --- SETUP ---
void setup(){
  Serial.begin(9600);
  pinMode(TRIG_AV, OUTPUT);
  pinMode(ECHO_AV, INPUT);
  pinMode(TRIG_DR, OUTPUT);
  pinMode(ECHO_DR, INPUT);
  pinMode(TRIG_GA, OUTPUT);
  pinMode(ECHO_GA, INPUT);
}

// --- LOOP/COMMUNICATION ---
void loop(){
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd == "STOP")            { stop(); }
    else if (cmd == "AVANCER")    { avancer(vitesse); }
    else if (cmd == "RECULER")    { reculer(vitesse); }
    else if (cmd == "TOURNER_D")  { tournerDroite(vitesse); }
    else if (cmd == "TOURNER_G")  { tournerGauche(vitesse); }
    else if (cmd == "DISTANCE")   { envoyerDistances(); }
  }
}
