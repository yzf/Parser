/* 
 * File:   HengZhang.h
 * Author: yzf
 *
 * Created on July 28, 2013, 3:08 PM
 */

#ifndef HENGZHANG_H
#define	HENGZHANG_H

#include <vector>
#include <string>
#include "structs.h"
#include "Formula.h"
#include "Formulas.h"

using namespace std;

class Formula;
class Formulas;

class HengZhang{
public:
    vector<int> terms_X;
    vector<int> terms_Y;
    vector<int> terms_Z;
    vector<int> terms_MIN;
    vector<int> terms_MAX;

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
    int num_NV;
    
private:
    Formulas transform(Formula _originalFml);
    Formula recordQuantifier(Formula _originalFml);
    Formula createFormula_1(Formula _originalFml);
    Formula createFormula_2(Formula _originalFml);
    Formula createFormula_3(Formula _originalFml);
    Formula createFormula_4_1(Formula _originalFml);
    Formula createFormula_4_2(Formula _originalFml);
    Formula createFormula_5_1(Formula _originalFml);
    Formula createFormula_5_2(Formula _originalFml);
    _formula* generateFormulaLeft_4();
    
    HengZhang();
    HengZhang(const HengZhang& _rhs);
    ~HengZhang();
    HengZhang& operator = (const HengZhang& _rhs);
public:
    //creators
    Formulas* convert(const Formulas& _originalFmls);

    static HengZhang& instance();
    
};

#endif	/* HENGZHANG_H */

