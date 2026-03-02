
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

    virtual std::any visitBloc(ifccParser::BlocContext *context) = 0;

    virtual std::any visitAffectation_statement(ifccParser::Affectation_statementContext *context) = 0;

    virtual std::any visitDefinition_statement(ifccParser::Definition_statementContext *context) = 0;

    virtual std::any visitDefinition_affectation_statement(ifccParser::Definition_affectation_statementContext *context) = 0;

    virtual std::any visitReturn_statement(ifccParser::Return_statementContext *context) = 0;

    virtual std::any visitReturn_const(ifccParser::Return_constContext *context) = 0;

    virtual std::any visitReturn_var(ifccParser::Return_varContext *context) = 0;

    virtual std::any visitAffectation_var(ifccParser::Affectation_varContext *context) = 0;

    virtual std::any visitAffectation_const(ifccParser::Affectation_constContext *context) = 0;

    virtual std::any visitDef_stmt(ifccParser::Def_stmtContext *context) = 0;

    virtual std::any visitDefinition_affectation_const(ifccParser::Definition_affectation_constContext *context) = 0;

    virtual std::any visitDefinition_affectation_var(ifccParser::Definition_affectation_varContext *context) = 0;


};

