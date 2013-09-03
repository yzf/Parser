#!/bin/bash
# 生成benchmark文件

function randomize() {
    l=${#like[@]}
    for count in `seq $(($l / 2))`
    do
        first=$(($RANDOM % $l))
        second=$(($RANDOM % $l))
        tmp=${like[$first]}
        like[$first]=${like[$second]}
        like[$second]=$tmp
    done
}

function generateFact() {
    subjects=$1
    objects=$2
    for person in $subjects
    do
        r=1
        tmpLike=""
        tmpRank=""
        for j in $objects
        do
            # 2/3机会喜欢
            if [ $(($RANDOM % 3)) -gt 0 ]
            then
                tmpLike="$tmpLike$j "
                tmpRank="$tmpRank$r "
                if [ $(($RANDOM % 4)) -gt 0 ]
                then
                    r=$(($r + 1))
                fi
            fi
        done
        like=($tmpLike)
        randomize
        rank=($tmpRank)
        len=$((${#like[@]} - 1))
        for j in `seq 0 $len`
        do
            echo "like($person,${rank[$j]},${like[$j]})."
        done
        for j in `seq 0 $len`
        do
            for k in `seq $j $len`
            do
                if [ ${rank[$j]} -lt ${rank[$k]} ]
                then
                    echo "prefer($person,${like[$j]},${like[$k]})."
                fi
            done
        done
    done
}

# 主程序
if [ $# != 1  ]
then
    echo "usage: $0 number"
    exit 1
fi

priInputFile="pri_input/benchmark.in"
priBenchmarkFactFilePrefix="pri_fact/benchmark_"
priIn="resource/pri_in"
priFactBody="resource/pri_fact_body"
priFactTail="resource/pri_fact_tail"
tmpFile=".tmp"
priTmpFile=".pri"

touch $tmpFile $priTmpFile

cat $priIn > $priInputFile

for i in `seq $1`
do
    echo "Generating benchmark_$i......."
    factFile="${priBenchmarkFactFilePrefix}${i}.fact"

    echo "#const size = $(($i * 2))." > $factFile
    cat $priFactBody >> $factFile
    # 男生的喜好
    men=`seq $i`
    women=`seq $(($i + 1)) $(($i * 2))`
    generateFact "$men" "$women" >> $factFile
    # 女生的喜好
    generateFact "$women" "$men" >> $factFile
    cat $priFactTail >> $factFile
done


rm $tmpFile $priTmpFile

exit 0

