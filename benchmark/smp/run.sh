#!/bin/bash

priCircInputFile="pri_input/benchmark.in"
priCircFactFile="pri_fact/benchmark_$1.fact"

tmpFile=".tmp"
result="result/result_$1"
sleepDelay=1
maxTime=3600

if [ ! -f $priCircInputFile -o ! -f $priCircFactFile ]
then
    echo "No such test cast !!!!!!"
    exit 1
fi

touch $tmpFile

if [ $2 -ne 0 ]
then
    title="priCirc"
    result=${result}_priCirc
    priCirc $priCircInputFile $tmpFile > /dev/null
    { gringo $priCircFactFile $tmpFile | claspD 0 > $result;  } &
else
    title="circ2dlp"
    result=${result}_circ2dlp
    gringo $circ2dlpInputFile > $tmpFile
    { circ2dlp $tmpFile -m "abx* : abo* : aba*" -v "ina* inb* out*" | claspD 0 > $result;  } &
fi

echo "Running $title ......"
# 等待后台程序运行
execTime=0
while :
do
    sleep $sleepDelay
    execTime=$(($execTime + $sleepDelay))
    # 超时
    if [ $execTime -gt $maxTime ]
    then
        echo "It has taken more than $maxTime seconds !!!!!!"
        kill -9 $!
        sleep 1
        rm $result
        echo "It has taken more than $maxTime seconds !!!!!!" > $result
        echo -e "Being forced to stop !!!!!!\n"
        break
    fi
    # 运行完
    if [ "`ps -ef | awk '{print $2}' | grep $!`" != "$!" ]
    then
        tail -n 2 $result
        echo -e "To see more Models' detail, please look the files in folder 'result' !!!!!!\n"
        break
    fi
done

rm $tmpFile > /dev/null 2>&1

exit 0
