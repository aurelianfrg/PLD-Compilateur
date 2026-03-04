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

std::any CodeGenVisitor::visitInstruction_def_aff_stmt(ifccParser::Instruction_def_aff_stmtContext *ctx) {

    return visitChildren(ctx);
}

std::any CodeGenVisitor::visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) {
    return visitChildren(ctx);
}

std::any CodeGenVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    
    // resolve return value expression 
    string resultLocation = any_cast<string> (this->visit(ctx->expr()));

    cout << "    movl    " << resultLocation << ", %eax" << endl;

    return 0;
}

std::any CodeGenVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx) {
    // all expressions visits return a string : the registry or adress where the result is stored
    string value = ctx->CONST()->getText();
    string constValue = string("$") + value;
    return constValue;
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
    // move variable to a registry
    string registry = getAvailableRegistry();
    cout << "    movl    " << location << ", "  << registry << endl;
    return registry;
}

std::any CodeGenVisitor::visitExpr_sub(ifccParser::Expr_subContext *ctx){
    // resolve value for both operands
    string expr1ResultRegistry = any_cast<string>(this->visit(ctx->expr(0))); 
    string expr2ResultRegistry = any_cast<string>(this->visit(ctx->expr(1))); 

    // move 1st operand to eax registry where operation result will be stored
    cout << "    movl    " << expr1ResultRegistry << ", %eax" << endl;
    // perform operation
    cout << "    subl    " << expr2ResultRegistry << ", %eax" << endl;

    // reuse first registry to store result and return the 2nd
    string resultRegistry = expr1ResultRegistry;
    cout << "    movl    %eax, " << resultRegistry << endl;
    returnRegistry(expr2ResultRegistry);
    return resultRegistry;
}

std::any CodeGenVisitor::visitExpr_add(ifccParser::Expr_addContext *ctx){
    // resolve value for both operands
    string expr1ResultRegistry = any_cast<string>(this->visit(ctx->expr(0))); 
    string expr2ResultRegistry = any_cast<string>(this->visit(ctx->expr(1))); 

    // move 1st operand to eax registry where operation result will be stored
    cout << "    movl    " << expr1ResultRegistry << ", %eax" << endl;
    // perform operation
    cout << "    addl    " << expr2ResultRegistry << ", %eax" << endl;

    // reuse first registry to store result and return the 2nd
    string resultRegistry = expr1ResultRegistry;
    cout << "    movl    %eax, " << resultRegistry << endl;

    returnRegistry(expr2ResultRegistry);
    return resultRegistry;
}
                
std::any CodeGenVisitor::visitExpr_mult(ifccParser::Expr_multContext *ctx){
    // resolve value for both operands
    string expr1ResultRegistry = any_cast<string>(this->visit(ctx->expr(0))); 
    string expr2ResultRegistry = any_cast<string>(this->visit(ctx->expr(1))); 

    // move 1st operand to eax registry where operation result will be stored
    cout << "    movl    " << expr1ResultRegistry << ", %eax" << endl;
    // perform operation
    cout << "    imull    " << expr2ResultRegistry << ", %eax" << endl;

    // reuse first registry to store result and return the 2nd
    string resultRegistry = expr1ResultRegistry;
    cout << "    movl    %eax, " << resultRegistry << endl;
    returnRegistry(expr2ResultRegistry);
    return resultRegistry;
}

std::any CodeGenVisitor::visitAff_stmt(ifccParser::Aff_stmtContext *ctx) {
    string varName = ctx->VAR()->getText();
    if (!symbolsTable.contains(varName)) {
        undeclaredVariableError(varName);
    }

    // resolve expression value first
    string resultRegistry = any_cast<string> (this->visit(ctx->expr()));

    Symbol & var = symbolsTable.access(varName);

    // write assembly
    cout << "    movl    " << resultRegistry << ", " << var.getOffset() << "(%rbp)" << endl;

    var.affect();
    returnRegistry(resultRegistry);

    return 0;
}

std::any CodeGenVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx) {
    string newVarName = ctx->VAR()->getText();
    string type = ctx->TYPE()->getText();

    if (symbolsTable.contains(newVarName)) {
        redeclarationError(newVarName);
    }  

    Symbol newVar = Symbol(newVarName, newVarOffset(type), false);
    symbolsTable.add(newVar);

    return 0;
}

std::any CodeGenVisitor::visitDef_aff_stmt(ifccParser::Def_aff_stmtContext *ctx) {
    string newVarName = ctx->VAR()->getText();
    string type = ctx->TYPE()->getText();

    if (symbolsTable.contains(newVarName)) {
        redeclarationError(newVarName);
    }

    // resolve expression value first
    string resultRegistry = any_cast<string> (this->visit(ctx->expr()));

    Symbol newVar = Symbol(newVarName, newVarOffset(type), false);
    newVar.affect();
    symbolsTable.add(newVar);

    // write assembly
    cout << "    movl    " << resultRegistry << ", " << newVar.getOffset() << "(%rbp)" << endl;
    returnRegistry(resultRegistry);
        
    return 0;
}
