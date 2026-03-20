
#include <iostream>
#include "VariablesTable.h"

void VariablesTable::checkUsage()
{
    for (auto &[key, value] : table)
    {
        if (value.used() == false)
        {
            std::cerr << "\e[35mWarning :\e[39m variable '" << key << "' defined on line " << value.getLine() <<" is never used.\n";
        }
    }
}

list<string> VariablesTable::getVarNames()
{
    list<string> varNames;
    for(auto &[key, value] : table)
    {
        varNames.push_back(key);
    }
    return varNames;
}