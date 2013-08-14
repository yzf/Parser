#!/bin/bash
if [ $# = 0 ]
then
    lexFile="sm_lex.l"
    parseFile="sm_parse.y"
fi

lex -o ../src/lex.cpp $lexFile
yacc --defines=../src/parse.h -o ../src/parse.cpp $parseFile