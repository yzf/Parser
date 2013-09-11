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
#include "PriCircTranslator.h"


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
    
    const char* inputFileName = "res/input/sample.in";
    const char* outputFileName = "res/output/sample.out";
    
    int n = 1;
    FILE* in = fopen(inputFileName, "w+");
    for (int y = 1; y <= 2*n; ++ y) {
        for (int r = 1; r <= n; ++ r) {
            fprintf(in, "(pair(X,%d)&like(X,%d,%d))", y, r, y);
            if (r != n) {
                fprintf(in, "|");
            }
        }
        if (y != 2*n) {
            fprintf(in, "|");
        }
    }
    fprintf(in, ".\n\n");
    fprintf(in, "{pair;like}\n\n");
    fprintf(in, "<X@i;Y@i>\n");
    fclose(in);
    
    if(argc < 3) {
        io(inputFileName, outputFileName);
    }
    else {
        io(argv[1], argv[2]);
    }
    
    yyparse();
    fclose(yyin);
    
    Formula f = Formula(gformula, true);
    f.convertToCNF();
    Formulas* fmls = f.divideFormula();
    fmls->output(fout);
    delete fmls;
    fclose(fout);
    Vocabulary::instance().dumpVocabulary(stdout);
    
    return 0;
}

