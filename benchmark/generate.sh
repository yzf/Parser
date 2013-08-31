#!/bin/bash
# 生成benchmark文件
if [ $# != 1 ]
then
    echo "usage: $0 number"
    exit 1
fi

priBenchmarkInputFilePrefix="pri_input/benchmark_"
priBenchmarkFactFilePrefix="pri_fact/benchmark_"
circ2dlpBenchmarkInputFilePrefix="circ2dlp_input/benchmark_"
priFactTail="resource/pri_fact_tail"
priInHead="resource/pri_in_head"
priInTail="resource/pri_in_tail"
circ2dlpBody="resource/circ2dlp_body"
tmpFile=".tmp"
priTmpFile=".pri"
circ2dlpTmpFile=".circ"

touch $priTmpFile
touch $circ2dlpTmpFile
touch $tmpFile

# 生成正确的输入
for i in `seq $1`
do
    echo "ina($i,1)." >> $tmpFile
    echo "inb($i,1)." >> $tmpFile
    echo "out($i,2)." >> $tmpFile
    echo "out($i,5)." >> $tmpFile
done

for i in `seq $1`
do
    echo "generating benchmark_$i......"
    # 生成优先级circ的输入和事实文件
    inputFile="${priBenchmarkInputFilePrefix}${i}.in"
    factFile="${priBenchmarkFactFilePrefix}${i}.fact"
    circInputFile="${circ2dlpBenchmarkInputFilePrefix}${i}.lp"
    # 出错的加法器的个数
    errorCount=$(($RANDOM % $i + 1))
    if [ $errorCount -gt 20 ]
    then
        errorCount=20
    fi

    #  生成circ2dlp的输入
    echo "#const adder_size = $i." > $circInputFile
    cat $circ2dlpBody >> $circInputFile

    # 生成事实文件
    echo "#const adder_size = $i." > $factFile
    cat $priFactTail >> $factFile
    # 生成一阶逻辑文件
    cat $priInHead > $inputFile

    line=$(($i * 4))
    head -n $line $tmpFile > $priTmpFile
    head -n $line $tmpFile > $circ2dlpTmpFile

    # 生成错误
    for j in `seq $errorCount`
    do
        index=$(($RANDOM % $i + 1))
        random=$(($RANDOM % 4))
        if [ $random -eq 0 ]
        then
            sed -i "s/^out($index,2)/~&/" $priTmpFile
            sed -i "s/^out($index,2)/:- &/" $circ2dlpTmpFile
        elif [ $random -eq 1 ]
        then
            sed -i "s/^out($index,5)/~&/" $priTmpFile
            sed -i "s/^out($index,5)/:- &/" $circ2dlpTmpFile
        elif [ $random -eq 2 ]
        then
            sed -i "s/^ina($index,1)/~&/" $priTmpFile
            sed -i "s/^ina($index,1)/:- &/" $circ2dlpTmpFile
        else
            sed -i "s/^inb($index,1)/~&/" $priTmpFile
            sed -i "s/^inb($index,1)/:- &/" $circ2dlpTmpFile
        fi
    done

    cat $priTmpFile >> $inputFile
    cat $priInTail >> $inputFile
    cat $circ2dlpTmpFile >> $circInputFile

done

rm $tmpFile $priTmpFile $circ2dlpTmpFile

exit 0
