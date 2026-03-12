#include "VariableVisitor.h"

#include <string>

std::any VariableVisitor::visitProg(ifccParser::ProgContext *ctx)
{

    this->visit( ctx->bloc() );
    checkUsage(); // Check for unused variables
    // printSymbolTable(); // Print symbol table for debugging. CAREFUL: it print the symbol table in the assembly code (cout)
    return 0;
}

std::any VariableVisitor::visitBloc(ifccParser::BlocContext *ctx)
{
    
    return visitChildren(ctx);
}

std::any VariableVisitor::visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitor::visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitor::visitInstruction_aff_stmt(ifccParser::Instruction_aff_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx)
{
    return visitChildren(ctx);
}

std::any VariableVisitor::visitDef_stmt(ifccParser::Def_stmtContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    if (varOffsets.find(varName) != varOffsets.end())
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' already declared." << std::endl;
        error = true;
    }
    varOffsets[varName] = currentOffset;
    varUse[varName] = false;
    currentOffset -= 4;
    if (ctx->expr() != nullptr)
    {
        this->visit(ctx->expr());
    }
    return 0;
}

std::any VariableVisitor::visitAff_stmt(ifccParser::Aff_stmtContext *ctx)
{
    std::string varName = ctx->VAR()->getText();
    if (varOffsets.find(varName) == varOffsets.end())
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' used before declaration.\n";
        error = true;
    }
    this->visit(ctx->expr());
    return 0;
}

std::any VariableVisitor::visitExpr_comp(ifccParser::Expr_compContext *ctx){
    return visitChildren(ctx);
}

std::any VariableVisitor::visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx){
    return visitChildren(ctx);
}

std::any VariableVisitor::visitExpr_minus(ifccParser::Expr_minusContext *ctx){
    return visitChildren(ctx);
}

std::any VariableVisitor::visitExpr_mult(ifccParser::Expr_multContext *ctx){
    return visitChildren(ctx);
}

std::any VariableVisitor::visitExpr_eq_diff(ifccParser::Expr_eq_diffContext *ctx){
    return visitChildren(ctx);
}

std::any VariableVisitor::visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx){
    return visitChildren(ctx);
}

std::any VariableVisitor::visitExpr_aff(ifccParser::Expr_affContext *ctx){
    std::string varName = ctx->VAR()->getText();
    if (varOffsets.find(varName) == varOffsets.end())
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' used before declaration.\n";
        error = true;
    }
    varUse[varName] = true;
    this->visit(ctx->expr());
    return 0;
}

std::any VariableVisitor::visitExpr_var(ifccParser::Expr_varContext *ctx){
    std::string varName = ctx->VAR()->getText();
    if (varOffsets.find(varName) == varOffsets.end())
    {
        int line = ctx->getStart()->getLine();
        std::cerr << "\e[31mError:\e[39m \e[33mLigne " << line << ":\e[39m Variable '" << varName << "' used before declaration.\n";
        error = true;
    }
    varUse[varName] = true;
    return 0;
}

void VariableVisitor::printSymbolTable()
{
    std::cout << "Symbol Table:\n";
    for (const auto &[key, value] : varOffsets)
    {
        std::cout << "Variable: " << key << ", Offset: " << value << ", Already used: " << varUse[key] << "\n";
    }
}

void VariableVisitor::checkUsage()
{
    for (const auto &[key, value] : varUse)
    {
        if (value == false)
        {
            std::cerr << "\e[35mWarning :\e[39m variable '" << key << "' is never used.\n";
        }
    }
}