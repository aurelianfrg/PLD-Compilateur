#include "VariableVisitor.h"

#include <string>

antlrcpp::Any VariableVisitor::visitProg(ifccParser::ProgContext *ctx) {

    this->visit( ctx->block());
    checkUsage();   // Check for unused variables
    // printSymbolTable(); // Print symbol table for debugging. CAREFUL: it print the symbol table in the assembly code (cout)
    return 0;
}

antlrcpp::Any VariableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any VariableVisitor::visitDeclaration_item(ifccParser::Declaration_itemContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    if (varOffsets.find(varName) != varOffsets.end()) {
        std::cerr << "Error: Variable '" << varName << "' already declared." << std::endl;
        exit(1);
    }
    varOffsets[varName] = currentOffset;
    varUse[varName] = false;
    currentOffset -= 4;
    if (ctx->expr()) {
        this->visit(ctx->expr());
    }
    return 0;
}

antlrcpp::Any VariableVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    if (varOffsets.find(varName) == varOffsets.end()) {
        std::cerr << "Error: Variable '" << varName << "' used before declaration.\n";
        exit(1);
    }
    this->visit(ctx->expr());
    return 0;
}


antlrcpp::Any VariableVisitor::visitMultiplication(ifccParser::MultiplicationContext *ctx) {
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any VariableVisitor::visitAddSub(ifccParser::AddSubContext *ctx) {
    this->visit(ctx->expr(0));
    this->visit(ctx->expr(1));
    return 0;
}

antlrcpp::Any VariableVisitor::visitMinus(ifccParser::MinusContext *ctx) {
    this->visit(ctx->expr());
    return 0;
}

antlrcpp::Any VariableVisitor::visitParenthese(ifccParser::ParentheseContext *ctx) {
    return this->visit(ctx->expr());
}

antlrcpp::Any VariableVisitor::visitLiteral_expr(ifccParser::Literal_exprContext *ctx) {
    return this->visit(ctx->literal());
}

antlrcpp::Any VariableVisitor::visitLiteral(ifccParser::LiteralContext *ctx) {
    if (ctx->VAR()) {
        std::string varName = ctx->VAR()->getText();
        if (varOffsets.find(varName) == varOffsets.end()) {
            std::cerr << "Error: Variable '" << varName << "' used before declaration.\n";
            exit(1);
        }
        varUse[varName] = true;
    }
    return 0;
}

void VariableVisitor::printSymbolTable() {
    std::cout << "Symbol Table:\n";
    for (const auto& [key, value] : varOffsets) {
        std::cout << "Variable: " << key << ", Offset: " << value << ", Already used: " << varUse[key] << "\n";
    } 
}

void VariableVisitor::checkUsage() {
    for (const auto& [key, value] : varUse) {
        if (value == false) {
            std::cout << "Warning : variable '" << key << "' is never used.\n";
        }
    }
}