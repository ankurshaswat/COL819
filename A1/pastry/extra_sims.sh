#!/bin/bash

NUM_DATA_ADD=10000
NUM_SEARCH=0
ENABLE_LOGS=0
LOG_NODE=0
make
mkdir logs

time ./main.out 1000 10000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var1
time ./main.out 1000 20000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var2
time ./main.out 1000 30000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var3
time ./main.out 1000 40000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var4
time ./main.out 1000 50000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var5
time ./main.out 1000 60000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var6
time ./main.out 1000 70000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var7
time ./main.out 1000 80000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var8
time ./main.out 1000 90000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var9
time ./main.out 1000 100000 $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/keys_var10

NUM_SEARCH=500

time ./main.out 100 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/nodes_var1
time ./main.out 500 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/nodes_var2
time ./main.out 1000 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/nodes_var3
time ./main.out 2000 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/nodes_var4
time ./main.out 5000 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/nodes_var5
time ./main.out 10000 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/nodes_var6