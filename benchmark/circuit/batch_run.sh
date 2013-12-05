#!/bin/bash

#if [ $# -ne 3 ]
#then
#    echo "usage: $0 begin end type"
#    exit 1
#fi

a="1 2 4 8 12 16 20 24 28 32 36 40"
for i in $a
do
    echo "Running test of data_size $i ......"
    ./run.sh $i 1
done

exit 0
