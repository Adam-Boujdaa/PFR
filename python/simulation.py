"""
PFR1 - Module de simulation robot
Programme complet avec architecture structurée
"""

import turtle as tl
import math
from datetime import datetime

# Configuration
config = {}

# État du robot
robot_state = {
    'x': 0.0,
    'y': 0.0,
    'orientation': 0.0  # en degrés
}

# Environnement
environnement = {
    'echelle': 100,  # 1 mètre = 100 pixels
    'longueur': 8,   # en mètres
    'largeur': 6,    # en mètres
    'obstacles': []
}

# Objets Turtle
ecran = None
robot = None

# Fichier de log
fichier_log = None

def charger_configuration(fichier_conf):
    global config
    
    config = {}
    try:
        with open(fichier_conf, 'r', encoding='utf-8') as f:
            for ligne in f:
                ligne = ligne.strip()
                # Ignore commentaires et lignes vides
                if ligne and not ligne.startswith('#'):
                    if '=' in ligne:
                        cle, valeur = ligne.split('=', 1)
                        config[cle.strip()] = valeur.strip()
        
        log_message("Configuration chargée avec succès")
        return True
    
    except FileNotFoundError:
        print(f"ERREUR : Fichier {fichier_conf} introuvable")
        return False
    except Exception as e:
        print(f"ERREUR lors du chargement de la configuration : {e}")
        return False

def initialiser_environnement(fichier_conf):
    global ecran, robot, environnement
    
    # Charge la configuration
    if not charger_configuration(fichier_conf):
        print("Utilisation des paramètres par défaut")
    
    # Récupération des paramètres (avec valeurs par défaut)
    largeur_fenetre = int(config.get('WINDOW_WIDTH', 800))
    hauteur_fenetre = int(config.get('WINDOW_HEIGHT', 600))
    titre = config.get('WINDOW_TITLE', 'Simulation Robot PFR1')
    couleur_fond = config.get('BACKGROUND_COLOR', 'white')
    
    environnement['echelle'] = int(config.get('SCALE', 100))
    
    forme_robot = config.get('ROBOT_SHAPE', 'triangle')
    couleur_robot = config.get('ROBOT_COLOR', 'blue')
    taille_robot = float(config.get('ROBOT_SIZE', 1.5))
    
    # Création de la fenêtre
    ecran = tl.Screen()
    ecran.setup(width=largeur_fenetre, height=hauteur_fenetre)
    ecran.title(titre)
    ecran.bgcolor(couleur_fond)
    ecran.tracer(0)  # Désactive l'animation automatique pour plus de contrôle
    
    # Création du robot
    robot = tl.Turtle()
    robot.shape(forme_robot)
    robot.color(couleur_robot)
    robot.shapesize(taille_robot, taille_robot)
    robot.speed(0)
    
    ecran.update()
    log_message("Environnement graphique initialisé")
    print("Environnement initialisé")

def charger_salle(fichier_salle):
    global environnement
    
    try:
        with open(fichier_salle, 'r', encoding='utf-8') as f:
            for ligne in f:
                ligne = ligne.strip()
                
                if ligne.startswith('LONGUEUR='):
                    environnement['longueur'] = float(ligne.split('=')[1])
                
                elif ligne.startswith('LARGEUR='):
                    environnement['largeur'] = float(ligne.split('=')[1])
                
                elif ligne.startswith('OBSTACLE='):
                    # Parse: type,x,y,param1,param2
                    donnees = ligne.split('=')[1].split(',')
                    obstacle = {
                        'type': donnees[0].strip(),
                        'x': float(donnees[1]),
                        'y': float(donnees[2]),
                        'param1': float(donnees[3]),
                        'param2': float(donnees[4]) if len(donnees) > 4 else 0.0
                    }
                    environnement['obstacles'].append(obstacle)
        
        log_message(f"Salle chargée : {environnement['longueur']}x{environnement['largeur']}m avec {len(environnement['obstacles'])} obstacle(s)")
        print(f"Salle : {environnement['longueur']}x{environnement['largeur']}m")
        print(f"Obstacles : {len(environnement['obstacles'])}")
        return True
    
    except FileNotFoundError:
        print(f"ERREUR : Fichier {fichier_salle} introuvable")
        return False
    except Exception as e:
        print(f"ERREUR lors du chargement de la salle : {e}")
        return False

