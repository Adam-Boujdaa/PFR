"""
PFR1
Module de simulation - Python Turtle
"""

import turtle as tl

class RobotSimulation:
    
    def __init__(self):
        print("Lancement de la simulation")
        
        # fenêtre
        self.screen = tl.Screen()
        self.screen.setup(width=800, height=600)
        self.screen.title("Simulation Robot PFR1")
        
        # robot
        self.robot = tl.Turtle()
        self.robot.shape("triangle")
        self.robot.color("blue")

        # position robot
        self.x = 0.0
        self.y = 0.0
        self.alpha = 0.0

        # echelle
        self.scale = 100
        
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