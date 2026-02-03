# Projet Fil Rouge - Partie 1

## Description

Le PFR est un projet de simulation de robot. Le projet permet de :
- Simuler les mouvements d'un robot (avancer, reculer, tourner, etc.)
- Traiter les commandes vocales en français et en anglais
- Traiter les commandes textuelles
- Gérer les images et le traitement d'images
- Maintenir un système de journalisation des opérations

Le système propose un menu interactif avec authentification et deux modes d'accès :
- **Mode utilisateur** : pour utiliser le robot
- **Mode administrateur** : pour voir les logs

## Prérequis

- **Linux/macOS** ou système compatible
- **GCC** (compilateur C)
- **Python 3.x** avec pip
- **Make**

## Installation et Utilisation

### Étape 1 : Créer l'environnement virtuel Python

```bash
make venv
```

Cette commande crée un environnement virtuel Python (`.venv`) et installe les dépendances Python nécessaires :
- `SpeechRecognition` : reconnaissance vocale
- `PyAudio` : gestion audio
- `gTTS` : synthèse vocale (texte vers parole)
- `pillow` : traitement d'images

### Étape 2 : Compiler le projet

```bash
make
```

Cette commande compile tous les fichiers source C et génère l'exécutable `main.out`.

### Étape 3 : Lancer l'application

```bash
./main.out
```

L'application démarre le menu principal et vous demande de choisir une langue (français ou anglais).

## Structure du projet

```
PFR/
├── config
│   ├── image.conf
│   ├── lang_en.conf
│   ├── lang_fr.conf
│   ├── salle_test.conf
│   └── simulation.conf
├── include
│   ├── config.h
│   ├── dictionnaire.h
│   ├── intercom.h
│   ├── log.h
│   ├── menu.h
│   ├── module_image.h
│   ├── objet.h
│   ├── parseur.h
│   ├── pile.h
│   └── vocal.h
├── src
│   ├── config.c
│   ├── creer_image.py
│   ├── demo_tres_primitive.txt
│   ├── dictionnaire.c
│   ├── intercom.c
│   ├── log.c
│   ├── main.c
│   ├── menu.c
│   ├── module_confirmer.py
│   ├── module_image.c
│   ├── module_tts.py
│   ├── parseur.c
│   ├── pile.c
│   ├── simulation.py
│   └── vocal.c
├── test
│   ├── test_dictionnaire.c
│   └── test_image.c
├── word_dictionnary
│   └── ...
├── logs.txt
├── simulation_log.txt
└── Makefile
```

## Commandes utiles

### Compiler uniquement
```bash
make
```

### Nettoyer les fichiers compilés
```bash
make clean
```

### Compiler les tests
```bash
make test_dico.out    # Test du dictionnaire
make test_image.out   # Test du module image
```

## Fonctionnalités principales

### 🎤 Commandes vocales
Le système reconnaît les commandes vocales en français et en anglais :
- **Actions élémentaires** : avancer, reculer, tourner, tourner à gauche, tourner à droite...
- **Actions macros** : carré, demi-tour, zigzag...

### 📝 Commandes textuelles
Permet de saisir des commandes au clavier pour contrôler le robot

### 🖼️ Traitement d'images
Analyse et traite les images pour extraire les positions et les attributs des objets dans la scène.
Un jeu d'images est diponible dans `images/`

### 📊 Journalisation
Tous les événements sont enregistrés dans des fichiers de journalisation pour le débogage et le suivi

## ⚙️ Configuration

Les fichiers de configuration dans le répertoire `config/` permettent de personnaliser :
- Les paramètres de simulation (dimensions, couleurs, vitesse du robot)
- Le traitement d'images
- Les dictionnaires pour chaque langue

## 🔑 Authentification

Le mode administrateur est protégé par un mot de passe. Lors du lancement, vous pouvez accéder à :
- **Menu utilisateur** : utilisation standard du robot
- **Menu administrateur** : accès aux paramètres avancés (nécessite le mot de passe)