def afficher_obstacles():
    echelle = environnement['echelle']
    
    for i, obstacle in enumerate(environnement['obstacles']):
        dessinateur = tl.Turtle()
        dessinateur.hideturtle()
        dessinateur.speed(0)
        dessinateur.penup()
        
        if obstacle['type'] == 'rectangle':
            largeur = obstacle['param1']
            hauteur = obstacle['param2']
            x = obstacle['x']
            y = obstacle['y']
            
            # Position du coin inférieur gauche
            coin_x = (x - largeur/2) * echelle
            coin_y = (y - hauteur/2) * echelle
            
            dessinateur.goto(coin_x, coin_y)
            dessinateur.pendown()
            dessinateur.color('orange')
            dessinateur.fillcolor('orange')
            dessinateur.begin_fill()
            
            for _ in range(2):
                dessinateur.forward(largeur * echelle)
                dessinateur.left(90)
                dessinateur.forward(hauteur * echelle)
                dessinateur.left(90)
            
            dessinateur.end_fill()
        
        elif obstacle['type'] == 'cercle':
            rayon = obstacle['param1']
            x = obstacle['x']
            y = obstacle['y']
            
            # Position pour dessiner (bord inférieur du cercle)
            centre_x = x * echelle
            centre_y = (y - rayon) * echelle
            
            dessinateur.goto(centre_x, centre_y)
            dessinateur.pendown()
            dessinateur.color('red')
            dessinateur.fillcolor('red')
            dessinateur.begin_fill()
            dessinateur.circle(rayon * echelle)
            dessinateur.end_fill()
    
    ecran.update()
    log_message(f"{len(environnement['obstacles'])} obstacle(s) affiché(s)")
    print(f" {len(environnement['obstacles'])} obstacle(s) affiché(s)")

def point_dans_obstacle(x, y, obstacle):
    if obstacle['type'] == 'rectangle':
        demi_largeur = obstacle['param1'] / 2
        demi_hauteur = obstacle['param2'] / 2
        
        return (obstacle['x'] - demi_largeur <= x <= obstacle['x'] + demi_largeur and
                obstacle['y'] - demi_hauteur <= y <= obstacle['y'] + demi_hauteur)
    
    elif obstacle['type'] == 'cercle':
        rayon = obstacle['param1']
        distance = math.sqrt((x - obstacle['x'])**2 + (y - obstacle['y'])**2)
        return distance <= rayon
    
    return False


def verifier_collision(x, y):
    for obstacle in environnement['obstacles']:
        if point_dans_obstacle(x, y, obstacle):
            return obstacle
    return None

def calculer_position_future(distance, recule=False):
    rad = math.radians(robot_state['orientation'])
    
    if recule:
        futur_x = robot_state['x'] - distance * math.cos(rad)
        futur_y = robot_state['y'] - distance * math.sin(rad)
    else:
        futur_x = robot_state['x'] + distance * math.cos(rad)
        futur_y = robot_state['y'] + distance * math.sin(rad)
    
    return futur_x, futur_y

def valider_deplacement(distance, recule=False):
    futur_x, futur_y = calculer_position_future(distance, recule)
    obstacle = verifier_collision(futur_x, futur_y)
    
    if obstacle:
        return False, obstacle, (futur_x, futur_y)
    else:
        return True, None, (futur_x, futur_y)

def contourner_obstacle(obstacle, distance_voulue):
    log_message("Tentative de contournement")
    print("\nContournement en cours...")

    tourner_gauche(45)
    
    # Vérifie si on peut avancer latéralement
    if not valider_deplacement(1.5)[0]:
        print("Contournement impossible (obstacle sur la gauche)")
        tourner_droite(45)  # Revenir à l'orientation initiale
        return False
    
    avancer_direct(1.5)
    tourner_droite(45)

    if valider_deplacement(distance_voulue)[0]:
        avancer_direct(distance_voulue)
        print("Contournement réussi")
        log_message("Contournement réussi")
        return True
    else:
        print("Impossible de continuer après contournement")
        return False

