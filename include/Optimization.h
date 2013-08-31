/* 
 * File:   Optimization.h
 * Author: yzf
 *
 * Created on August 16, 2013, 4:31 PM
 */

#ifndef OPTIMIZATION_H
#define	OPTIMIZATION_H

#include "Formula.h"
#include "Formulas.h"
#include "HengZhang.h"
#include "CircTranslator.h"
#include <string>
#include <vector>

using namespace std;

class Optimization : protected HengZhang, protected CircTranslator {
protected:
    int m_nSymbolW;
private:
    Formulas transform(const Formula& _originalFml);
    Formula createFormula_1();
    Formula createFormula_2(const Formula& _originalFml);
    Formula createFormula_3(const Formula& _originalFml);
    Formula createFormula_4(const Formula& _originalFml);
    Formula createFormula_5_1(const Formula& _originalFml);
    Formula createFormula_5_2(const Formula& _originalFml);
    _formula* create_6_left();
    Formula createFormula_6_1(const Formula& _originalFml);
    Formula createFormula_6_2(const Formula& _originalFml);
protected:
    Formula preProcessing(const Formula& _originalFml);
    void postProcessing();
public:
    Optimization();
    ~Optimization();
    Formulas* convert(const Formula& _originalFml);
};

//class Optimization {
//private:
//    const static string ms_sZVariPostfix;
//    const static string ms_sPredicateNameS;
//    const static string ms_sPredicateNameW;
//    const static string ms_sPredicateNameT;
//    
//    string ms_sPredicateNameSucc;
//    
//    vector<int> m_vTermsX;
//    vector<int> m_vTermsY;
//    vector<int> m_vTermsZ;
//    vector<int> m_vTermsMIN;
//    vector<int> m_vTermsMAX;
//    
//    vector<string> m_vNewPredicatesName;
//
//private:
//    Optimization();
//    ~Optimization();
//    Optimization(const Optimization&);
//    Optimization& operator = (const Optimization&);
//    
//    Formula preProcessing(const Formula& _originalFml);
//    Formula createFormula_1();
//    Formula createFormula_2(const Formula& _originalFml);
//    Formula createFormula_3(const Formula& _originalFml);
//    Formula createFormula_4(const Formula& _originalFml);
//    Formula createFormula_5_1(const Formula& _originalFml);
//    Formula createFormula_5_2(const Formula& _originalFml);
//    _formula* create_6_left();
//    Formula createFormula_6_1(const Formula& _originalFml);
//    Formula createFormula_6_2(const Formula& _originalFml);
//    _formula* createP_P();
//    Formula createFormula_7_1();
//    Formula createFormula_7_2();
//    Formula createFormula_8();
//public:
//    static Optimization& instance();
//    Formulas* convert(const Formula& _originalFml);
//};

#endif	/* OPTIMIZATION_H */

