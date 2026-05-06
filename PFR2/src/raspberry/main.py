import asyncio
import json
from contextlib import asynccontextmanager
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.staticfiles import StaticFiles
import port_serie
import navigation
from config_deplacement import SEUIL_AVANT, SEUIL_LATERAL

dist_av = 999
dist_dr = 999
dist_ga = 999
nav_task = None

def get_dist():
    return (dist_av, dist_dr, dist_ga)

def nav_actif():
    return nav_task is not None

def nav_arreter():
    global nav_task
    if nav_task:
        nav_task.cancel()
        nav_task = None
    port_serie.stop()

@asynccontextmanager
async def lifespan(app: FastAPI):
    yield
    nav_arreter()
    port_serie.close()

app = FastAPI(lifespan=lifespan)

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    push_task = asyncio.create_task(_push_sensors(websocket))
    try:
        while True:
            msg = json.loads(await websocket.receive_text())
            await _handle(msg, websocket)
    except WebSocketDisconnect:
        pass
    finally:
        push_task.cancel()
        nav_arreter()

async def _push_sensors(websocket: WebSocket):
    global dist_av, dist_dr, dist_ga
    try:
        while True:
            av, dr, ga = port_serie.lire_distances()
            if av is not None:
                dist_av, dist_dr, dist_ga = av, dr, ga
            await websocket.send_text(json.dumps({
                "type": "telemetry",
                "avant": dist_av, "droite": dist_dr, "gauche": dist_ga,
                "auto": nav_actif()
            }))
            await asyncio.sleep(0.2)
    except:
        pass

async def _handle(msg: dict, websocket: WebSocket):
    global nav_task
    cmd = msg.get("cmd")
    
    # Contrôle manuel → coupe le mode autonome
    if cmd in ("move", "stop", "action") and nav_actif():
        nav_arreter()
        await websocket.send_text(json.dumps({"type": "auto_state", "actif": False}))
    
    if cmd == "move":
        x = msg.get("x", 0)
        y = msg.get("y", 0)
        if y > 0 and dist_av < SEUIL_AVANT:   port_serie.stop(); return
        if x < 0 and dist_ga < SEUIL_LATERAL: port_serie.stop(); return
        if x > 0 and dist_dr < SEUIL_LATERAL: port_serie.stop(); return
        if   y > 0: port_serie.avancer()
        elif y < 0: port_serie.reculer()
        elif x < 0: port_serie.tourner_gauche()
        elif x > 0: port_serie.tourner_droite()
    
    elif cmd == "stop":
        port_serie.stop()
    
    elif cmd == "action":
        name = msg.get("name", "")
        if name == "AVANCER"   and dist_av < SEUIL_AVANT:   port_serie.stop(); return
        if name == "TOURNER_G" and dist_ga < SEUIL_LATERAL: port_serie.stop(); return
        if name == "TOURNER_D" and dist_dr < SEUIL_LATERAL: port_serie.stop(); return
        if   name == "AVANCER":   port_serie.avancer()
        elif name == "RECULER":   port_serie.reculer()
        elif name == "TOURNER_G": port_serie.tourner_gauche()
        elif name == "TOURNER_D": port_serie.tourner_droite()
    
    elif cmd == "auto":
        mode = msg.get("mode", "")
        nav_arreter()
        if mode == "exploration":
            nav_task = navigation.exploration(get_dist)
            await websocket.send_text(json.dumps({"type": "auto_state", "actif": True, "mode": "exploration"}))
        elif mode == "suivre_mur":
            nav_task = navigation.suivre_mur(get_dist)
            await websocket.send_text(json.dumps({"type": "auto_state", "actif": True, "mode": "suivre_mur"}))
        elif mode == "stop":
            await websocket.send_text(json.dumps({"type": "auto_state", "actif": False}))

app.mount("/", StaticFiles(directory="../web", html=True), name="static")