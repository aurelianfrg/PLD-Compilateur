#include "VariableVisitor.h"

#include <string>

std::any VariableVisitor::visitProg(ifccParser::ProgContext *ctx)
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

std::any VariableVisitor::visitFunction_def(ifccParser::Function_defContext *ctx)
{
    int line = ctx->getStart()->getLine();
    firstBloc = true;


    newVariableTable();
    string functionType;
    if(ctx -> type_function() -> TYPE() == nullptr){
        functionType = ctx -> type_function() -> getText();
    }
    else{
        functionType = ctx -> type_function() -> TYPE() -> getText();
    }

    std::string functionName = ctx -> VAR(0) -> getText();
    if(functionTable.find(functionName) == functionTable.end() ){
        functionTable[functionName] = std::pair<int,string>((ctx -> VAR()).size() - 1,functionType);
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

std::any VariableVisitor::visitBloc(ifccParser::BlocContext *ctx)
{
    if(not firstBloc and not onLoopWithOwnVariableTable){
        newVariableTable();
    }
    else{
        firstBloc = false;
    }
    
    visitChildren(ctx);
    
    if(not onLoopWithOwnVariableTable){
        delVariableTable();
    }

    return 0;
}

std::any VariableVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx)
{

    std::string fullText = ctx->getText();
    if (fullText.size() >= 5 && fullText.substr(0, 5) == "const")
    {
        constVar = true;
    }
    for (auto item : ctx->def_item())
    {
        this->visit(item);
    }
    constVar = false;
    return 0;
}

std::any VariableVisitor::visitDef_item(ifccParser::Def_itemContext *ctx)
{
    std::string res = "0";
    
    std::string varName = ctx->VAR()->getText();
    int line = ctx->getStart()->getLine();
    varConst[varName] = constVar;
    if (isKeyword(varName))
    {
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' is a reserved keyword." << std::endl;
        error = true;
    }
    
    else if (getLastIndexOfVariablesTable(varName) == getCurrentIndex())
    {
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' already declared in this scope." << std::endl;
        error = true;
    }

    else {
        variablesTableVector.back() -> add(Variable(varName, line));
        addIndexVariable(varName, getCurrentIndex());
        res = varName;
    }


    if (ctx->expr() != nullptr)
    {
        std::string varNameExpr = any_cast<string>(this->visit(ctx->expr()));
        if(varNameExpr == "void")
        {
            int line = ctx->getStart()->getLine();
            std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m The function is of type 'void' and cannot be assigned to a variable.\n";
            error = true;
        }
        
        else if(varNameExpr != "int" && varNameExpr != "0")
        {
            Variable & var = variablesTableVector[getLastIndexOfVariablesTable(varNameExpr)] -> access(varNameExpr);
            var.use();
        }
    }

    return res;
}


std::any VariableVisitor::visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx){
    bool onSwitchAct = onSwitch;
    onSwitch = true;

    visitChildren(ctx);

    onSwitch = onSwitchAct;
    return 0;
}

std::any VariableVisitor::visitCase_item(ifccParser::Case_itemContext *ctx){

    newVariableTable();
    
    for(auto instr : ctx -> instruction())
    {
        this -> visit(instr);
    }
    
    delVariableTable();

    return 0;
}

std::any VariableVisitor::visitCase_default(ifccParser::Case_defaultContext *ctx){

    newVariableTable();
    
    for(auto instr : ctx -> instruction())
    {
        this -> visit(instr);
    }
    
    delVariableTable();

    return 0;
}


std::any VariableVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx){
    bool onLoopAct = onLoop;
    onLoop = true;

    visitChildren(ctx);

    onLoop = onLoopAct;
    return 0;
}

std::any VariableVisitor::visitDowhile_stmt(ifccParser::Dowhile_stmtContext *ctx){
    bool onLoopWithOwnVariableTableAct = onLoopWithOwnVariableTable;
    bool onLoopAct = onLoop;
    
    onLoopWithOwnVariableTable = true;
    onLoop = true;
    
    newVariableTable();
    
    this -> visit(ctx->bloc());
    this -> visit(ctx->expr());
    
    delVariableTable();

    onLoopWithOwnVariableTable = onLoopWithOwnVariableTableAct;
    onLoop = onLoopAct;

    return 0;
}

std::any VariableVisitor::visitFor_stmt(ifccParser::For_stmtContext *ctx){
    bool onLoopWithOwnVariableTableAct = onLoopWithOwnVariableTable;
    bool onLoopAct = onLoop;
    
    onLoopWithOwnVariableTable = true;
    onLoop = true;
    
    newVariableTable();
    
    visitChildren(ctx);
    
    delVariableTable();

    onLoopWithOwnVariableTable = onLoopWithOwnVariableTableAct;
    onLoop = onLoopAct;

    return 0;
}

