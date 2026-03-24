#pragma once

#include <map>
#include <string>
#include <list>
#include <vector>
#include <set>
#include <utility>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include "VariablesTable.h"
#include "Variable.h"

// Définition du set statique
const unordered_set<string> keywords = {
    // Standard C keywords
    "alignas", "alignof", "auto", "bool", "break", "case", "char", "const",
    "constexpr", "continue", "default", "do", "double", "else", "enum",
    "extern", "false", "float", "for", "goto", "if", "inline", "int",
    "long", "nullptr", "register", "restrict", "return", "short", "signed",
    "sizeof", "static", "static_assert", "struct", "switch", "thread_local",
    "true", "typedef", "typeof", "typeof_unqual", "union", "unsigned",
    "void", "volatile", "while",
    
    // Underscore-prefixed keywords
    "_Alignas", "_Alignof", "_Atomic", "_BitInt", "_Bool", "_Complex",
    "_Decimal32", "_Decimal64", "_Decimal128", "_Generic", "_Imaginary",
    "_Noreturn", "_Static_assert", "_Thread_local", "_Pragma",
    
    // Extensions
    "asm", "fortran"
};

class VariableVisitorV2 : public ifccBaseVisitor
{
public:
    virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
    virtual std::any visitFunction_def(ifccParser::Function_defContext *ctx) override;
    virtual std::any visitBloc(ifccParser::BlocContext *ctx) override;
    virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override;
    virtual std::any visitDef_item(ifccParser::Def_itemContext *ctx) override;
    virtual std::any visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) override;
    virtual std::any visitCase_item(ifccParser::Case_itemContext *ctx) override;
    virtual std::any visitCase_default(ifccParser::Case_defaultContext *ctx) override;
    virtual std::any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;
    virtual std::any visitDowhile_stmt(ifccParser::Dowhile_stmtContext *ctx) override;
    virtual std::any visitFor_stmt(ifccParser::For_stmtContext *ctx) override;
    virtual std::any visitBreak_stmt(ifccParser::Break_stmtContext *ctx) override;
    virtual std::any visitContinue_stmt(ifccParser::Continue_stmtContext *ctx) override;



    virtual std::any visitExpr_postfix(ifccParser::Expr_postfixContext *ctx) override;
    virtual std::any visitExpr_aff(ifccParser::Expr_affContext *ctx) override;
    virtual std::any visitExpr_var(ifccParser::Expr_varContext *ctx) override;
    virtual std::any visitExpr_const(ifccParser::Expr_constContext *ctx) override;
    virtual std::any visitExpr_call(ifccParser::Expr_callContext *ctx) override;
    virtual std::any visitExpr_char(ifccParser::Expr_charContext *ctx) override;

    virtual std::any visitCall(ifccParser::CallContext *ctx) override;

    void newVariableTable(); //Create a new VariableTable and adds it on the list
    void delVariableTable(); //Destroy the last VariableTable and check the use of variables

    Variable & getVariable(string name, int index) {return variablesTableVector[index] -> access(name);}; // index : index of the VariableTable
    
    int getLastIndexOfVariablesTable(string name); //Give the index of the last VariablesTable of a variable
    void clearIndexVariables(list<string> varNames);
    void addIndexVariable(string name,int index);
    
    int getCurrentIndex() {return variablesTableVector.size() - 1;};

    bool getError() { return error; }

    bool isKeyword(string& name) { return keywords.find(name) != keywords.end(); }

    VariableVisitorV2();

protected:
    
    std::vector<VariablesTable*> variablesTableVector;
    std::map<std::string, std::list<int>> indexVariables; // map variables and the index of their VariablesTable
    std::map<std::string,std::pair<int,string>> functionTable;
    std::map<std::string, bool> varConst;
    bool constVar = false;                   
    bool firstBloc;
    bool onLoopWithOwnVariableTable;
    bool onLoop;
    bool onSwitch;
    bool error;
};
