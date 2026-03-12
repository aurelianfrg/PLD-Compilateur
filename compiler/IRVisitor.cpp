#include "IRVisitor.h"

using namespace std;

IRVisitor::IRVisitor(tree::ParseTree *parseTree)
{
    cfg = new CFG(parseTree);
}

antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx) {
    BasicBlock* firstBlock = cfg->createBasicBlock();
    this->visit( ctx->bloc() );
    return 0;
}

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
std::any IRVisitor::visitInstruction_if_stmt(ifccParser::Instruction_if_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any IRVisitor::visitBloc(ifccParser::BlocContext *ctx)
{
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

std::any IRVisitor::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx)
{
    return visit(ctx->expr());
}

std::any IRVisitor::visitExpr_minus(ifccParser::Expr_minusContext *ctx)
{
    // cout << "visit expr minus" << endl;
    string tempVarName = any_cast<string>(this->visit(ctx->expr()));
    Symbol &tempVar = cfg->create_new_tempvar(Type::INT);
    cfg->current_bb->add_IRInstr(IRInstr::neg, Type::INT, {tempVar.getName(), tempVarName});
    return tempVar.getName();
}

std::any IRVisitor::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx)
{

    string tempVarName1 = any_cast<string>(this->visit(ctx->expr(0)));
    string op = ctx->OP->getText();
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

std::any IRVisitor::visitExpr_mult(ifccParser::Expr_multContext *ctx)
{
    string tempVarName0 = any_cast<string>(this->visit(ctx->expr(0)));
    string tempVarName1 = any_cast<string>(this->visit(ctx->expr(1)));
    Symbol &tempVar = cfg->create_new_tempvar(Type::INT);
    cfg->current_bb->add_IRInstr(IRInstr::mul, Type::INT, {tempVar.getName(), tempVarName0, tempVarName1});

    return tempVar.getName();
}

std::any IRVisitor::visitAff_stmt(ifccParser::Aff_stmtContext *ctx)
{
    // cout << "visit aff stmt" << endl;
    string tempVarName = any_cast<string>(this->visit(ctx->expr()));
    string varName = ctx->VAR()->getText();
    cfg->current_bb->add_IRInstr(IRInstr::copy, Type::INT, {varName, tempVarName});
    return 0;
}

std::any IRVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx)
{
    string varName = ctx->VAR()->getText();
    Symbol &tempVar = cfg->create_new_var(Type::INT, varName);
    if (ctx->expr() != nullptr)
    {
        string tempVarName = any_cast<string>(this->visit(ctx->expr()));
        cfg->current_bb->add_IRInstr(IRInstr::copy, Type::INT, {varName, tempVarName});
    }
    return 0;
}

std::any IRVisitor::visitExpr_comp(ifccParser::Expr_compContext *ctx) {
    string op = ctx->OP->getText();

    // resolve both operands
    string expr1Address = any_cast<string>(this->visit(ctx->expr(0)));
    string expr2Address = any_cast<string>(this->visit(ctx->expr(1)));

    Symbol & resultTempVar = cfg->create_new_tempvar(Type::INT);

    if (op == string("<")) {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_lt, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    }
    else if (op == string("<=")) {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_le, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    }
    else if (op == string(">")) {
        // use '<' by putting the expression in the reverse order
        cfg->current_bb->add_IRInstr(IRInstr::cmp_lt, Type::INT, {resultTempVar.getName(), expr2Address, expr1Address });
    }
    else {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_le, Type::INT, {resultTempVar.getName(), expr2Address, expr1Address });
    }

    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *ctx) {
    string op = ctx->OP->getText();

    // resolve both operands
    string expr1Address = any_cast<string>(this->visit(ctx->expr(0)));
    string expr2Address = any_cast<string>(this->visit(ctx->expr(1)));

    Symbol & resultTempVar = cfg->create_new_tempvar(Type::INT);

    if (op == string("==")) {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_eq, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    }
    else {
        cfg->current_bb->add_IRInstr(IRInstr::cmp_diff, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    }

    return resultTempVar.getName();
}

std::any IRVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx) {
    int elseif_count = ctx->expr().size() - 1;
    bool else_bloc = !(ctx->expr().size() == ctx->bloc().size());

    // create a new testing block 
    BasicBlock* start_bb = cfg->current_bb;
    BasicBlock* end_bb = cfg->createBasicBlock();    
    BasicBlock* test_bb = cfg->createBasicBlock();
    start_bb->exit_true = test_bb;

    string condVarName = any_cast<string>(this->visit(ctx->expr(0)));
    test_bb->test_var_name = condVarName;
    
    BasicBlock* true_bb = cfg->createBasicBlock();
    this->visit(ctx->bloc(0));
    test_bb->exit_true = true_bb;
    true_bb->exit_true = end_bb;

    BasicBlock* prev_test_bb_i = test_bb;
    for (int i = 1; i < elseif_count+1; ++i) {
        // chain previous false exit to new test block
        BasicBlock* test_bb_i = cfg->createBasicBlock();
        prev_test_bb_i->exit_false = test_bb_i;

        condVarName = any_cast<string>(this->visit(ctx->expr(i)));
        test_bb_i->test_var_name = condVarName;

        BasicBlock* true_bb_i = cfg->createBasicBlock();
        this->visit(ctx->bloc(i));
        test_bb_i->exit_true = true_bb_i;
        true_bb_i->exit_true = end_bb;

        prev_test_bb_i = test_bb_i;
    }

    if (else_bloc) {
        BasicBlock* else_bb = cfg->createBasicBlock();
        this->visit(ctx->bloc().back());
        else_bb->exit_true = end_bb;
    }
    else {
        prev_test_bb_i->exit_false = end_bb;
    }

    cfg->current_bb = end_bb;

    return 0;
}

std::any IRVisitor::visitExpr_aff(ifccParser::Expr_affContext *ctx) {
    string varName = ctx->VAR()->getText();
    string exprResultAddress = any_cast<string> (visit(ctx->expr()));

    cfg->current_bb->add_IRInstr(IRInstr::copy, Type::INT, {varName, exprResultAddress});

    // return the newly affected variable so that affectations can be chained
    return varName;
}
