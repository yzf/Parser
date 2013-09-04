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
                tmpLike="$tmpLike$j "
                tmpRank="$tmpRank$r "
            if [ $(($RANDOM % 4)) -gt 0 ]
                then
                    r=$(($r + 1))
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
circ2dlpBenchmarkFilePrefix="circ2dlp_input/benchmark_"
priIn="resource/pri_in"
priFactBody="resource/pri_fact_body"
priFactTail="resource/pri_fact_tail"
circ2dlpBody="resource/circ2dlp_body"
tmpFile=".tmp"

touch $tmpFile

cat $priIn > $priInputFile

for i in `seq $1`
do
    echo "Generating benchmark_$i......."
    factFile="${priBenchmarkFactFilePrefix}${i}.fact"
    circ2dlpIn="${circ2dlpBenchmarkFilePrefix}${i}.lp"

    echo "#const size = $(($i * 2))." > $factFile
    echo "#const size = $(($i * 2))." > $circ2dlpIn
    cat $priFactBody >> $factFile
    cat $circ2dlpBody >> $circ2dlpIn
    # circ2dlp 的存在量词
    for y in `seq $(($i * 2))`
    do
        for p in `seq $i`
        do
            echo
        done
    done
    # 男生的喜好
    men=`seq $i`
    women=`seq $(($i + 1)) $(($i * 2))`
    generateFact "$men" "$women" > $tmpFile
    cat $tmpFile >> $factFile
    cat $tmpFile >> $circ2dlpIn
    # 女生的喜好
    generateFact "$women" "$men" > $tmpFile
    cat $tmpFile >> $factFile
    cat $tmpFile >> $circ2dlpIn
    # unaccept n*n*5% 概率
    unacceptCount=$(($i*$i*5/100))
    for ((j=1;j<=$unacceptCount;))
    do
        x=$(($RANDOM % $i + 1))
        y=$(($RANDOM % $i + 1 + $i))
        if [ $x -eq $y ]
        then
            continue
        fi
        echo "unaccept($x,$y)." >> $factFile
        echo "unaccept($x,$y)." >> $circ2dlpIn
        j=$(($j + 1))
    done
    cat $priFactTail >> $factFile
done

rm $tmpFile

exit 0

