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

    static int num_s;
    static int num_t;
    static int num_MAX;
    static int num_MIN;
    static int num_succ;
    
public:
    static vector<string> succ_names;
    
private:
    //utilities
    int add_symbol(const char* name, SYMBOL_TYPE type, int arity);
    Formulas transform(Formula fml);
    Formula record_quantifier(Formula originalFml);
    Formula create_formula_1(Formula originalFml);
    Formula create_formula_2(Formula originalFml);
    Formula create_formula_3(Formula originalFml);
    Formula create_formula_4_1(Formula originalFml);
    Formula create_formula_4_2(Formula originalFml);
    Formula create_formula_5(Formula originalFml);
    void save_succ_name(string succ_name);

public:
    //creators
    Formulas create(Formulas fmls);
    
};

#endif

