#ifndef VOCABULARY_H
#define VOCABULARY_H

#include "structs.h"
#include <cstdio>
#include <vector>
#include <map>
#include <string>

using namespace std;

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
     
    
public:   
    vector<_formula*> atom_list;

private:
    Vocabulary();
    Vocabulary(const Vocabulary& rhs){}
    Vocabulary& operator =(const Vocabulary& rhs){return *this;}
    ~Vocabulary();

public:
    
    void add_atom(_formula*);
    _formula* get_atom(int predicate_id);
    
    static Vocabulary& instance();
    void addIntensionPredicate(const char*_name);
    const char* getVariableDomain(int _variableId);
    void setVariableDomain(const char* _variable, const char* _domain);
    int getSymbolId(const char* _name, SYMBOL_TYPE _type);
    int addSymbol(const char* _name, SYMBOL_TYPE _type, int _arity = 0);
    int getPredicateArity(int _id);
    int getFunctionArity(int _id);
    int addRenameVariable();
    bool isIntensionPredicate(int _predicateId);
    const char* getNameById(int _id, SYMBOL_TYPE _type) const;
    void dumpVocabulary(FILE* _out);
};

#endif
