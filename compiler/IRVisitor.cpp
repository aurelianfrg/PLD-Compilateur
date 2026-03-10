#include "IRVisitor.h"

using namespace std;

IRVisitor::IRVisitor(tree::ParseTree* parseTree) { 
    cfg = new CFG(parseTree);
}

antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx) {
    BasicBlock* firstBlock = cfg->createBasicBlock();
    this->visit( ctx->bloc() );
    return 0;
}

std::any IRVisitor::visitInstruction_aff_stmt(ifccParser::Instruction_aff_stmtContext *ctx) {
    return visitChildren(ctx);
}
std::any IRVisitor::visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx) {
    return visitChildren(ctx);
}
std::any IRVisitor::visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) {
    return visitChildren(ctx);
}
std::any IRVisitor::visitInstruction_if_stmt(ifccParser::Instruction_if_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any IRVisitor::visitBloc(ifccParser::BlocContext *ctx) 
{
    return visitChildren(ctx);
}

std::any IRVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    // cout << "visit return" << endl;
	// resolve return value expression 
    string tempVarName = any_cast<string>(this->visit(ctx->expr()));
	cfg->current_bb->add_IRInstr(IRInstr::ret, Type::INT, { tempVarName });
    return 0;
}

std::any IRVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx) {
    // cout << "visit expr const" << endl;
	string value = ctx->CONST()->getText();
    Symbol & tempVar = cfg->create_new_tempvar(Type::INT);
    cfg->current_bb->add_IRInstr(IRInstr::ldconst, Type::INT, {value, tempVar.getName()});
	
    return tempVar.getName();
}

std::any IRVisitor::visitExpr_comp(ifccParser::Expr_compContext *ctx) {

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
    // create a new testing block 
    BasicBlock* start_bb = cfg->current_bb;
    BasicBlock* test_bb = cfg->createBasicBlock();
    start_bb->exit_true = test_bb;

    string condVarName = any_cast<string>(this->visit(ctx->IF_EXPR));
    test_bb->test_var_name = condVarName;

    BasicBlock* true_bb = cfg->createBasicBlock();
    this->visit(ctx->IF_BLOC);
    test_bb->exit_true = true_bb;

    // case without else statement
    BasicBlock* end_bb = cfg->createBasicBlock();
    test_bb->exit_false = end_bb;
    true_bb->exit_true = end_bb;

    return 0;
}

// std::any IRVisitor::visitExpr_var(ifccParser::Expr_varContext *ctx) {}
// std::any IRVisitor::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) {}
// std::any IRVisitor::visitExpr_minus(ifccParser::Expr_minusContext *ctx) {}
// std::any IRVisitor::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx) {}
// std::any IRVisitor::visitExpr_mult(ifccParser::Expr_multContext *ctx) {}
// std::any IRVisitor::visitAff_stmt(ifccParser::Aff_stmtContext *ctx) {}
// std::any IRVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx) {}
