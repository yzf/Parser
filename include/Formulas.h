#ifndef _FORMULAS_H_
#define _FORMULAS_H_
#include "Formula.h"
#pragma once

#include <deque>
#include <cstdio>

using namespace std;

class Formula;

class Formulas{
    private:
        deque<Formula> _formulas;

    public:
        Formulas();
        ~Formulas();
        Formulas(deque<Formula> _formula);
        Formulas(const Formulas&);

        deque<Formula> get_formulas();
        void set_formulas(deque<Formula> fs);

        void join_formulas(Formulas tail);
        void push_formula(Formula tail);
        void pop_formula ();//DO NOT DELETE formula
        Formula top_formula ();
        void copy_formulas(Formulas sigma);
        void delete_formulas();
        int size_formulas();
        
        bool equal(Formulas f);
        
        void output_formulas(FILE* out);
        Formulas& operator = (const Formulas&);
};

#endif
