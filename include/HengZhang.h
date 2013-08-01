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
    vector<int> m_vTermsX;
    vector<int> m_vTermsY;
    vector<int> m_vTermsZ;
    vector<int> m_vTermsMIN;
    vector<int> m_vTermsMAX;

    int m_nSymbolS;
    int m_nSymbolT;
    int m_nSymbolMAX;
    int m_nSymbolMIN;
    int m_nSymbolSucc;

    int m_nNumS;
    int m_nNumT;
    int m_nNumMAX;
    int m_nNumMIN;
    int m_nNumSucc;
    int m_nNumNV;
    
    vector< vector<string> > m_vDomainNames;
    
private:
    Formulas transform(Formula _originalFml);
    Formula recordQuantifier(const Formula& _originalFml);
    Formula createFormula_1(const Formula& _originalFml);
    Formula createFormula_2(const Formula& _originalFml);
    Formula createFormula_3(const Formula& _originalFml);
    Formula createFormula_4_1(const Formula& _originalFml);
    Formula createFormula_4_2(const Formula& _originalFml);
    Formula createFormula_5_1(const Formula& _originalFml);
    Formula createFormula_5_2(const Formula& _originalFml);
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

