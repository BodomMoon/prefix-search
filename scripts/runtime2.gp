reset
set xlabel 'clock(nano sec)'
set ylabel ''
set term png enhanced font 'Verdana,10'
set output 'runtime2.png'

plot [:5000][:150] 'caculate2.txt' using 1:2 with points title 'cpy', 
