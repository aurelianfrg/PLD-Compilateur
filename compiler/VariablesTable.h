#pragma once

#include "Variable.h"
#include <unordered_map>
#include <list>

using namespace std;

class VariablesTable {
    public:
        VariablesTable() {}
        bool contains(const string & name) {
            return (table.find(name) != table.end());
        }
        Variable & access(const string & name) {
            // clog << "accessed tempvar " << name << endl;
            return table.at(name);
        }
        void add(Variable s) {
            // clog << "added tempvar " << s.getName() << endl;
            table.insert({s.getName(), s});
        }

        void checkUsage();
        list<string> getVarNames();

    protected:
        unordered_map<string,Variable> table;
};