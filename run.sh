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

elif [ "$1" = "op" ]
then 
    tranType="op"

elif [ "$1" = "pri" ]
then
    tranType="pri"

else
    echo "error!!!"
    exit 1
fi
echo -e "Begin to run $tranType ......\n"
# 生成parser文件
echo "Enter folder 'parse' ......"
cd parse
echo "Generate parser file"
./generate.sh $tranType
echo "Exit folder 'parse' ......"
cd -

# 生成主函数
echo -e "\nGenerate $tranType main function...\n"
cat ${tranType}_main.cpp > src/main.cpp


# 编译代码
echo -e "Run makefile\n"
make > /dev/null
runner="/home/yzf/NetBeansProjects/alpha/build/Debug/GNU-Linux-x86/tests/TestFiles/f2"
# 开始转化
echo -e "\nRunning tests...\n"
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
        echo -e "run: gringo $factFile $outFile | claspD 0 > $resultFile\n"
        gringo $factFile $outFile | claspD 0 > $resultFile
    fi
done
echo
echo "Run finished!!"
exit 0


