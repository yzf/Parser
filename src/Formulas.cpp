#include "Formulas.h"
#include "Formula.h"
#include <cstdlib>
#include <deque>

using namespace std;

Formulas::Formulas() {
    
}

Formulas::Formulas(deque<Formula> _fs) {
    this->_formulas = _fs;
}

Formulas::~Formulas() {
    while(!(this->_formulas.empty())) {
        this->_formulas.pop_back();
    }
}

deque<Formula>  Formulas::get_formulas()
{
    return this->_formulas;
}
      

void Formulas::set_formulas(deque<Formula> fs)
{
    this->_formulas = fs;
}

void Formulas::join_formulas(Formulas tail)
{
    
    deque<Formula> temphead = this->_formulas;
    deque<Formula> temptail = tail.get_formulas();
    
    while(!temptail.empty())
    {
        temphead.push_back(temptail.front());
        temptail.pop_front();
    }
    
    this->set_formulas(temphead);
}

void Formulas::push_formula(Formula tail)
{  
    this->_formulas.push_front(tail);
}
	
void Formulas::pop_formula()//DO NOT DELETE formula
{
    this->_formulas.pop_front();
}

Formula  Formulas::top_formula()
{
    return this->_formulas.front();
}
        	
void Formulas::copy_formulas(Formulas sigma)       
{
    this->set_formulas(sigma.get_formulas());
}
        	
void Formulas::delete_formulas()        
{
    this->get_formulas().clear();
}
        
int Formulas::size_formulas()      
{
    return this->_formulas.size();
}

void Formulas::output_formulas(FILE* out)       
{
    deque<Formula> tempfmls;
    tempfmls = this->_formulas;
    while(!tempfmls.empty())
    {
        tempfmls.front().output(out);
        tempfmls.pop_front();
    } 
}