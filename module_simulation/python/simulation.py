"""
Module de simulation du robot (Python Turtle)
"""

import turtle as tl
import configparser as confp
import os

class RobotSimulation:
    """Définition de l'objet RobotSimulation (UML)"""
    def __init__(self, config_file="../PFR/config/simulation.conf"):
        
        #Chargeement de la conf
        self.config = self._load_config(config_file)

        #Récupération des paramètres

        #Fenêtre
        w_width = self.config.getint('WINDOW', 'witdh')
        w_height = self.config.getint('WINDOW', 'height')
        w_title = self.config.get('WINDOW', 'title')
        bg_color = self.config.get('WINDOW', 'background_color')

        #Echelle
        self.scale = self.config.getint('SCALE', 'm_to_p')

        #Robot
        rb_shape = self.config.get('ROBOT', 'shape')
        rb_color = self.config.get('ROBOT', 'color')
        rb_size = self.config.getfloat('ROBOT', 'size')

        #Création des éléments

        #Création de la fenêtre
        self.screen = turtle.Screen()
        self.screen.setup(width=w_width, height=w_height)
        self.screen.title(w_title)
        self.screen.bgcolor(bg_color)

        #Création du robot
        self.robot = turtle.Turtle()
        self.robot.shape(rb_shape)
        self.robot.color(rb_color)
        self.robot.shapesize(rb_size, rb_size)

        #Position de départ
        self.X = 0
        self.y = 0
        self.theta = 0 #angle

        print("Simulation initialisée")
        print("Echelle : 1m = {self.scale} pixels")

        def _load_config(self, config_file):
            #Permet de charger le fichier de configuration
            if not os.path.exists(config_file):
                raise FileNotFoundError(f"Fichier de configuration introuvable: {config_file}")

            config = confp.ConfigParser()
            config.read(config_file)
            print(f"La configuration a bien été chargée depuis : {config_file}")
            return config

        #Commande de base

        #Avance
        def forward(self, distance):
            pixels = distance * self.scale
            self.robot.forward(pixels)
            print(f"Robot avance de {distance}m")

        #Tests
        if __name__ == "__main__":
            print("Démarrage de la simulation")
            sim = RobotSimulation()

            #Test1 : Déplacement de base
            sim.forward(2)

            input("Touche 'Entrée' pour fermer")
