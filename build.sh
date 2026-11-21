#!/bin/bash

CC=g++
LDFLAGS=$(pkg-config --libs glew)
LDFLAGS="$LDFLAGS -lglut"

$CC main.cpp transform.cpp camera.cpp projection.cpp math.cpp $LDFLAGS -o build/test
