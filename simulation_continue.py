"""
PFR1 - Module de simulation robot
"""
import turtle as tl
import math
import time
import sys
from datetime import datetime

# VARIABLES GLOBALES

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
    'echelle': 100,  # 1 mètre = 100 pixels
    'longueur': 8,   # en mètres
    'largeur': 6,    # en mètres
    'obstacles': []
}

# Objets Turtle
fenetre = None
robot = None

# Fichier de log
fichier_log = None

# Fichiers de synchronisation (module traitement texte)
FICHIER_CONFIRMATION = "config/confirmation.txt"
FICHIER_RESULTAT = "config/resultat.txt"
FICHIER_COMMANDES = "config/commande_actuelle.txt"

# FICHIER CONFIRMATION

def lire_confirmation():
    try:
        with open(FICHIER_CONFIRMATION, 'r', encoding='utf-8') as f:
            return f.read().strip()
    except FileNotFoundError:
        return "0"

def ecrire_confirmation(message):
    try:
        with open(FICHIER_CONFIRMATION, 'w', encoding='utf-8') as f:
            f.write(message)
    except Exception as e:
        print(f"Erreur écriture confirmation : {e}")

def attendre_confirmation():
    print("Attente de l'autorisation de démarrage")
    while True:
        etat = lire_confirmation()
        if etat == "0":
            print("Autorisation reçue, démarrage de la simulation")
            log_message("Autorisation de démarrage reçue")
            break
        elif etat == "COMMANDE_EN_COURS_DE_TRAITEMENT":
            print("Commande en cours de traitement, attente...")
            time.sleep(0.5)
        else:
            break

def ecrire_resultat(success, message=""):
    try:
        with open(FICHIER_RESULTAT, 'w', encoding='utf-8') as f:
            if success:
                f.write("COMMANDE_SIMULEE\n")
                if message:
                    f.write(message + "\n")
            else:
                f.write("COMMANDE_IMPOSSIBLE\n")
                if message:
                    f.write(message + "\n")
        log_message(f"Résultat écrit : {'SUCCÈS' if success else 'ÉCHEC'}")
    except Exception as e:
        print(f"Erreur écriture résultat : {e}")

# CONVERSION D'UNITÉS

def convertir_distance(valeur, unite):
    unite = unite.lower().strip()
    
    if unite == "m" or unite == "-":
        return valeur
    elif unite == "c":
        return valeur / 100.0
    elif unite == "l":
        return valeur / 10.0
    elif unite == "f":
        return valeur * 0.3048
    elif unite == "i":
        return valeur * 0.0254
    else:
        print(f"Unité inconnue : {unite}, utilisation du mètre par défaut")
        return valeur

def convertir_angle(valeur, unite):
    """convertit angle (radiant) vers degrés si nécessaire"""
    unite = unite.lower().strip()
    
    if unite == "deg" or unite == "-":
        return valeur
    elif unite == "rad":
        return math.degrees(valeur)
    else:
        print(f"Unité d'angle inconnue : {unite}, utilisation des degrés par défaut")
        return valeur

# CHARGEMENT DE LA CONFIGURATION

def charger_configuration(fichier_conf):
    global config
    
    config = {}
    try:
        with open(fichier_conf, 'r', encoding='utf-8') as f:
            for ligne in f:
                ligne = ligne.strip()
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

# INITIALISATION DE L'ENVIRONNEMENT GRAPHIQUE

def initialiser_environnement(fichier_conf):
    global fenetre, robot, environnement
    
    if not charger_configuration(fichier_conf):
        print("Utilisation des paramètres par défaut")
    
    largeur_fenetre = int(config.get('WINDOW_WIDTH', 800))
    hauteur_fenetre = int(config.get('WINDOW_HEIGHT', 600))
    titre = config.get('WINDOW_TITLE', 'Simulation Robot PFR1 - Mode Continu')
    couleur_fond = config.get('BACKGROUND_COLOR', 'white')
    
    environnement['echelle'] = int(config.get('SCALE', 100))
    
    forme_robot = config.get('ROBOT_SHAPE', 'triangle')
    couleur_robot = config.get('ROBOT_COLOR', 'blue')
    taille_robot = float(config.get('ROBOT_SIZE', 1.5))
    vitesse_robot = int(config.get('ROBOT_SPEED', 6))
    
    fenetre = tl.Screen()
    fenetre.setup(width=largeur_fenetre, height=hauteur_fenetre)
    fenetre.title(titre)
    fenetre.bgcolor(couleur_fond)
    fenetre.tracer(0)
    
    robot = tl.Turtle()
    robot.shape(forme_robot)
    robot.color(couleur_robot)
    robot.shapesize(taille_robot, taille_robot)
    robot.speed(vitesse_robot)
    
    fenetre.update()
    log_message("Environnement graphique initialisé")
    print("Environnement initialisé")

