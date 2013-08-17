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

//#define SM
#define CIRC
//#define OP

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
    
#ifdef SM
    SMTranslator::instance().init(f);
    SMTranslator::instance().convert();
    SMTranslator::instance().outputFinalResult(fout);
    SMTranslator::instance().destroy();
#endif

#ifdef CIRC
    CircTranslator circTranslator;
    Formulas* fmls = circTranslator.convert(f);
    fmls->output(stdout);
    SMTranslator::instance().init(*fmls);
    SMTranslator::instance().convert();
    SMTranslator::instance().outputFinalResult(fout);
    SMTranslator::instance().destroy();
    delete fmls;
#endif
    
#ifdef OP
    Formulas* fmls = Optimization::instance().convert(f);
    fmls->output(stdout);
    printf("\n\n");
    SMTranslator::instance().init(*fmls);
    SMTranslator::instance().convert();
    SMTranslator::instance().outputFinalResult(fout);
//    SMTranslator::instance().outputOriginalFormulas(stdout);
//    SMTranslator::instance().outputHengZhangFormulas(stdout);
    SMTranslator::instance().destroy();
    delete fmls;
#endif
    fclose(fout);
    Vocabulary::instance().dumpVocabulary(stdout);
    
    return 0;
}

