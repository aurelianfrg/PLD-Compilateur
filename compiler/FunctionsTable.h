#pragma once

#include "Function.h"
#include "IR.h"
#include <unordered_map>

using namespace std;

class FunctionsTable {
    public:

        FunctionsTable() {}

        bool contains(const string & name) {
            return (functions.find(name) != functions.end());
        }
        Function & access(const string & name) {
            return functions.at(name);
        }
        void add(Function f) {
            functions.insert({f.getName(), f});
        }

        friend ostream & operator<<(ostream & os, const FunctionsTable & ft) {
            for (auto it : ft.functions) {
                os << it.second << endl;
            }
            return os;
        }

    protected:
        unordered_map<string,Function> functions;  
};