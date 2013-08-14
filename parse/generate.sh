#!/bin/bash
if [ $# -eq 0 ]
then
    lexFile="sm_lex.l"
    parseFile="sm_parse.y"
elif [ "$1" = "sm" ]
then
    lexFile="sm_lex.l"
    parseFile="sm_parse.y"
elif [ "$1" = "circ" ]
then
    lexFile="circ_lex.l"
    parseFile="circ_parse.y"
else
    echo "error!!!!"
    exit 1
fi

lex -o ../src/lex.cpp $lexFile
yacc --defines=../src/parse.h -o ../src/parse.cpp $parseFile
exit 0
