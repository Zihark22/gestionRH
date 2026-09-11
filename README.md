# 🏢 gestionRH - Mini ERP de Gestion des Ressources Humaines

Système modulaire **Client / Serveur** complet en **C++20 / Qt 6** pour l'administration et la gestion du personnel.  
La communication s'effectue au travers d'une **API REST synchrone/asynchrone** échangeant des payloads au format **JSON**.

---

## 🎯 Objectifs et Contraintes Techniques

- **Backend / Serveur :** Développé en **C++ moderne standard (STL)** pour garantir une portabilité maximale, une faible empreinte mémoire et une haute performance. Utilisation de **SQLite3** pour la persistance locale des données.
- **Frontend / Client :** Application Desktop moderne développée avec **Qt 6 (Widgets, QSS, QtNetwork)** intégrant des formulaires validés en temps réel et un tableau de bord à onglets.
- **Protocole REST :** Points d'entrée (endpoints) standardisés (GET, POST, PUT, DELETE) pour les opérations CRUD sur les collaborateurs.
- **Hot-Reload / Redémarrage à chaud :** Prise en charge du rechargement de la configuration serveur sans rupture d'exploitation.
- **Qualité de code :** Suite de tests unitaires automatisés avec **Google Test**.

---

## 📂 Architecture du Projet

```text
gestionRH/
├── client/HRmanagement         # Application IHM Qt 6 (HRmanagement)
│   ├── sources & headers       # Fenêtres (MainWindow), formulaires, etc
│   ├── style.qss               # Définit le style / la forme de l'app
│   └── config.ini              # Fichier de configuration de l'application
│
├── server/                     # Backend C++ STL & API REST
│   ├── config/                 # Fichiers de configuration du serveur (config.ini)
│   ├── data/                   # BDD SQLite (employees.db)
│   │   ├── db_sql_files/       # Scripts SQL pour mettre en place la BDD de test
│   ├── includes/               # Headers C++ (.hpp / .h)
│   ├── src/                    # Implémentation (.cpp)
│   │   ├── CMakeLists.txt      # Build du projet
│   ├── tests/                  # Tests unitaires Google Test
│   │   ├── CMakeLists.txt      # Build des tests
│   └── CMakeLists.txt          # Build du projet et des tests
│
├── tests/                      # Bac à sable (sandbox) et prototypage
├── diagrammes.excalidraw       # Diagrammes d'architecture, BDD et flux réseau (éditable sur Excalidraw)
└── README.md
```

---

## 🌐 Spécification de l'API REST (Aperçu)

| Méthode | Route | Description | Corps (JSON) |
| :--- | :--- | :--- | :--- |
| `GET` | `/api/employees` | Récupère la liste de tous les employés | *Aucun* |
| `POST` | `/api/employees` | Ajoute un nouvel employé | `{"firstname": "...", "lastname": "...", ...}` |
| `GET` | `/api/employees/{id}` | Récupère le détail d'un employé par son ID | *Aucun* |
| `PUT` | `/api/employees/{id}` | Met à jour les données d'un employé | `{"id": "...", "firstname": ...}` |
| `DELETE`| `/api/employees/{id}` | Supprime un collaborateur de la BDD | *Aucun* |
| `GET` | `/api/config` | Récupère la configuration du serveur | *Aucun* |
| `PUT` | `/api/config` | Modifie la configuration du serveur et déclenche le redémarrage pour appliquer la nouvelle config | `{"host": "...", "port": ..., "db_path":"..."}` |

---

## 🛠️ Prérequis et Installation (Linux / Ubuntu / Debian)

### 1. Dépendances système et outils de compilation
```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake gdb pkg-config \
                       sqlite3 libsqlite3-dev libgtest-dev
```

### 2. Dépendances Qt 6 (pour le client)
```bash
sudo apt-get install -y qt6-base-dev qt6-tools-dev libgl1-mesa-dev
```

---

## 🚀 Compilation et Exécution

### 🖥️ 1. Compilation du Serveur manuelle

Le projet utilise un dossier de build hors-source (*out-of-source*) :

```bash
cd server
mkdir -p build && cd build

# Configuration avec CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

# Compilation multi-thread
make -j$(nproc)

# Lancer le serveur
./ServerApp
```
> **PS :** Dans **VS-Code** il suffit d'utiliser l'extension **CMake Tools** pour tout compiler automatiquement (app+tests).


### 💻 2. Compilation du Client Qt
> **Choix IDE :** Le projet est prêt à l'emploi avec **Qt Creator**. L'utilisation de **VS-Code** peut nécessiter des modifications. 

---

## 🧪 Lancer la suite de Tests Unitaires (Google Test)

Les tests vérifient pour le moment :
- la cohérence des dates

Pour lancer tous les tests :
```bash
cd server/
./run_tests.sh
```

---

## 🗺️ Schémas et Diagrammes
Les diagrammes de flux et schémas d'architecture sont stockés dans le fichier **`diagrammes.excalidraw`**.  
Pour les visualiser ou les modifier :
1. Rendez-vous sur [Excalidraw](https://excalidraw.com/).
2. Glissez-déposez le fichier `.excalidraw` directement dans votre navigateur.