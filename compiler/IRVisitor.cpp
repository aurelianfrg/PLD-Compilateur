#include "IRVisitor.h"

using namespace std;

IRVisitor::IRVisitor(tree::ParseTree *parseTree) { cfg = new CFG(parseTree); }

antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx) {
    for (auto &function_def : ctx->function_def()) {
        this->visit(function_def);
    }
    return 0;
}

std::any IRVisitor::visitFunction_def(ifccParser::Function_defContext *ctx) {
    string function_name = ctx->VAR(0)->getText(); // first VAR is the function name
    string return_type = ctx->type_function()->getText();

    // handle parameters and add them to the FunctionBlock SymbolsTable
    vector<Type> paramsType;
    vector<string> paramsName;
    for (int i = 1; i < ctx->VAR().size(); ++i) {
        paramsType.push_back(typeFromString.at(ctx->TYPE(i - 1)->getText()));
        paramsName.push_back(ctx->VAR(i)->getText());
    }

    // add the newly declared functions to the function table
    cfg->functionsTable.add(
        Function(function_name, typeFromString.at(return_type), paramsType, paramsName));

    // create the first block for this function and start filling it with intructions from its
    // content
    FunctionBlock *fb = cfg->createFunctionBlock(function_name, paramsType, paramsName);
    this->visit(ctx->bloc());

    return 0;
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

std::any IRVisitor::visitInstruction_while_stmt(ifccParser::Instruction_while_stmtContext *ctx) {
    return visitChildren(ctx);
}

std::any IRVisitor::visitInstruction_switch_stmt(ifccParser::Instruction_switch_stmtContext *ctx) {
    return visitChildren(ctx);
}

antlrcpp::Any IRVisitor::visitBloc(ifccParser::BlocContext *ctx) { return visitChildren(ctx); }

std::any IRVisitor::visitInstruction_bloc(ifccParser::Instruction_blocContext *ctx) {

    // create a new child block
    Block *start_bb = cfg->current_block;
    SymbolsTable inheritedSymbols = start_bb->symbolsTable;
    BasicBlock *end_bb = cfg->createSiblingBasicBlock(inheritedSymbols);
    BasicBlock *new_bb = cfg->createChildBasicBlock(inheritedSymbols);
    start_bb->exit_true = new_bb;
    new_bb->exit_true = end_bb;

    // fill block with its instructions
    this->visit(ctx->bloc());

    // go back to end block with the rest
    cfg->current_block = end_bb;

    return 0;
}

std::any IRVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    // cout << "visit return" << endl;
    // resolve return value expression
    string tempVarName = any_cast<string>(this->visit(ctx->expr()));
    cfg->current_block->add_IRInstr(IRInstr::ret, Type::INT, {tempVarName});
    return 0;
}

std::any IRVisitor::visitCall(ifccParser::CallContext *ctx) {
    string functionName = ctx->VAR()->getText();
    Function &callee = cfg->functionsTable.access(functionName);
    Type returnType = callee.getType();

    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(returnType);

    vector<string> callArgs = {functionName, tempVar.getName()};
    // resolve value for all parameters expressions
    for (auto expr : ctx->expr()) {
        callArgs.push_back(any_cast<string>(this->visit(expr)));
    }

    cfg->current_block->add_IRInstr(IRInstr::call, returnType, callArgs);
    return tempVar.getName();
}

std::any IRVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx) {
    // cout << "visit expr const" << endl;
    string value = ctx->CONST()->getText();
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);
    cfg->current_block->add_IRInstr(IRInstr::ldconst, Type::INT, {value, tempVar.getName()});

    return tempVar.getName();
}

std::any IRVisitor::visitExpr_var(ifccParser::Expr_varContext *ctx) {
    string value = ctx->VAR()->getText();

    return value;
}

std::any IRVisitor::visitExpr_char(ifccParser::Expr_charContext *ctx) {
    string value = ctx->CHAR()->getText();
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);
    cfg->current_block->add_IRInstr(IRInstr::ldchar, Type::INT, {value, tempVar.getName()});

    return tempVar.getName();
}

std::any IRVisitor::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) {
    return visit(ctx->expr());
}

