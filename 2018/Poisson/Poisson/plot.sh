#!/usr/bin/gnuplot -persist

set xrange [-1.1:1.1]
set yrange [-1.1:1.1]

set xlabel 'x'
set ylabel 'y'

set size square

set palette gray

set terminal png size 2500,2000 enhanced font "Consolas,24"
set output 'vectors.png'
set title 'Поле направлений скоростей'
plot 'mesh.dat' u 1:2:(0.024*$4/sqrt($4**2 + $5**2)):(0.024*$5/(sqrt($4**2 + $5**2))) lt -1 notitle w vectors

set terminal png size 2500,2000 enhanced font "Consolas,24"
set output 'circulation.png'
set title 'Циркуляция'
plot 'circulation.dat' using 1:2:3 notitle with image

set contour
unset surface
set dgrid3d
set cntrparam bspline
set view map
unset clabel
set table 'mesh-contour.dat'
splot 'mesh.dat' using 1:2:3 notitle
unset table
unset view
unset dgrid3d
unset contour

set terminal png size 2500,2000 enhanced font "Consolas,24"
set output 'mesh-colormap.png'
set title 'Результат расчёта'
plot 'mesh.dat' using 1:2:3 notitle with image, 'mesh-contour.dat' using 1:2 lt -1 lw 2 with lines notitle 

set xrange [1:*]
set yrange [*:*]

set logscale y 10

set xlabel 'Количество итераций'
set ylabel 'Невязка'
set terminal png size 2500,2000 enhanced font "Consolas,24"
set output 'errors.png'
plot 'errors.dat' using 1:2 lt -1 lw 2 with lines notitle