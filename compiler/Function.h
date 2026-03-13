#pragma once

#include <string>
#include <vector>
#include "Type.h"
#include "Symbol.h"

using namespace std;

class Function {
    public:
        Function(string name, vector<Type> params, Type return_type) {
            this->name = name;
            this->params = params;
            this->return_type = return_type;
        }

        string getName() {return this->name;}
        Type getType() {return this->return_type;}
        vector<Type> getParams() {return this->params;}

        friend ostream & operator << (ostream & os, const Function & f) {
            os << typeToString.at(f.return_type) << " " << f.name << "(";
            for (int i = 0; i+1 < f.params.size(); ++i) {   
                // CAUTION : condition must be written this way because f.params.size() is unsigned, thus f.params.size()-1 caused an overflow
                os << typeToString.at(f.params.at(i)) << ", ";
            }
            if (f.params.size() > 0) {
                os << typeToString.at(f.params.back());
            }
            os << ")";
            return os;
        }

    protected:

        string name;
        Type return_type;
        vector<Type> params;
};

