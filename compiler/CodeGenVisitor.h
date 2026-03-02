#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class CodeGenVisitor : public ifccBaseVisitor {
	public:
        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual std::any visitReturn_stmt_const(ifccParser::Return_stmt_constContext *ctx) override;

};

