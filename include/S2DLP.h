#ifndef _S2DLP_H_
#define _S2DLP_H_


#include "Vocabulary.h"
#include "Formula.h"
#include "Cabalar.h"
#include "HengZhang.h"
#include "Rule.h"
#include "FormulaTree.h"
#include <vector>

class S2DLP{
public:
    Formulas origin_formulas;
    Formulas zhangheng_formulas;
    Formulas dlp_formulas;
    vector<Rule> dlp_rules;
    FILE* output_file;
    vector<Formula> nega_predicates;
    
    FormulaTree formula_tree;
public:
    static S2DLP& instance();
    void convert();
    void output_origin_formulas();
    void output_zhangheng_formulas();
    void output_dlp_formulas();
    void set_output_file(FILE*&);
    void set_origin_formulas(_formula*);
    void set_origin_formulas(Formulas);
    void output_addition();
    void output_asp();
    
    
private:
    S2DLP();
    S2DLP(const S2DLP& rhs);
    ~S2DLP();
    S2DLP& operator = (const S2DLP& rhs);
    void addSucc(vector<string> domains); 
    void printAtom(_formula* atom, FILE* out);
};

#endif
