#include <vector>
#include <string>

using namespace std;

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
	int HengZhang_addSymbol(const char* name, SYMBOL_TYPE type, int arity);
	Formula HengZhang_recordQuantifier(Formula originalFml);

	//creators
	static Formulas HengZhang_create(Formula originalFml);
	Formula HengZhang_createFormula_1(Formula originalFml);
	Formula HengZhang_createFormula_2(Formula originalFml);
	Formula HengZhang_createFormula_3(Formula originalFml);
	Formula HengZhang_createFormula_4(Formula originalFml);
	Formula HengZhang_createFormula_5(Formula originalFml);
};

