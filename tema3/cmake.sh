#!/bin/bash

set -e

BUILD_DIR="build"

echo "=== Configurare si Rulare Proiect Tema 3 ==="

if [ "$1" == "--clean" ]; then
    echo "[Info] Stergem folderul de build vechi..."
    rm -rf "$BUILD_DIR"
fi

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
    echo "[Info] Directorul '$BUILD_DIR' a fost creat."
fi

echo "[Info] Configurare CMake..."
cmake -S . -B "$BUILD_DIR"

echo "[Info] Compilare si Executie..."
cmake --build "$BUILD_DIR" --target run -- -j$(nproc)

echo ""
echo "=== Executie Incheiata ==="