# CHARGEMENT DES OBSTACLES

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

# AFFICHAGE DES OBSTACLES

def afficher_obstacles():
    echelle = environnement['echelle']
    
    for obstacle in environnement['obstacles']:
        dessinateur = tl.Turtle()
        dessinateur.hideturtle()
        dessinateur.speed(6)
        dessinateur.penup()
        
        if obstacle['type'] == 'rectangle':
            largeur = obstacle['param1']
            hauteur = obstacle['param2']
            x = obstacle['x']
            y = obstacle['y']
            
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
    print(f"{len(environnement['obstacles'])} obstacle(s) affiché(s)")

# DÉTECTION DE COLLISION

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

# CALCUL DE POSITION FUTURE

def calculer_position_future(distance, recule=False):
    rad = math.radians(etat_robot['orientation'])
    
    if recule:
        futur_x = etat_robot['x'] - distance * math.cos(rad)
        futur_y = etat_robot['y'] - distance * math.sin(rad)
    else:
        futur_x = etat_robot['x'] + distance * math.cos(rad)
        futur_y = etat_robot['y'] + distance * math.sin(rad)
    
    return futur_x, futur_y

# VALIDATION DE COMMANDE

def valider_deplacement(distance, recule=False):
    futur_x, futur_y = calculer_position_future(distance, recule)
    obstacle = verifier_collision(futur_x, futur_y)
    
    if obstacle:
        return False, obstacle, (futur_x, futur_y)
    else:
        return True, None, (futur_x, futur_y)

# CONTOURNEMENT D'OBSTACLE

def contourner_obstacle(obstacle, distance_voulue):
    log_message("Tentative de contournement")
    print("\nContournement en cours...")
    
    # Calcul de la distance de décalage selon le type et la taille de l'obstacle
    if obstacle['type'] == 'rectangle':
        # Largeur du rectangle + marge de sécurité de 0.5m
        decalage = obstacle['param1'] + 0.5
    elif obstacle['type'] == 'cercle':
        # Diamètre du cercle + marge de sécurité de 0.5m
        decalage = (obstacle['param1'] * 2) + 0.5
    else:
        # Fallback si type inconnu
        decalage = 1.5
    
    print(f"Décalage calculé : {decalage:.2f}m pour obstacle de type {obstacle['type']}")
    log_message(f"Décalage de contournement : {decalage:.2f}m")

    tourner_gauche(45)
    
    # Vérifie si on peut se décaler latéralement
    if not valider_deplacement(decalage)[0]:
        print("Contournement impossible (obstacle sur la gauche)")
        tourner_droite(45)  # Revenir à l'orientation initiale
        return False
    
    avancer_direct(decalage)
    tourner_droite(45)

    # Vérifie si on peut continuer la trajectoire voulue
    if valider_deplacement(distance_voulue)[0]:
        avancer_direct(distance_voulue)
        print("Contournement réussi")
        log_message("Contournement réussi")
        return True
    else:
        print("Impossible de continuer après contournement")
        return False

# DÉPLACEMENTS DE BASE

def avancer_direct(distance):
    global etat_robot
    
    pixels = distance * environnement['echelle']
    robot.forward(pixels)
    
    rad = math.radians(etat_robot['orientation'])
    etat_robot['x'] += distance * math.cos(rad)
    etat_robot['y'] += distance * math.sin(rad)
    
    fenetre.update()

def reculer_direct(distance):
    global etat_robot
    
    pixels = distance * environnement['echelle']
    robot.backward(pixels)
    
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

# DÉPLACEMENTS AVEC VALIDATION

def avancer(distance):
    _, obstacle, (futur_x, futur_y) = valider_deplacement(distance)
    
    if obstacle:
        msg = f"OBSTACLE DÉTECTÉ à ({futur_x:.2f}, {futur_y:.2f}) - Type : {obstacle['type']}"
        print(f"\n{msg}")
        log_message(msg)
        return False, msg
    
    avancer_direct(distance)
    msg = f"Avance {distance}m → Position ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})"
    print(msg)
    log_message(msg)
    return True, ""

