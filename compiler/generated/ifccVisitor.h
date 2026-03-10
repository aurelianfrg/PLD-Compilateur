
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ifccParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by ifccParser.
 */
class  ifccVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by ifccParser.
   */
    virtual std::any visitAxiom(ifccParser::AxiomContext *context) = 0;

    virtual std::any visitProg(ifccParser::ProgContext *context) = 0;

    virtual std::any visitBlock(ifccParser::BlockContext *context) = 0;

    virtual std::any visitStmt(ifccParser::StmtContext *context) = 0;

    virtual std::any visitDeclaration_stmt(ifccParser::Declaration_stmtContext *context) = 0;

    virtual std::any visitAssign_stmt(ifccParser::Assign_stmtContext *context) = 0;

    virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *context) = 0;

    virtual std::any visitMinus(ifccParser::MinusContext *context) = 0;

    virtual std::any visitParenthese(ifccParser::ParentheseContext *context) = 0;

    virtual std::any visitAddSub(ifccParser::AddSubContext *context) = 0;

    virtual std::any visitLiteral_expr(ifccParser::Literal_exprContext *context) = 0;

    virtual std::any visitMultiplication(ifccParser::MultiplicationContext *context) = 0;

    virtual std::any visitLiteral(ifccParser::LiteralContext *context) = 0;


};

