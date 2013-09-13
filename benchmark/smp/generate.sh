#!/bin/bash
# 生成benchmark文件

function generateFact() {
    subjects=$1
    objects=$2
    for person in $subjects
    do
        r=1
        tmpLike="" tmpRank=""
        for j in $objects
        do
                tmpLike="$tmpLike$j\n"
                tmpRank="$tmpRank$r "
            if [ $((RANDOM % 4)) -gt 0 ]
            then
                r=$((r + 1))
            fi
        done
        # 打乱循序
        tmpLike=`echo -e "$tmpLike" | sort -R | sed "/^$/d"`

        like=($tmpLike)
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

    echo "#const size = $((i * 2))." > $factFile
    echo "#const size = $((i * 2))." > $circ2dlpIn
    cat $priFactBody >> $factFile
    cat $circ2dlpBody >> $circ2dlpIn

    echo > $tmpFile
    for x in `seq $((i * 2))`
    do
        r_cnt=0
        for y in `seq $((i * 2))`
        do
            for r in `seq $i`
            do
                r_cnt=$((r_cnt + 1))
                echo "pair($x,$y) :- r_${x}_$r_cnt." >> $tmpFile
                echo "like($x,$r,$y) :- r_${x}_$r_cnt." >> $tmpFile
                echo "r_${x}_$r_cnt :- pair($x,$y), like($x,$r,$y)." >> $tmpFile
            done
        done
        for j in `seq $r_cnt`
        do
            echo -n "r_${x}_$j | " >> $tmpFile
        done
        echo >> $tmpFile
    done
    sed -i "s/ | $/.\n/" $tmpFile
    cat $tmpFile >> $circ2dlpIn
    echo -e ":- like(X,R,Y), ssex(X,Y).\n" >> $circ2dlpIn

    men=`seq $i`
    women=`seq $((i + 1)) $((i * 2))`
    # 男生的喜好
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
        x=$((RANDOM % i + 1))
        y=$((RANDOM % i + 1 + i))
        if [ $x -eq $y ]
        then
            continue
        fi
        echo "unaccept($x,$y)." >> $factFile
        echo "unaccept($x,$y)." >> $circ2dlpIn
        j=$((j + 1))
    done
    cat $priFactTail >> $factFile
done

rm $tmpFile

exit 0

