#include "Vocabulary.h"
#include "SMTranslator.h"
#include "Utils.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <assert.h>

int Vocabulary::ms_nVariableId = 0;
int Vocabulary::ms_nDomainId = 0;
int Vocabulary::ms_nFunctionId = 0;
int Vocabulary::ms_nPredicateId = 0;
int Vocabulary::ms_nRenameVariablePostfix = 0;

Vocabulary& Vocabulary::instance() {
    static Vocabulary vocabulary;
    return vocabulary;
}

Vocabulary::Vocabulary() {
    m_mapVariableName.clear();
    m_mapDomainName.clear();
    m_mapFunctionName.clear();
    m_mapPredicateName.clear();
    
    m_mapVariableDomain.clear();
    m_mapFunctionArity.clear();
    m_mapPredicateArity.clear();
    m_mapIsIntensionPredicate.clear();
    m_mapIsVaryPredicate.clear();
    m_mapDomainVariables.clear();
    m_fmlAtomList = new Formulas();
}

Vocabulary::~Vocabulary() {
    if (m_fmlAtomList != NULL) {
        delete m_fmlAtomList;
        m_fmlAtomList = NULL;
    }
    m_mapVariableName.clear();
    m_mapDomainName.clear();
    m_mapFunctionName.clear();
    m_mapPredicateName.clear();
    m_mapVariableDomain.clear();
    m_mapFunctionArity.clear();
    m_mapPredicateArity.clear();
    m_mapIsIntensionPredicate.clear();
    m_mapIsVaryPredicate.clear();
    m_mapDomainVariables.clear();
}
/**
 * 保存内涵谓词
 * @param _name 内涵谓词名
 */
void Vocabulary::addIntensionPredicate(const char* _name) {
    int id = getSymbolId(_name, PREDICATE);
    m_mapIsIntensionPredicate.insert(make_pair<int, bool>(id, true));
}
/**
 * 保存可变谓词
 * @param _name
 */
void Vocabulary::addVaryPredicate(const char* _name) {
    int id = getSymbolId(_name, PREDICATE);
    m_mapIsVaryPredicate.insert(make_pair<int, bool>(id, true));
}
/**
 * 获取变量所在论域
 * @param _variableId
 * @return 
 */
const char* Vocabulary::getVariableDomain(int _variableId) {
    int domainId = m_mapVariableDomain[_variableId];
    return getNameById(domainId, DOMAIN);
}
/**
 * 记录变量的论域
 * @param _variable 变量名
 * @param _domain 论域名
 */
void Vocabulary::setVariableDomain(const char* _variable, const char* _domain) {
    int domainId = getSymbolId(_domain, DOMAIN);
    int variableId;
    
    if(domainId == -1) {
        domainId = Vocabulary::ms_nDomainId ++;
        m_mapDomainName[domainId] = string(_domain);
    }
    if((variableId = getSymbolId(_variable, VARIABLE)) != -1) {
        m_mapVariableDomain[variableId] = domainId;
        m_mapDomainVariables[domainId].push_back(variableId);
    }
}
/**
 * 查询符号的id
 * @param _name 符号名字
 * @param _type 符号类型
 * @return 符号的id 
 */
int Vocabulary::getSymbolId(const char* _name, SYMBOL_TYPE _type) {
    
    //return value: 1. >=0 id; 2. -1 no such symbol; 3. -2 type error
    assert(_name);

    string name = _name;
    map<int, string> mapIdName;
    switch(_type)
    {
    case VARIABLE:
        mapIdName = m_mapVariableName;
        break;
    case FUNCTION:
        mapIdName = m_mapFunctionName;
        break;
    case PREDICATE:
        mapIdName = m_mapPredicateName;
        break;
    case DOMAIN:
        mapIdName = m_mapDomainName;
        break;  
    default:
        assert(0);
        return -2;
    }

    for (map<int, string>::const_iterator it = mapIdName.begin(); 
            it != mapIdName.end(); ++ it) {
        if (it->second == name) {
            return it->first;
        }
    }

    return -1;
}
/**
 * 把符号标识添加到字符表
 * @param _name 符号标识
 * @param _type 类型(变量、函词、谓词)
 * @param _arity 参数个数
 * @return 符号标识的id
 */
