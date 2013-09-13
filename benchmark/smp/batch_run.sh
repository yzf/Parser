#!/bin/bash

if [ $# -ne 3 ]
then
    echo "Usage: $0 begin end type"
    exit 1
fi

for i in `seq $1 $2`
do
    echo "Running test of $i men and $i women ......"
    ./run.sh $i $3
done

exit 0
