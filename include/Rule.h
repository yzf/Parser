#ifndef _RULE_H_
#define _RULE_H_
#include "Formula.h"
#include "Formulas.h"
#include <vector>

class Rule{
    public:
        vector<Formula> head;
        vector<Formula> body;

        void convert_formula_rule(_formula*);
        void divide_body(_formula* body);
        void divide_head(_formula* head);
        void asp_modify();
        
        void printAtom(_formula*, FILE*);
        vector<Formula> nega_atoms;
    public:
        Rule(Formula);
        ~Rule();
        Rule(const Rule& rhs);
        Rule& operator = (const Rule& rhs);
        

        bool isUseless();
        void output(FILE* out);
};

#endif