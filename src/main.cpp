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
#include "FormulaTree.h"
#include "PointerSensitive.h"
using namespace std;

#define SHOW_RESULT
//#define SHOW_ALL_PROCESS
//#define SHOW_HZ_PROCESS
//#define SHOW_CABALAR_PROCESS
//#define RUN_ASP

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
    vocabulary.dump_vocabulary(stdout);
    
    _formula* input = Cabalar::instance().convert_negative_normal_form(gformula);
    
    Formulas fmls = PointerSensitive::instance().PointerSensitive_Convert(input);
//    printf("\nPointerSensitive : \n");fmls.output_formulas(stdout);printf("\n");
    S2DLP::instance().set_origin_formulas(fmls);
    S2DLP::instance().set_output_file(fout);
    S2DLP::instance().convert();
    //输出最终的Rule结果
#ifdef SHOW_RESULT 
    S2DLP::instance().output_asp();
#endif
    //输出整个转化过程
#ifdef SHOW_ALL_PROCESS
    S2DLP::instance().formula_tree.output_all_process(fout);
#endif
    //输出章衡转化过程
#ifdef SHOW_HZ_PROCESS
   S2DLP::instance().formula_tree.output_hengzhang_process(fout);
#endif
   //输出Cabalar转化过程
#ifdef SHOW_CABALAR_PROCESS
   S2DLP::instance().formula_tree.output_cabalar_process(fout);
#endif

    
#ifdef RUN_ASP
    fflush(fout);
    FILE* asp = popen("gringo output/C.sample/sample.fact output/C.sample/sample.out | claspD 0", "r");
    const int MAX = 1024;
    char line[MAX];
    while (fgets(line, MAX, asp) != NULL) {
        printf("%s", line);
    }
    pclose(asp);
#endif

    return 0;
}

