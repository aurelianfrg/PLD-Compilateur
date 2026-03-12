#include "IRVisitor.h"

using namespace std;

IRVisitor::IRVisitor(tree::ParseTree *parseTree)
{
    cfg = new CFG(parseTree);
}

antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx)
{
    for (auto & function_def : ctx->function_def()) {
        this->visit(function_def);
    }
    return 0;
}

std::any IRVisitor::visitFunction_def(ifccParser::Function_defContext *ctx) {
    string function_name = ctx->VAR(0)->getText();
    string return_type = ctx->type_function()->getText();

    // TODO : handle parameters and add them to the FunctionBlock SymbolsTable
    vector<Type> paramsType;
    vector<string> paramsName;

    FunctionBlock* fb = cfg->createFunctionBlock(function_name, paramsType, paramsName);
    this->visit(ctx->bloc());

    return 0;
}

std::any IRVisitor::visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx)
{
    return visitChildren(ctx);
}
std::any IRVisitor::visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx)
{
    return visitChildren(ctx);
}
std::any IRVisitor::visitInstruction_if_stmt(ifccParser::Instruction_if_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any IRVisitor::visitInstruction_while_stmt(ifccParser::Instruction_while_stmtContext *ctx) {
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
    cfg->current_block->add_IRInstr(IRInstr::ret, Type::INT, {tempVarName});
    return 0;
}

std::any IRVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx)
{
    // cout << "visit expr const" << endl;
    string value = ctx->CONST()->getText();
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);
    cfg->current_block->add_IRInstr(IRInstr::ldconst, Type::INT, {value, tempVar.getName()});

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

std::any IRVisitor::visitExpr_minus_not(ifccParser::Expr_minus_notContext *ctx)
{
    string op = ctx->OP->getText();
    string tempVarName = any_cast<string>(this->visit(ctx->expr()));
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == string("-")) {
        cfg->current_block->add_IRInstr(IRInstr::neg, Type::INT, {tempVar.getName(), tempVarName});
    }
    else {
        cfg->current_block->add_IRInstr(IRInstr::not_, Type::INT, {tempVar.getName(), tempVarName});
    }

    return tempVar.getName();
}

std::any IRVisitor::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx)
{

    string tempVarName1 = any_cast<string>(this->visit(ctx->expr(0)));
    string op = ctx->OP->getText();
    string tempVarName2 = any_cast<string>(this->visit(ctx->expr(1)));
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == "+")
    {
        cfg->current_block->add_IRInstr(IRInstr::add, Type::INT, {tempVar.getName(), tempVarName1, tempVarName2});
    }

    else if (op == "-")
    {
        cfg->current_block->add_IRInstr(IRInstr::sub, Type::INT, {tempVar.getName(), tempVarName1, tempVarName2});
    }

    return tempVar.getName();
}

std::any IRVisitor::visitExpr_mult_div_mod(ifccParser::Expr_mult_div_modContext *ctx)
{
    string tempVarName1 = any_cast<string>(this->visit(ctx->expr(0)));
    string op = ctx->OP->getText();
    string tempVarName2 = any_cast<string>(this->visit(ctx->expr(1)));
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == "*")
    {
        cfg->current_block->add_IRInstr(IRInstr::mul, Type::INT, {tempVar.getName(), tempVarName1, tempVarName2});
    }
    else if (op == "/")
    {
        cfg->current_block->add_IRInstr(IRInstr::div, Type::INT, {tempVar.getName(), tempVarName1, tempVarName2});
    }
    else if (op == "%")
    {
        cfg->current_block->add_IRInstr(IRInstr::mod, Type::INT, {tempVar.getName(), tempVarName1, tempVarName2});
    }

    return tempVar.getName();
}

std::any IRVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx)
{
    for (auto item : ctx->def_item())
    {
        string varName = item->VAR()->getText();
        Symbol &tempVar = cfg->current_block->symbolsTable.create_new_var(Type::INT, varName);

        if (item->expr() != nullptr)
        {
            string tempVarName = any_cast<string>(this->visit(item->expr()));
            cfg->current_block->add_IRInstr(IRInstr::copy, Type::INT, {varName, tempVarName});
        }
    }
    return 0;
}

