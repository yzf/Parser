#ifndef _S2DLP_H_
#define _S2DLP_H_


#include "Vocabulary.h"
#include "Formula.h"
#include "Cabalar.h"
#include "HengZhang.h"
#include "Rule.h"
#include <vector>

class S2DLP{
public:
	Formulas origin_formulas;
        Formulas zhangheng_formulas;
        Formulas dlp_formulas;
	vector<Rule> dlp_rules;
        FILE* output_file;
public:
	S2DLP();
	void convert(FILE* output);
        void output_origin_formulas();
        void output_zhangheng_formulas();
        void output_dlp_formulas();
        void set_output_file(FILE*&);
        void set_origin_formulas(_formula*);
};

#endif
