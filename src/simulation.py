"""
PFR1 - Module de simulation robot
"""
import turtle as tl
import math
from datetime import datetime
import socket

fichier_config = "config/simulation.conf"
fichier_salle = "config/salle_test.conf"
fichier_commandes = "config/commandes_test.txt"

# Configuration
config = {}

# État du robot
etat_robot = {
    'x': 0.0,
    'y': 0.0,
    'orientation': 0.0  # en degrés
}

# Environnement
environnement = {
    'echelle': 100,  # 1 mètre = 1 pixel
    'longueur': 8,   # en mètres
    'largeur': 6,    # en mètres
    'obstacles': []
}

# Objets Turtle
fenetre = None
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
    global fenetre, robot, environnement
    
    # Charge la configuration
    if not charger_configuration(fichier_conf):
        print("Utilisation des paramètres par défaut")
    
    # Récupération des paramètres 
    largeur_fenetre = int(config.get('WINDOW_WIDTH', 800)) # valeurs par défaut
    hauteur_fenetre = int(config.get('WINDOW_HEIGHT', 600))
    titre = config.get('WINDOW_TITLE', 'Simulation Robot PFR1')
    couleur_fond = config.get('BACKGROUND_COLOR', 'white')
    
    environnement['echelle'] = int(config.get('SCALE', 100))
    
    forme_robot = config.get('ROBOT_SHAPE', 'triangle')
    couleur_robot = config.get('ROBOT_COLOR', 'blue')
    taille_robot = float(config.get('ROBOT_SIZE', 1.5))
    vitesse_robot = int(config.get('ROBOT_SPEED', 6))
    
    # Création de la fenêtre
    fenetre = tl.Screen()
    fenetre.setup(width=largeur_fenetre, height=hauteur_fenetre)
    fenetre.title(titre)
    fenetre.bgcolor(couleur_fond)
    fenetre.tracer(0)  # pour afficher tout d'un coup
    
    # Création du robot
    robot = tl.Turtle()
    robot.shape(forme_robot)
    robot.color(couleur_robot)
    robot.shapesize(taille_robot, taille_robot)
    robot.speed(vitesse_robot)
    
    fenetre.update()
    log_message("Environnement graphique initialisé")
    print("Environnement initialisé")

def charger_salle(fichier_salle):
    global environnement
    
    try:
        with open(fichier_salle, 'r', encoding='utf-8') as f:
            for ligne in f:
                ligne = ligne.strip()

                # Ignorer lignes vides et commentaires
                if not ligne or ligne.startswith('#'):
                    continue
                
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
        dessinateur.speed(6)
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
    
    fenetre.update()
    log_message(f"{len(environnement['obstacles'])} obstacle(s) affiché(s)")
    print(f" {len(environnement['obstacles'])} obstacle(s) affiché(s)")

def afficher_murs():
    """
    Trace les murs de la salle (bordures)
    """
    echelle = environnement['echelle']
    longueur = environnement['longueur']
    largeur = environnement['largeur']
    
    dessinateur = tl.Turtle()
    dessinateur.hideturtle()
    dessinateur.speed(0)
    dessinateur.penup()
    
    # Aller au coin inférieur gauche
    dessinateur.goto(0, 0)
    dessinateur.pendown()
    dessinateur.color('black')
    dessinateur.width(3)
    
    # Tracer le rectangle de la salle
    for _ in range(2):
        dessinateur.forward(longueur * echelle)
        dessinateur.left(90)
        dessinateur.forward(largeur * echelle)
        dessinateur.left(90)
    
    fenetre.update()
    log_message(f"Murs de la salle affichés : {longueur}x{largeur}m")
    print(f"Murs de la salle tracés")

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
    rad = math.radians(etat_robot['orientation'])
    
    if recule:
        futur_x = etat_robot['x'] - distance * math.cos(rad)
        futur_y = etat_robot['y'] - distance * math.sin(rad)
    else:
        futur_x = etat_robot['x'] + distance * math.cos(rad)
        futur_y = etat_robot['y'] + distance * math.sin(rad)
    
    return futur_x, futur_y

