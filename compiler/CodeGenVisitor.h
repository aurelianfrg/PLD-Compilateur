#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class  CodeGenVisitor : public ifccBaseVisitor {
	public:
                virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
                virtual antlrcpp::Any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
                virtual antlrcpp::Any visitDeclaration_stmt(ifccParser::Declaration_stmtContext *ctx) override;
                virtual antlrcpp::Any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override;
                virtual antlrcpp::Any visitParenthese(ifccParser::ParentheseContext *ctx) override;
                virtual antlrcpp::Any visitLiteral_expr(ifccParser::Literal_exprContext *ctx) override;
                virtual antlrcpp::Any visitAddSub(ifccParser::AddSubContext *ctx) override;
                virtual antlrcpp::Any visitMultiplication(ifccParser::MultiplicationContext *ctx) override;
                virtual antlrcpp::Any visitMinus(ifccParser::MinusContext *ctx) override;
                virtual antlrcpp::Any visitLiteral(ifccParser::LiteralContext *ctx) override;

                // Constructor
                CodeGenVisitor(std::map<std::string, int> varOffsets) : varOffsets(varOffsets) {}

        protected:
                std::map<std::string, int> varOffsets;
};

