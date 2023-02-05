#!/bin/bash

iterations='10'
if [[ $# -eq 1 ]] ; then
    iterations=$1
fi

echo "Running program using Mutex and Condition Variables"
gcc \
cigerette-smoker-main.c cigerette-agent.c cigerette-smoker.c cigerette-material.c \
-o cigerette-smokers-lock-n-cv -lpthread

./cigerette-smokers-lock-n-cv $iterations

echo "Running program using Zemaphore"
gcc \
cigerette-smoker-main.c cigerette-agent.c cigerette-smoker.c cigerette-material.c ../zemaphore/zemaphore.c \
-DNAME=USE_ZEMAPHORE -o cigerette-smokers-zemaphore -lpthread

./cigerette-smokers-zemaphore $iterations
