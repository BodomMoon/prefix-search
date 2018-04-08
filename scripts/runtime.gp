reset
set xlabel 'clock(nano sec)'
set ylabel ''
set term png enhanced font 'Verdana,10'
set output 'runtime.png'

plot [:10000][:2500] 'caculate.txt' using 1:2 with points title 'cpy', \
'caculate_ref.txt' using 1:2 with points title 'ref'

