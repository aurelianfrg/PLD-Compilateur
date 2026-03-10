#include "IRVisitor.h"

using namespace std;

IRVisitor::IRVisitor(tree::ParseTree *parseTree)
{
    cfg = new CFG(parseTree);
}

// antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx)
// {
//     this->visit( ctx->bloc() );
//     return 0;
// }

std::any IRVisitor::visitInstruction_aff_stmt(ifccParser::Instruction_aff_stmtContext *ctx)
{
    return visitChildren(ctx);
}
std::any IRVisitor::visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx)
{
    return visitChildren(ctx);
}
std::any IRVisitor::visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx)
{
    return visitChildren(ctx);
}

antlrcpp::Any IRVisitor::visitBloc(ifccParser::BlocContext *ctx)
{
    // cout << "visitblock" << endl;
    BasicBlock *firstBlock = cfg->createBasicBlock();
    this->visit(ctx->children[0]);
    return visitChildren(ctx);
}

std::any IRVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    // cout << "visit return" << endl;
    // resolve return value expression
    string tempVarName = any_cast<string>(this->visit(ctx->expr()));
    cfg->current_bb->add_IRInstr(IRInstr::ret, Type::INT, {tempVarName});
    return 0;
}

std::any IRVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx)
{
    // cout << "visit expr const" << endl;
    string value = ctx->CONST()->getText();
    Symbol &tempVar = cfg->create_new_tempvar(Type::INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, Type::INT, {value, tempVar.getName()});

    return tempVar.getName();
}

std::any IRVisitor::visitExpr_var(ifccParser::Expr_varContext *ctx)
{
    string value = ctx->VAR()->getText();

    return value;
}
// std::any IRVisitor::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) {}
// std::any IRVisitor::visitExpr_minus(ifccParser::Expr_minusContext *ctx) {}
std::any IRVisitor::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx)
{

    string tempVarName1 = any_cast<string>(this->visit(ctx->expr(0)));
    string op = ctx->OP()->getText();
    string tempVarName2 = any_cast<string>(this->visit(ctx->expr(1)));
    Symbol &tempVar = cfg->create_new_tempvar(Type::INT);

    if (op == "+")
    {
        cfg->current_bb->add_IRInstr(IRInstr::add, Type::INT, {tempVar.getName(), tempVarName1, tempVarName2});
    }

    else if (op == "-")
    {
        cfg->current_bb->add_IRInstr(IRInstr::sub, Type::INT, {tempVar.getName(), tempVarName1, tempVarName2});
    }

    return tempVar.getName();
}
// std::any IRVisitor::visitExpr_mult(ifccParser::Expr_multContext *ctx) {}
// std::any IRVisitor::visitAff_stmt(ifccParser::Aff_stmtContext *ctx) {}
// std::any IRVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx) {}
