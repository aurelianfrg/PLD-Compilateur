#pragma once

class SymbolsTable;

#include "Symbol.h"
#include "Type.h"
#include <unordered_map>

class Block;

using namespace std;

class SymbolsTable
{
public:
    SymbolsTable();
    
    SymbolsTable(Block *block);
    
    SymbolsTable(const SymbolsTable &st);

    bool contains(const string &name);
    Symbol &access(const string &name);
    void add(Symbol s);
    int newVarOffset(Type type);
    Symbol &create_new_tempvar(Type t);
    Symbol &create_new_var(Type t, string varName);

    void setInheritedSymbols(unordered_map<string, Symbol> inheritedSymbols);
    void setLocalSymbols(unordered_map<string, Symbol> localSymbols);
    unordered_map<string, Symbol> getSymbols();
    const unordered_map<string, Symbol> &getInheritedSymbols();
    const unordered_map<string, Symbol> &getLocalSymbols();
    int getCurrentOffset();
    int getTemporaryVarCount();
    void setCurrentOffset(int currentOffset);
    void setTemporaryVarCount(int temporaryVarCount);

    void setBlock(Block *block);

protected:
    Block *block;
    unordered_map<string, Symbol> inheritedSymbols; // symbols originating from a parent block
    unordered_map<string, Symbol> localSymbols;     // symbols originating from inside the block or a sibling block (ex : the blocks before and after a if/else statement are sibling = they share the same scope)

    int currentOffset;
    int temporaryVarCount;
};