def avancer_direct(distance):
    global robot_state
    
    pixels = distance * environnement['echelle']
    robot.forward(pixels)
    
    # Mise à jour de la position
    rad = math.radians(robot_state['orientation'])
    robot_state['x'] += distance * math.cos(rad)
    robot_state['y'] += distance * math.sin(rad)
    
    ecran.update()


def reculer_direct(distance):
    global robot_state
    
    pixels = distance * environnement['echelle']
    robot.backward(pixels)
    
    # Mise à jour de la position
    rad = math.radians(robot_state['orientation'])
    robot_state['x'] -= distance * math.cos(rad)
    robot_state['y'] -= distance * math.sin(rad)
    
    ecran.update()


def tourner_gauche(angle):
    global robot_state
    
    robot.left(angle)
    robot_state['orientation'] = (robot_state['orientation'] + angle) % 360
    
    ecran.update()
    log_message(f"Rotation gauche : {angle}° | Orientation : {robot_state['orientation']:.1f}°")


def tourner_droite(angle):
    global robot_state
    
    robot.right(angle)
    robot_state['orientation'] = (robot_state['orientation'] - angle) % 360
    
    ecran.update()
    log_message(f"Rotation droite : {angle}° | Orientation : {robot_state['orientation']:.1f}°")

def avancer(distance):
    valide, obstacle, (futur_x, futur_y) = valider_deplacement(distance)
    
    if obstacle:
        print(f"\n OBSTACLE DÉTECTÉ à ({futur_x:.2f}, {futur_y:.2f})")
        print(f"    Type : {obstacle['type']}")
        print("\n  Options :")
        print("    1. Contourner l'obstacle")
        print("    2. Annuler le déplacement")
        
        choix = input("\n  Votre choix (1 ou 2) : ").strip()
        
        if choix == "1":
            if contourner_obstacle(obstacle, distance):
                return True
            else:
                print("Déplacement impossible")
                log_message(f"Échec : obstacle non contournable")
                return False
        else:
            print("Déplacement annulé")
            log_message(f"Déplacement annulé par l'utilisateur")
            return False
    
    avancer_direct(distance)
    print(f"Avance {distance}m → Position ({robot_state['x']:.2f}, {robot_state['y']:.2f})")
    log_message(f"Avance {distance}m → ({robot_state['x']:.2f}, {robot_state['y']:.2f})")
    return True


def reculer(distance):
    valide, obstacle, (futur_x, futur_y) = valider_deplacement(distance, recule=True)
    
    if obstacle:
        print(f"\n OBSTACLE DÉTECTÉ à ({futur_x:.2f}, {futur_y:.2f})")
        print(f"    Type : {obstacle['type']}")
        print("Déplacement impossible (pas de contournement en marche arrière)")
        log_message(f"Échec recul : obstacle à ({futur_x:.2f}, {futur_y:.2f})")
        return False
    
    # Pas d'obstacle, déplacement normal
    reculer_direct(distance)
    print(f"Recule {distance}m → Position ({robot_state['x']:.2f}, {robot_state['y']:.2f})")
    log_message(f"Recule {distance}m → ({robot_state['x']:.2f}, {robot_state['y']:.2f})")
    return True

def analyser_commande(ligne_commande):
    commande = ligne_commande.strip()
    if not commande:
        return True

    parties = commande.split(',')
    if len(parties) != 3:
        print(f"Format invalide : {commande}")
        print("   Format attendu : ACTION,DIRECTION,VALEUR")
        return False
    
    action = parties[0].strip()
    direction = parties[1].strip()
    
    try:
        valeur = float(parties[2].strip())
    except ValueError:
        print(f"Valeur invalide : {parties[2]}")
        return False
    
    if action == "A":  # Avancer
        if direction == "-":
            return avancer(valeur)
        elif direction == "D":
            tourner_droite(90)
            print(f"Tourne droite 90° → Orientation {robot_state['orientation']:.1f}°")
            return avancer(valeur)
        elif direction == "G":
            tourner_gauche(90)
            print(f"Tourne gauche 90° → Orientation {robot_state['orientation']:.1f}°")
            return avancer(valeur)
        else:
            print(f"Direction inconnue : {direction}")
            return False
    
    elif action == "R":  # Reculer
        if direction == "-":
            return reculer(valeur)
        elif direction == "D":
            tourner_droite(90)
            print(f"Tourne droite 90° : Orientation {robot_state['orientation']:.1f}°")
            return reculer(valeur)
        elif direction == "G":
            tourner_gauche(90)
            print(f"Tourne gauche 90° : Orientation {robot_state['orientation']:.1f}°")
            return reculer(valeur)
        else:
            print(f"Direction inconnue : {direction}")
            return False
    
    elif action == "T":  # Tourner
        if direction == "G":
            tourner_gauche(valeur)
            print(f"Tourne gauche {valeur}° → Orientation {robot_state['orientation']:.1f}°")
            return True
        elif direction == "D":
            tourner_droite(valeur)
            print(f"Tourne droite {valeur}° → Orientation {robot_state['orientation']:.1f}°")
            return True
        else:
            print(f"Direction de rotation invalide : {direction}")
            return False
    
    else:
        print(f"Action inconnue : {action}")
        print("   Actions valides : A (avancer), R (reculer), T (tourner)")
        return False

