#include "S2DLP.h"
#include "HengZhang.h"
#include "Cabalar.h"
#include "Vocabulary.h"
#include "Utils.h"
#include <assert.h>
#include <cstring>

S2DLP::S2DLP() :
        m_pOriginalFormulas(NULL),
        m_pHengZhangFormulas(NULL),
        m_pDlpFormulas(NULL) {
    this->m_pNegaPredicates = new Formulas();
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
    if (this->m_pNegaPredicates != NULL) {
        delete this->m_pNegaPredicates;
        this->m_pNegaPredicates = NULL;
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
void S2DLP::outputHengZhangFormulas(FILE* _out) const {
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
void S2DLP::outputCabalarFormulas(FILE* _out) const {
    assert(this->m_pDlpFormulas);
    this->m_pDlpFormulas->output(_out);
}
void S2DLP::ruleTransform() {
    for (FORMULAS_ITERATOR it = m_pDlpFormulas->begin();
            it != m_pDlpFormulas->end(); ++ it) {
        this->m_listRules.push_back(Rule(*it));
    }
}
void S2DLP::outputRules(FILE* _out) {
    int i = 0;
    for (list<Rule>::iterator it = m_listRules.begin();
            it != m_listRules.end(); ++ it) {
        it->output(_out);
        if (++ i % 500 == 0) {
            fflush(_out);
        }
    }
}
/**
 * 获取所有出现过非非的谓词公式
 * @return 
 */
Formulas* S2DLP::getNegaPredicates() const {
    return this->m_pNegaPredicates;
}
/**
 * 添加出现非非的谓词的公式
 * @param _negaPredicate
 */
void S2DLP::addNegaPredicates(const Formula& _negaPredicate) {
    this->m_pNegaPredicates->pushBack(_negaPredicate);
}
/**
 * 章衡、Cabalar、规则全套服务
 */
void S2DLP::convert() {
    hengZhangTransform();
    cabalarTransform();
    ruleTransform();
    //由于出现~~的谓词需要输出后再知道，所以先随便输出到一个文件，再把该文件删除
    FILE* tmpFile = fopen("out.tmp", "w+");
    outputRules(tmpFile);
    fclose(tmpFile);
    remove("out.tmp");
}
/**
 * 输出附加信息
 * @param _out
 */
void S2DLP::outputAddition(FILE* _out) {
    fprintf(_out, "%%MIN and MAX domain\n");
    map<int, string> domainNames = Vocabulary::instance().getDomainNames();
    for (map<int, string>::const_iterator it = domainNames.begin();
            it != domainNames.end(); ++ it) {
        fprintf(_out, "#domain min_%s(MIN_%s).\n", it->second.c_str(), it->second.c_str());
        fprintf(_out, "#domain max_%s(MAX_%s).\n", it->second.c_str(), it->second.c_str());
    }
    
    fprintf(_out, "%%Variable domain\n");
    map<int, int> variablesDomains = Vocabulary::instance().getVariablesDomains();
    for (map<int, int>::const_iterator it = variablesDomains.begin();
            it != variablesDomains.end(); ++ it) {
        const char* variName = Vocabulary::instance().getNameById(it->first, VARIABLE);
        const char* domainName = Vocabulary::instance().getNameById(it->second, DOMAIN);
        if (strncmp("MIN", variName, 3) == 0 || 
                strncmp("MAX", variName, 3) == 0) {
            continue;
        }
        fprintf(_out, "#domain %s(%s).\n", domainName, variName);
    }
    fprintf(_out, "%%NEW variable define\n");
    for (FORMULAS_ITERATOR iter = m_pNegaPredicates->begin(); 
            iter != m_pNegaPredicates->end(); ++ iter) {
        fprintf(_out, "_");
        Utils::printAtom(iter->getFormula(), _out);
        fprintf(_out, ":- not ");
        Utils::printAtom(iter->getFormula(), _out);        
        fprintf(_out, ".\n");
    }
    fprintf(_out, "%%Addition define\n");
    for(FORMULAS_ITERATOR iter = Vocabulary::instance().getAtomList()->begin(); 
            iter < Vocabulary::instance().getAtomList()->end(); ++ iter) {
        if(! Vocabulary::instance().isIntensionPredicate(iter->getFormula()->predicate_id)
                && ! Vocabulary::instance().isSuccOrMax(iter->getFormula()->predicate_id)) {
            fprintf(_out, "_");
            Utils::printAtom(iter->getFormula(), _out);
            fprintf(_out, " :- not ");
            Utils::printAtom(iter->getFormula(), _out);
            fprintf(_out, ".\n");
//            Utils::printAtom(iter->getFormula(), _out);
//            fprintf(_out, " | _");
//            Utils::printAtom(iter->getFormula(), _out);
//            fprintf(_out, ".\n");
        }
    }
    fprintf(_out, "%%Succ predicate definition\n");
//    printf("succ size : %u\n", HengZhang::instance().m_vDomainNames.size());
//    for(unsigned int si = 0; si < HengZhang::instance().m_vDomainNames.size(); si++)
//    {
//        for(int sj = 0; sj < HengZhang::instance().m_vDomainNames.at(si).size(); sj++)    
//                fprintf(_out, "%s", HengZhang::instance().m_vDomainNames.at(si).at(sj).c_str());
//        fprintf(_out, "\n");
//    }
//    
    for(unsigned int i = 0; i < HengZhang::instance().m_vDomainNames.size(); i++) {
        addSucc(_out, HengZhang::instance().m_vDomainNames.at(i));
    }  
    fprintf(_out, "\n");
  
    fflush(_out);
}
/**
 * 输出线序
 * @param _out
 * @param domains
 */
void S2DLP::addSucc(FILE* _out, vector<string> domains) {
    int size = domains.size();
    
    if (size == 1) {
        fprintf(_out, "succ");
        for (int i = 0; i < size; ++ i) {
            fprintf(_out, "_%s", domains[i].c_str());
        }
        fprintf(_out, "(A1, A2):-A1==A2-1,%s(A1),%s(A2).\n", 
                domains[0].c_str(), domains[0].c_str());
    }
    else {
        for (int i = 0; i < size; ++ i) {
            fprintf(_out, "succ");
            for (int j = 0; j < size; ++ j) {
                fprintf(_out, "_%s", domains[j].c_str());
            }
            fprintf(_out, "(");
            for (int j = 0; j < size; ++ j) {
                if (j >= size - i) {
                    fprintf(_out, "MAX_%s,", domains[j].c_str());
                }
                else if (j == size - i - 1) {
                    fprintf(_out, "%c1,", 'A' + j);
                }
                else {
                    fprintf(_out, "%c,", 'A' + j); 
                }
            }
            for (int j = 0; j < size; ++ j) {
                if (j >= size - i) {
                    fprintf(_out, "MIN_%s", domains.at(j).c_str());
                }
                else if (j == size - i - 1) {
                    fprintf(_out, "%c2", 'A' + j);
                }
                else {
                    fprintf(_out, "%c", 'A' + j);
                }
                if (j == size - 1) {
                    fprintf(_out, ")");
                }
                else {
                    fprintf(_out, ",");
                }
            }
            
            bool exis = false;
            for (unsigned int j = 0; j < HengZhang::instance().m_vDomainNames.size(); ++ j) {
                if(HengZhang::instance().m_vDomainNames[j].size() == 1 && HengZhang::instance().m_vDomainNames[j][0] == domains[size - j - 1]) {
                    exis = true;
                }
            }
            if (! exis) {
                vector<string> d;
                d.push_back(domains[size - i - 1]);
                HengZhang::instance().m_vDomainNames.push_back(d);
            }
            fprintf(_out, ":-succ_%s(%c1,%c2),", 
                    domains[size - i - 1].c_str(), 'A' + size - i - 1, 'A' + size - i - 1);
            
            for (int j = 0; j < size - i; ++ j) {
                if (j == size - i - 1) {
                    fprintf(_out, "%s(%c1),%s(%c2).", 
                            domains[j].c_str(), 'A' + j, domains[j].c_str(), 'A' + j);
                }
                else {
                    fprintf(_out, "%s(%c),", domains[j].c_str(), 'A' + j);            
                }
            }
            
            fprintf(_out, "\n");
        }      
    }
}
/**
 * 输出供ASP使用的结果
 * @param _out
 */
void S2DLP::outputFinalResult(FILE* _out) {
    outputAddition(_out);
    outputRules(_out);
}