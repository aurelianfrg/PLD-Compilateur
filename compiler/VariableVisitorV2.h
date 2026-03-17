#pragma once

#include <map>
#include <string>
#include <list>
#include <vector>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include "VariablesTable.h"
#include "Variable.h"

class VariableVisitorV2 : public ifccBaseVisitor
{
public:
    virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
    virtual std::any visitBloc(ifccParser::BlocContext *ctx) override;
    virtual std::any visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) override;
    virtual std::any visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx) override;
    virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
    virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override;
    virtual std::any visitDef_item(ifccParser::Def_itemContext *ctx) override;

    virtual std::any visitExpr_comp(ifccParser::Expr_compContext *ctx) override;
    virtual std::any visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) override;
    virtual std::any visitExpr_minus_not(ifccParser::Expr_minus_notContext *ctx) override;
    virtual std::any visitExpr_aff(ifccParser::Expr_affContext *ctx) override;
    virtual std::any visitExpr_mult_div_mod(ifccParser::Expr_mult_div_modContext *ctx) override;
    virtual std::any visitExpr_var(ifccParser::Expr_varContext *ctx) override;
    virtual std::any visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *ctx) override;
    virtual std::any visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx) override;
    virtual std::any visitExpr_const(ifccParser::Expr_constContext *ctx) override;

    void newVariableTable(); //Create a new VariableTable and adds it on the list
    void delVariableTable(); //Destroy the last VariableTable and check the use of variables

    Variable & getVariable(string name, int index) {return variablesTableVector[index] -> access(name);}; // index : index of the VariableTable
    
    int getLastIndexOfVariablesTable(string name); //Give the index of the last VariablesTable of a variable
    void clearIndexVariables(list<string> varNames);
    void addIndexVariable(string name,int index);
    
    int getCurrentIndex() {return variablesTableVector.size() - 1;};


    bool getError() { return error; }

protected:
    
    std::vector<VariablesTable*> variablesTableVector;
    std::map<std::string, std::list<int>> indexVariables; // map variables and the index of their VariablesTable
    bool error = false;
};
