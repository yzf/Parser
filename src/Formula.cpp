#include "Formula.h"
#include "Utils.h"
#include <cstring>
#include <algorithm>
#include "Vocabulary.h"
#include "PNFUtils.h"
#include "NNFUtils.h"


Formula::Formula() : m_pFormula(NULL) {
}
Formula::Formula(const Formula& _rhs) {
    m_pFormula = Utils::copyFormula(_rhs.getFormula());
}
Formula::Formula(_formula* _fml, bool _bIsCopy) {
    if (_bIsCopy) {
        m_pFormula = Utils::copyFormula(_fml);
    }
    else {
        m_pFormula = _fml;
    }
}
Formula::~Formula() {
    if (m_pFormula != NULL) {
        Utils::deleteFormula(m_pFormula);
        m_pFormula = NULL;
    }
}
Formula& Formula::operator = (const Formula& _rhs) {
    m_pFormula = Utils::copyFormula(_rhs.getFormula());
    return *this;
}
bool Formula::operator == (const Formula& _rhs) const {
    return Utils::compareFormula(m_pFormula, _rhs.getFormula());
}

bool Formula::operator != (const Formula& _rhs) const {
    return ! ((*this) == _rhs);
}

_formula* Formula::getFormula() const {
    return m_pFormula;
}
void Formula::setFormula(_formula* _newFormula) {
    if (m_pFormula == _newFormula) {
        return;
    }
    if (m_pFormula !=  NULL) {
        Utils::deleteFormula(m_pFormula);
    }
    m_pFormula = _newFormula;
}
/**
 * 判断公式是否含有存在量词
 * @return 
 */
bool Formula::isUniversal() const {
    return Utils::isUniversal(m_pFormula);
}
/**
 * 把公式转化成前束范式
 */
void Formula::convertToPNF() {
    fixUniversalQuantifier();
    PNFUtils::convertToPNF(m_pFormula);
}
/**
 * 把公式转化为否定范式
 * @param _bIsSM
 */
void Formula::convertToNNF(bool _bIsSM) {
    NNFUtils::ms_bIsSM = _bIsSM;
    m_pFormula = NNFUtils::convertToNegativeNormalForm(m_pFormula);
}
/**
 * 为公式中没有量词限定的参数补上全称量词限定
 */
void Formula::fixUniversalQuantifier() {
    if (m_pFormula == NULL) {
        return;
    }
    map<int, bool> variablesFlag;
    vector<int> variables;
    Utils::getNoQuantifierVariables(variablesFlag, variables, m_pFormula);
    sort(variables.begin(), variables.end());
    for (vector<int>::iterator it = variables.begin();
            it != variables.end(); ++ it) {
        const char* name = Vocabulary::instance().getNameById(-(*it), VARIABLE);
        if (strncmp("MAX", name, 3) == 0 ||
                strncmp("MIN", name, 3) == 0) {
            continue;
        }
        m_pFormula = Utils::compositeByQuantifier(UNIV, m_pFormula, -(*it));
    }
}
void Formula::removeUniversalQuantifier() {
    if (m_pFormula == NULL) {
        return;
    }
    while (m_pFormula->formula_type == UNIV) {
        _formula* pre = m_pFormula;
        m_pFormula = m_pFormula->subformula_l;
        free(pre);
    }
}
/**
 * 输出公式
 * @param _out 输出文件
 */
void Formula::output(FILE* _out) const {
    Utils::outputFormula(_out, m_pFormula);
    fprintf(_out, "\n");
}
/**
 * 将所有 p(X) => ~~p(X)
 * @param _vPredicates
 */
void Formula::doubleNegationPredicates(const map<int, string>& _mapPredicates) {
    m_pFormula = Utils::doubleNegationPredicates(m_pFormula, _mapPredicates);
}
/**
 * 替换参数
 * @param _originals
 * @param _replacements
 */
void Formula::replaceTerms(const vector<int>& _originals, 
				const vector<int>& _replacements) {
    Utils::replaceFormulaTerms(m_pFormula, _originals, _replacements);
}
void Formula::removeImpl() {
    m_pFormula = Utils::removeImpl(m_pFormula);
}
/**
 * 对公式进行拆分，结果是对公式的每段进行拷贝，原公式不影响
 * @return Formulas* 需要手动销毁
 */
Formulas* Formula::divideFormula() const{
    Formulas* pResult = new Formulas();
    Utils::divideFormula(m_pFormula, pResult);
    return pResult;
}

void Formula::convertToCNF() {
    m_pFormula = Utils::convertDNFtoCNF(m_pFormula);
}