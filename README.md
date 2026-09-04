# gestionRH
Projet client/serveur en Qt6/C++ pour gérer une base de données d'employés avec une API REST  

## Etapes du projet
1. Mettre en place la base de données
2. Créer en C++ la partie API (traitement des requêtes)
3. Créer l'IHM côté client (visuel + requêtes API)
4. Intégration des partiesyoutube



## Installer les librairies sous Linux
Installer SQLite, outils de dev C++, cmake :
> sudo apt-get install sqlite3 libsqlite3-dev cmake build-essential

Vérifier version cmake :
> cmake --version

Installer Qt6 :
> sudo apt-get install build-essential libgl1-mesa-dev

Installer GoogleTest (ou direct via CMake) :
> sudo apt install libgtest-dev

### Etapes de compilation CMake

1. Se placer à la racine du projet serveur
> cd path/to/server

2. Créer un dossier de build hors-source (Out-of-source build)
> mkdir build && cd build

3. Générer les fichiers Makefiles avec CMake
> cmake ..

4. Lancer la compilation (utilise tous les cœurs du processeur)
> make -j$(nproc)

5. Lancer l'exécutable du serveur
> ./ServerApp
