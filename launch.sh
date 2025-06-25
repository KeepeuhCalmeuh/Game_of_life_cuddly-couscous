#!/bin/bash
set -e

# Build and run the project (Linux)
mkdir -p build
cd build
cmake ..
cmake --build .
./GLFW_Interface
