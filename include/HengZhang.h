#ifndef _HENGZHANG_H_
#define _HENGZHANG_H_

#include "structs.h"
#include "Formula.h"
#include "Formulas.h"
#include <vector>
#include <string>

using namespace std;

class Formula;
class Formulas;

class HengZhang{
private:
	vector<int> terms_X;
	vector<int> terms_Y;
	vector<int> terms_Z;
	vector<int> terms_MIN;//waste space
	vector<int> terms_MAX;//waste space

	int symbol_s;
	int symbol_t;
	int symbol_MAX;
	int symbol_MIN;
	int symbol_succ;
	
	char* succ[];
	

public:
	//utilities
	int addSymbol(const char* name, SYMBOL_TYPE type, int arity);
	Formula recordQuantifier(Formula originalFml);

	//creators
	Formulas create(Formula originalFml);
	Formula createFormula_1(Formula originalFml);
	Formula createFormula_2(Formula originalFml);
	Formula createFormula_3(Formula originalFml);
	Formula createFormula_4(Formula originalFml);
	Formula createFormula_5(Formula originalFml);
};

#endif