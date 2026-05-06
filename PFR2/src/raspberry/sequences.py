"""
Séquences de mouvements
"""
import time
import port_serie
from raspberry.config import duree_pour_distance, duree_pour_angle

# SÉQUENCES
def tour_sur_soi(sens="droite"):
    if sens == "droite":
        port_serie.tourner_droite()
    else:
        port_serie.tourner_gauche()
    time.sleep(duree_pour_angle(360))
    port_serie.stop()

def faire_carre(cote_cm=50):
    for _ in range(4):
        port_serie.avancer()
        time.sleep(duree_pour_distance(cote_cm))
        port_serie.stop()
        time.sleep(0.2)
        port_serie.tourner_droite()
        time.sleep(duree_pour_angle(90))
        port_serie.stop()
        time.sleep(0.2)

def faire_rond():
    for _ in range(16):
        port_serie.avancer()
        time.sleep(0.3)
        port_serie.tourner_droite()
        time.sleep(0.15)
    port_serie.stop()

def faire_zigzag(nb_zigzags=3):
    for _ in range(nb_zigzags):
        port_serie.tourner_gauche()
        time.sleep(duree_pour_angle(45))
        port_serie.avancer()
        time.sleep(duree_pour_distance(30))
        port_serie.stop()
        port_serie.tourner_droite()
        time.sleep(duree_pour_angle(90))
        port_serie.avancer()
        time.sleep(duree_pour_distance(30))
        port_serie.stop()
    port_serie.tourner_gauche()
    time.sleep(duree_pour_angle(45))
    port_serie.stop()

def demi_tour():
    port_serie.tourner_droite()
    time.sleep(duree_pour_angle(180))
    port_serie.stop()

def avancer_de(cm):
    port_serie.avancer()
    time.sleep(duree_pour_distance(cm))
    port_serie.stop()

def reculer_de(cm):
    port_serie.reculer()
    time.sleep(duree_pour_distance(cm))
    port_serie.stop()

def tourner_de(deg, sens="droite"):
    if sens == "droite":
        port_serie.tourner_droite()
    else:
        port_serie.tourner_gauche()
    time.sleep(duree_pour_angle(deg))
    port_serie.stop()