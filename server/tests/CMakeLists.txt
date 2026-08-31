# 1. Récupération / Détection de GoogleTest
# Option A : Si vous l'avez installé via `apt install libgtest-dev`
find_package(GTest REQUIRED)

# (Option B alternative : Si vous préférez FetchContent, remplacez find_package par FetchContent)

# 2. Exécutable de test (listez vos fichiers de tests unitaires)
add_executable(unit_tests 
    DateTest.cpp
    # DatabaseTest.cpp
)

# 3. Liaison avec votre code (ServerLib) et GoogleTest
target_link_libraries(unit_tests PRIVATE 
    ServerLib
    GTest::gtest_main
)

# 4. Enregistrement automatique des tests dans CTest
include(GoogleTest)
gtest_discover_tests(unit_tests)