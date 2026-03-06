#pragma once

#include <map>
#include <string>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class  VariableVisitor : public ifccBaseVisitor {
	public:
        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
        virtual antlrcpp::Any visitDeclaration_stmt(ifccParser::Declaration_stmtContext *ctx) override;
        virtual antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
        virtual antlrcpp::Any visitAddSub(ifccParser::AddSubContext *ctx) override;
        virtual antlrcpp::Any visitMultiplication(ifccParser::MultiplicationContext *ctx) override;
        virtual antlrcpp::Any visitMinus(ifccParser::MinusContext *ctx) override;
        virtual antlrcpp::Any visitParenthese(ifccParser::ParentheseContext *ctx) override;
        virtual antlrcpp::Any visitLiteral_expr(ifccParser::Literal_exprContext *ctx) override;
        virtual antlrcpp::Any visitLiteral(ifccParser::LiteralContext *ctx) override;
        
        void printSymbolTable();
        void checkUsage();

        std::map<std::string, int> getVarOffsets() const { return varOffsets; }
        

    protected:
        std::map<std::string, int> varOffsets; // map variables and their offset
        std::map<std::string, bool> varUse; // track if variables are used at least once
        int currentOffset = -4; // Track the current stack offset
};
