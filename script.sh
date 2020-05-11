#!/bin/sh

for i in `ls -Sr instances/*.tsp`; do ./a.out $i; done >> oneLevelResult.csv