def valider_deplacement(distance, recule=False):

    # Calculer la position finale
    futur_x, futur_y = calculer_position_future(distance, recule)
    
    # Échantillonner la trajectoire
    pas = 0.1  # 10 cm
    nb_points = max(int(distance / pas), 1)
    
    # Pour chaque point intermédiaire
    for i in range(nb_points + 1):  # +1 pour inclure le point final
        # Calculer la distance parcourue à ce point
        distance_partielle = (i / nb_points) * distance
        
        # Calculer la position à ce point
        x_test, y_test = calculer_position_future(distance_partielle, recule)
        
        # Vérifier s'il y a collision à ce point
        obstacle = verifier_collision(x_test, y_test)
        if obstacle:
            # Retourner la position où l'obstacle a été détecté
            return False, obstacle, (x_test, y_test)
    
    # Aucun obstacle sur toute la trajectoire
    return True, None, (futur_x, futur_y)

def contourner_obstacle(obstacle, distance_voulue):
    log_message("Tentative de contournement")
    print("\nContournement en cours...")
    
    # Calcul distance adaptée selon taille obstacle
    if obstacle['type'] == 'rectangle':
        decalage = obstacle['param1'] + 0.5
    elif obstacle['type'] == 'cercle':
        decalage = (obstacle['param1'] * 2) + 0.5
    else:
        decalage = 1.5
    
    print(f"Décalage : {decalage:.2f}m pour {obstacle['type']}")

    tourner_gauche(45)
    
    # Vérifie si on peut avancer latéralement
    if not valider_deplacement(decalage)[0]:
        print("Contournement impossible (obstacle sur la gauche)")
        tourner_droite(45)  # Revenir à l'orientation initiale
        return False
    
    avancer_direct(decalage)
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
    global etat_robot
    
    pixels = distance * environnement['echelle']
    robot.forward(pixels)
    
    # Mise à jour de la position
    rad = math.radians(etat_robot['orientation'])
    etat_robot['x'] += distance * math.cos(rad)
    etat_robot['y'] += distance * math.sin(rad)
    
    fenetre.update()


def reculer_direct(distance):
    global etat_robot
    
    pixels = distance * environnement['echelle']
    robot.backward(pixels)
    
    # Mise à jour de la position
    rad = math.radians(etat_robot['orientation'])
    etat_robot['x'] -= distance * math.cos(rad)
    etat_robot['y'] -= distance * math.sin(rad)
    
    fenetre.update()


def tourner_gauche(angle):
    global etat_robot
    
    robot.left(angle)
    etat_robot['orientation'] = (etat_robot['orientation'] + angle) % 360
    
    fenetre.update()
    log_message(f"Rotation gauche : {angle}° | Orientation : {etat_robot['orientation']:.1f}°")


def tourner_droite(angle):
    global etat_robot
    
    robot.right(angle)
    etat_robot['orientation'] = (etat_robot['orientation'] - angle) % 360
    
    fenetre.update()
    log_message(f"Rotation droite : {angle}° | Orientation : {etat_robot['orientation']:.1f}°")

def avancer(distance):
    valide, obstacle, (futur_x, futur_y) = valider_deplacement(distance)
    
    if obstacle:
        print(f"\n OBSTACLE DÉTECTÉ à ({futur_x:.2f}, {futur_y:.2f})")
        print(f"    Type : {obstacle['type']}")
        print("\n  Options :")
        print("    1. Contourner l'obstacle")
        print("    2. Annuler le déplacement")
        

        if contourner_obstacle(obstacle, distance):
            return True
        else:
            print("Déplacement impossible")
            log_message(f"Échec : obstacle non contournable")
            return False
    
    avancer_direct(distance)
    print(f"Avance {distance}m → Position ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})")
    log_message(f"Avance {distance}m → ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})")
    return True

def retour_origine():
    """Remet le robot à (0, 0) orientation 0° - Commande O,-,-,-"""
    global etat_robot
    
    ancienne_x = etat_robot['x']
    ancienne_y = etat_robot['y']
    ancienne_orientation = etat_robot['orientation']
    
    etat_robot['x'] = 0.0
    etat_robot['y'] = 0.0
    etat_robot['orientation'] = 0.0
    
    robot.penup()
    robot.goto(0, 0)
    robot.setheading(0)
    robot.pendown()
    fenetre.update()
    
    print(f"Retour à l'origine (0, 0)")
    print(f"  Depuis : ({ancienne_x:.2f}, {ancienne_y:.2f}) | {ancienne_orientation:.1f}°")
    log_message(f"Reset → (0.0, 0.0) | 0.0°")
    
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
    print(f"Recule {distance}m → Position ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})")
    log_message(f"Recule {distance}m → ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})")
    return True

