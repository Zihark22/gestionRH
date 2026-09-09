# 1. Lister tous les fichiers .cpp de vos classes MÉTIER (SANS main.cpp)
# Conseil : listez-les explicitement au fur et à mesure pour éviter les bugs de cache CMake
set(SERVER_CORE_SOURCES
    date.cpp
    apiserver.cpp
    dbhandler.cpp
    employees.cpp
    iniparser.cpp
    server.cpp
)

# 2. Création de la bibliothèque contenant toute la logique métier
add_library(ServerLib STATIC ${SERVER_CORE_SOURCES})

# 3. Association des headers et des dépendances à la bibliothèque
target_include_directories(ServerLib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${SQLite3_INCLUDE_DIRS}
)
target_link_libraries(ServerLib PUBLIC ${SQLite3_LIBRARIES})

# 4. Création du binaire applicatif final
add_executable(ServerApp main.cpp)
target_link_libraries(ServerApp PRIVATE ServerLib)

# 5. Copie du fichier de configuration à la compilation
add_custom_command(
    TARGET ServerApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/config/config.ini"
            "$<TARGET_FILE_DIR:ServerApp>/config/config.ini"
    COMMENT "Copie de config.ini vers le repertoire de sortie"
)

# 6. Copie de la DB à la compilation
add_custom_command(
    TARGET ServerApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/data/employees.db"
            "$<TARGET_FILE_DIR:ServerApp>/data/employees.db"
    COMMENT "Copie de la base de données vers le repertoire de sortie"
)