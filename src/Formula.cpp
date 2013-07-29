#include "Formula.h"
#include "Utils.h"
#include <cstring>
#include <algorithm>
#include "Vocabulary.h"

int Formula::ms_nNewFormulaId = 0;

Formula::Formula() {
    this->m_pFormula = NULL;
    this->m_nFormulaId = Formula::ms_nNewFormulaId ++;
}
Formula::Formula(const Formula& rhs) {
    this->m_pFormula = Utils::copyFormula(rhs.getFormula());
    this->m_nFormulaId = rhs.m_nFormulaId;
}
Formula::Formula(_formula* _fml, bool _isCopy) {
    if (_isCopy) {
        this->m_pFormula = Utils::copyFormula(_fml);
    }
    else {
        this->m_pFormula = _fml;
    }
    this->m_nFormulaId = Formula::ms_nNewFormulaId ++;
}
Formula::~Formula() {
    if (this->m_pFormula != NULL) {
        Utils::deleteFormula(this->m_pFormula);
        this->m_pFormula = NULL;
    }
}
Formula& Formula::operator = (const Formula& _rhs) {
    this->m_pFormula = Utils::copyFormula(_rhs.getFormula());
    this->m_nFormulaId = _rhs.m_nFormulaId;
    return *this;
}
bool Formula::operator == (const Formula& _rhs) const {
    return Utils::compareFormula(this->m_pFormula, _rhs.getFormula());
}

bool Formula::operator != (const Formula& _rhs) const {
    return ! ((*this) == _rhs);
}

_formula* Formula::getFormula() const {
    return this->m_pFormula;
}
/**
 * 判断公式是否含有存在量词
 * @return 
 */
bool Formula::isUniversal() const {
    return Utils::isUniversal(this->m_pFormula);
}
/**
 * 把公式转化成前束范式
 */
void Formula::convertToPrenex() {
    Utils::convertToPrenex(this->m_pFormula);
}
/**
 * 为公式中没有量词限定的参数补上全称量词限定
 */
void Formula::fixUniversalQuantifier() {
    map<int, bool> variablesFlag;
    vector<int> variables;
    Utils::getNoQuantifierVariables(variablesFlag, variables, this->m_pFormula);
    sort(variables.begin(), variables.end());
    for (vector<int>::iterator it = variables.begin();
            it != variables.end(); ++ it) {
        const char* name = Vocabulary::instance().getNameById(-(*it), VARIABLE);
        if (strncmp("MAX", name, 3) == 0 ||
                strncmp("MIN", name, 3) == 0) {
            continue;
        }
        this->m_pFormula = Utils::compositeByQuantifier(UNIV, this->m_pFormula, -(*it));
    }
}
/**
 * 输出公式
 * @param _out 输出文件
 */
void Formula::output(FILE* _out) const {
    Utils::outputFormula(_out, this->m_pFormula);
    fprintf(_out, "\n");
}
/**
 * 将公式中的没有带非的内涵谓词加上 "非非(~~)"
 * @param _intensionPredicates 内涵谓词名单
 */
void Formula::doubleNegationIntensionPredicates() {
    this->m_pFormula = Utils::doubleNegationIntensionPredicates(this->m_pFormula);
}

void Formula::replaceTerms(const vector<int>& _originals, 
				const vector<int>& _replacements) {
    Utils::replaceFormulaTerms(this->m_pFormula, _originals, _replacements);
}