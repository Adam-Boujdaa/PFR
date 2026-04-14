import asyncio
import json
from contextlib import asynccontextmanager
from fastapi import FastAPI, WebSocket, WebSocketDisconnect
from fastapi.staticfiles import StaticFiles

import port_serie

@asynccontextmanager
async def lifespan(app: FastAPI):
    yield
    port_serie.close()

app = FastAPI(lifespan=lifespan)

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await websocket.accept()
    push_task = asyncio.create_task(_push_sensors(websocket))
    
    try:
        while True:
            msg = json.loads(await websocket.receive_text())
            _handle(msg)
    except WebSocketDisconnect:
        pass
    finally:
        push_task.cancel()
        port_serie.stop()

async def _push_sensors(websocket: WebSocket):
    """Envoie distances ultrason au client ~5Hz"""
    try:
        while True:
            av, dr, ga = port_serie.lire_distances()
            await websocket.send_text(json.dumps({
                "type": "telemetry",
                "avant": av,
                "droite": dr,
                "gauche": ga
            }))
            await asyncio.sleep(0.2)
    except:
        pass

def _handle(msg: dict):
    cmd = msg.get("cmd")
    
    # Contrôle manuel (D-pad)
    if cmd == "move":
        x = msg.get("x", 0)
        y = msg.get("y", 0)
        
        if y > 0:
            port_serie.avancer()
        elif y < 0:
            port_serie.reculer()
        elif x < 0:
            port_serie.tourner_gauche()
        elif x > 0:
            port_serie.tourner_droite()
    
    elif cmd == "stop":
        port_serie.stop()
    
    # Actions autonomes
    elif cmd == "action":
        name = msg.get("name", "")
        if name == "AVANCER":
            port_serie.avancer()
        elif name == "RECULER":
            port_serie.reculer()
        elif name == "TOURNER_G":
            port_serie.tourner_gauche()
        elif name == "TOURNER_D":
            port_serie.tourner_droite()
        # CONTOURNER_G/D → à implémenter plus tard

app.mount("/", StaticFiles(directory="../web", html=True), name="static")