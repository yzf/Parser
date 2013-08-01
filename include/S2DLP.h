/* 
 * File:   S2DLP.h
 * Author: yzf
 *
 * Created on July 29, 2013, 2:18 PM
 */

#ifndef S2DLP_H
#define	S2DLP_H

#include "Formula.h"
#include "Formulas.h"
#include "Rule.h"
#include <list>
#include <string>
#include <vector>
#include <cstdio>

class Formula;
class Formulas;
class Rule;
/*
 * StableModel转换器，单例
 */
class S2DLP {
private:
    Formulas* m_pOriginalFormulas;      //原公式
    Formulas* m_pHengZhangFormulas;     //章衡转化结果
    Formulas* m_pDlpFormulas;           //Cabalar转化结果
    Formulas* m_pNegaPredicates;        //出现非非的谓词公式
    list<Rule> m_listRules;             //转换后的所有规则
private:
    S2DLP();
    S2DLP(const S2DLP& _rhs) {};
    S2DLP& operator = (const S2DLP& _rhs) {return *this;}
    ~S2DLP();
    
    void hengZhangTransform();
    void cabalarTransform();
    void ruleTransform();
    void addSucc(FILE* _out, vector<string> domains);
    void outputAddition(FILE* _out);
public:
    static S2DLP& instance();
    
    void init(const Formula& _originalFml);
    void init(const Formulas& _originalFmls);
    void destroy();
    
    void convert();
    void outputHengZhangFormulas(FILE* _out) const;
    void outputCabalarFormulas(FILE* _out) const;
    void outputRules(FILE* _out);
    
    void outputFinalResult(FILE* _out);
    Formulas* getNegaPredicates() const;
    void addNegaPredicates(const Formula& _negaPredicate);
};

#endif	/* S2DLP_H */

