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
priFactTail="pri_fact_tail"
priInHead="pri_in_head"
priInTail="pri_in_tail"
circ2dlpBody="circ2dlp_body"
priTmpFile=".pri"
circ2dlpTmpFile=".circ"

touch $priTmpFile
touch $circ2dlpTmpFile

for i in $(seq $1)
do
    echo "generating benchmark_$i......"

    # 生成优先级circ的输入和事实文件
    inputFile="${priBenchmarkInputFilePrefix}${i}.in"
    factFile="${priBenchmarkFactFilePrefix}${i}.fact"
    circInputFile="${circ2dlpBenchmarkInputFilePrefix}${i}.lp"
    # 生成circ2dlp的输入
    echo "#const adder_size = $i." > $circInputFile
    cat $circ2dlpBody >> $circInputFile

    # 生成事实文件
    echo "#const adder_size = $i." > $factFile
    cat $priFactTail >> $factFile
    # 生成一阶逻辑文件
    cat $priInHead > $inputFile

    # A: ina(I,1). 输入
    random=`expr ${RANDOM} % 2`
    if [ $random -eq 0 ]
    then
        echo -n "~" >> $priTmpFile
        echo -n ":- " >> $circ2dlpTmpFile
    fi
    echo "ina($i,1)." >> $priTmpFile
    echo "ina($i,1)." >> $circ2dlpTmpFile
    # B: inb(I,1). 输入
    random=`expr ${RANDOM} % 2`
    if [ $random -eq 0 ]
    then
        echo -n "~" >> $priTmpFile
        echo -n ":- " >> $circ2dlpTmpFile
    fi
    echo "inb($i,1)." >> $priTmpFile
    echo "inb($i,1)." >> $circ2dlpTmpFile
#    out(I,2). 结果
    random=`expr ${RANDOM} % 2`
    if [ $random -eq 0 ]
    then
        echo -n "~" >> $priTmpFile
        echo -n ":- " >> $circ2dlpTmpFile
    fi
    echo "out($i,2)." >> $priTmpFile
    echo "out($i,2)." >> $circ2dlpTmpFile
# C: out(I,5). 进位
    random=`expr ${RANDOM} % 2`
    if [ $random -eq 0 ]
    then
        echo -n "~" >> $priTmpFile
        echo -n ":- " >> $circ2dlpTmpFile
    fi
    echo "out($i,5)." >> $priTmpFile
    echo "out($i,5)." >> $circ2dlpTmpFile

    cat $priTmpFile >> $inputFile
    cat $priInTail >> $inputFile
    cat $circ2dlpTmpFile >> $circInputFile

done
# 删除临时文件
if [ -f $priTmpFile ]
then
    rm $priTmpFile
fi
if [ -f $circ2dlpTmpFile ]
then
    rm $circ2dlpTmpFile
fi

exit 0
