#include "S2DLP.h"

S2DLP::S2DLP() {
   /* this->origin_formulas = NULL;
    this->zhangheng_formulas = NULL;
    this->dlp_formulas = NULL;
    this->dlp_rules = NULL;*/
}

void S2DLP::set_output_file(FILE*& f) {
    this->output_file = f;
}
void S2DLP::output_origin_formulas() {
    fprintf(this->output_file, "origin_formulas:\n");
    this->origin_formulas.output_formulas(this->output_file);
}
void S2DLP::output_zhangheng_formulas() {    
    fprintf(this->output_file, "zhangheng_formulas:\n");
    this->zhangheng_formulas.output_formulas(this->output_file);
}
void S2DLP::output_dlp_formulas() {   
    fprintf(this->output_file, "dlp_formulas:\n");
    this->dlp_formulas.output_formulas(this->output_file);
}
void S2DLP::set_origin_formulas(_formula* input) {
    Formula* origin = new Formula(input);
    this->origin_formulas = origin->divide_clause();
}