std::any IRVisitor::visitExpr_comp(ifccParser::Expr_compContext *ctx)
{
    string op = ctx->OP->getText();

    // resolve both operands
    string expr1Address = any_cast<string>(this->visit(ctx->expr(0)));
    string expr2Address = any_cast<string>(this->visit(ctx->expr(1)));

    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == string("<"))
    {
        cfg->current_block->add_IRInstr(IRInstr::cmp_lt, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    }
    else if (op == string("<="))
    {
        cfg->current_block->add_IRInstr(IRInstr::cmp_le, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    }
    else if (op == string(">"))
    {
        // use '<' by putting the expression in the reverse order
        cfg->current_block->add_IRInstr(IRInstr::cmp_lt, Type::INT, {resultTempVar.getName(), expr2Address, expr1Address});
    }
    else
    {
        cfg->current_block->add_IRInstr(IRInstr::cmp_le, Type::INT, {resultTempVar.getName(), expr2Address, expr1Address});
    }

    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *ctx)
{
    string op = ctx->OP->getText();

    // resolve both operands
    string expr1Address = any_cast<string>(this->visit(ctx->expr(0)));
    string expr2Address = any_cast<string>(this->visit(ctx->expr(1)));

    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == string("=="))
    {
        cfg->current_block->add_IRInstr(IRInstr::cmp_eq, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    }
    else
    {
        cfg->current_block->add_IRInstr(IRInstr::cmp_diff, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    }

    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_and(ifccParser::Expr_andContext *context)
{
    string expr1Address = any_cast<string>(this->visit(context->expr(0)));
    string expr2Address = any_cast<string>(this->visit(context->expr(1)));
    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    cfg->current_block->add_IRInstr(IRInstr::bit_and, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_xor(ifccParser::Expr_xorContext *context)
{
    string expr1Address = any_cast<string>(this->visit(context->expr(0)));
    string expr2Address = any_cast<string>(this->visit(context->expr(1)));
    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    cfg->current_block->add_IRInstr(IRInstr::bit_xor, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_or(ifccParser::Expr_orContext *context)
{
    string expr1Address = any_cast<string>(this->visit(context->expr(0)));
    string expr2Address = any_cast<string>(this->visit(context->expr(1)));
    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    cfg->current_block->add_IRInstr(IRInstr::bit_or, Type::INT, {resultTempVar.getName(), expr1Address, expr2Address});
    return resultTempVar.getName();
}

std::any IRVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx) {
    int elseif_count = ctx->expr().size() - 1;
    bool else_bloc = !(ctx->expr().size() == ctx->bloc().size());

    // create a new testing block 
    Block* start_bb = cfg->current_block;
    if (start_bb == nullptr) {
        cerr << "cast failed" << endl;
    }
    SymbolsTable inheritedSymbols = start_bb->symbolsTable;

    BasicBlock* end_bb = cfg->createSiblingBasicBlock(inheritedSymbols);    
    BasicBlock* test_bb = cfg->createChildBasicBlock(inheritedSymbols);
    start_bb->exit_true = test_bb;

    string condVarName = any_cast<string>(this->visit(ctx->expr(0)));
    test_bb->test_var_name = condVarName;
    
    BasicBlock* true_bb = cfg->createChildBasicBlock(inheritedSymbols);
    this->visit(ctx->bloc(0));
    test_bb->exit_true = true_bb;
    true_bb->exit_true = end_bb;

    BasicBlock* prev_test_bb_i = test_bb;
    for (int i = 1; i < elseif_count+1; ++i) {
        // chain previous false exit to new test block
        BasicBlock* test_bb_i = cfg->createChildBasicBlock(inheritedSymbols);
        prev_test_bb_i->exit_false = test_bb_i;

        condVarName = any_cast<string>(this->visit(ctx->expr(i)));
        test_bb_i->test_var_name = condVarName;

        BasicBlock* true_bb_i = cfg->createChildBasicBlock(inheritedSymbols);
        this->visit(ctx->bloc(i));
        test_bb_i->exit_true = true_bb_i;
        true_bb_i->exit_true = end_bb;

        prev_test_bb_i = test_bb_i;
    }

    if (else_bloc) {
        BasicBlock* else_bb = cfg->createChildBasicBlock(inheritedSymbols);
        this->visit(ctx->bloc().back());
        else_bb->exit_true = end_bb;
    }
    else {
        prev_test_bb_i->exit_false = end_bb;
    }

    cfg->current_block = end_bb;

    return 0;
}

std::any IRVisitor::visitExpr_aff(ifccParser::Expr_affContext *ctx)
{
    string varName = ctx->VAR()->getText();
    string exprResultAddress = any_cast<string>(visit(ctx->expr()));

    cfg->current_block->add_IRInstr(IRInstr::copy, Type::INT, {varName, exprResultAddress});

    // return the newly affected variable so that affectations can be chained
    return varName;
}

std::any IRVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx) {

    BasicBlock* start_bb = dynamic_cast<BasicBlock*>(cfg->current_block);
    SymbolsTable inheritedSymbols = start_bb->symbolsTable;
    BasicBlock* end_bb = cfg->createSiblingBasicBlock(inheritedSymbols);
    
    BasicBlock* test_bb = cfg->createChildBasicBlock(inheritedSymbols);
    start_bb->exit_true = test_bb;
    string condVarName = any_cast<string>(this->visit(ctx->expr()));
    test_bb->test_var_name = condVarName;

    BasicBlock* while_bb = cfg->createChildBasicBlock(inheritedSymbols);
    this->visit(ctx->bloc());
    test_bb->exit_true = while_bb;
    test_bb->exit_false = end_bb;
    while_bb->exit_true = test_bb;

    cfg->current_block = end_bb;

    return 0;
}

