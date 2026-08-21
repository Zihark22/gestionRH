# 1. Version minimale de CMake requise
cmake_minimum_required(VERSION 3.16)

# 2. Nom du projet et langage
project(ServerApp VERSION 1.0.0 LANGUAGES CXX)

# 3. Spécification du standard C++ (C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 4. Activation de warnings stricts (recommandé en ingénierie logicielle C++)
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 5. Recherche des dépendances externes (ex: SQLite3)
find_package(SQLite3 REQUIRED)

# 2. Inclusion du dossier SQLite3 s'il n'est pas dans le path standard + ton dossier include/
include_directories(include ${SQLite3_INCLUDE_DIRS})


# 6. Inclusions des dossiers d'en-têtes (.hpp)
include_directories(include)

# 7. Collecte automatique des fichiers sources (.cpp)
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 8. Définition de l'exécutable
add_executable(ServerApp 
    # src/main.cpp
    ${SOURCES}
)

# 9. Liaison avec la bibliothèque SQLite3
# target_link_libraries(ServerApp PRIVATE SQLite3::SQLite3)
# 3. Utilisation de la variable ${SQLite3_LIBRARIES} à la place de SQLite3::SQLite3
target_link_libraries(ServerApp PRIVATE ${SQLite3_LIBRARIES})

# 10. Copie automatique du fichier config.ini vers le dossier de build
# Cela permet d'exécuter le binaire directement après la compilation sans erreur d'ouverture de config.
add_custom_command(
    TARGET ServerApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/config/config.ini"
            "$<TARGET_FILE_DIR:ServerApp>/config/config.ini"
    COMMENT "Copie du fichier config.ini dans le dossier de binaire"
)