std::any IRVisitor::visitExpr_minus_not(ifccParser::Expr_minus_notContext *ctx) {
    string op = ctx->OP->getText();
    string tempVarName = any_cast<string>(this->visit(ctx->expr()));
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == string("-")) {
        cfg->current_block->add_IRInstr(IRInstr::neg, Type::INT, {tempVar.getName(), tempVarName});
    } 
    else if (op == string("!")) {
        cfg->current_block->add_IRInstr(IRInstr::not_, Type::INT, {tempVar.getName(), tempVarName});
    }
    else if (op == string("~")) {
        cfg->current_block->add_IRInstr(IRInstr::bit_not, Type::INT, {tempVar.getName(), tempVarName});
    }
    if (op == string("++")) {
        cfg->current_block->add_IRInstr(IRInstr::incr_prefix, Type::INT, {tempVarName});
    }
    if (op == string("--")) {
        cfg->current_block->add_IRInstr(IRInstr::decr_prefix, Type::INT, {tempVarName});
    }
    return tempVar.getName();
}

std::any IRVisitor::visitExpr_postfix(ifccParser::Expr_postfixContext *ctx) {
    string op = ctx->OP->getText();
    string VarName = any_cast<string>(this->visit(ctx->expr()));
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == string("++")) {
        cfg->current_block->add_IRInstr(IRInstr::incr_postfix, Type::INT, {tempVar.getName(), VarName});
    }
    if (op == string("--")) {
        cfg->current_block->add_IRInstr(IRInstr::decr_postfix, Type::INT, {tempVar.getName(), VarName});
    }
    return tempVar.getName();
}

std::any IRVisitor::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx) {

    string tempVarName1 = any_cast<string>(this->visit(ctx->expr(0)));
    string op = ctx->OP->getText();
    string tempVarName2 = any_cast<string>(this->visit(ctx->expr(1)));
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == "+") {
        cfg->current_block->add_IRInstr(IRInstr::add, Type::INT,
                                        {tempVar.getName(), tempVarName1, tempVarName2});
    }

    else if (op == "-") {
        cfg->current_block->add_IRInstr(IRInstr::sub, Type::INT,
                                        {tempVar.getName(), tempVarName1, tempVarName2});
    }

    return tempVar.getName();
}

std::any IRVisitor::visitExpr_mult_div_mod(ifccParser::Expr_mult_div_modContext *ctx) {
    string tempVarName1 = any_cast<string>(this->visit(ctx->expr(0)));
    string op = ctx->OP->getText();
    string tempVarName2 = any_cast<string>(this->visit(ctx->expr(1)));
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == "*") {
        cfg->current_block->add_IRInstr(IRInstr::mul, Type::INT,
                                        {tempVar.getName(), tempVarName1, tempVarName2});
    } else if (op == "/") {
        cfg->current_block->add_IRInstr(IRInstr::div, Type::INT,
                                        {tempVar.getName(), tempVarName1, tempVarName2});
    } else if (op == "%") {
        cfg->current_block->add_IRInstr(IRInstr::mod, Type::INT,
                                        {tempVar.getName(), tempVarName1, tempVarName2});
    }

    return tempVar.getName();
}

std::any IRVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx) {
    for (auto item : ctx->def_item()) {
        string varName = item->VAR()->getText();
        Symbol &tempVar = cfg->current_block->symbolsTable.create_new_var(Type::INT, varName);

        if (item->expr() != nullptr) {
            string tempVarName = any_cast<string>(this->visit(item->expr()));
            cfg->current_block->add_IRInstr(IRInstr::copy, Type::INT, {varName, tempVarName});
        }
    }
    return 0;
}

