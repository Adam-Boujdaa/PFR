"""
PFR1
Module de simulation - Python Turtle
"""

import turtle as tl

class RobotSimulation:
    
    # explication self : # (https://www.geeksforgeeks.org/python/self-in-python-class/)
    # lecture fichier config (https://zestedesavoir.com/tutoriels/2514/un-zeste-de-python/6-entrees-sorties/2-lecture-fichier/)
    def load_config(self, fichier_conf):
        config = {}
        try:
            with open(fichier_conf, 'r') as file: # 'r' = mode read
                for line in file:
                    line = line.strip()
                    if line and not line.startswith('#'):
                        if '=' in line:
                            key, value = line.split('=',1)
                            config[key.strip()] = value.strip()
            print("Fichier de configuration chargé")
        except FileNotFoundError:
            print("Fichier de configuration ", fichier_conf, "introuvable.")
        return config
    def __init__(self, fichier_conf="config/simulation.txt"):
        print("Lancement de la simulation")
        
        # chargement config
        self.config = self.load_config(fichier_conf)

        # récupération paramètres

        # fenêtre
        f_width = int(self.config.get('WINDOW_WIDTH', 800))
        f_height = int(self.config.get('WINDOW_HEIGHT', 600))
        f_title = self.config.get('WINDOW_TITLE', 'Simulation Robot')
        bg_color = self.config.get('BACKGROUND_COLOR', 'white')
        
        # échelle
        self.scale = int(self.config.get('SCALE', 100))

        # robot
        r_shape = self.config.get('ROBOT_SHAPE', 'triangle')
        r_color = self.config.get('ROBOT_COLOR', 'blue')
        r_size = float(self.config.get('ROBOT_SIZE', 1.5))

        # création fenêtre
        self.screen = tl.Screen()
        self.screen.setup(width=f_width, height=f_height)
        self.screen.title(f_title)
        self.screen.bgcolor(bg_color)

        # création orbot
        self.robot = tl.Turtle()
        self.robot.shape(r_shape)
        self.robot.color(r_color)
        self.robot.shapesize(r_size, r_size)

        # position robot
        self.x = 0.0
        self.y = 0.0
        self.alpha = 0.0

        print("Simulation initialisée")

    # fonctions de base
    def forward(self, distance):
        pixels = distance * self.scale
        self.robot.forward(pixels)
        self.x += distance
        print("Le robot a avancé de ", distance,"m - Position X : ", self.x)

    def backward(self, distance):
        pixels = distance * self.scale
        self.robot.backward(pixels)
        self.x -= distance
        print("Le robot a reculer de ", distance, "m - Position X : ", self.x,"m")

    def turn_left(self, angle):
        self.robot.left(angle)
        self.alpha += angle
        print ("Le robot a tourner à gauche de ", angle, "° - Orientation : ", self.alpha,"°")

    def turn_right(self, angle):
        self.robot.right(angle)
        self.alpha -= angle
        print ("Le robot a tourner à droite de ", angle, "° - Orientation : ", self.alpha,"°")
    
# test
if __name__ == "__main__":
    sim = RobotSimulation()

    # test mouvements
    sim.forward(2)
    sim.turn_right(90)
    sim.forward(1.5)
    sim.turn_left(45)
    sim.backward(1)

    input("Appuyez sur Entrée pour fermer")