#include "Vocabulary.h"
#include "Formula.h"
#include "Cabalar.h"
#include "HengZhang.h"
#include "Rule.h"

class S2DLP{
private:
	Formulas origin_formulas;
        Formulas zhangheng_formulas;
        Formulas dlp_formulas;
	Rule dlp_rules;
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
