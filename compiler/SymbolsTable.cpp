#include "SymbolsTable.h"
#include "IR.h"

using namespace std;

SymbolsTable::SymbolsTable()
{
    this->currentOffset = 0;
    this->temporaryVarCount = 0;
}
SymbolsTable::SymbolsTable(Block *block)
{
    this->block = block;
    this->currentOffset = 0;
    this->temporaryVarCount = 0;
}
SymbolsTable::SymbolsTable(const SymbolsTable &st)
{
    this->block = st.block;
    this->inheritedSymbols = st.inheritedSymbols;
    this->localSymbols = st.localSymbols;
    this->currentOffset = st.currentOffset;
    this->temporaryVarCount = st.temporaryVarCount;
}

bool SymbolsTable::contains(const string &name)
{
    return ((inheritedSymbols.find(name) != inheritedSymbols.end()) or (localSymbols.find(name) != localSymbols.end()));
}
Symbol & SymbolsTable::access(const string &name)
{
    // check for a local variable using that name first (shadowing)
    if (localSymbols.find(name) != localSymbols.end())
        return localSymbols.at(name);
    else
        return inheritedSymbols.at(name);
}
void SymbolsTable::add(Symbol s)
{
    localSymbols.insert({s.getName(), s});
}

int SymbolsTable::newVarOffset(Type type)
{
    currentOffset -= typeSizes.at(type);
    return currentOffset;
}
Symbol & SymbolsTable::create_new_tempvar(Type t)
{
    // create the Symbol inside the table and return a reference to it
    string varName = string("!tmp") + to_string(temporaryVarCount++);
    this->add(Symbol(t, varName, newVarOffset(t)));

    // reserve space for this var in the current function
    Function & currentFunction = block->cfg->functionsTable.access(block->cfg->currentFunctionName);
    currentFunction.addToLocalSize(t);
    
    return this->access(varName);
}
Symbol & SymbolsTable::create_new_var(Type t, string varName)
{
    // create the Symbol inside the table and return a reference to it
    this->add(Symbol(t, varName, newVarOffset(t)));

    // reserve space for this var in the current function
    Function & currentFunction = block->cfg->functionsTable.access(block->cfg->currentFunctionName);
    currentFunction.addToLocalSize(t);

    return this->access(varName);
}

void SymbolsTable::setInheritedSymbols(unordered_map<string, Symbol> inheritedSymbols)
{
    this->inheritedSymbols = inheritedSymbols;
}
void SymbolsTable::setLocalSymbols(unordered_map<string, Symbol> localSymbols)
{
    this->localSymbols = localSymbols;
}
unordered_map<string, Symbol> SymbolsTable::getSymbols()
{
    // get symbols from this table to create a new block from it
    // the new symbols include both local and inherited symbols, with possible shadowing from the local over the inherited ones
    unordered_map<string, Symbol> localSymbolsCopy = localSymbols;
    localSymbolsCopy.merge(inheritedSymbols);
    return localSymbolsCopy;
}
const unordered_map<string, Symbol> & SymbolsTable::getInheritedSymbols()
{
    return inheritedSymbols;
}
const unordered_map<string, Symbol> & SymbolsTable::getLocalSymbols()
{
    return localSymbols;
}
int SymbolsTable::getCurrentOffset()
{
    return currentOffset;
}
int SymbolsTable::getTemporaryVarCount()
{
    return temporaryVarCount;
}
void SymbolsTable::setCurrentOffset(int currentOffset)
{
    this->currentOffset = currentOffset;
}
void SymbolsTable::setTemporaryVarCount(int temporaryVarCount)
{
    this->temporaryVarCount = temporaryVarCount;
}

void SymbolsTable::setBlock(Block *block)
{
    this->block = block;
}

