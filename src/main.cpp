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


using namespace std;

FILE* fout;
extern FILE* yyin;
extern _formula* gformula;
extern int yyparse();

//#define SM
#define CIRC

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
        io("res/input/circ/2clique.in","res/output/circ/2clique.out");
    }
    else {
        io(argv[1], argv[2]);
    }
    
    yyparse();
    fclose(yyin);
    
#ifdef SM
    Formula f = Formula(gformula, false);
    SMTranslator::instance().init(f);
    SMTranslator::instance().convert();
    SMTranslator::instance().outputFinalResult(fout);
    SMTranslator::instance().destroy();
#endif

#ifdef CIRC
    Formula f = Formula(gformula, false);
    CircTranslator circTranslator;
    Formulas* fmls = circTranslator.convert(f);
    SMTranslator::instance().init(*fmls);
    SMTranslator::instance().convert();
    SMTranslator::instance().outputFinalResult(fout);
    SMTranslator::instance().destroy();
    delete fmls;
#endif
    fclose(fout);
    Vocabulary::instance().dumpVocabulary(stdout);
    
    return 0;
}

