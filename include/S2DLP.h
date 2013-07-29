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

class S2DLP {
private:
    Formulas* m_pOriginalFormulas;      //原公式
    Formulas* m_pHengZhangFormulas;     //章衡转化结果
    Formulas* m_pDlpFormulas;           //Cabalar转化结果
private:
    S2DLP();
    S2DLP(const S2DLP& _rhs) {};
    S2DLP& operator = (const S2DLP& _rhs) {return *this;}
    ~S2DLP();
public:
    static S2DLP& instance();
    void init(Formula* _originalFml);
    void init(Formulas* _originalFmls);
    void hengZhangTransform();
    void outputHengZhangFormulas(FILE* _out);
    void cabalarTransform();
    void outputCabalarFormulas(FILE* _out);
};

#endif	/* S2DLP_H */

