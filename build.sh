#!/bin/bash

COMPILER_C_CHOSEN=""
COMPILER_CXX_CHOSEN=""

COMPILER_C_1="gcc"
COMPILER_C_2="clang"
COMPILER_CXX_1="g++"
COMPILER_CXX_2="clang++"

case $1 in
	gcc)
		COMPILER_C_CHOSEN="gcc"
		COMPILER_CXX_CHOSEN="g++"
		;;
	clang)
		COMPILER_C_CHOSEN="clang"
		COMPILER_CXX_CHOSEN="clang++"
		;;
	*)
		echo "Supported compiler names: gcc, clang"
		echo $"Usage $0 {compiler_name}"
		exit 1
		;;
esac


cmake -S . -B build -DCMAKE_C_COMPILER="$COMPILER_C_CHOSEN" -DCMAKE_CXX_COMPILER="$COMPILER_CXX_CHOSEN"

cd ./build
make
make install

