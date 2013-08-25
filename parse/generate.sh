#!/bin/bash
if [ $# -eq 0 ]
then
    lexFile="sm_lex.l"
    parseFile="sm_parse.y"
# 稳定模型
elif [ "$1" = "sm" ]
then
    lexFile="sm_lex.l"
    parseFile="sm_parse.y"
# 并行circ以及其优化
elif [ "$1" = "circ" -o "$1" = "op" ]
then
    lexFile="circ_lex.l"
    parseFile="circ_parse.y"
# 优先级circ
elif [ "$1" = "pri" ]
then
    lexFile="pri_lex.l"
    parseFile="pri_parse.y"
# 错误提示
else
    echo "error!!!!"
    exit 1
fi
# 生成cpp文件
lex -o ../src/lex.cpp $lexFile
yacc --defines=../src/parse.h -o ../src/parse.cpp $parseFile
exit 0
