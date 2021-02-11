#!/usr/bin/gnuplot -persist

set xrange [-0.1:1.1]
set yrange [-0.1:1.1]

set xlabel 'x'
set ylabel 'y'
set zlabel 'F'

set size square

set palette gray

set terminal png size 2500,2000 enhanced font "Consolas,24"
set output 'mesh-3d.png'
set hidden3d
set contour surface
set cntrparam bspline
set dgrid3d 100,100
set title 'Результат расчёта'
splot 'mesh.dat' using 1:2:3 notitle with pm3d, 'mesh.dat' using 1:2:3 notitle lw 2 lc -1 with lines nosurf
