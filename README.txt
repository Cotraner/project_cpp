==========================================
          Albert's Adventure
==========================================

📌 Description :
----------------
Albert's Adventure est un mini-RPG en 2D développé en C++ avec Qt par Clément Robin et Louis Marvillet.
Le joueur explore une carte peuplée d’ennemis, combat avec des attaques au corps à corps ou à distance, et fait apparaître un boss final une fois tous les sbires éliminés.

Contrôles :
--------------
- Z / ↑ : Déplacement vers le haut
- Q / ← : Déplacement vers la gauche
- S / ↓ : Déplacement vers le bas
- D / → : Déplacement vers la droite
- Clic gauche : Attaque à l’épée (directionnelle)
- Clic droit : Lancer de Molotov (à distance)
- Échap : Quitter le jeu en cours

Carte :
----------
Le jeu utilise une carte JSON (`bonmap.json`) avec :
- Décors tuilés
- Zones de collision invisibles
- Calque "arbres" affiché par-dessus les entités

Fonctionnalités :
---------------------
- Ennemis basiques : poursuite et attaque du joueur
- Boss qui apparaît après la mort de tous les sbires
- Projectiles circulaires lancés par le boss
- Gestion des collisions avec les murs et attaques
- Score mis à jour dynamiquement
- Sauvegarde automatique des scores dans `score.txt`
- Les ennemis réaparaissent après avoir tué le boss (ils deviennent plus forts)

Score :
----------
- Le score est enregistré en fin de partie.
- Il est comparé aux scores existants.
- Le fichier `score.txt` contient la liste triée (score le plus élevé en haut).

Compilation :
-----------------
Projet Qt (Widgets)
- Qt version : 5.15+ recommandé
- Créer un projet sur CLion
- Ajouter les fichiers sources et ressources nécessaires
- Chargé votre CMake
- Vérifier que les fichiers `.json`, `.gif`, `.ttf` sont bien présents dans les bons dossiers (`map/`, `anim/`, `fonts/`)
- Compiler et exécuter le projet

Ressources à prévoir :
--------------------------
- `../map/bonmap.json`
- `../anim/*.gif` (animations des personnages, armes, etc.)
- `../fonts/game_over.ttf`

---

Développé dans le cadre d’un mini-projet étudiant — ISEN.