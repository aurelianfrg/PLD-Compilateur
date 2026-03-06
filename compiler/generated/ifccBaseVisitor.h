
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"
#include "ifccVisitor.h"


/**
 * This class provides an empty implementation of ifccVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  ifccBaseVisitor : public ifccVisitor {
public:

  virtual std::any visitAxiom(ifccParser::AxiomContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitProg(ifccParser::ProgContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitBlock(ifccParser::BlockContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitStmt(ifccParser::StmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDeclaration_stmt(ifccParser::Declaration_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAssign_stmt(ifccParser::Assign_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturn_stmt(ifccParser::Return_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMinus(ifccParser::MinusContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitParenthese(ifccParser::ParentheseContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAddSub(ifccParser::AddSubContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral_expr(ifccParser::Literal_exprContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitMultiplication(ifccParser::MultiplicationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitLiteral(ifccParser::LiteralContext *ctx) override {
    return visitChildren(ctx);
  }


};

