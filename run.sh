#!/bin/bash
# 编译代码
echo "run makefile\n"
make > /dev/null 2>&1
runner="/home/yzf/NetBeansProjects/alpha/build/Debug/GNU-Linux-x86/tests/TestFiles/f2"
# 清除操作
echo "rm old files\n"
for oldFile in `ls res/output/*.out res/output/*.result`
do
    rm $oldFile
done
# 开始转化
input=`ls res/input/*.in`
factDir="res/output"
outputDir="res/output"
# 以每个文件作为输入运行程序
for inFile in $input
do
    fileName=`echo $inFile | awk -F '/' '{print $3}' | awk -F '.' '{print $1}'`
    outFile="$outputDir/$fileName.out"
    factFile="$factDir/$fileName.fact"
    resultFile="$outputDir/$fileName.result"
    $runner $inFile $outFile > /dev/null 2>&1
    # 执行ASP
    if [ -f $factFile -a -f $outFile ]
    then
        echo "using $factFile $outFile to run asp, output is saved to $resultFile"
        gringo $factFile $outFile | claspD 0 > $resultFile
    fi
done
echo "\nrun success!!"
exit 0


