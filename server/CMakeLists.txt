# 1. Version minimale et nom du projet
cmake_minimum_required(VERSION 3.16)
project(ServerApp VERSION 1.0.0 LANGUAGES CXX)

# 2. Standard C++ (C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 3. Warnings stricts
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 4. Recherche des dépendances
find_package(SQLite3 REQUIRED)
# 5. Recherche des dépendances externes (ex: SQLite3)
find_package(SQLite3 REQUIRED)

# 2. Inclusion du dossier SQLite3 s'il n'est pas dans le path standard + ton dossier include/
include_directories(include ${SQLite3_INCLUDE_DIRS})

# 5. Sous-dossier contenant le code source de l'application
add_subdirectory(src)

# 6. Gestion des Tests Unitaires
include(CTest) # Déclare automatiquement l'option BUILD_TESTING (activée par défaut)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()