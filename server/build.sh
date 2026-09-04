#!/bin/bash

g++ src/connectDB.cpp -o build/connectDB -l sqlite3 -O
# ou pour le projet commande : mkdir -p build ; cd build && cmake .. && make -j$(nproc) && ./ServerApp

if [ $? -eq 0 ]; then
    echo -e "\033[32mCompilation Ok\033[0m"
else
    echo -e "\033[31mCompilation Error\033[0m"
fi