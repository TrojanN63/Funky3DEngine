#!/bin/bash

CC=g++
LDFLAGS=$(pkg-config --libs glew)
LDFLAGS="$LDFLAGS -lglut"

$CC test.cpp $LDFLAGS -o test
