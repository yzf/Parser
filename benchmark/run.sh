#!/bin/bash

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

date
if [ $2 -ne 0 ]
then
    result=${result}_priCirc
    # 运行pricirc
    echo "running priCirc ......"
    priCirc $priCircInputFile $tmpFile > /dev/null
    gringo $priCircFactFile $tmpFile | claspD 0 > $result
    if [ $? -eq 0 ]
    then
        tail -n 2 $result
    else
        echo -e "\nBeing forced to stop !!!!!!"
    fi
else
    result=${result}_circ2dlp
    # 运行circ2dlp
    echo "running circ2dlp ......"
    gringo $circ2dlpInputFile > $tmpFile
    circ2dlp $tmpFile -m "abx* : abo* : aba*" -v "ina* inb* out*" | claspD 0 > $result
    if [ $? -eq 0 ]
    then
        tail -n 2 $result
    else
        echo -e "\nBeing forced to stop !!!!!!"
    fi
fi

echo "To see more Models' detail, please look the files in folder 'result' !!!!!!"
echo

rm $tmpFile

exit 0
