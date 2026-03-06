#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class  CodeGenVisitor : public ifccBaseVisitor {
	public:
                virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
                virtual antlrcpp::Any visitReturn_expr(ifccParser::Return_exprContext *ctx) override;
                virtual antlrcpp::Any visitDeclaration_int(ifccParser::Declaration_intContext *ctx) override;
                virtual antlrcpp::Any visitAssign_var_expr(ifccParser::Assign_var_exprContext *ctx) override;
                virtual antlrcpp::Any visitDeclaration_assign_var_expr(ifccParser::Declaration_assign_var_exprContext *ctx) override;
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

