#!/bin/bash

CXXFLAGS=$(pkg-config --cflags raylib)
LDFLAGS=$(pkg-config --libs raylib)

g++ -std=c++20 -Ofast -g -o catjobs main.cpp $CXXFLAGS $LDFLAGS -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo
if [ $? -eq 0 ]; then
    echo "Compilation successful! Running..."
    ./catjobs
else
    echo "Compilation failed!"
fi