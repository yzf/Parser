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
    S2DLP Translator = S2DLP::instance();
    Translator.set_origin_formulas(gformula);
    Translator.set_output_file(fout);
    Translator.convert();
    Translator.output_asp();
    //输出章衡转化过程
//    Formulas hz_result = HengZhang::instance().create(Translator.origin_formulas);
//    hz_result.output_formulas(fout);
//    HengZhang::instance().hz_tree.output(fout);
//    printf("leaf_count %d\n", HengZhang::instance().hz_tree.leaf_count);
    //输出Cabalar转化过程
//    Formulas hz_result = Translator.zhangheng_formulas;
//    while (hz_result.size_formulas() > 0) {
//        Formula cur_fml = hz_result.top_formula();
//        hz_result.pop_formula();
//        Formulas cur_fmls;
//        cur_fmls.push_formula(cur_fml);
//        
//        Formulas cabalar_result = Cabalar::instance().convert_Cabalar(cur_fmls);
//        fprintf(fout, "HengZhang:\n");
//        cur_fml.output(fout);
//        fprintf(fout, "Cabalar:\n");
//        cabalar_result.output_formulas(fout);
//        fprintf(fout, "Rule:\n");
//        while (cabalar_result.size_formulas() > 0) {
//            Formula f = cabalar_result.top_formula();
//            cabalar_result.pop_formula();
//            Rule rule(f);
//            rule.output(fout);
//        }
//        fprintf(fout, "\n\n");
//    }
    
//    fflush(fout);
//    
//    FILE* file = popen("sort output/C.sample/sample.out | uniq", "r");
//    FILE* final_output = fopen("output/C.sample/final.out", "w+");
//    char line[1024];
//    while (fgets(line, 1024, file) != NULL) {
//        fprintf(final_output, "%s", line);
//    }
//    pclose(file);
//    fclose(final_output);

    return 0;
}

