#!/bin/bash

priCircInputFile="pri_input/benchmark_$1.in"
priCircFactFile="pri_fact/benchmark_$1.fact"
circ2dlpInputFile="circ2dlp_input/benchmark_$1.lp"
tmpFile=".tmp"
result="result/result_$1"
sleepDelay=1
maxTime=3600

if [ ! -f $priCircInputFile -o ! -f $circ2dlpInputFile ]
then
    echo "No such test case !!!!!!"
    exit 1
fi

touch $tmpFile

if [ $2 -ne 0 ]
then
    title="priCirc"
    result=${result}_priCirc
    priCirc $priCircInputFile $tmpFile > /dev/null
    { gringo $priCircFactFile $tmpFile | claspD 0 > $result; } &
else
    title="circ2dlp"
    result=${result}_circ2dlp
    gringo $circ2dlpInputFile > $tmpFile
    { circ2dlp $tmpFile -m "abx* : abo* : aba*" -v "ina* inb* out*" | claspD 0 > $result; } &
fi

echo "Running $title ......"
# 等待后台程序运行
execTime=0
while :
do
    sleep $sleepDelay
    execTime=$(($execTime + $sleepDelay))
    # 超时或者运行完就退出循环
    if [ "`ps -ef | awk '{print $2}' | grep $!`" != "$!" -o $execTime -gt $maxTime ]
    then
        break
    fi
done

if [ $execTime -gt $maxTime ]
then
    echo "It has taken more than $maxTime seconds !!!!!!"
    kill -9 $!
    sleep 1
    rm $result
    echo "It has taken more than $maxTime seconds !!!!!!" > $result
    echo -e "Being forced to stop !!!!!!\n"
else
    tail -n 2 $result
    echo -e "To see more Models' detail, please look the files in folder 'result' !!!!!!\n"
fi

rm $tmpFile

exit 0


