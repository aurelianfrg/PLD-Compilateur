#pragma once

#include "Symbol.h"
#include <unordered_map>

using namespace std;

class SymbolsTable {
    public:
        SymbolsTable() {}
        bool contains(const string & name) {
            return (table.find(name) != table.end());
        }
        Symbol & access(const string & name) {
            return table.at(name);
        }
        void add(Symbol s) {
            table.insert({s.getName(), s});
        }
    protected:
        unordered_map<string,Symbol> table;
};