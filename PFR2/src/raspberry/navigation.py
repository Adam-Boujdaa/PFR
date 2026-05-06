import asyncio
import port_serie
from config import SEUIL_AVANT, SEUIL_LATERAL, DIST_MUR, TOLERANCE_MUR

# Exploration
def exploration(get_dist):
    return asyncio.create_task(_exploration_loop(get_dist))

async def _exploration_loop(get_dist):
    derniere_cmd = None
    try:
        while True:
            av, dr, ga = get_dist()
            if av is None:
                av, dr, ga = 999, 999, 999
            
            if av < SEUIL_AVANT:
                if dr >= ga:
                    action = "TOURNER_D"  
                else: 
                    action ="TOURNER_G"
            elif ga < SEUIL_LATERAL:
                action = "TOURNER_D"
            elif dr < SEUIL_LATERAL:
                action = "TOURNER_G"
            else:
                action = "AVANCER"
            
            if action != derniere_cmd:
                _exec(action)
                derniere_cmd = action
            
            await asyncio.sleep(0.15)
    except asyncio.CancelledError:
        pass
    finally:
        port_serie.stop()

# Suivre mur (droite)
def suivre_mur(get_dist):  
    return asyncio.create_task(_suivre_mur_loop(get_dist))

async def _suivre_mur_loop(get_dist):
    try:
        while True:
            av, dr, ga = get_dist()
            if av is None:
                av, dr, ga = 999, 999, 999
            
            if av < SEUIL_AVANT:
                # tourne pour esquiver
                port_serie.tourner_gauche()
                await asyncio.sleep(0.3)
            elif dr < DIST_MUR - TOLERANCE_MUR:
                # # si trop loin, tourne legerement puis avance
                port_serie.tourner_gauche()
                await asyncio.sleep(0.1)
                port_serie.avancer()
                await asyncio.sleep(0.2)
            elif dr > DIST_MUR + TOLERANCE_MUR:
                # si trop loin, tourne legerement puis avance
                port_serie.tourner_droite()
                await asyncio.sleep(0.1)
                port_serie.avancer()
                await asyncio.sleep(0.2)
            else:
                # si distance à droite bonne, on avance
                port_serie.avancer()
                await asyncio.sleep(0.15)
    except asyncio.CancelledError:
        pass
    finally:
        port_serie.stop()

# com avec port serie
def _exec(action):
    if   action == "AVANCER" :   port_serie.avancer()
    elif action == "TOURNER_G": port_serie.tourner_gauche()
    elif action == "TOURNER_D": port_serie.tourner_droite()
    elif action == "RECULER":   port_serie.reculer()