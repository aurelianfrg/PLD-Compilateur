#include "VariableVisitorV2.h"

#include <string>

std::any VariableVisitorV2::visitProg(ifccParser::ProgContext *ctx)
{

    for (auto def : ctx->function_def()){
        this -> visit(def);
    }

    if(functionTable.find("main") == functionTable.end() ){
        std::cerr << "\e[31mError:\e[39m function 'main' undefined " << std::endl;
        error = true;
    }

    return 0;
}

std::any VariableVisitorV2::visitFunction_def(ifccParser::Function_defContext *ctx)
{
    int line = ctx->getStart()->getLine();
    firstBloc = true;


    newVariableTable();

    std::string functionName = ctx -> VAR(0) -> getText();
    if(functionTable.find(functionName) == functionTable.end() ){
        functionTable[functionName] = (ctx -> VAR()).size() - 1;
    }

    else{
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Function '" << functionName << "' already declared." << std::endl;
        error = true;
    }

    for( int i = 1; i<(ctx -> VAR()).size();++i)
    {
        std::string varName = ctx -> VAR(i) -> getText();
        if(indexVariables.find(varName) == indexVariables.end() ){
            addIndexVariable(varName,getCurrentIndex());
            variablesTableVector.back() -> add(Variable(varName, line));
        }

        else{
            std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' already declared as function's parameter." << std::endl;
            error = true;
        }
    }

    this -> visit(ctx -> bloc());    

    return 0;
}

std::any VariableVisitorV2::visitBloc(ifccParser::BlocContext *ctx)
{
    if(not firstBloc){
        newVariableTable();
    }
    else{
        firstBloc = false;
    }
    
    visitChildren(ctx);
    
    delVariableTable();

    return 0;
}

std::any VariableVisitorV2::visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitDef_stmt(ifccParser::Def_stmtContext *ctx)
{
    for (auto item : ctx->def_item())
    {
        this->visit(item);
    }
    return 0;
}

std::any VariableVisitorV2::visitDef_item(ifccParser::Def_itemContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    int line = ctx->getStart()->getLine();

    if (getLastIndexOfVariablesTable(varName) == getCurrentIndex())
    {
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' already declared in this scope." << std::endl;
        error = true;
    }

    variablesTableVector.back() -> add(Variable(varName, line));
    addIndexVariable(varName, getCurrentIndex());
    
    if (ctx->expr() != nullptr)
    {
        std::string varNameExpr = any_cast<string>(this->visit(ctx->expr()));
        if(varNameExpr != "0")
        {
            Variable & var = variablesTableVector[getLastIndexOfVariablesTable(varNameExpr)] -> access(varNameExpr);
            var.use();
        }
    }

    return 0;
}

std::any VariableVisitorV2::visitExpr_comp(ifccParser::Expr_compContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitExpr_minus_not(ifccParser::Expr_minus_notContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitExpr_mult_div_mod(ifccParser::Expr_mult_div_modContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitorV2::visitExpr_aff(ifccParser::Expr_affContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    if (indexVariables.find(varName) == indexVariables.end())
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' not declared in this scope.\n";
        error = true;
    }
    
    std::string varNameExpr = any_cast<string>(this->visit(ctx->expr()));

    if(varNameExpr != "0")
    {
        Variable & var = variablesTableVector[getLastIndexOfVariablesTable(varNameExpr)] -> access(varNameExpr);
        var.use();
    }

    return varName;
}

std::any VariableVisitorV2::visitExpr_const(ifccParser::Expr_constContext *ctx){
    return string("0");
}

std::any VariableVisitorV2::visitExpr_call(ifccParser::Expr_callContext *ctx){
    
    this -> visit(ctx -> call());
    return string("0");
}

std::any VariableVisitorV2::visitExpr_var(ifccParser::Expr_varContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    if (indexVariables.find(varName) == indexVariables.end())
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' not declared in this scope.\n";
        error = true;
    }

    else
    {
        Variable & var = variablesTableVector[getLastIndexOfVariablesTable(varName)] -> access(varName);
        var.use();
    }

    return varName;
}

// void VariableVisitorV2::printSymbolTable()
// {
//     std::cout << "Symbol Table:\n";
//     for (const auto &[key, value] : varOffsets)
//     {
//         std::cout << "Variable: " << key << ", Offset: " << value << ", Already used: " << varUse[key] << "\n";
//     }
// }


std::any VariableVisitorV2::visitCall(ifccParser::CallContext *ctx){
    int line = ctx->getStart()->getLine();
    std::string functionName = ctx -> VAR() -> getText();

    if(functionTable.find(functionName) == functionTable.end())
    {
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Function '" << functionName << "' undefined.\n";
        error = true;
    }
    else{
        int NbParameters = functionTable[functionName];
        if((ctx -> expr()).size() != NbParameters)
        {
            std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m " << NbParameters << " parameters expected but only "<<(ctx -> expr()).size()<<" are given.\n";
            error = true;
        }
    }

    return 0;
}



void VariableVisitorV2::newVariableTable()
{
    VariablesTable * variableTablePointer = new VariablesTable();
    variablesTableVector.push_back(variableTablePointer);
}

void VariableVisitorV2::delVariableTable()
{
    VariablesTable * variableTablePointer= variablesTableVector.back();
    variableTablePointer -> checkUsage();
    list<string> varNames = variableTablePointer -> getVarNames();
    clearIndexVariables(varNames);
    variablesTableVector.pop_back();
    delete variableTablePointer;    
}

int VariableVisitorV2::getLastIndexOfVariablesTable(string name)
{
    if(indexVariables.find(name) != indexVariables.end()){
        return indexVariables[name].back();
    }
    
    else
    {
        return -1;
    }
}

void VariableVisitorV2::clearIndexVariables(list<string> varNames)
{
    int currentIndex = getCurrentIndex();
    for(auto name : varNames){
        if(getLastIndexOfVariablesTable(name) == currentIndex){
            indexVariables[name].pop_back();
            if(indexVariables[name].empty()){
                indexVariables.erase(name);
            } 
        }
    }
}

void VariableVisitorV2::addIndexVariable(string name, int index){
    
    if(indexVariables.find(name) == indexVariables.end()){
        
        indexVariables[name] = std::list<int>();
    }

    indexVariables[name].push_back(index);
}