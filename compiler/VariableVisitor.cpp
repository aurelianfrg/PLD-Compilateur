#include "VariableVisitor.h"

#include <string>

antlrcpp::Any VariableVisitor::visitProg(ifccParser::ProgContext *ctx) {

    this->visit( ctx->block());
    checkUsage();
    return 0;
}

antlrcpp::Any VariableVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
    // Visiter l'expression pour marquer les variables utilisées
    this->visit(ctx->expr());

    return 0;
}

antlrcpp::Any VariableVisitor::visitDeclaration_stmt(ifccParser::Declaration_stmtContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    if (varOffsets.find(varName) != varOffsets.end()) {
        int line = ctx -> getStart() -> getLine();
        std::cerr <<"Ligne "<<line<<": \e[31mError :\e[39m Variable '" << varName << "' already declared." << std::endl;
        error = true;
    }
    else {
        varOffsets[varName] = currentOffset; 
        varUse[varName] = false; // Usage false
        currentOffset = currentOffset - 4; // 4 bc int
    }

    if (ctx->expr()) {
        this->visit(ctx->expr());
    }
    
    return 0;
}

antlrcpp::Any VariableVisitor::visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) {
    std::string varName = ctx->VAR()->getText();
    if (varOffsets.find(varName) == varOffsets.end()) {
        int line = ctx -> getStart() -> getLine();
        std::cerr <<"Ligne " <<line<<": \e[31mError :\e[39m Variable '" << varName << "' used before declaration.\n";
        error = true;
    }
    // Visiter l'expression pour marquer les variables utilisées
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
            int line = ctx -> getStart() -> getLine();
            std::cerr <<"Ligne " << line<<": \e[31mError :\e[39m Variable '" << varName << "' used before declaration.\n";
            error = true;
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
            std::cout << "\e[35mWarning :\e[39m variable '" << key << "' is never used.\n";
        }
    }
}