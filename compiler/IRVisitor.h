#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "antlr4-runtime.h"
#include "generated/ifccBaseVisitor.h"

#include "IR.h"

using namespace std;
using namespace antlr4;

class IRVisitor : public ifccBaseVisitor
{
public:
	IRVisitor(tree::ParseTree *tree);
	// virtual std::any visitProg(ifccParser::ProgContext *ctx) override;
	virtual std::any visitBloc(ifccParser::BlocContext *ctx) override;
	virtual std::any visitInstruction_aff_stmt(ifccParser::Instruction_aff_stmtContext *ctx) override;
	virtual std::any visitInstruction_def_stmt(ifccParser::Instruction_def_stmtContext *ctx) override;
	virtual std::any visitInstruction_return_stmt(ifccParser::Instruction_return_stmtContext *ctx) override;
	virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override;
	virtual std::any visitExpr_const(ifccParser::Expr_constContext *ctx) override;
	virtual std::any visitExpr_var(ifccParser::Expr_varContext *ctx) override;
	virtual std::any visitExpr_parenthesis(ifccParser::Expr_parenthesisContext *ctx) override;
	// virtual std::any visitExpr_minus(ifccParser::Expr_minusContext *ctx) override;
	virtual std::any visitExpr_add_sub(ifccParser::Expr_add_subContext *ctx) override;
	virtual std::any visitExpr_mult(ifccParser::Expr_multContext *ctx) override;
	// virtual std::any visitAff_stmt(ifccParser::Aff_stmtContext *ctx) override;
	virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override;

	CFG *cfg;
};