def reculer(distance):
    _, obstacle, (futur_x, futur_y) = valider_deplacement(distance, recule=True)
    
    if obstacle:
        msg = f"OBSTACLE DÉTECTÉ à ({futur_x:.2f}, {futur_y:.2f}) - Impossible de reculer"
        print(f"\n{msg}")
        log_message(msg)
        return False, msg
    
    reculer_direct(distance)
    msg = f"Recule {distance}m → Position ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})"
    print(msg)
    log_message(msg)
    return True, ""

# ANALYSE DES COMMANDES

def analyser_commande(ligne_commande):
    """
    Analyse et exécute une commande au format : ACTION,DIRECTION,VALEUR,UNITE
    """
    commande = ligne_commande.strip()
    if not commande:
        return True, ""

    parties = commande.split(',')
    if len(parties) != 4:
        msg = f"Format invalide : {commande}\nFormat attendu : ACTION,DIRECTION,VALEUR,UNITE"
        print(msg)
        return False, msg
    
    action = parties[0].strip()
    direction = parties[1].strip()
    unite = parties[3].strip()
    
    try:
        valeur_brute = float(parties[2].strip())
    except ValueError:
        msg = f"Valeur invalide : {parties[2]}"
        print(msg)
        return False, msg
    
    # Conversion de l'unité
    if action in ["A", "R"]:
        valeur = convertir_distance(valeur_brute, unite)
        print(f"Conversion : {valeur_brute} {unite} → {valeur:.3f} m")
    elif action == "T":
        valeur = convertir_angle(valeur_brute, unite)
        print(f"Conversion : {valeur_brute} {unite} → {valeur:.3f} deg")
    else:
        valeur = valeur_brute
    
    # Exécution selon l'action
    if action == "A":  # Avancer
        if direction == "-":
            return avancer(valeur)
        elif direction == "D":
            # Sauvegarde l'orientation au cas où ça échoue
            orientation_initiale = etat_robot['orientation']
            tourner_droite(90)
            print(f"Tourne droite 90° → Orientation {etat_robot['orientation']:.1f}°")
            success, msg = avancer(valeur)
            if not success:
                # Annule la rotation si l'avancement a échoué
                etat_robot['orientation'] = orientation_initiale
                robot.setheading(orientation_initiale)
                fenetre.update()
                print(f"Rotation annulée, retour à {orientation_initiale:.1f}°")
                log_message(f"Rotation annulée, retour à {orientation_initiale:.1f}°")
            return success, msg
        elif direction == "G":
            # Sauvegarde l'orientation au cas où ça échoue
            orientation_initiale = etat_robot['orientation']
            tourner_gauche(90)
            print(f"Tourne gauche 90° → Orientation {etat_robot['orientation']:.1f}°")
            success, msg = avancer(valeur)
            if not success:
                # Annule la rotation si l'avancement a échoué
                etat_robot['orientation'] = orientation_initiale
                robot.setheading(orientation_initiale)
                fenetre.update()
                print(f"Rotation annulée, retour à {orientation_initiale:.1f}°")
                log_message(f"Rotation annulée, retour à {orientation_initiale:.1f}°")
            return success, msg
        else:
            msg = f"Direction inconnue : {direction}"
            print(msg)
            return False, msg
    
    elif action == "R":  # Reculer
        if direction == "-":
            return reculer(valeur)
        elif direction == "D":
            # Sauvegarde l'orientation au cas où ça échoue
            orientation_initiale = etat_robot['orientation']
            tourner_droite(90)
            print(f"Tourne droite 90°")
            success, msg = reculer(valeur)
            if not success:
                # Annule la rotation si le recul a échoué
                etat_robot['orientation'] = orientation_initiale
                robot.setheading(orientation_initiale)
                fenetre.update()
                print(f"Rotation annulée, retour à {orientation_initiale:.1f}°")
                log_message(f"Rotation annulée, retour à {orientation_initiale:.1f}°")
            return success, msg
        elif direction == "G":
            # Sauvegarde l'orientation au cas où ça échoue
            orientation_initiale = etat_robot['orientation']
            tourner_gauche(90)
            print(f"Tourne gauche 90°")
            success, msg = reculer(valeur)
            if not success:
                # Annule la rotation si le recul a échoué
                etat_robot['orientation'] = orientation_initiale
                robot.setheading(orientation_initiale)
                fenetre.update()
                print(f"Rotation annulée, retour à {orientation_initiale:.1f}°")
                log_message(f"Rotation annulée, retour à {orientation_initiale:.1f}°")
            return success, msg
        else:
            msg = f"Direction inconnue : {direction}"
            print(msg)
            return False, msg
    
    elif action == "T":  # Tourner
        if direction == "G":
            tourner_gauche(valeur)
            print(f"Tourne gauche {valeur}° → Orientation {etat_robot['orientation']:.1f}°")
            return True, ""
        elif direction == "D":
            tourner_droite(valeur)
            print(f"Tourne droite {valeur}° → Orientation {etat_robot['orientation']:.1f}°")
            return True, ""
        else:
            msg = f"Direction de rotation invalide : {direction}"
            print(msg)
            return False, msg
    
    else:
        msg = f"Action inconnue : {action}"
        print(msg)
        return False, msg

