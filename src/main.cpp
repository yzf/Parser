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
#include "HengZhang.h"
#include "S2DLP.h"
#include <iostream>


using namespace std;

extern FILE *yyin;
extern _formula* gformula;
FILE* fout;
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
        io("res/C.sample/sample.in","output/C.sample/sample.out");
    }
    else {
        io(argv[1], argv[2]);
    }
    
    yyparse();
    fclose(yyin);
    
    Vocabulary::instance().dumpVocabulary(stdout);
    Formula* f = new Formula(gformula, false);
    S2DLP::instance().init(f);
    S2DLP::instance().hengZhangTransform();
    S2DLP::instance().outputHengZhangFormulas(fout);
    
    delete f;
    fclose(fout);
    return 0;
}

