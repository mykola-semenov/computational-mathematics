#!/bin/sh

mkdir Dirichlet
mkdir Poisson
mkdir Neumann

cc -pedantic -lm -std=c11 -D DIRICHLET main.c
./a
cc -pedantic -lm -std=c11 -D POISSON main.c 
./a
cc -pedantic -lm -std=c11 -D NEUMANN main.c 
./a

cd Dirichlet
./plot.sh
./plot3d.sh
cd ..

cd Poisson
./plot.sh
./plot3d.sh
cd ..

cd Neumann
./plot.sh
./plot3d.sh
cd ..

rm a.exe
rm a.out