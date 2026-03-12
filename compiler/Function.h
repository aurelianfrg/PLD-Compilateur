#pragma once

#include <string>
#include <vector>
#include "Type.h"
#include "Symbol.h"

using namespace std;

class Function {
    public:
        Function(string name, vector<Type> params, Type type_retour) {
            this->name = name;
            this->params = params;
            this->type_retour = type_retour;
        }

        string getName() {return this->name;}
        Type getType() {return this->type_retour;}
        vector<Type> getParams() {return this->params;}

        friend ostream & operator << (ostream & os, const Function & f) {
            os << string("Function (name=") << f.name << string(")");
            return os;
        }

    protected:

        string name;
        Type type_retour;
        vector<Type> params;
};

