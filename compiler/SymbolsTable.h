#pragma once

#include "Symbol.h"
#include "IR.h"
#include <unordered_map>

using namespace std;

class SymbolsTable {
    public:

        SymbolsTable() {}
        SymbolsTable(Block* block) {
            this->block = block;
        }

        bool contains(const string & name) {
            return ((inheritedSymbols.find(name) != inheritedSymbols.end()) or (localSymbols.find(name) != localSymbols.end()));
        }
        Symbol & access(const string & name) {
            // check for a local variable using that name first (shadowing)
            if (localSymbols.find(name) != localSymbols.end()) return localSymbols.at(name);
            else return inheritedSymbols.at(name);
        }
        void add(Symbol s) {
            localSymbols.insert({s.getName(), s});
        }

	    int newVarOffset(Type type) {
            currentOffset -= typeSizes.at(type);
	        return currentOffset;
        }
        Symbol &create_new_tempvar(Type t) {
            // create the Symbol inside the table and return a reference to it
            string varName = string("!tmp") + to_string(temporaryVarCount++);
            this->add(Symbol(t, varName, newVarOffset(t)));
            return this->access(varName);
        } 
        Symbol &create_new_var(Type t, string varName) {
            // create the Symbol inside the table and return a reference to it
            this->add(Symbol(t, varName, newVarOffset(t)));
            return this->access(varName);
        }

        void setInheritedSymbols(unordered_map<string,Symbol> inheritedSymbols) {
            this->inheritedSymbols = inheritedSymbols;
        }
        void setLocalSymbols(unordered_map<string,Symbol> localSymbols) {
            this->localSymbols = localSymbols;
        }
        unordered_map<string,Symbol> getSymbols() {
            // get symbols from this table to create a new block from it
            // the new symbols include both local and inherited symbols, with possible shadowing from the local over the inherited ones
            unordered_map<string,Symbol> localSymbolsCopy = localSymbols;
            localSymbolsCopy.merge(inheritedSymbols);
            return localSymbolsCopy;
        }
        const unordered_map<string,Symbol>& getInheritedSymbols() {
            return inheritedSymbols;
        }
        const unordered_map<string,Symbol>& getLocalSymbols() {
            return localSymbols;
        }
        int getCurrentOffset() {
            return currentOffset;
        }
        int getTemporaryVarCount() {
            return temporaryVarCount;
        }
        void setCurrentOffset(int currentOffset) {
            this->currentOffset = currentOffset;
        }
        void setTemporaryVarCount(int temporaryVarCount) {
            this->temporaryVarCount = temporaryVarCount;
        }

    protected:
        Block* block;
        unordered_map<string,Symbol> inheritedSymbols;  // symbols originating from a parent block    
        unordered_map<string,Symbol> localSymbols;      // symbols originating from inside the block or a sibling block (ex : the blocks before and after a if/else statement are sibling = they share the same scope)
        
        int currentOffset = 0;
        int temporaryVarCount = 0;
        unordered_map<Type, int> typeSizes = {
            {Type::INT, 4}
        };
};