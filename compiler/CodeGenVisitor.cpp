#include "CodeGenVisitor.h"

#include <string>

antlrcpp::Any CodeGenVisitor::visitProg(ifccParser::ProgContext *ctx) 
{
    #ifdef __APPLE__
    std::cout<< ".globl _main\n" ;
    std::cout<< " _main: \n" ;
    #else
    std::cout<< ".globl main\n" ;
    std::cout<< " main: \n" ;
    #endif

    // Prologue 
    std::cout << "    pushq %rbp\n";
    std::cout << "    movq %rsp, %rbp\n";

    this->visit( ctx->block());
    
    // Epilogue
    std::cout << "    popq %rbp\n";
    std::cout << "    ret\n";

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_expr(ifccParser::Return_exprContext *ctx) {
    this->visit(ctx->expr());

return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclaration_int(ifccParser::Declaration_intContext *ctx) {
    std::string varName = ctx->VAR()->getText();

    std::cout << "    movl $0, " << varOffsets[varName] << "(%rbp)\n"; // Initialize variable to 0, a enlever ?

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAssign_var_expr(ifccParser::Assign_var_exprContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    this->visit(ctx->expr());

    // Move value into variable
    std::cout << "    movl %eax, " << varOffsets[varName] << "(%rbp)\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitDeclaration_assign_var_expr(ifccParser::Declaration_assign_var_exprContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    this->visit(ctx->expr());

    // Move value into variable
    std::cout << "    movl %eax, " << varOffsets[varName] << "(%rbp)\n"; 

    return 0;
}

antlrcpp::Any CodeGenVisitor::visitMultiplication(ifccParser::MultiplicationContext *ctx) {
    // Left
    this->visit(ctx->expr(0));
    std::cout << "    subq $8, %rsp\n";    
    std::cout << "    movl %eax, (%rsp)\n";    

    // Right
    this->visit(ctx->expr(1));
    std::cout << "    movl %eax, %ecx\n";
    std::cout << "    movl (%rsp), %eax\n";
    std::cout << "    addq $8, %rsp\n";

    // Multiplication
    std::cout << "    imull %ecx, %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitAddSub(ifccParser::AddSubContext *ctx) {
    // Left
    this->visit(ctx->expr(0));
    std::cout << "    subq $8, %rsp\n";    
    std::cout << "    movl %eax, (%rsp)\n";    

    // Right
    this->visit(ctx->expr(1));
    std::cout << "    movl %eax, %ecx\n";
    std::cout << "    movl (%rsp), %eax\n";
    std::cout << "    addq $8, %rsp\n";

    std::string op = ctx->children[1]->getText();  // get op
    if (op == "+") {
        std::cout << "    addl %ecx, %eax\n";
    } else if (op == "-") {
        std::cout << "    subl %ecx, %eax\n";
    }
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitMinus(ifccParser::MinusContext *ctx) {
    this->visit(ctx->expr());
    std::cout << "    negl %eax\n";
    return 0;
}

antlrcpp::Any CodeGenVisitor::visitParenthese(ifccParser::ParentheseContext *ctx) {
    return this->visit(ctx->expr());
}

antlrcpp::Any CodeGenVisitor::visitLiteral_expr(ifccParser::Literal_exprContext *ctx) {
    return this->visit(ctx->literal());
}

antlrcpp::Any CodeGenVisitor::visitLiteral(ifccParser::LiteralContext *ctx) {
    if (ctx->CONST()) {
        std::cout << "    movl $" << ctx->CONST()->getText() << ", %eax\n";
    } else if (ctx->VAR()) {
        std::string varName = ctx->VAR()->getText();
        std::cout << "    movl " << varOffsets[varName] << "(%rbp), %eax\n";
    }
    return 0;
}