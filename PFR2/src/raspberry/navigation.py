import asyncio
import port_serie

# Paramètres
SEUIL_AVANT = 10
SEUIL_LATERAL = 8
DIST_MUR = 15   
TOLERANCE_MUR = 3 

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
    derniere_cmd = None 
    try:
        while True:
            av, dr, ga = get_dist()
            if av is None:
                av, dr, ga = 999, 999, 999
            
            if av < SEUIL_AVANT:
                cmd = "TOURNER_G"
            elif dr > DIST_MUR + TOLERANCE_MUR:
                cmd = "TOURNER_D"
            elif dr < DIST_MUR - TOLERANCE_MUR:
                cmd = "TOURNER_G"
            else:
                cmd = "AVANCER"
            
            if cmd != derniere_cmd:
                _exec(cmd)
                derniere_cmd = cmd
            
            await asyncio.sleep(0.1)
    except asyncio.CancelledError:
        pass
    finally:
        port_serie.stop()

# com avec port serie
def _exec(cmd):
    if   cmd == "AVANCER":   port_serie.avancer()
    elif cmd == "TOURNER_G": port_serie.tourner_gauche()
    elif cmd == "TOURNER_D": port_serie.tourner_droite()
    elif cmd == "RECULER":   port_serie.reculer()