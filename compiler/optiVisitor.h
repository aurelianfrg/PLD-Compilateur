#pragma once

#include <iostream>
#include <sstream>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

using namespace std;

class OptiVisitor : public ifccBaseVisitor
{
public:
    OptiVisitor(stringstream in) {this.in = in};

    virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
    virtual std::any visitBloc(ifccParser::BlocContext *ctx) override;
    virtual std::any visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) override;
    virtual std::any visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx) override;
    virtual std::any visitInstruction_aff_stmt(ifccParser::Instruction_aff_stmtContext *ctx) override;
    virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
    virtual std::any visitAff_stmt(ifccParser::Aff_stmtContext *ctx) override;
    virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override;
    virtual std::any visitDef_item(ifccParser::Def_itemContext *ctx) override;

    virtual std::any visitExpr_comp(ifccParser::Expr_compContext *ctx) override;
    virtual std::any visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) override;
    virtual std::any visitExpr_minus(ifccParser::Expr_minusContext *ctx) override;
    virtual std::any visitExpr_aff(ifccParser::Expr_affContext *ctx) override;
    virtual std::any visitExpr_mult_div_mod(ifccParser::Expr_mult_div_modContext *ctx) override;
    virtual std::any visitExpr_var(ifccParser::Expr_varContext *ctx) override;
    virtual std::any visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *ctx) override;
    virtual std::any visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx) override;

protected:
    stringstream in;
    stringstream out;
};