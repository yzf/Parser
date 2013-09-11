#!/bin/bash

for i in `seq $(($1 * 2))`
do
    sed -i "s/CONST_$i/$i/g" $2
done
sed -i "s/^(*//g" $2
sed -i "s/))*/)/g" $2
sed -i "s/$/.&/g" $2
