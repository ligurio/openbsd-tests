#!/bin/sh

gnuplot <<HERE_IN
set title 'OpenBSD code coverage'
set ylabel 'Coverage'
set xlabel 'Release'
set grid
set term png
set output 'coverage.svg'
set datafile separator ','

plot 'coverage.csv'
HERE_IN