int Vocabulary::addSymbol(const char* _name, SYMBOL_TYPE _type, int _arity )
{
    // return value: 1. >=0 - id; 2. -1 - too many symbols; 3. -2 - _type error
    assert(_name);
    
    int id;
    string name = _name;
    
    if((id = getSymbolId(_name, _type)) < 0) {

        switch (_type) {
        case VARIABLE:
            id = Vocabulary::ms_nVariableId ++;
            m_mapVariableName[id] = name;
            break;
        case FUNCTION:
            id = Vocabulary::ms_nFunctionId ++;
            m_mapFunctionName[id] = name;
            m_mapFunctionArity[id] = _arity;
            break;
        case PREDICATE:
            id = Vocabulary::ms_nPredicateId ++;
            m_mapPredicateName[id] = name;
            m_mapPredicateArity[id] = _arity;
            break;
        default:
            assert(0);
            return -2;
        }
    }
    
    return id;
}
/**
 * 获取谓词的参数个数
 * @param _id 谓词id
 * @return 参数个数
 */
int Vocabulary::getPredicateArity(int _id) {
    if(_id < 0) {
        return spec_pred_arities[-_id-1];
    }
    else { 
        map<int, int>::const_iterator it = m_mapPredicateArity.find(_id);
        assert(it != m_mapPredicateArity.end());
        return it->second;
    }
}
/**
 * 获取函词的参数个数
 * @param _id 函词id
 * @return 参数个数
 */
int Vocabulary::getFunctionArity(int _id) {
    map<int, int>::const_iterator it = m_mapFunctionArity.find(_id);
    assert(it != m_mapFunctionArity.end());
    return it->second;
}
/**
 * 保存重命名变量
 * @return 变量Id
 */
int Vocabulary::addRenameVariable() {
    char nameBuf[10];
    sprintf(nameBuf, "PN_%i", Vocabulary::ms_nRenameVariablePostfix);
    return addSymbol(nameBuf, VARIABLE);
}
/**
 * 查询谓词是否为内涵谓词
 * @param _variableId
 * @return 
 */
bool Vocabulary::isIntensionPredicate(int _nPredicateId) const {
    map<int, bool>::const_iterator it = m_mapIsIntensionPredicate.find(_nPredicateId);
    return it != m_mapIsIntensionPredicate.end() ? true : false;
}
/**
 * 查询谓词是否为可变谓词
 * @param _nPredicateId
 * @return 
 */
bool Vocabulary::isVaryPredicate(int _nPredicateId) const {
    map<int, bool>::const_iterator it = m_mapIsVaryPredicate.find(_nPredicateId);
    return it != m_mapIsVaryPredicate.end() ? true : false;
}
/**
 * 获取id对应的名字
 * @param _id 
 * @param _type 类型
 * @return 
 */
const char* Vocabulary::getNameById(int _id, SYMBOL_TYPE _type) const {
    assert(0 <= _id);
    
    map<int, string>::const_iterator it;
    switch (_type)
    {
    case VARIABLE:
        it = m_mapVariableName.find(_id);
        if (it != m_mapVariableName.end()) {
            return (it->second).c_str();
        }
        break;
    case FUNCTION:
        it = m_mapFunctionName.find(_id);
        if (it != m_mapFunctionName.end()) {
            return (it->second).c_str();
        }
        break;
    case PREDICATE:
        it = m_mapPredicateName.find(_id);
        if (it != m_mapPredicateName.end()) {
            return (it->second).c_str();
        }
        break;
    case DOMAIN:
        it = m_mapDomainName.find(_id);
        if (it != m_mapDomainName.end()) {
            return (it->second).c_str();
        }
        break;
    default:
        assert ( 0 );
    }
    return NULL;
}
/**
 * 输出字符表的信息
 * @param out
 */
