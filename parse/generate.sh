lex -o ../src/lex.cpp sm_lex.l
yacc --defines=../src/parse.h -o ../src/parse.cpp sm_parse.y
