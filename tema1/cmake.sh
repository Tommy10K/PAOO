#!/bin/bash

set -e

BUILD_DIR="build"

echo "Configurare proiect CMake în folderul '$BUILD_DIR'..."


if [ "$1" == "--clean" ]; then
    echo "Stergem build ul vechi"
    rm -rf "$BUILD_DIR"
fi

if [ ! -d "$BUILD_DIR" ]; then
    mkdir "$BUILD_DIR"
    echo "Directorul '$BUILD_DIR' a fost creat."
fi

cmake -S . -B "$BUILD_DIR"
cmake --build "$BUILD_DIR" --target run -- -j$(nproc)

echo ""
echo "Build complet!"
echo "Executabilul se gaseste în: $BUILD_DIR/"