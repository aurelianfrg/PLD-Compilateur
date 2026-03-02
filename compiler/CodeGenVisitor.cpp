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
    cout << "    pushq %rbp\n";
    cout << "    movq %rsp, %rbp\n";
    cout << "\n";

    this->visit( ctx->bloc() );

    cout << "\n";
    cout << "    # epilogue\n";
    cout << "    popq %rbp\n";
    
    std::cout << "    ret\n";

    return 0;
}


antlrcpp::Any CodeGenVisitor::visitReturn_stmt_const(ifccParser::Return_stmt_constContext *ctx)
{
    int retval = stoi(ctx->CONST()->getText());

    std::cout << "    movl $"<<retval<<", %eax\n" ;

    return 0;
}
