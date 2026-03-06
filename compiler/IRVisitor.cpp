#include "IRVisitor.h"

using namespace std;

IRVisitor::IRVisitor() { }

// antlrcpp::Any IRVisitor::visitProg(ifccParser::ProgContext *ctx) 
// {
//     this->visit( ctx->bloc() );
//     return 0;
// }

antlrcpp::Any IRVisitor::visitBloc(ifccParser::BlocContext *ctx) 
{
	string name = cfg->new_BB_name();
	cfg->add_bb(new BasicBlock(cfg, name));
    this->visit( ctx->children[0] );
    return 0;
}

std::any IRVisitor::visitReturn_stmt(ifccParser::Return_stmtContext *ctx) {
	    // resolve return value expression 
    string address = any_cast<string>(this->visit(ctx->expr()));
	cfg->current_bb->add_IRInstr(IRInstr::ret, Type::INT, { address });
    return 0;
}

std::any IRVisitor::visitExpr_const(ifccParser::Expr_constContext *ctx) {
	cfg->create_new_tempvar(Type::INT);
	
    return 0;
}
