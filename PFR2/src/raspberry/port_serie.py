"""
FICHIER DE COMMUNICATION RASPBERRY/ARDUINO
VOIR : https://roboticsbackend.com/raspberry-pi-arduino-serial-communication/
"""

# --- BIBLIOTHÈQUES
import serial
import serial.tools.list_ports
import time


# --- CONNEXION ---
def _find_arduino_port():
    for p in serial.tools.list_ports.comports():
        if "ttyACM" in p.device or "Arduino" in (p.description or ""):
            return p.device
    return "/dev/ttyACM0"

ser = serial.Serial(_find_arduino_port(), 9600, timeout=1)
time.sleep(2)  # Attente bootloader Arduino
print(f"[serial] Connecté sur {ser.port}")


# --- ENVOI ---
def send(cmd):
    ser.write((cmd + '\n').encode())


# --- COMMANDES MOTEURS ---
def avancer():
    send("AVANCER")

def reculer():
    send("RECULER")

def tourner_gauche():
    send("TOURNER_G")

def tourner_droite():
    send("TOURNER_D")

def stop():
    send("STOP")


# --- CAPTEURS ---
def lire_distances():
    ser.reset_input_buffer()
    send("DISTANCE")
    line = ser.readline().decode(errors="ignore").strip()
    if line.startswith("DIST:"):
        parts = line.split(":")
        if len(parts) == 4:
            return int(parts[1]), int(parts[2]), int(parts[3])
    return None, None, None


# --- FERMETURE ---
def close():
    stop()
    ser.close()