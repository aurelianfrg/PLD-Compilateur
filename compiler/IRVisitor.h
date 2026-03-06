#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include "IR.h"


class IRVisitor : public ifccBaseVisitor {
	public:
		IRVisitor();
		// virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
		virtual std::any visitBloc(ifccParser::BlocContext *ctx) override;

		virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
		virtual std::any visitExpr_const(ifccParser::Expr_constContext *ctx) override;
	protected:
		CFG* cfg;
};

