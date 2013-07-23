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

//#define SHOW_RESULT
//#define SHOW_HZ_RESULT
#define SHOW_HZ_PROCESS
//#define SHOW_HZ_CABALAR

extern FILE *yyin;
extern S2DLP Translator;
extern Vocabulary vocabulary;
extern _formula* gformula;
FILE* fout;
extern int yyparse();

void io(const char* iPathName, const char* oPathName)
{
    yyin = fopen (iPathName, "r");
    fout = fopen (oPathName, "w+");

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
    S2DLP::instance().set_origin_formulas(gformula);
    S2DLP::instance().set_output_file(fout);
    //输出最终的Rule结果
#ifdef SHOW_RESULT 
    S2DLP::instance().convert();
    S2DLP::instance().output_asp();
#endif
    //输出章衡转化后的结果
#ifdef SHOW_HZ_RESULT
    Formulas hz_result = HengZhang::instance().create(S2DLP::instance().origin_formulas);
    hz_result.output_formulas(fout);
#endif
    //输出章衡转化过程
#ifdef SHOW_HZ_PROCESS
    Formulas hz_result = HengZhang::instance().create(S2DLP::instance().origin_formulas);
    S2DLP::instance().formula_tree.output(fout);
#endif
    //输出章衡转化到Cabalar转化的过程
#ifdef SHOW_HZ_CABALAR
    Formulas hz_result = HengZhang::instance().create(S2DLP::instance().origin_formulas);
    while (hz_result.size_formulas() > 0) {
        Formula cur_fml = hz_result.top_formula();
        hz_result.pop_formula();
        Formulas cur_fmls;
        cur_fmls.push_formula(cur_fml);
        
        Formulas cabalar_result = Cabalar::instance().convert_Cabalar(cur_fmls);
        fprintf(fout, "HengZhang:\n");
        cur_fml.output(fout);
        fprintf(fout, "Cabalar:\n");
        cabalar_result.output_formulas(fout);
        fprintf(fout, "Rule:\n");
        while (cabalar_result.size_formulas() > 0) {
            Formula f = cabalar_result.top_formula();
            cabalar_result.pop_formula();
            Rule rule(f);
            rule.output(fout);
        }
        fprintf(fout, "\n\n");
    }
#endif

    return 0;
}

