#!/bin/bash
if [ $# -eq 0 ]
then
    tranType="sm"
elif [ "$1" = "sm" ]
then
    tranType="sm"
elif [ "$1" = "circ" ]
then
    tranType="circ"
else
    echo "error!!!"
    exit 1
fi

echo "begin to run $tranType..."
echo
cd parse
./generate.sh $tranType
cd ..

# 编译代码
echo
echo "run makefile"
echo
make > /dev/null
echo
runner="/home/yzf/NetBeansProjects/alpha/build/Debug/GNU-Linux-x86/tests/TestFiles/f2"
# 开始转化
input=`ls res/input/$tranType/*.in`
factDir="res/input/$tranType"
outputDir="res/output/$tranType"
# 以每个文件作为输入运行程序
for inFile in $input
do
    fileName=`echo $inFile | awk -F '/' '{print $4}' | awk -F '.' '{print $1}'`
    outFile="$outputDir/$fileName.out"
    factFile="$factDir/$fileName.fact"
    resultFile="$outputDir/$fileName.result"
    echo -n > $outFile
    echo -n > $resultFile
    echo "run: $runner $inFile $outFile"
    $runner $inFile $outFile > /dev/null
    if [ $? -ne 0 ]
    then
        echo "error!!!!!!!!!!!!!!!!!"
        exit 1
    fi
    # 执行ASP
    if [ -f $factFile -a -f $outFile ]
    then
        echo "run: gringo $factFile $outFile | claspD 0 > $resultFile"
        echo
        gringo $factFile $outFile | claspD 0 > $resultFile
    fi
done
echo
echo "run success!!"
exit 0


