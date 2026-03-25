#pragma once

#include <string>
#include <vector>
#include "Type.h"
#include "Symbol.h"

using namespace std;

class Function {
    public:
        Function(string name, Type returnType, vector<Type> paramsType, vector<string> paramsName) {
            this->name = name;
            this->returnType = returnType;
            this->paramsType = paramsType;
            this->paramsName = paramsName;
            this->localSize = 0;
            // for now we can only handle 6 parameters at most
            if (paramsType.size() > 6) {
                cerr << "Unable to handle function with more than 6 parameters for now" << endl;
            }
        }

        string getName() {return this->name;}
        Type getType() {return this->returnType;}
        vector<Type> getParamsType() {return this->paramsType;}
        vector<string> getParamsName() {return this->paramsName;}

        unsigned int getLocalSize() {
            // round total up to 16 bytes (AMD64 ABI REQUIREMENT)
            return localSize + (16 - localSize % 16);
        }
        void addToLocalSize(Type type) {
            this->localSize += typeSizes.at(type);
        }

        friend ostream & operator << (ostream & os, const Function & f) {
            os << typeToString.at(f.returnType) << " " << f.name << "(";
            for (int i = 0; i+1 < f.paramsType.size(); ++i) {   
                // CAUTION : condition must be written this way because f.params.size() is unsigned, thus f.params.size()-1 caused an overflow
                os << typeToString.at(f.paramsType.at(i)) << ", ";
            }
            if (f.paramsType.size() > 0) {
                os << typeToString.at(f.paramsType.back());
            }
            os << ")";
            return os;
        }

    protected:

        string name;
        Type returnType;
        vector<Type> paramsType;
        vector<string> paramsName;
        unsigned int localSize;
};