def executer_fichier_commandes(fichier_commandes):
    try:
        with open(fichier_commandes, 'r', encoding='utf-8') as f:
            commandes = [ligne.strip() for ligne in f if ligne.strip()]
        
        print("\n" + "="*60)
        print(f"  EXÉCUTION DE {len(commandes)} COMMANDE(S)")
        print("="*60 + "\n")
        
        log_message(f"Début exécution : {len(commandes)} commande(s)")
        
        for i, commande in enumerate(commandes, 1):
            print(f"[{i}/{len(commandes)}] {commande}")
            
            if not analyser_commande(commande):
                print(f"\n Erreur à la commande {i}, arrêt de l'exécution\n")
                log_message(f"Arrêt à la commande {i}/{len(commandes)}")
                break
            
            print()  # Ligne vide entre les commandes
        
        print("="*60)
        print("  FIN DE L'EXÉCUTION")
        print("="*60)
        
        log_message("Exécution terminée")
        return True
    
    except FileNotFoundError:
        print(f"Fichier {fichier_commandes} introuvable")
        return False
    except Exception as e:
        print(f"Erreur lors de l'exécution : {e}")
        return False

def initialiser_log(nom_fichier="simulation_log.txt"):
    """
    Initialise le fichier de log avec horodatage
    """
    global fichier_log
    
    try:
        fichier_log = open(nom_fichier, 'a', encoding='utf-8')
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        fichier_log.write(f"\n{'='*60}\n")
        fichier_log.write(f"SESSION DÉMARRÉE : {timestamp}\n")
        fichier_log.write(f"{'='*60}\n")
        fichier_log.flush()
        print(f"Log initialisé : {nom_fichier}")
        return True
    except Exception as e:
        print(f"Erreur initialisation log : {e}")
        return False


def log_message(message):
    if fichier_log:
        timestamp = datetime.now().strftime("%H:%M:%S")
        fichier_log.write(f"[{timestamp}] {message}\n")
        fichier_log.flush()


def fermer_log():
    if fichier_log:
        timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        fichier_log.write(f"\n SESSION TERMINÉE : {timestamp}\n")
        fichier_log.write(f"{'='*60}\n")
        fichier_log.close()

def main():
    print("  SIMULATION ROBOT PFR1")
    
    fichier_config = "../config/simulation.txt"
    fichier_salle = "../config/salle_test.txt"
    fichier_commandes = "../scripts/commandes_tests.txt"
    
    initialiser_log("simulation_log.txt")
    
    initialiser_environnement(fichier_config)

    if not charger_salle(fichier_salle):
        print("Impossible de continuer sans fichier de salle")
        return
    

    afficher_obstacles()

    print("\n Démarrage de l'exécution des commandes...\n")
    executer_fichier_commandes(fichier_commandes)

    print(f"\n Position finale : ({robot_state['x']:.2f}, {robot_state['y']:.2f})")
    print(f"Orientation finale : {robot_state['orientation']:.1f}°")

    fermer_log()
    
    print("\n" + "="*60)
    input("Appuyez sur Entrée pour fermer la fenêtre...")
    ecran.bye()

if __name__ == "__main__":
    main()
