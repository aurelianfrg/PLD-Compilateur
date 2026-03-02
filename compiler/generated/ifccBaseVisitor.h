
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

  virtual std::any visitBloc(ifccParser::BlocContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAffectation_statement(ifccParser::Affectation_statementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDefinition_statement(ifccParser::Definition_statementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDefinition_affectation_statement(ifccParser::Definition_affectation_statementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturn_statement(ifccParser::Return_statementContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturn_const(ifccParser::Return_constContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitReturn_var(ifccParser::Return_varContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAffectation_var(ifccParser::Affectation_varContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitAffectation_const(ifccParser::Affectation_constContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDefinition_affectation_const(ifccParser::Definition_affectation_constContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual std::any visitDefinition_affectation_var(ifccParser::Definition_affectation_varContext *ctx) override {
    return visitChildren(ctx);
  }


};

