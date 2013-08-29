#!/bin/bash
# 生成benchmark文件
if [ $# != 1 ]
then
    echo "usage: $0 number"
    exit 1
fi

benchmarkInputFilePrefix="input/benchmark_"
benchmarkFactFilePrefix="fact/benchmark_"
tmpFile="tmp.tmp"

for i in $(seq $1)
do
    echo "" > $tmpFile

    inputFile="${benchmarkInputFilePrefix}${i}.in"
    factFile="${benchmarkFactFilePrefix}${i}.fact"

    for j in $(seq $i)
    do
        # A: ina(I,1). 输入
        # B: inb(I,1). 输入
        #    out(I,2). 结果
        # C: out(I,5). 进位
        random=`expr ${RANDOM} % 2`
        if [ $random -eq 0 ]
        then
            echo -n "~" >> $tmpFile
        fi
        echo "ina($j,1)." >> $tmpFile

        random=`expr ${RANDOM} % 2`
        if [ $random -eq 0 ]
        then
            echo -n "~" >> $tmpFile
        fi
        echo "inb($j,1)." >> $tmpFile

        random=`expr ${RANDOM} % 2`
        if [ $random -eq 0 ]
        then
            echo -n "~" >> $tmpFile
        fi
        echo "out($j,2)." >> $tmpFile

        random=`expr ${RANDOM} % 2`
        if [ $random -eq 0 ]
        then
            echo -n "~" >> $tmpFile
        fi
        echo "out($j,5)." >> $tmpFile
    done
    # 生成事实文件
    echo "#const adder_size = $i." > $factFile
    cat fact_tail >> $factFile
    # 生成一阶逻辑文件
    cat in_head tmp.tmp in_tail > $inputFile
done

exit 0
