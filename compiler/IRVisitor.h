#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include "IR.h"

using namespace std;
using namespace antlr4;

class IRVisitor : public ifccBaseVisitor {
  public:
    IRVisitor(tree::ParseTree *tree);
    ~IRVisitor();
    virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
    virtual std::any visitBloc(ifccParser::BlocContext *ctx) override;
    virtual std::any visitFunction_def(ifccParser::Function_defContext *ctx) override;
    virtual std::any
    visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx) override;
    virtual std::any
    visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) override;
    virtual std::any
    visitInstruction_if_stmt(ifccParser::Instruction_if_stmtContext *context) override;
    virtual std::any
    visitInstruction_while_stmt(ifccParser::Instruction_while_stmtContext *ctx) override;
    virtual std::any
    visitInstruction_switch_stmt(ifccParser::Instruction_switch_stmtContext *ctx) override;
    virtual std::any
    visitInstruction_break_stmt(ifccParser::Instruction_break_stmtContext *ctx) override;
    virtual std::any
    visitInstruction_continue_stmt(ifccParser::Instruction_continue_stmtContext *ctx) override;
    virtual std::any visitInstruction_bloc(ifccParser::Instruction_blocContext *ctx) override;
    virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
    virtual std::any visitBreak_stmt(ifccParser::Break_stmtContext *ctx) override;
    virtual std::any visitContinue_stmt(ifccParser::Continue_stmtContext *ctx) override;
    virtual std::any visitCall(ifccParser::CallContext *ctx) override;
    virtual std::any visitExpr_const(ifccParser::Expr_constContext *ctx) override;
    virtual std::any visitExpr_comp(ifccParser::Expr_compContext *context) override;
    virtual std::any visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *context) override;
    virtual std::any visitIf_stmt(ifccParser::If_stmtContext *context) override;
    virtual std::any visitExpr_var(ifccParser::Expr_varContext *ctx) override;
    virtual std::any visitExpr_char(ifccParser::Expr_charContext *ctx) override;
    virtual std::any visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) override;
    virtual std::any visitExpr_minus_not(ifccParser::Expr_minus_notContext *ctx) override;
    virtual std::any visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx) override;
    virtual std::any visitExpr_and(ifccParser::Expr_andContext *context) override;
    virtual std::any visitExpr_xor(ifccParser::Expr_xorContext *context) override;
    virtual std::any visitExpr_or(ifccParser::Expr_orContext *context) override;
    virtual std::any visitExpr_mult_div_mod(ifccParser::Expr_mult_div_modContext *ctx) override;
    virtual std::any visitExpr_aff(ifccParser::Expr_affContext *ctx) override;
    virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override;
    virtual std::any visitWhile_stmt(ifccParser::While_stmtContext *ctx) override;
    virtual std::any visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) override;
    virtual std::any visitExpr_log_and(ifccParser::Expr_log_andContext *ctx) override;
    virtual std::any visitExpr_log_or(ifccParser::Expr_log_orContext *ctx) override;
    virtual std::any visitExpr_postfix(ifccParser::Expr_postfixContext *ctx) override;

    CFG *cfg;
};
