#include "Formulas.h"
#include "Formula.h"
#include <cstdlib>
#include <deque>

using namespace std;


Formulas::Formulas(deque<Formula> _fs) {
    this->_formulas = _fs;
}

Formulas::Formulas()
{
    
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

bool Formulas::equal(Formulas f)
{
    if(this->_formulas.size() != f.get_formulas().size())
        return false;
    
//    deque<Formula>::iterator it = this->_formulas.begin();
//    deque<Formula>::iterator itf = f.get_formulas().begin();
//    while(it != this->_formulas.end())
//    {
//        if(*it != *itf)
//            return false;
//        it++;
//        itf++;
//    }
    
    int size = this->_formulas.size();
    deque<Formula> f1 = this->_formulas;
    deque<Formula> f2 = f.get_formulas();
    
    for(int i = 0; i < size; i++)
    {
        
        if(!f1.at(i).compare(f2.at(i)))
            return false;
    }
    
    return true;
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