# MODE CONTINU

def mode_continu():
    print("En attente de commandes du module C")
    print("États possibles : EXECUTE, QUIT")
    log_message("Mode continu démarré")
    
    fenetre.tracer(1)  # Active l'animation
    
    while True:
        # Lit l'état du fichier confirmation
        etat = lire_confirmation()
        
        if etat == "EXECUTE":
            print("\n-> Nouvelle commande détectée")
            log_message("Nouvelle commande détectée")
            
            # Lit la commande depuis le fichier
            try:
                with open(FICHIER_COMMANDES, 'r', encoding='utf-8') as f:
                    commande = f.read().strip()
                
                print(f"Commande : {commande}")
                
                # Exécute la commande
                success, msg = analyser_commande(commande)
                
                # Écrit le résultat
                ecrire_resultat(success, msg)
                
                # Affiche la position actuelle
                print(f"Position actuelle : ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})")
                print(f"Orientation : {etat_robot['orientation']:.1f}°")
                
                # Signal au module C que c'est terminé
                ecrire_confirmation("0")
                print("→ Commande terminée, en attente...")
                
            except FileNotFoundError:
                msg = f"Fichier {FICHIER_COMMANDES} introuvable"
                print(f"Erreur : {msg}")
                ecrire_resultat(False, msg)
                ecrire_confirmation("0")
            except Exception as e:
                msg = f"Erreur : {e}"
                print(msg)
                ecrire_resultat(False, msg)
                ecrire_confirmation("0")
        
        elif etat == "QUIT":
            print("\n→ Commande d'arrêt reçue")
            log_message("Arrêt du mode continu")
            break
        
        # Pause avant la prochaine vérification
        time.sleep(0.2)
    
    print("\n=== FIN DU MODE CONTINU ===")

# SYSTÈME DE LOGGING

def initialiser_log(nom_fichier="logs/simulation_log.txt"):
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
        fichier_log.write(f"\nSESSION TERMINÉE : {timestamp}\n")
        fichier_log.write(f"{'='*60}\n")
        fichier_log.close()

# FONCTION PRINCIPALE

def main():
    print("\n" + "="*60)
    print("  SIMULATION ROBOT PFR1 - MODE CONTINU")
    print("="*60 + "\n")
    
    # Vérification des arguments
    if len(sys.argv) != 3:
        print("Usage: python3 simulation_continue.py <config> <salle>")
        print("\nExemple:")
        print("  python3 simulation_continue.py config/simulation.txt config/salle_test.txt")
        return 1
    
    # Récupération des arguments
    fichier_config = sys.argv[1]
    fichier_salle = sys.argv[2]
    
    # Initialisation du log
    initialiser_log("logs/simulation_log.txt")
    
    # Attendre autorisation
    attendre_confirmation()
    
    # Initialiser l'environnement
    initialiser_environnement(fichier_config)

    # Charger la salle
    if not charger_salle(fichier_salle):
        print("Impossible de continuer sans fichier de salle")
        ecrire_resultat(False, "Fichier de salle introuvable")
        fermer_log()
        return 1
    
    # Afficher les obstacles
    afficher_obstacles()

    # LANCER LE MODE CONTINU
    # La fenêtre reste ouverte, attend les commandes
    mode_continu()

    # Afficher l'état final
    print(f"\nPosition finale : ({etat_robot['x']:.2f}, {etat_robot['y']:.2f})")
    print(f"Orientation finale : {etat_robot['orientation']:.1f}°")
    
    # Fermer le log
    fermer_log()
    
    # Fermer la fenêtre
    print("\n" + "="*60)
    print("Fermeture de la simulation...")
    fenetre.bye()
    
    return 0

if __name__ == "__main__":
    sys.exit(main())
