#!/bin/bash

if [ $# -ne 1 ]
then
    echo "usage: $0 data_size"
    echo "example: $0 5"
    exit 1
fi

priCircInputFile="pri_input/benchmark_$1.in"
priCircFactFile="pri_fact/benchmark_$1.fact"
circ2dlpInputFile="circ2dlp_input/benchmark_$1.lp"
tmpFile=".tmp"
result="result/result_$1"

if [ ! -f $priCircInputFile -o ! -f $circ2dlpInputFile ]
then
    echo "nosuch test case !!!!!!"
    exit 1
fi

touch $tmpFile
# 运行pricirc
echo "running priCirc ......"
priCirc $priCircInputFile $tmpFile > /dev/null
gringo $priCircFactFile $tmpFile | claspD 0 > $result
grep -m 1 "Models" $result
grep -m 1 "Time" $result

# 运行circ2dlp
echo "running circ2dlp ......"
gringo $circ2dlpInputFile > $tmpFile
circ2dlp $tmpFile -m "abx* : abo* : aba*" -v "ina* inb* out*" | claspD 0 >> $result
grep "Models" $result | tail -n 1
grep "Time" $result | tail -n 1

echo "To see more Models' detail, please look the files in folder 'result' !!!!!!"
echo

rm $tmpFile

exit 0