void Vocabulary::dumpVocabulary(FILE* _out)  {
    
    fprintf(_out, "\nvariable:\n");
    for (map<int, string>::const_iterator it = m_mapVariableName.begin(); 
            it != m_mapVariableName.end(); ++ it) {
        fprintf(_out, "%s ", (it->second).c_str());
    }
    
    fprintf(_out, "\nfunction:\n");
    for (map<int, string>::const_iterator it = m_mapFunctionName.begin(); 
            it != m_mapFunctionName.end(); ++ it) {
        fprintf(_out, "%s ", (it->second).c_str());
    }
    
    fprintf(_out, "\npredicate:\n");
    for (map<int, string>::const_iterator it = m_mapPredicateName.begin(); 
            it != m_mapPredicateName.end(); ++ it) {
        fprintf(_out, "%s:%d\t", (it->second).c_str(), it->first);
    }
    
    fprintf(_out, "\nintension predicate:\n");
    for (map<int, string>::const_iterator it = m_mapPredicateName.begin(); 
            it != m_mapPredicateName.end(); ++ it) {
        if (isIntensionPredicate(it->first)) {
            fprintf(_out, "%s ", (it->second).c_str());
        }
    }
    fprintf(_out, "\nvary predicate:\n");
    for (map<int, string>::const_iterator it = m_mapPredicateName.begin(); 
            it != m_mapPredicateName.end(); ++ it) {
        if (isVaryPredicate(it->first)) {
            fprintf(_out, "%s ", (it->second).c_str());
        }
    }
    
    fprintf(_out, "\ndomains:\n");    
    for (map<int, string>::const_iterator it = m_mapDomainName.begin(); 
            it != m_mapDomainName.end(); ++ it) {
        fprintf(_out, "variables at domain %s: ", (it->second).c_str());
        vector<int> variables = m_mapDomainVariables[it->first];
        for (vector<int>::const_iterator it2 = variables.begin();
                it2 != variables.end(); ++ it2) {
            fprintf(_out, "%s", m_mapVariableName[*it2].c_str());
            if (it2 != variables.end() - 1) {
                fprintf(_out, ", ");
            }
        }
    }
    
    fprintf(_out, "\natom\n");
    for (FORMULAS_CONST_ITERATOR it = m_fmlAtomList->begin();
            it != m_fmlAtomList->end(); ++ it) {
        it->output(stdout);
    }
}

Formula Vocabulary::getAtom(int _predicateId) const {
    for (FORMULAS_CONST_ITERATOR it = m_fmlAtomList->begin();
            it != m_fmlAtomList->end(); ++ it) {
        if (it->getFormula()->predicate_id == _predicateId) {
            return *it;
        }
    }
    return Formula();
}
void Vocabulary::addAtom(const Formula& _newAtom) {
    for (FORMULAS_CONST_ITERATOR iter = m_fmlAtomList->begin();
            iter != m_fmlAtomList->end(); ++ iter) {
        if (iter->getFormula()->predicate_id == _newAtom.getFormula()->predicate_id) {
            return;
        }
    }
    m_fmlAtomList->pushBack(_newAtom);
}

map<int, string> Vocabulary::getDomainNames() const {
    return m_mapDomainName;
}

map<int, int> Vocabulary::getVariablesDomains() const {
    return m_mapVariableDomain;
}

Formulas* Vocabulary::getAtomList() const {
    return m_fmlAtomList;
}
/**
 * 获取所有谓词
 * @return 
 */
map<int, string> Vocabulary::getAllPredicates() const {
    return m_mapPredicateName;
}
/**
 * 获取所有内涵谓词
 * @return 
 */
map<int, string> Vocabulary::getAllIntensionPredicates() const {
    map<int, string> vRet;
    for (map<int, string>::const_iterator it = m_mapPredicateName.begin();
            it != m_mapPredicateName.end(); ++ it) {
        if (isIntensionPredicate(it->first)) {
            vRet.insert(make_pair<int, string>(it->first, it->second));
        }
    }
    return vRet;
}
/**
 * 获取所有可变谓词
 * @return 
 */
map<int, string> Vocabulary::getAllVaryPredicates() const {
    map<int, string> vRet;
    for (map<int,string>::const_iterator it = m_mapPredicateName.begin();
            it != m_mapPredicateName.end(); ++ it) {
        if (isVaryPredicate(it->first)) {
            vRet.insert(make_pair<int, string>(it->first, it->second));
        }
    }
    return vRet;
}