std::any VariableVisitor::visitBreak_stmt(ifccParser::Break_stmtContext *ctx)
{
    if(not onLoop and not onSwitch){
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m a break statement may only be used within a loop or switch.\n";
        error = true;
    }

    return 0;
}

std::any VariableVisitor::visitContinue_stmt(ifccParser::Continue_stmtContext *ctx)
{
    if(not onLoop){
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m a continue statement may only be used within a loop.\n";
        error = true;
    }

    return 0;
}

//////////////////////////////////////////EXPRESSIONS ////////////////////////////////////////////

std::any VariableVisitor::visitExpr_postfix(ifccParser::Expr_postfixContext *ctx)
{
    this -> visit(ctx -> expr());
    return string("0");
}

std::any VariableVisitor::visitExpr_aff(ifccParser::Expr_affContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    std::string res = varName;

    if (indexVariables.find(varName) == indexVariables.end())
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' not declared in this scope.\n";
        error = true;
        res = "0";
    }
    
    else if(ctx -> OP -> getText() != "="){
        Variable & var = variablesTableVector[getLastIndexOfVariablesTable(varName)] -> access(varName);
        var.use();
    }

    if (varConst[varName])
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' is constant and cannot be reassigned.\n";
        error = true;
    }

    std::string varNameExpr = any_cast<string>(this->visit(ctx->expr()));

    if(varNameExpr == "void")
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m The function is of type 'void' and cannot be assigned to a variable.\n";
        error = true;
    }
    
    else if(varNameExpr != "int" && varNameExpr != "0")
    {
        Variable & var = variablesTableVector[getLastIndexOfVariablesTable(varNameExpr)] -> access(varNameExpr);
        var.use();
    }

    return res;
}

std::any VariableVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx){
    return string("0");
}

std::any VariableVisitor::visitExpr_call(ifccParser::Expr_callContext *ctx){
    
    return this -> visit(ctx -> call());
}

std::any VariableVisitor::visitExpr_var(ifccParser::Expr_varContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    std::string res = varName;

    if (indexVariables.find(varName) == indexVariables.end())
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' not declared in this scope.\n";
        error = true;
        res = "0";
    }

    else
    {
        Variable & var = variablesTableVector[getLastIndexOfVariablesTable(varName)] -> access(varName);
        var.use();
    }

    return res;
}

std::any VariableVisitor::visitCall(ifccParser::CallContext *ctx){
    int line = ctx->getStart()->getLine();
    std::string functionName = ctx -> VAR() -> getText();

    if(functionTable.find(functionName) == functionTable.end())
    {
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Function '" << functionName << "' undefined.\n";
        error = true;
        return string("0");
    }
    else{
        int NbParameters = functionTable[functionName].first;
        if((ctx -> expr()).size() != NbParameters)
        {
            std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m " << NbParameters << " parameters expected but only "<<(ctx -> expr()).size()<<" are given.\n";
            error = true;
        }
    }

    return functionTable[functionName].second;
}

std::any VariableVisitor::visitExpr_char(ifccParser::Expr_charContext *ctx){
    return string("0");
}

//////////////////////////////////////////UTILS ////////////////////////////////////////////

void VariableVisitor::newVariableTable()
{
    VariablesTable * variableTablePointer = new VariablesTable();
    variablesTableVector.push_back(variableTablePointer);
}

void VariableVisitor::delVariableTable()
{
    VariablesTable * variableTablePointer= variablesTableVector.back();
    variableTablePointer -> checkUsage();
    list<string> varNames = variableTablePointer -> getVarNames();
    clearIndexVariables(varNames);
    variablesTableVector.pop_back();
    delete variableTablePointer;    
}

int VariableVisitor::getLastIndexOfVariablesTable(string name)
{
    if(indexVariables.find(name) != indexVariables.end()){
        return indexVariables[name].back();
    }
    
    else
    {
        return -1;
    }
}

void VariableVisitor::clearIndexVariables(list<string> varNames)
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

void VariableVisitor::addIndexVariable(string name, int index){
    
    if(indexVariables.find(name) == indexVariables.end()){
        
        indexVariables[name] = std::list<int>();
    }

    indexVariables[name].push_back(index);
}

VariableVisitor::VariableVisitor(){
    variablesTableVector = std::vector<VariablesTable*>();
    indexVariables = std::map<std::string, std::list<int>>();
    functionTable = std::map<std::string,std::pair<int,string>>();
    functionTable["getchar"] = std::pair<int,string>(0,"int");
    functionTable["putchar"] = std::pair<int,string>(1,"void");
    firstBloc = false;
    onLoopWithOwnVariableTable = false;
    onLoop = false;
    onSwitch = false;
    error = false;
}