#pragma once


#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"


class VariableVisitor : public ifccBaseVisitor {
	public:
        virtual antlrcpp::Any visitProg(ifccParser::ProgContext *ctx) override ;
        virtual std::any visitReturn_const(ifccParser::Return_constContext *ctx) override;

};