#!/bin/bash

mkdir inps
mkdir out

for i in {1..100}
do
    # echo "Loop Iteration $i"
    # echo "Generating Test Case"
    python3 graph_gen.py > inps/auto_test_$i.txt
    # echo "Checking generated graph"
    python3 graph_check.py inps/auto_test_$i.txt
    # echo "Running Code"
    ./main_ppt inps/auto_test_$i.txt > out/auto_test_res_$i.txt
    # echo "Running Checker"
    python3 result_checker.py inps/auto_test_$i.txt out/auto_test_res_$i.txt $i
done