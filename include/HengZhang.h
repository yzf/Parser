#ifndef _HENGZHANG_H_
#define _HENGZHANG_H_

#include <vector>
#include <string>
#include "structs.h"
#include "Formula.h"
#include "Formulas.h"
using namespace std;

class HengZhang{
private:
    vector<int> terms_X;
    vector<int> terms_Y;
    vector<int> terms_Z;
    vector<int> terms_MIN;//waste space
    vector<int> terms_MAX;//waste space

    int symbol_s;
    int symbol_t;
    int symbol_MAX;
    int symbol_MIN;
    int symbol_succ;

    int num_s;
    int num_t;
    int num_MAX;
    int num_MIN;
    int num_succ;
    
public:
    vector<string> succ_names;
    vector< vector<string> > domain_names;
    
private:
    //utilities
    int add_symbol(const char* name, SYMBOL_TYPE type, int arity);
    Formulas transform(Formula fml);
    Formula record_quantifier(Formula original_fml);
    Formula create_formula_1(Formula original_fml);
    Formula create_formula_2(Formula original_fml);
    Formula create_formula_3(Formula original_fml);
    Formula create_formula_4(Formula original_fml);
    Formula create_formula_4_1(Formula original_fml);
    Formula create_formula_4_2(Formula original_fml);
    Formula create_formula_5(Formula original_fml);
    void save_succ_name(string succ_name, vector<string> domain_name);
    
    HengZhang();

public:
    //creators
    Formulas create(Formulas fmls);

    static HengZhang& instance();
    
};

#endif