std::any IRVisitor::visitExpr_comp(ifccParser::Expr_compContext *ctx) {
    string op = ctx->OP->getText();

    // resolve both operands
    string expr1Address = any_cast<string>(this->visit(ctx->expr(0)));
    string expr2Address = any_cast<string>(this->visit(ctx->expr(1)));

    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == string("<")) {
        cfg->current_block->add_IRInstr(IRInstr::cmp_lt, Type::INT,
                                        {resultTempVar.getName(), expr1Address, expr2Address});
    } else if (op == string("<=")) {
        cfg->current_block->add_IRInstr(IRInstr::cmp_le, Type::INT,
                                        {resultTempVar.getName(), expr1Address, expr2Address});
    } else if (op == string(">")) {
        // use '<' by putting the expression in the reverse order
        cfg->current_block->add_IRInstr(IRInstr::cmp_lt, Type::INT,
                                        {resultTempVar.getName(), expr2Address, expr1Address});
    } else {
        cfg->current_block->add_IRInstr(IRInstr::cmp_le, Type::INT,
                                        {resultTempVar.getName(), expr2Address, expr1Address});
    }

    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *ctx) {
    string op = ctx->OP->getText();

    // resolve both operands
    string expr1Address = any_cast<string>(this->visit(ctx->expr(0)));
    string expr2Address = any_cast<string>(this->visit(ctx->expr(1)));

    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    if (op == string("==")) {
        cfg->current_block->add_IRInstr(IRInstr::cmp_eq, Type::INT,
                                        {resultTempVar.getName(), expr1Address, expr2Address});
    } else {
        cfg->current_block->add_IRInstr(IRInstr::cmp_diff, Type::INT,
                                        {resultTempVar.getName(), expr1Address, expr2Address});
    }

    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_and(ifccParser::Expr_andContext *context) {
    string expr1Address = any_cast<string>(this->visit(context->expr(0)));
    string expr2Address = any_cast<string>(this->visit(context->expr(1)));
    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    cfg->current_block->add_IRInstr(IRInstr::bit_and, Type::INT,
                                    {resultTempVar.getName(), expr1Address, expr2Address});
    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_xor(ifccParser::Expr_xorContext *context) {
    string expr1Address = any_cast<string>(this->visit(context->expr(0)));
    string expr2Address = any_cast<string>(this->visit(context->expr(1)));
    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    cfg->current_block->add_IRInstr(IRInstr::bit_xor, Type::INT,
                                    {resultTempVar.getName(), expr1Address, expr2Address});
    return resultTempVar.getName();
}

std::any IRVisitor::visitExpr_or(ifccParser::Expr_orContext *context) {
    string expr1Address = any_cast<string>(this->visit(context->expr(0)));
    string expr2Address = any_cast<string>(this->visit(context->expr(1)));
    Symbol &resultTempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);

    cfg->current_block->add_IRInstr(IRInstr::bit_or, Type::INT,
                                    {resultTempVar.getName(), expr1Address, expr2Address});
    return resultTempVar.getName();
}

std::any IRVisitor::visitIf_stmt(ifccParser::If_stmtContext *ctx) {
    int elseif_count = ctx->expr().size() - 1;
    bool else_bloc = !(ctx->expr().size() == ctx->bloc().size());

    // create a new testing block
    Block *start_bb = cfg->current_block;
    SymbolsTable inheritedSymbols = start_bb->symbolsTable;

    BasicBlock *end_bb = cfg->createSiblingBasicBlock(inheritedSymbols);
    BasicBlock *test_bb = cfg->createSiblingBasicBlock(inheritedSymbols);
    start_bb->exit_true = test_bb;

    string condVarName = any_cast<string>(this->visit(ctx->expr(0)));
    test_bb->test_var_name = condVarName;

    BasicBlock *true_bb = cfg->createChildBasicBlock(inheritedSymbols);
    this->visit(ctx->bloc(0));
    test_bb->exit_true = true_bb;
    true_bb->exit_true = end_bb;

    BasicBlock *prev_test_bb_i = test_bb;
    for (int i = 1; i < elseif_count + 1; ++i) {
        // chain previous false exit to new test block
        BasicBlock *test_bb_i = cfg->createSiblingBasicBlock(inheritedSymbols);
        prev_test_bb_i->exit_false = test_bb_i;

        condVarName = any_cast<string>(this->visit(ctx->expr(i)));
        test_bb_i->test_var_name = condVarName;

        BasicBlock *true_bb_i = cfg->createChildBasicBlock(inheritedSymbols);
        this->visit(ctx->bloc(i));
        test_bb_i->exit_true = true_bb_i;
        true_bb_i->exit_true = end_bb;

        prev_test_bb_i = test_bb_i;
    }

    if (else_bloc) {
        BasicBlock *else_bb = cfg->createChildBasicBlock(inheritedSymbols);
        prev_test_bb_i->exit_false = else_bb;
        this->visit(ctx->bloc().back());
        else_bb->exit_true = end_bb;
    } else {
        prev_test_bb_i->exit_false = end_bb;
    }

    cfg->current_block = end_bb;

    return 0;
}

std::any IRVisitor::visitExpr_aff(ifccParser::Expr_affContext *ctx)
{
    string varName = ctx->VAR()->getText();
    string op = ctx->OP->getText();
    string exprAddress = any_cast<string>(visit(ctx->expr()));
    if (op == "=")
    {
        cfg->current_block->add_IRInstr(IRInstr::copy, Type::INT, {varName, exprAddress});
    }
    else if (op == "+=")
    {
        cfg->current_block->add_IRInstr(IRInstr::add, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == "-=")
    {
        cfg->current_block->add_IRInstr(IRInstr::sub, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == "*=")
    {
        cfg->current_block->add_IRInstr(IRInstr::mul, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == "/=")
    {
        cfg->current_block->add_IRInstr(IRInstr::div, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == "%=")
    {
        cfg->current_block->add_IRInstr(IRInstr::mod, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == "&=")
    {
        cfg->current_block->add_IRInstr(IRInstr::bit_and, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == "^=")
    {
        cfg->current_block->add_IRInstr(IRInstr::bit_xor, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == "|=")
    {
        cfg->current_block->add_IRInstr(IRInstr::bit_or, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == "<<=")
    {
        cfg->current_block->add_IRInstr(IRInstr::shl, Type::INT, {varName, varName, exprAddress});
    }
    else if (op == ">>=")
    {
        cfg->current_block->add_IRInstr(IRInstr::shr, Type::INT, {varName, varName, exprAddress});
    }
    // return the newly affected variable so that affectations can be chained
    return varName;
}

std::any IRVisitor::visitWhile_stmt(ifccParser::While_stmtContext *ctx) {

    Block *start_bb = cfg->current_block;
    SymbolsTable inheritedSymbols = start_bb->symbolsTable;
    BasicBlock *end_bb = cfg->createSiblingBasicBlock(inheritedSymbols);

    BasicBlock *test_bb = cfg->createSiblingBasicBlock(inheritedSymbols);
    start_bb->exit_true = test_bb;
    string condVarName = any_cast<string>(this->visit(ctx->expr()));
    test_bb->test_var_name = condVarName;

    BasicBlock *while_bb = cfg->createChildBasicBlock(inheritedSymbols);
    this->visit(ctx->bloc());
    test_bb->exit_true = while_bb;
    test_bb->exit_false = end_bb;
    while_bb->exit_true = test_bb;

    cfg->current_block = end_bb;

    return 0;
}

std::any IRVisitor::visitSwitch_stmt(ifccParser::Switch_stmtContext *ctx) {
    int case_count = ctx->case_item().size();
    bool has_default = ctx->case_default() != nullptr;

    Block *start_block = cfg->current_block;
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);
    SymbolsTable inheritedSymbols = start_block->symbolsTable;

    // evaluate source expr
    string condVarName = any_cast<string>(this->visit(ctx->expr()));
    BasicBlock *end_bb = cfg->createSiblingBasicBlock(inheritedSymbols);

    // create blocks
    vector<BasicBlock *> test_bbs;
    vector<BasicBlock *> code_bbs;
    for (int i = 0; i < case_count; i++) {
        test_bbs.push_back(cfg->createSiblingBasicBlock(inheritedSymbols));
        code_bbs.push_back(cfg->createSiblingBasicBlock(inheritedSymbols));
    }

    BasicBlock *default_bb = has_default ? cfg->createSiblingBasicBlock(inheritedSymbols) : end_bb;
    start_block->exit_true = test_bbs.at(0);
    // wire test chain
    for (int i = 0; i < case_count; i++) {
        bool has_range = false;
        string caseValueLow, caseValueHigh;
        if (ctx->case_item(i)->RANGE() != nullptr) {
            has_range = true;
            caseValueHigh = ctx->case_item(i)->HIGH_VALUE->getText();
        }
        caseValueLow = ctx->case_item(i)->LOW_VALUE->getText();
        cfg->current_block = test_bbs.at(i);
        test_bbs.at(i)->test_var_name = condVarName;

        // load the case constant into a temporary variable first
        Symbol &caseConstVarLow = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);
        cfg->current_block->add_IRInstr(IRInstr::ldconst, Type::INT,
                                        {caseValueLow, caseConstVarLow.getName()});

        // compare the condition variable with the case constant variable
        cfg->current_block->add_IRInstr(
            IRInstr::cmp_eq, Type::INT,
            {tempVar.getName(), condVarName, caseConstVarLow.getName()});

        if (has_range) {
            Symbol &caseConstVarHigh =
                cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);
            cfg->current_block->add_IRInstr(IRInstr::ldconst, Type::INT,
                                            {caseValueHigh, caseConstVarHigh.getName()});
            cfg->current_block->add_IRInstr(
                IRInstr::cmp_le, Type::INT,
                {tempVar.getName(), condVarName, caseConstVarHigh.getName()});
        }
        test_bbs.at(i)->exit_true = code_bbs.at(i);
        test_bbs.at(i)->exit_false = (i < case_count - 1) ? test_bbs.at(i + 1) : default_bb;
    }
    // wire code chain
    for (int i = 0; i < case_count; i++) {
        cfg->current_block = code_bbs.at(i);
        this->visit(ctx->case_item(i));
        code_bbs.at(i)->exit_true = (i < case_count - 1) ? code_bbs.at(i + 1) : default_bb;
    }
    // wire default block
    if (has_default) {
        cfg->current_block = default_bb;
        this->visit(ctx->case_default());
        default_bb->exit_true = end_bb;
    }
    cfg->current_block = end_bb;

    return 0;
}

std::any IRVisitor::visitExpr_log_and(ifccParser::Expr_log_andContext *ctx) {

    // evaluate left operand
    string leftVarName = any_cast<string>(this->visit(ctx->expr(0)));

    Block *test_left = cfg->current_block;
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);
    SymbolsTable inheritedSymbols = cfg->current_block->symbolsTable;

    cfg->current_block->add_IRInstr(IRInstr::copy, Type::INT, {tempVar.getName(), leftVarName});

    BasicBlock *eval_right = cfg->createChildBasicBlock(inheritedSymbols);
    BasicBlock *end_bb = cfg->createChildBasicBlock(inheritedSymbols);

    test_left->test_var_name = leftVarName;

    // short circuit
    test_left->exit_true = eval_right;
    test_left->exit_false = end_bb;

    // evaluate right operand
    cfg->current_block = eval_right;
    string rightVarName = any_cast<string>(this->visit(ctx->expr(1)));
    cfg->current_block->add_IRInstr(IRInstr::copy, Type::INT, {tempVar.getName(), rightVarName});
    eval_right->exit_true = end_bb;

    cfg->current_block = end_bb;

    return tempVar.getName();
}

std::any IRVisitor::visitExpr_log_or(ifccParser::Expr_log_orContext *ctx) {

    // evaluate left operand
    string leftVarName = any_cast<string>(this->visit(ctx->expr(0)));

    Block *test_left = cfg->current_block;
    Symbol &tempVar = cfg->current_block->symbolsTable.create_new_tempvar(Type::INT);
    SymbolsTable inheritedSymbols = cfg->current_block->symbolsTable;

    cfg->current_block->add_IRInstr(IRInstr::copy, Type::INT, {tempVar.getName(), leftVarName});

    BasicBlock *eval_right = cfg->createChildBasicBlock(inheritedSymbols);
    BasicBlock *end_bb = cfg->createChildBasicBlock(inheritedSymbols);

    test_left->test_var_name = leftVarName;

    // short circuit
    test_left->exit_true = end_bb;
    test_left->exit_false = eval_right;

    // evaluate right operand
    cfg->current_block = eval_right;
    string rightVarName = any_cast<string>(this->visit(ctx->expr(1)));
    eval_right->add_IRInstr(IRInstr::copy, Type::INT, {tempVar.getName(), rightVarName});
    eval_right->exit_true = end_bb;

    cfg->current_block = end_bb;

    return tempVar.getName();
}