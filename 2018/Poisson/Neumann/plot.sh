#!/usr/bin/gnuplot -persist

set xrange [-0.1:1.1]
set yrange [-0.1:1.1]

set xlabel 'x'
set ylabel 'y'

set size square

set palette gray

set terminal png size 2500,2000 enhanced font "Consolas,24"
set output 'vectors.png'
set title 'Поле направлений скоростей'
plot 'mesh.dat' u 1:2:(0.024*$4/sqrt($4**2 + $5**2)):(0.024*$5/(sqrt($4**2 + $5**2))) lt -1 notitle w vectors

set terminal png size 2500,2000 enhanced font "Consolas,24"
set output 'streamline.png'
set title 'Линии тока'
plot 'sl_1.dat' lt -1 not w l, 'sl_2.dat' lt -1 not w l, 'sl_3.dat' lt -1 not w l, 'sl_4.dat' lt -1 not w l, 'sl_5.dat' lt -1 not w l, 'sl_6.dat' lt -1 not w l, 'sl_7.dat' lt -1 not w l, 'sl_8.dat' lt -1 not w l, 'sl_9.dat' lt -1 not w l, 'sl_10.dat' lt -1 not w l, 'sl_11.dat' lt -1 not w l, 'sl_12.dat' lt -1 not w l, 'sl_13.dat' lt -1 not w l, 'sl_14.dat' lt -1 not w l, 'sl_15.dat' lt -1 not w l, 'sl_16.dat' lt -1 not w l, 'sl_17.dat' lt -1 not w l, 'sl_18.dat' lt -1 not w l, 'sl_19.dat' lt -1 not w l, 'sl_20.dat' lt -1 not w l, 'sl_21.dat' lt -1 not w l, 'sl_22.dat' lt -1 not w l, 'sl_23.dat' lt -1 not w l, 'sl_24.dat' lt -1 not w l, 'sl_25.dat' lt -1 not w l, 'sl_26.dat' lt -1 not w l, 'sl_27.dat' lt -1 not w l, 'sl_28.dat' lt -1 not w l, 'sl_29.dat' lt -1 not w l, 'sl_30.dat' lt -1 not w l, 'sl_31.dat' lt -1 not w l, 'sl_32.dat' lt -1 not w l, 'sl_33.dat' lt -1 not w l, 'sl_34.dat' lt -1 not w l, 'sl_35.dat' lt -1 not w l, 'sl_36.dat' lt -1 not w l, 'sl_37.dat' lt -1 not w l, 'sl_38.dat' lt -1 not w l, 'sl_39.dat' lt -1 not w l, 'sl_40.dat' lt -1 not w l

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