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
#include "S2DLP.h"
#include <iostream>
#include <unistd.h>


using namespace std;

extern FILE *yyin;
extern _formula* gformula;
FILE* fout;
extern int yyparse();

#define RUN_S2DLP
#ifdef RUN_S2DLP
#define RUN_ASP
#endif

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
    
#ifdef RUN_S2DLP
    Formula f = Formula(gformula, false);
    S2DLP::instance().init(f);
    S2DLP::instance().convert();
    S2DLP::instance().outputFinalResult(fout);
    S2DLP::instance().destroy();
    fclose(fout);
#endif
    Vocabulary::instance().dumpVocabulary(stdout);
#ifdef RUN_ASP
    FILE* asp = popen("gringo res/output/sample.fact res/output/sample.out | claspD 0", "r");
    const int MAX = 1024;
    char line[MAX];
    while (fgets(line, MAX, asp) != NULL) {
        printf("%s", line);
    }
    pclose(asp);
#endif
    
    return 0;
}

