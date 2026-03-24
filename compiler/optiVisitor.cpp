#include "OptiVisitor.h"

std::any OptiVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitBloc(ifccParser::BlocContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitInstruction_aff_stmt(ifccParser::Instruction_aff_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitAff_stmt(ifccParser::Aff_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitDef_item(ifccParser::Def_itemContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitExpr_comp(ifccParser::Expr_compContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitExpr_minus(ifccParser::Expr_minusContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitExpr_aff(ifccParser::Expr_affContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitExpr_mult_div_mod(ifccParser::Expr_mult_div_modContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitExpr_var(ifccParser::Expr_varContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *ctx)
{
    return visitChildren(ctx);
}

std::any OptiVisitor::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx)
{
    return visitChildren(ctx);
}

