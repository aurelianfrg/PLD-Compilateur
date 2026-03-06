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
    Address resultAddress = any_cast<Address> (this->visit(ctx->expr()));

    cout << "    movl    " << resultAddress.address << ", %eax" << endl;

    return 0;
}

std::any CodeGenVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx) {
    string value = ctx->CONST()->getText();
    string constValue = string("$") + value;

    // create a single-use temporary variable 
    // Symbol temporaryVar(to_string(temporaryVarCount++), newVarOffset("int"), true);
    // Address temporaryVarAddress(MEMORY, to_string(temporaryVar.getOffset())+"(%rbp)");
    // cout << "    movl    " << constValue << ", " << temporaryVarAddress.address << endl;

    Address constValueAdress(CONSTANT, constValue);
    return constValueAdress;
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
    Address varAddress = Address(MEMORY, location);
    return varAddress;
}

std::any CodeGenVisitor::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) {
    return visit(ctx->expr());
}

std::any CodeGenVisitor::visitExpr_minus(ifccParser::Expr_minusContext *ctx) {
    Address exprResultAddress = any_cast<Address>(this->visit(ctx->expr())); 

    if (exprResultAddress.type == REGISTER) {
        cout << "    negl    " << exprResultAddress.address << endl;
        return exprResultAddress;
    }
    else if (exprResultAddress.type == MEMORY) {
        string reg = getAvailableRegistry();
        cout << "    movl    " << exprResultAddress.address << ", " << reg << endl;
        cout << "    negl    " << reg << endl;
        Symbol temp = newTemporaryVariable("int");
        Address resultAddress = Address(MEMORY, temp.getAdress());
        cout << "    movl    " << reg << ", " << resultAddress.address << endl;
        return resultAddress;
    }
    else {
        // impossible case
        exit(-1);
    }
}


std::any CodeGenVisitor::visitExpr_sub(ifccParser::Expr_subContext *ctx){
    // resolve value for both operands
    Address expr1ResultAddress = any_cast<Address>(this->visit(ctx->expr(0))); 
    Address expr2ResultAddress = any_cast<Address>(this->visit(ctx->expr(1))); 

    // move 1st operand to eax registry where operation result will be stored
    cout << "    movl    " << expr1ResultAddress.address << ", %eax" << endl;
    // perform operation
    cout << "    subl    " << expr2ResultAddress.address << ", %eax" << endl;

    // reuse first registry to store result if there was one, else store back to memory in temporary var
    Address resultAddress;
    if (expr1ResultAddress.type = REGISTER) {
        resultAddress = expr1ResultAddress;
    }
    else {
        Symbol temporaryVar = newTemporaryVariable("int");
        resultAddress = Address(MEMORY, temporaryVar.getAdress());
    }
    cout << "    movl    %eax, " << resultAddress.address << endl;

    // free other registry because no longer needed
    if (expr2ResultAddress.type == REGISTER) {
        returnRegistry(expr2ResultAddress.address);
    }
    return resultAddress;
}

std::any CodeGenVisitor::visitExpr_add(ifccParser::Expr_addContext *ctx){
    // resolve value for both operands
    Address expr1ResultAddress = any_cast<Address>(this->visit(ctx->expr(0))); 
    Address expr2ResultAddress = any_cast<Address>(this->visit(ctx->expr(1))); 

    // move 1st operand to eax registry where operation result will be stored
    cout << "    movl    " << expr1ResultAddress.address << ", %eax" << endl;
    // perform operation
    cout << "    addl    " << expr2ResultAddress.address << ", %eax" << endl;

    // reuse first registry to store result if there was one, else store back to memory in temporary var
    Address resultAddress;
    if (expr1ResultAddress.type = REGISTER) {
        resultAddress = expr1ResultAddress;
    }
    else {
        Symbol temporaryVar = newTemporaryVariable("int");
        resultAddress = Address(MEMORY, temporaryVar.getAdress());
    }
    cout << "    movl    %eax, " << resultAddress.address << endl;

    // free other registry because no longer needed
    if (expr2ResultAddress.type == REGISTER) {
        returnRegistry(expr2ResultAddress.address);
    }
    return resultAddress;
}
                
std::any CodeGenVisitor::visitExpr_mult(ifccParser::Expr_multContext *ctx){
    // resolve value for both operands
    Address expr1ResultAddress = any_cast<Address>(this->visit(ctx->expr(0))); 
    Address expr2ResultAddress = any_cast<Address>(this->visit(ctx->expr(1))); 

    // move 1st operand to eax registry where operation result will be stored
    cout << "    movl    " << expr1ResultAddress.address << ", %eax" << endl;
    // perform operation
    cout << "    imull    " << expr2ResultAddress.address << ", %eax" << endl;

    // reuse first registry to store result if there was one, else store back to memory in temporary var
    Address resultAddress;
    if (expr1ResultAddress.type = REGISTER) {
        resultAddress = expr1ResultAddress;
    }
    else {
        Symbol temporaryVar = newTemporaryVariable("int");
        resultAddress = Address(MEMORY, temporaryVar.getAdress());
    }
    cout << "    movl    %eax, " << resultAddress.address << endl;

    // free other registry because no longer needed
    if (expr2ResultAddress.type == REGISTER) {
        returnRegistry(expr2ResultAddress.address);
    }
    return resultAddress;
}

std::any CodeGenVisitor::visitAff_stmt(ifccParser::Aff_stmtContext *ctx) {
    string varName = ctx->VAR()->getText();
    if (!symbolsTable.contains(varName)) {
        undeclaredVariableError(varName);
    }

    // resolve expression value first
    Address resultAddress = any_cast<Address> (this->visit(ctx->expr()));

    Symbol & var = symbolsTable.access(varName);

    if (resultAddress.type == MEMORY) {
        // send value to a register before copying to variable
        string registry = getAvailableRegistry();
        cout << "    movl    " << resultAddress.address << ", " << registry << endl;
        cout << "    movl    " << registry << ", " << var.getOffset() << "(%rbp)" << endl;
        returnRegistry(registry);
    }
    else {
        cout << "    movl    " << resultAddress.address << ", " << var.getOffset() << "(%rbp)" << endl;
    }

    var.affect();
    if (resultAddress.type == REGISTER) {
        returnRegistry(resultAddress.address);
    }

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
    Address resultAddress = any_cast<Address> (this->visit(ctx->expr()));

    Symbol newVar = Symbol(newVarName, newVarOffset(type), false);
    newVar.affect();
    symbolsTable.add(newVar);

    if (resultAddress.type == MEMORY) {
        // send value to a register before copying to variable
        string registry = getAvailableRegistry();
        cout << "    movl    " << resultAddress.address << ", " << registry << endl;
        cout << "    movl    " << registry << ", " << newVar.getOffset() << "(%rbp)" << endl;
        returnRegistry(registry);
    }
    else {
        cout << "    movl    " << resultAddress.address << ", " << newVar.getOffset() << "(%rbp)" << endl;
    }

    if (resultAddress.type == REGISTER) {
        returnRegistry(resultAddress.address);
    }
        
    return 0;
}
