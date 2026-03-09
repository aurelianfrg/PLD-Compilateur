#include "CodeGenVisitor.h"

using namespace std;


antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    #ifdef __APPLE__
    std::cout<< ".globl _main\n" ;
    std::cout<< " _main: \n" ;
    #else
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;
    #endif

    //prologue 
    cout << "    # prologue\n";
    cout << "    pushq   %rbp\n";
    cout << "    movq    %rsp, %rbp\n";
    cout << "\n";

    this->visit( ctx->bloc() );

    cout << "\n";
    cout << "    # epilogue\n";
    cout << "    popq    %rbp\n";
    
    std::cout << "    ret\n";

    return 0;
}


std::any CodeGenVisitor::visitInstruction_aff_stmt(ifccParser::Instruction_aff_stmtContext *ctx) {

    return visitChildren(ctx);
}

std::any CodeGenVisitor::visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx) {

    return visitChildren(ctx);
}

std::any CodeGenVisitor::visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) {
    return visitChildren(ctx);
}

std::any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    
    string reg = any_cast<string>(this->visit(ctx->expr()));
    if (reg == "%eax") {
        // result of expression is in %eax already
        return 0;
    }
    else {
        move_op(reg, "%eax");
    }

    return visitChildren(ctx);
}

std::any CodeGenVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx) {
    string value = ctx->CONST()->getText();
    string constValue = string("$") + value;

    string reg = "%eax";
    move_op(constValue, reg);
    return reg;
}

std::any CodeGenVisitor::visitExpr_var(ifccParser::Expr_varContext *ctx) {
    string varName = ctx->VAR()->getText();

    if (!symbolsTable.contains(varName)) {
        undeclaredVariableError(varName);
    }
    Symbol & var = symbolsTable.access(varName);
    if (!var.affected()) {
        uninitializedVariableError(varName);
    }

    // variable was used
    var.use();

    string location = to_string(var.getOffset()) + "(%rbp)";
    string reg = "%eax";
    move_op(location, reg);
    return reg;
}

std::any CodeGenVisitor::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) {
    return visit(ctx->expr());
}

std::any CodeGenVisitor::visitExpr_minus(ifccParser::Expr_minusContext *ctx) {
    string exprReg = any_cast<string>(this->visit(ctx->expr()));

    minus_op(exprReg);
    return exprReg;
}

std::any CodeGenVisitor::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx){
    // resolve value for right operand, store in temporary var then resolve left
    string expr2Reg = any_cast<string>(this->visit(ctx->expr(1))); 
    Symbol & temporaryVar2 = newTemporaryVariable("int");
    move_op(expr2Reg, temporaryVar2.getAdress());

    string expr1Reg = any_cast<string>(this->visit(ctx->expr(0))); 

    string op = ctx->children.at(1)->getText();

    // perform operation
    if (op == "+") {
        add_op(expr1Reg, temporaryVar2.getAdress());
    }
    else {
        sub_op(expr1Reg, temporaryVar2.getAdress());
    }

    return expr1Reg;
}
                
std::any CodeGenVisitor::visitExpr_mult(ifccParser::Expr_multContext *ctx){
    // resolve value for right operand, store in temporary var then resolve left
    string expr2Reg = any_cast<string>(this->visit(ctx->expr(1))); 
    Symbol & temporaryVar2 = newTemporaryVariable("int");
    move_op(expr2Reg, temporaryVar2.getAdress());

    string expr1Reg = any_cast<string>(this->visit(ctx->expr(0))); 
    // perform operation
    mult_op(expr1Reg, temporaryVar2.getAdress());

    return expr1Reg;
}

std::any CodeGenVisitor::visitAff_stmt(ifccParser::Aff_stmtContext *ctx) {
    string varName = ctx->VAR()->getText();
    if (!symbolsTable.contains(varName)) {
        undeclaredVariableError(varName);
    }

    // resolve expression value first
    string resultReg = any_cast<string> (this->visit(ctx->expr()));
    Symbol & var = symbolsTable.access(varName);

    move_op(resultReg, var.getAdress());

    return 0;
}

std::any CodeGenVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx) {
    string newVarName = ctx->VAR()->getText();
    string type = ctx->TYPE()->getText();
    ifccParser::ExprContext* e = ctx->expr();

    if (symbolsTable.contains(newVarName)) {
        redeclarationError(newVarName);
    }  

    
    symbolsTable.add(Symbol(newVarName, newVarOffset(type), false));
    Symbol & newVar = symbolsTable.access(newVarName);

    if (e!=nullptr) {
        string resultReg = any_cast<string> (this->visit(ctx->expr()));
        newVar.affect();
        move_op(resultReg, newVar.getAdress()); 
    }

    return 0;
}

// std::any CodeGenVisitor::visitDef_aff_stmt(ifccParser::Def_aff_stmtContext *ctx) {
//     string newVarName = ctx->VAR()->getText();
//     string type = ctx->TYPE()->getText();

//     if (symbolsTable.contains(newVarName)) {
//         redeclarationError(newVarName);
//     }

//     // resolve expression value first
//     Address resultAddress = any_cast<Address> (this->visit(ctx->expr()));

//     Symbol newVar = Symbol(newVarName, newVarOffset(type), false);
//     newVar.affect();
//     symbolsTable.add(newVar);

    // if (resultAddress.type == MEMORY) {
    //     // send value to a register before copying to variable
    //     string registry = getAvailableRegistry();
    //     cout << "    movl    " << resultAddress.address << ", " << registry << endl;
    //     cout << "    movl    " << registry << ", " << newVar.getOffset() << "(%rbp)" << endl;
    //     returnRegistry(registry);
    // }
    // else {
    //     cout << "    movl    " << resultAddress.address << ", " << newVar.getOffset() << "(%rbp)" << endl;
    // }

    // if (resultAddress.type == REGISTER) {
    //     returnRegistry(resultAddress.address);
    // }
        
//     return 0;
// }
