#!/bin/bash

if [ $# != 1 ]
then
    echo "Usage: $0 number"
    exit 1
fi

for y in `seq $1`
do
    echo -n "("
    for r in `seq $1`
    do
        echo -n "pair(X,Y_$y)&like(X,R_$r,Y_$y)"
        if [ $r -ne $1 ]
        then
            echo -n "|"
        fi
    done
    echo -n ")"
    if [ $y -ne $1 ]
    then
        echo -n "|"
    fi
done

echo "."
echo "{pair;}"
echo "<X@i>"

