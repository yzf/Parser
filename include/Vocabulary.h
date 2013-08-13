#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "structs.h"
#include <cstdio>
#include <vector>
#include <map>
#include <string>
#include "Formulas.h"

class Formulas;

using namespace std;
/*
 * 字符表类，保存各种信息
 */
class Vocabulary {
private:
    static int ms_nVariableId;
    static int ms_nDomainId;
    static int ms_nFunctionId;
    static int ms_nPredicateId;
    static int ms_nRenameVariablePostfix;

    map<int, string> m_mapVariableName;         // map[变量id] = 对应的变量名
    map<int, string> m_mapDomainName;           // map[论域id] = 对应的论域名
    map<int, string> m_mapFunctionName;         // map[函词id] = 对应的函词名
    map<int, string> m_mapPredicateName;        // map[谓词id] = 对应的谓词名
    
    map<int, int> m_mapVariableDomain;          // map[变量id] = 所在的论域id
    map<int, int> m_mapFunctionArity;           // map[函词id] = 函词的参数个数
    map<int, int> m_mapPredicateArity;          // map[谓词id] = 谓词的参数个数
    
    map<int, vector<int> > m_mapDomainVariables;// map[论域id] = 在该论域的变量的id的数组 
    
    map<int, bool> m_mapIsIntensionPredicate;   // 记录是否为内涵谓词
     
    
    Formulas* m_fmlAtomList;

private:
    Vocabulary();
    Vocabulary(const Vocabulary&);
    Vocabulary& operator =(const Vocabulary&);
    ~Vocabulary();

public:
    static Vocabulary& instance();
    void dumpVocabulary(FILE* _out) ;
    //setter adder
    void setVariableDomain(const char* _sVariable, const char* _sDomain);
    int addSymbol(const char* _sName, SYMBOL_TYPE _type, int _nArity = 0);
    int addRenameVariable();
    void addIntensionPredicate(const char*_sName);
    void addAtom(const Formula& _newAtom);
    //getter
    bool isIntensionPredicate(int _nPredicateId) const;
    int getSymbolId(const char* _sName, SYMBOL_TYPE _type);
    int getPredicateArity(int _id);
    int getFunctionArity(int _id);
    const char* getVariableDomain(int _nVariableId);
    const char* getNameById(int _nId, SYMBOL_TYPE _type) const;
    map<int, string> getDomainNames() const;
    map<int, int> getVariablesDomains() const;
    Formula getAtom(int _nPredicateId) const;
    Formulas* getAtomList() const;
};

#endif
