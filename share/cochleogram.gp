load '/home/mathieu/dev/libgammatone/share/parula.pal'
set term pdfcairo enhanced color dashed font ",10"
set output 'cochleo.pdf'

set style line 101 lc rgb '#808080' lt 1
set border 3 back ls 101
set tics nomirror out scale 0.75
set style line 102 lc rgb '#808080' lt 0 lw 1
set grid back ls 102
set tics textcolor rgb 'black'

set xlabel 'time (s)'
set ylabel 'frequency (Hz)'
set logscale y

set view map
set style data pm3d
