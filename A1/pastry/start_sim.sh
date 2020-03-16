#!/bin/bash

NUM_DATA_ADD=10000
NUM_SEARCH=1000000
ENABLE_LOGS=0
LOG_NODE=0
make
mkdir logs

echo "Sim 1 started"
time ./main.out 100 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/sim1
echo "Sim 2 started"
time ./main.out 500 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/sim2
echo "Sim 3 started"
time ./main.out 1000 $NUM_DATA_ADD $NUM_SEARCH $ENABLE_LOGS $LOG_NODE logs/sim3
