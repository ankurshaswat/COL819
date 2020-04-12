#!/bin/bash

# rm txt.txt

# for ((n=10;n<101;n+=10))
# do
#     for i in {1..100}; 
#     do 
#         ./main_ppt inps/drawing_tests_$n.txt | grep Sending | wc -l >> txt.txt ; 
#     done
# done


# rm txt2.txt

# for i in {1..12}
# do
#     export GOMAXPROCS=$i
#     for ((n=10;n<101;n+=10))
#     do
#         for i in {1..10}; 
#         do 
#             { time -p ./main_ppt inps/drawing_tests_$n.txt >> dump.txt ; } 2>> txt2.txt ;
#         # ./main_ppt inps/drawing_tests_$n.txt | grep Sending | wc -l >> txt.txt ; 
#         done
#     done
# done

# rm dump.txt