# Simon

Le fameux jeu électronique Simon.
Le jeu n'est pas tout à fait complet puisqu'il ne comporte qu'un mode de jeu.
Le logiciel détermine une suite de couleur, montre une étape à la fois la suite à jouer, et suit les touches du joueur.
Dès que le joueur se trompe, le jeu s'arrête. Si le joueur arrive au bout de la suite, les lumières clignotent.

Projet simple qui ne nécessite que le matériel suivant :
- Arduino uno
- Breadboard
- LEDs
- Boutons
- Buzzer

Côté logiciel, rien de particulier :
- Gestion des LEDs, boutons, buzzer
- Génération d'une suite de touches
- Suivi



# Bombe

Ce projet est utilisé dans le cadre d'un escape game.
Il s'agit en apparence d'un simple minuteur. Cependant, avec l'aide du module, la bombe peut être désamorcé.

Matériel nécessaire :
- Ecran LCD
- Potentiometre
- Interrupteur
- Buzzer

Côté logiciel, ce qui est intéressant à voir :
- Affichage sur écran LCD
- Communication filaire avec un autre arduino


# Module

Le module sert à désamorcer la bombe utilisée dans l'escape game.

Matériel :
- Module RFID (125khz)
- Clavier

Côté logiciel :
- Lecture du clavier numérique
- Communication filaire avec un autre arduino
- Lecture de badge RFID


# Bombe / module

La bombe affiche un minuteur. Pour la désamorcer, il faut :
- Connecter le module à la bombe
- Passer la bombe en mode 
- Activer le module avec un badge RFID
- Saisir la combinaison



