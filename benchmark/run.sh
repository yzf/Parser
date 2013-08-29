#!/bin/bash

if [ $# -ne 2 ]
then
    echo "usage: $0 data_size flag   (flag != 0 to output the answer set)"
    echo "example: $0 5 0"
    exit 1
fi

priCircInputFile="pri_input/benchmark_$1.in"
priCircFactFile="pri_fact/benchmark_$1.fact"
circ2dlpInputFile="circ2dlp_input/benchmark_$1.lp"
tmpFile=".tmp"
result="result"

if [ ! -f $priCircInputFile -o ! -f $circ2dlpInputFile ]
then
    echo "nosuch test case !!!!!!"
    exit 1
fi

touch $tmpFile
# 运行pricirc
echo -n "running priCirc ......" > $result
priCirc $priCircInputFile $tmpFile > /dev/null
if [ $2 -eq 1 ]
then
    { time gringo $priCircFactFile $tmpFile | claspD 0; } >> $result 2>> $result
else
    { time gringo $priCircFactFile $tmpFile | claspD 0; } > /dev/null 2>> $result
fi

echo
# 运行circ2dlp
echo -n "running circ2dlp ......" >> $result
gringo $circ2dlpInputFile > $tmpFile
if [ $2 -eq 1 ]
then
    { time circ2dlp $tmpFile -m "abx* : abo* : aba*" -v "ina* inb* out*" | claspD 0; } >> $result 2>> $result
else
    { time circ2dlp $tmpFile -m "abx* : abo* : aba*" -v "ina* inb* out*" | claspD 0; } > /dev/null 2>> $result
fi

if [ -f $tmpFile ]
then
    rm $tmpFile
fi

exit 0
