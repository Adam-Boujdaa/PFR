"""
Séquences de mouvements
"""

import time
import port_serie

# Durée à ajuster
DUREE_90_DEG = 1.0
DUREE_360_DEG = 4.0
DUREE_AVANCE = 1.0

# --- SÉQUENCES ---
def tour_sur_soi(sens="droite"):
    if sens == "droite":
        port_serie.tourner_droite()
    else :
        port_serie.tourner_gauche()
    time.sleep(DUREE_360_DEG)
    port_serie.stop()

def faire_carre():
    for i in range(4):
        port_serie.avancer()
        time.sleep(DUREE_AVANCE)
        port_serie.stop()
        time.sleep(0.2)
        port_serie.tourner_droite()
        time.sleep(DUREE_90_DEG)
        port_serie.stop()
        time.sleep(0.2)

def faire_rond():
    for i in range(16):
        port_serie.avancer()
        time.sleep(0.3)
        port_serie.tourner_droite()
        time.sleep(0.15)
    port_serie.stop()

def faire_zigzag(nb_zigzags=3):
    for i in range(nb_zigzags):
        port_serie.tourner_gauche()
        time.sleep(DUREE_90_DEG)
        port_serie.avancer()
        time.sleep(DUREE_AVANCE)
        port_serie.stop()

        port_serie.tourner_droite()
        time.sleep(DUREE_90_DEG)
        port_serie.avancer()
        time.sleep(DUREE_AVANCE)
        port_serie.stop()
    port_serie.stop()

def demi_tour():
    port_serie.tourner_droite()
    time.sleep(DUREE_360_DEG/2)
    port_serie.stop()