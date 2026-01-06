"""
PFR1
Module de simulation - Python Turtle
"""

import turtle as tl

class SimulationRobot:
    
    # explication self : (https://www.geeksforgeeks.org/python/self-in-python-class/)
    # lecture fichier config (https://zestedesavoir.com/tutoriels/2514/un-zeste-de-python/6-entrees-sorties/2-lecture-fichier/)
    
    def charger_config(self, fichier_conf):
        """Charge la configuration depuis un fichier texte"""
        config = {}
        try:
            with open(fichier_conf, 'r') as fichier:  # 'r' = mode lecture
                for ligne in fichier:
                    ligne = ligne.strip()
                    if ligne and not ligne.startswith('#'):
                        if '=' in ligne:
                            cle, valeur = ligne.split('=', 1)
                            config[cle.strip()] = valeur.strip()
            print("Fichier de configuration charge")
        except FileNotFoundError:
            print("Fichier de configuration", fichier_conf, "introuvable.")
        return config
    
    def __init__(self, fichier_conf="../config/simulation.txt"):
        """Initialise la simulation"""
        print("Lancement de la simulation")
        
        # Chargement configuration
        self.config = self.charger_config(fichier_conf)

        # Recuperation des parametres

        # Fenetre
        largeur_fenetre = int(self.config.get('WINDOW_WIDTH', 800))
        hauteur_fenetre = int(self.config.get('WINDOW_HEIGHT', 600))
        titre_fenetre = self.config.get('WINDOW_TITLE', 'Simulation Robot')
        couleur_fond = self.config.get('BACKGROUND_COLOR', 'white')
        
        # Echelle (metres -> pixels)
        self.echelle = int(self.config.get('SCALE', 100))

        # Robot
        forme_robot = self.config.get('ROBOT_SHAPE', 'triangle')
        couleur_robot = self.config.get('ROBOT_COLOR', 'blue')
        taille_robot = float(self.config.get('ROBOT_SIZE', 1.5))

        # Creation fenetre
        self.ecran = tl.Screen()
        self.ecran.setup(width=largeur_fenetre, height=hauteur_fenetre)
        self.ecran.title(titre_fenetre)
        self.ecran.bgcolor(couleur_fond)

        # Creation robot
        self.robot = tl.Turtle()
        self.robot.shape(forme_robot)
        self.robot.color(couleur_robot)
        self.robot.shapesize(taille_robot, taille_robot)

        # Position robot (en metres)
        self.pos_x = 0.0
        self.pos_y = 0.0
        self.orientation = 0.0  # en degres

        print("Simulation initialisee")

    # Fonctions de deplacement de base
    def avancer(self, distance):
        """Fait avancer le robot de 'distance' metres"""
        pixels = distance * self.echelle
        self.robot.forward(pixels)
        self.pos_x += distance
        print("Le robot a avance de", distance, "m - Position X :", self.pos_x, "m")

    def reculer(self, distance):
        """Fait reculer le robot de 'distance' metres"""
        pixels = distance * self.echelle
        self.robot.backward(pixels)
        self.pos_x -= distance
        print("Le robot a recule de", distance, "m - Position X :", self.pos_x, "m")

    def tourner_gauche(self, angle):
        """Fait tourner le robot a gauche de 'angle' degres"""
        self.robot.left(angle)
        self.orientation += angle
        print("Le robot a tourne a gauche de", angle, "degres - Orientation :", self.orientation, "degres")

    def tourner_droite(self, angle):
        """Fait tourner le robot a droite de 'angle' degres"""
        self.robot.right(angle)
        self.orientation -= angle
        print("Le robot a tourne a droite de", angle, "degres - Orientation :", self.orientation, "degres")
    
    # Lecture et execution des commandes
    def analyser_commande(self, ligne_commande):
        """Analyse et execute une ligne de commande"""
        
        # Nettoyage (enleve \n)
        commande = ligne_commande.strip()

        # Separation fonction et parametre
        nom_fonction = commande.split('(')[0]
        chaine_param = commande.split('(')[1].split(')')[0]

        # Convertir parametre en nombre
        parametre = float(chaine_param)

        # Execution de la commande
        if nom_fonction == "forward":
            self.avancer(parametre)
        elif nom_fonction == "backward":
            self.reculer(parametre)
        elif nom_fonction == "turn_left":
            self.tourner_gauche(parametre)
        elif nom_fonction == "turn_right":
            self.tourner_droite(parametre)
        else:
            print("Commande inconnue :", nom_fonction)
            return False
        return True
    
    def executer_fichier_commandes(self, fichier_commandes):
        """Lit et execute toutes les commandes d'un fichier"""
        with open(fichier_commandes, 'r') as fichier:
            commandes = fichier.readlines()

        print("Execution de", len(commandes), "commande(s)")

        for i, commande in enumerate(commandes, 1):
            print(i,"-", commande.strip())
            self.analyser_commande(commande)
            print()

        print("Toutes les commandes ont ete executees")


# Test
if __name__ == "__main__":
    sim = SimulationRobot()
    sim.executer_fichier_commandes("../scripts/commands_test.txt")
    input("Appuyez sur Entree pour fermer")