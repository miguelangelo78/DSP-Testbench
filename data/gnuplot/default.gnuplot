set style fill solid
set boxwidth 0.5
set key off
set title "DSP Output"
set xlabel "x"
set ylabel "y(x)"

set datafile separator ","
set autoscale fix
set format x '%1.4f'
set term qt noraise

bind all 'q' 'exit gnuplot'
bind all 'Escape' 'exit gnuplot'
bind Close "exit gnuplot"

array colorarray[10]
colorarray[1]="blue"
colorarray[2]="red"
colorarray[3]="orange"
colorarray[4]="black"
colorarray[5]="red"
colorarray[6]="orange"
colorarray[7]="blue"
colorarray[8]="black"
colorarray[9]="violet"
colorarray[10]="orange"

i=2

while(1) {
	sample_count=`awk -F"," 'NR==1 {print NF; exit}' data/gnuplot/default.csv`
	do for[i=2:sample_count] {
		set term qt i-1
		plot "data/gnuplot/default.csv" using 1:i with lines linecolor rgb colorarray[i-1]
	}
	pause 0.1
}