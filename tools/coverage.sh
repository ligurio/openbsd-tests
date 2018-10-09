#!/bin/sh

gnuplot <<HERE_IN
set title 'OpenBSD userspace code coverage'
set ylabel 'Coverage'
set xlabel 'Release'
set grid
set term svg
set style fill transparent solid 0.5
set datafile separator ','

set output 'coverage-userspace.svg'
plot 'coverage.csv' using 2 title 'Lines' with lines, \
     'coverage.csv' using 3 title 'Branches' with lines
HERE_IN
