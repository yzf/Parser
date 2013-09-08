/* 
 * File:   main.cpp
 * Author: yzf
 *
 * Created on July 10, 2013, 2:17 PM
 */

#include <cstdlib>
#include <cstdio>
#include <assert.h>
#include "Vocabulary.h"
#include "structs.h"
#include "Utils.h"
#include "Formula.h"
#include "Formulas.h"
#include "SMTranslator.h"
#include <iostream>
#include <unistd.h>
#include "CircTranslator.h"
#include "Optimization.h"


using namespace std;

FILE* fout;
extern FILE* yyin;
extern _formula* gformula;
extern int yyparse();

void io(const char* iPathName, const char* oPathName) {
    yyin = fopen (iPathName, "r");
    fout = fopen (oPathName, "w+");

    if (! yyin) {
        printf("IO Error: cannot open the input file.\n" );
        assert(0);
    }
    if (! fout) {
        printf("IO Error: cannot open the output file.\n");
        assert(0);
    }
}

int main(int argc, char** argv) {
    
    if(argc < 3) {
        io("res/input/sample.in","res/output/sample.out");
    }
    else {
        io(argv[1], argv[2]);
    }
    
    yyparse();
    fclose(yyin);
    
    Formula f = Formula(gformula, false);
    
    Optimization* op = new Optimization();
    Formulas* fmls = op->convert(f);
    SMTranslator::instance().init(*fmls);
    SMTranslator::instance().convert();
    SMTranslator::instance().outputHengZhangFormulas(stdout);
    SMTranslator::instance().outputCabalarFormulas(stdout);
    SMTranslator::instance().outputFinalResult(fout);
    SMTranslator::instance().destroy();
    delete fmls;
    delete op;

    fclose(fout);
    Vocabulary::instance().dumpVocabulary(stdout);
    
    return 0;
}