def analyser_commande(ligne_commande):
    commande = ligne_commande.strip()
    if not commande:
        return True

    parties = commande.split(',')
    if len(parties) != 4:
        print(f"Format invalide : {commande}")
        print("   Format attendu : ACTION,DIRECTION,VALEUR,UNITE")
        return False
    
    action = parties[0].strip()
    direction = parties[1].strip()

    # Commande O (retour origine)
    if action == "O":
        return retour_origine()

    try:
        valeur = float(parties[2].strip())
    except ValueError:
        print(f"Valeur invalide : {parties[2]}")
        return False
    
    unite = parties[3].strip()

    unite_to_mult = {
        'm': 1,
        'c' : 0.01,
        'l': 0.1,
        'i': 0.025,
        'f': 0.31
    }
    
    valeur *= unite_to_mult.get(unite, 1)
    
    if action == "A":  # Avancer
        if direction == "-":
            return avancer(valeur)
        elif direction == "D":
            tourner_droite(90)
            print(f"Tourne droite 90° → Orientation {etat_robot['orientation']:.1f}°")
            return avancer(valeur)
        elif direction == "G":
            tourner_gauche(90)
            print(f"Tourne gauche 90° → Orientation {etat_robot['orientation']:.1f}°")
            return avancer(valeur)
        else:
            print(f"Direction inconnue : {direction}")
            return False
    
    elif action == "R":  # Reculer
        if direction == "-":
            return reculer(valeur)
        elif direction == "D":
            tourner_droite(90)
            print(f"Tourne droite 90° : Orientation {etat_robot['orientation']:.1f}°")
            return reculer(valeur)
        elif direction == "G":
            tourner_gauche(90)
            print(f"Tourne gauche 90° : Orientation {etat_robot['orientation']:.1f}°")
            return reculer(valeur)
        else:
            print(f"Direction inconnue : {direction}")
            return False
    
    elif action == "T":  # Tourner
        if direction == "G":
            tourner_gauche(valeur)
            print(f"Tourne gauche {valeur}° → Orientation {etat_robot['orientation']:.1f}°")
            return True
        elif direction == "D":
            tourner_droite(valeur)
            print(f"Tourne droite {valeur}° → Orientation {etat_robot['orientation']:.1f}°")
            return True
        else:
            print(f"Direction de rotation invalide : {direction}")
            return False
    
    else:
        print(f"Action inconnue : {action}")
        print("   Actions valides : A (avancer), R (reculer), T (tourner), O (origine)")
        return False

def executer_commandes(commandes):
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


def executer_fichier_commandes(fichier_commandes):
    try:
        with open(fichier_commandes, 'r', encoding='utf-8') as f:
            commandes = [ligne.strip() for ligne in f if ligne.strip()]
            executer_commandes(commandes)
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


def executer_commandes_reseau():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    adresse_serveur = ('localhost', 1234)
    sock.bind(adresse_serveur)

    print(f"Listening on {adresse_serveur[0]}:{adresse_serveur[1]}...")

    while True:
        data, addresse = sock.recvfrom(1024)

        if data:
            commande = data.decode('utf-8').strip()
            print(f"Reçu commande: {commande}")
            commandes = commande.split("\n")

            executer_commandes(commandes)


def main():
    print("SIMULATION ROBOT PFR1")
    
    initialiser_log("simulation_log.txt")
    
    initialiser_environnement(fichier_config)

    if not charger_salle(fichier_salle):
        print("Impossible de continuer sans fichier de salle")
        return
    
    afficher_obstacles()
    
    fenetre.tracer(1)  # Activer le tracé du robot

    print("\n Démarrage de l'exécution des commandes...\n")
    executer_commandes_reseau()
    
    print(f"\n Position finale : ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})")
    print(f"Orientation finale : {etat_robot['orientation']:.1f}°")

    fermer_log()
    
    print("\n" + "="*60)
    tl.exitonclick()
    #fenetre.bye()

if __name__ == "__main__":
    main()
