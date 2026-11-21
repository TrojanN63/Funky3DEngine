#!/bin/bash

CC=g++
LDFLAGS=$(pkg-config --libs glew)
LDFLAGS="$LDFLAGS -lglut"

$CC test.cpp transform.cpp camera.cpp projection.cpp $LDFLAGS -o build/test
