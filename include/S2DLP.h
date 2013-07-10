#include "Vocabulary.h"
#include "Formula.h"
#include "Cabalar.h"
#include "HengZhang.h"
#include "Rule.h"

class S2DLP{
private:
	Vocabulary _vocabulary;
	Formula _formula;
	Rule _rule;
public:
	S2DLP();
	void convert(FILE* output);

};
