# Web-multiserver
Création d'un serveur avec arduino UNO, la platine W5100 ethernet et une carte SD.

## Fonctionnement
Le code est assez simple c'est juste un serveur qui écoute des requêtes et renvoie ce qu'il faut.
La particularité de ce serveur est le stockage des informations: elles sont sur une carte SD ce qui offre une plus grande capacité et une meilleur ergonomie que de téléverser le code avec les pages web sur l'arduino.
Il y a la possibilité d'utiliser les sous dossier: http://--IP--/sousdossier/fichier

## Problèmes
Je n'arrive pas à lire les pages html depuis la carte SD directement. Le seul moyen est de transformer le fichier html en fichier texte (cela n'influe en aucun cas sur la page web, le code prend en compte cette erreur) et de ne mettre aucune extention sur le lien internet: http://--IP/fichier où fichier est le nom de votre fichier sans extension.
