# 0. Configuration et build
mkdir -p build

# 1. Génération des fichiers de build
cmake -B build -S .

# 2. Compilation de tout le projet (Application + Tests)
cmake --build build

# 3. Lancer l'application
# ./build/src/ServerApp

# 4. Lancer tous les tests unitaires
ctest --test-dir build --output-on-failure
# ou directement : ./build/tests/unit_tests