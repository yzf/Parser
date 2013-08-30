#!/bin/bash

if [ $# -ne 2 ]
then
    echo "usage: $0 begin end"
    exit 1
fi

for i in `seq $1 $2`
do
    echo "running test of data_size $i ......"
    ./run.sh $i
done

exit 0
