#include "Vocabulary.h"
#include "S2DLP.h"
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
    this->m_mapVariableName.clear();
    this->m_mapDomainName.clear();
    this->m_mapFunctionName.clear();
    this->m_mapPredicateName.clear();
    
    this->m_mapVariableDomain.clear();
    this->m_mapFunctionArity.clear();
    this->m_mapPredicateArity.clear();
    this->m_mapIsIntensionPredicate.clear();
    this->m_fmlAtomList = new Formulas();
}

Vocabulary::~Vocabulary() {
    if (this->m_fmlAtomList != NULL) {
        delete this->m_fmlAtomList;
        this->m_fmlAtomList = NULL;
    }
}
/**
 * 保存内涵谓词
 * @param _name 内涵谓词名
 * @return 内涵谓词的id
 */
void Vocabulary::addIntensionPredicate(const char* _name) {
    int id = getSymbolId(_name, PREDICATE);
    this->m_mapIsIntensionPredicate[id] = true;
}
/**
 * 获取变量所在论域
 * @param _variableId
 * @return 
 */
const char* Vocabulary::getVariableDomain(int _variableId) {
    int domainId = this->m_mapVariableDomain[_variableId];
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
        this->m_mapDomainName[domainId] = string(_domain);
    }
    if((variableId = getSymbolId(_variable, VARIABLE)) != -1) {
        this->m_mapVariableDomain[variableId] = domainId;
        this->m_mapDomainVariables[domainId].push_back(variableId);
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
        mapIdName = this->m_mapVariableName;
        break;
    case FUNCTION:
        mapIdName = this->m_mapFunctionName;
        break;
    case PREDICATE:
        mapIdName = this->m_mapPredicateName;
        break;
    case DOMAIN:
        mapIdName = this->m_mapDomainName;
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
            this->m_mapVariableName[id] = name;
            break;
        case FUNCTION:
            id = Vocabulary::ms_nFunctionId ++;
            this->m_mapFunctionName[id] = name;
            this->m_mapFunctionArity[id] = _arity;
            break;
        case PREDICATE:
            id = Vocabulary::ms_nPredicateId ++;
            this->m_mapPredicateName[id] = name;
            this->m_mapPredicateArity[id] = _arity;
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
        map<int, int>::const_iterator it = this->m_mapPredicateArity.find(_id);
        assert(it != this->m_mapPredicateArity.end());
        return it->second;
    }
}
/**
 * 获取函词的参数个数
 * @param _id 函词id
 * @return 参数个数
 */
int Vocabulary::getFunctionArity(int _id) {
    map<int, int>::const_iterator it = this->m_mapFunctionArity.find(_id);
    assert(it != this->m_mapFunctionArity.end());
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
bool Vocabulary::isIntensionPredicate(int _predicateId) {
    return this->m_mapIsIntensionPredicate[_predicateId];
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
        it = this->m_mapVariableName.find(_id);
        if (it != this->m_mapVariableName.end()) {
            return (it->second).c_str();
        }
        break;
    case FUNCTION:
        it = this->m_mapFunctionName.find(_id);
        if (it != this->m_mapFunctionName.end()) {
            return (it->second).c_str();
        }
        break;
    case PREDICATE:
        it = this->m_mapPredicateName.find(_id);
        if (it != this->m_mapPredicateName.end()) {
            return (it->second).c_str();
        }
        break;
    case DOMAIN:
        it = this->m_mapDomainName.find(_id);
        if (it != this->m_mapDomainName.end()) {
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
void Vocabulary::dumpVocabulary(FILE* _out) {
    
    fprintf(_out, "\nvariable:\n");
    for (map<int, string>::const_iterator it = this->m_mapVariableName.begin(); 
            it != this->m_mapVariableName.end(); ++ it) {
        fprintf(_out, "%s ", (it->second).c_str());
    }
    
    fprintf(_out, "\nfunction:\n");
    for (map<int, string>::const_iterator it = this->m_mapFunctionName.begin(); 
            it != this->m_mapFunctionName.end(); ++ it) {
        fprintf(_out, "%s ", (it->second).c_str());
    }
    
    fprintf(_out, "\npredicate:\n");
    for (map<int, string>::const_iterator it = this->m_mapPredicateName.begin(); 
            it != this->m_mapPredicateName.end(); ++ it) {
        fprintf(_out, "%s ", (it->second).c_str());
    }
    
    fprintf(_out, "\nintension predicate:\n");
    for (map<int, string>::const_iterator it = this->m_mapPredicateName.begin(); 
            it != this->m_mapPredicateName.end(); ++ it) {
        if (isIntensionPredicate(it->first)) {
            fprintf(_out, "%s ", (it->second).c_str());
        }
    }
    
    fprintf(_out, "\ndomains:\n");    
    for (map<int, string>::const_iterator it = this->m_mapDomainName.begin(); 
            it != this->m_mapDomainName.end(); ++ it) {
        fprintf(_out, "variables at domain %s: ", (it->second).c_str());
        vector<int> variables = this->m_mapDomainVariables[it->first];
        for (vector<int>::const_iterator it2 = variables.begin();
                it2 != variables.end(); ++ it2) {
            fprintf(_out, "%s", this->m_mapVariableName[*it2].c_str());
            if (it2 != variables.end() - 1) {
                fprintf(_out, ", ");
            }
        }
    }
    
    fprintf(_out, "\natom\n");
    for (FORMULAS_ITERATOR it = m_fmlAtomList->begin();
            it != m_fmlAtomList->end(); ++ it) {
        fprintf(_out, "%s", getNameById(it->getFormula()->predicate_id, PREDICATE));
    }
}

Formula Vocabulary::getAtom(int _predicateId) const {
    for (FORMULAS_ITERATOR it = m_fmlAtomList->begin();
            it != m_fmlAtomList->end(); ++ it) {
        if (it->getFormula()->predicate_id == _predicateId) {
            return *it;
        }
    }
    return Formula();
}
void Vocabulary::addAtom(const Formula& _newAtom) {
    for (FORMULAS_ITERATOR iter = m_fmlAtomList->begin();
            iter != m_fmlAtomList->end(); ++ iter) {
        if (iter->getFormula()->predicate_id == _newAtom.getFormula()->predicate_id) {
            return;
        }
    }
    this->m_fmlAtomList->pushBack(_newAtom);
}

bool Vocabulary::isSuccOrMax(int _predicateId) const {
    const char* predicateName = getNameById(_predicateId, PREDICATE);
    if (strncmp("max", predicateName, 3) == 0 ||
            strncmp("succ", predicateName, 4) == 0) {
        return true;
    }
    return false;
}
map<int, string> Vocabulary::getDomainNames() const {
    return this->m_mapDomainName;
}

map<int, int> Vocabulary::getVariablesDomains() const {
    return this->m_mapVariableDomain;
}

Formulas* Vocabulary::getAtomList() const {
    return this->m_fmlAtomList;
}