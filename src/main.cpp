/* 
 * File:   main.cpp
 * Author: yzf
 *
 * Created on July 10, 2013, 2:17 PM
 */

#include <cstdlib>
#include <cstdio>
#include <assert.h>

#include "S2DLP.h"
#include "structs.h"
#include "Cabalar.h"
#include "HengZhang.h"
using namespace std;

extern FILE *yyin;
extern S2DLP Translator;
extern Vocabulary vocabulary;
extern _formula* gformula;
FILE* fout;
extern int yyparse();

void io(const char* iPathName, const char* oPathName)
{
    yyin = fopen (iPathName, "r");
    fout = fopen (oPathName, "w");

    if(!yyin)
    {
        printf("IO Error: cannot open the input file.\n" );
        assert(0);
    }
    if(!fout)
    {
        printf("IO Error: cannot open the output file.\n");
        assert(0);
    }
}

int main(int argc, char** argv) {
    
    if(argc < 3)
    {
        io("res/C.sample/sample.in","output/C.sample/sample.out");
    }
    else{
        io(argv[1], argv[2]);
    }
    
    yyparse();
    Translator.set_origin_formulas(gformula);
    
    HengZhang hz;
    
    Formulas hz_result;
    Formulas result;
    Cabalar cabalar;
    
    Formulas fmls = Translator.origin_formulas;
    hz_result = hz.create(fmls);
    Formulas cabalar_result = cabalar.convert_Cabalar(hz_result);
    while (cabalar_result.size_formulas() > 0) {
        Formula cur_fml = cabalar_result.top_formula();
        cabalar_result.pop_formula();
        printf("formula: ");
        cur_fml.output(stdout);
        Rule rule(cur_fml);
        printf("rule:    ");
        rule.output(stdout);
    }


    return 0;
}

