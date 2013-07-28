/* 
 * File:   Util.h
 * Author: yzf
 *
 * Created on July 27, 2013, 1:33 AM
 */

#ifndef UTIL_H
#define	UTIL_H

#include "structs.h"
#include "utility.h"
#include <map>
#include <vector>

using namespace std;

/**
 * 工具类
 */
class Utils {
public:
    
    //对term的操作
    static void outputTerm(FILE* _out, const _term* _t);
    static void replaceTerm(_term* _ts, int _arity, const vector<int>& _originals, 
                        const vector<int>& _replacements);
    static _term* combineTerms(const vector<int>& _head, const vector<int>& _tail);
    static bool compareTerm(const _term* _lhs, const _term* _rhs);
    static _term* copyTerms(const _term* _ts, int _size);
    static void deleteTerms(_term* _ts, int _size);
    static void renameTermVariables(_term* _t, int _oldVariableId, int _newVariableId);
    //对formula的操作
    static void outputFormula(FILE* _out, const _formula* _fml);
    static bool compareFormula(const _formula* _lhs, const _formula* _rhs);
    static _formula* copyFormula(const _formula* _fml);
    static void deleteFormula(_formula* _fml);
    static bool isUniversal(_formula* _fml);
    static _formula* findPrenexQuanlifier(_formula* _fml, int _variableId);
    static void removeFromPrenex(_formula* _parent, int _d, _formula* _tag);
    static void renameFormulaVariables(_formula* _fml, int _oldVariableId, int _newVariableId);
    static void convertToPrenex(_formula* _fml);
    static void getNoQuantifierVariables(map<int, bool>& _flag, vector<int>& _varis, _formula* _fml);
    static bool isNegativeFormula(_formula* _fml, bool _negative);
    static _formula* doubleNegationIntensionPredicates(_formula* _fml);
    static void replaceFormulaTerms(_formula* _fml, 
                                const vector<int>& _originals, 
				const vector<int>& _replacements);
    
    static _formula* compositeByConnective(FORMULA_TYPE _formulaType, 
                         _formula* _subformulaL, _formula* _subformulaR);
    static _formula* compositeByQuantifier(FORMULA_TYPE _formulaType, 
                         _formula* _subformulaL, int _variableId);
    static _formula* compositeToAtom(int _predicateId, _term* _parameters);
    
};



#endif	/* UTIL_H */

