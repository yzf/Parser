#include "S2DLP.h"
#include "HengZhang.h"
#include "Cabalar.h"
#include <assert.h>

S2DLP::S2DLP() :
        m_pOriginalFormulas(NULL),
        m_pHengZhangFormulas(NULL),
        m_pDlpFormulas(NULL) {
}

S2DLP::~S2DLP() {
    if (this->m_pOriginalFormulas != NULL) {
        delete this->m_pOriginalFormulas;
        this->m_pOriginalFormulas = NULL;
    }
    if (this->m_pHengZhangFormulas != NULL) {
        delete this->m_pHengZhangFormulas;
        this->m_pHengZhangFormulas = NULL;
    }
    if (this->m_pDlpFormulas != NULL) {
        delete this->m_pDlpFormulas;
        this->m_pDlpFormulas = NULL;
    }
}

S2DLP& S2DLP::instance() {
    static S2DLP theInstance;
    return theInstance;
}
/**
 * 初始化S2DLP求解器
 * @param _originalFml  调用divedeFormula方法，将原公式划分为多条子公式
 */
void S2DLP::init(Formula* _originalFml) {
    assert(_originalFml);
    this->m_pOriginalFormulas = _originalFml->divideFormula();
}
/**
 * 初始化S2DLP求解器
 * @param _originalFmls
 */
void S2DLP::init(Formulas* _originalFmls) {
    assert(_originalFmls);
    this->m_pOriginalFormulas = _originalFmls;
}
/**
 * 进行章衡量词消去转化
 */
void S2DLP::hengZhangTransform() {
    assert(this->m_pOriginalFormulas);
    this->m_pHengZhangFormulas = HengZhang::instance().convert(*(this->m_pOriginalFormulas));
}
/**
 * 输出章衡转化结果
 * @param _out
 */
void S2DLP::outputHengZhangFormulas(FILE* _out) {
    assert(this->m_pHengZhangFormulas);
    this->m_pHengZhangFormulas->output(_out);
}
/**
 * 对章衡转化结果进行Cabalar转化
 */
void S2DLP::cabalarTransform() {
    assert(this->m_pOriginalFormulas);
    this->m_pDlpFormulas = Cabalar::instance().convert(*(this->m_pHengZhangFormulas));
}
/**
 * 输出Cabalar转化结果
 * @param _out
 */
void S2DLP::outputCabalarFormulas(FILE* _out) {
    assert(this->m_pDlpFormulas);
    this->m_pDlpFormulas->output(_out);
}