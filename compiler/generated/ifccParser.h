
// Generated from ifcc.g4 by ANTLR 4.13.2

#pragma once


#include "antlr4-runtime.h"




class  ifccParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    TYPE = 8, RETURN = 9, CONST = 10, VAR = 11, COMMENT = 12, DIRECTIVE = 13, 
    WS = 14
  };

  enum {
    RuleAxiom = 0, RuleProg = 1, RuleBloc = 2, RuleInstruction = 3, RuleReturn_stmt = 4, 
    RuleAff_stmt = 5, RuleDef_stmt = 6, RuleDef_aff_stmt = 7
  };

  explicit ifccParser(antlr4::TokenStream *input);

  ifccParser(antlr4::TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options);

  ~ifccParser() override;

  std::string getGrammarFileName() const override;

  const antlr4::atn::ATN& getATN() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;


  class AxiomContext;
  class ProgContext;
  class BlocContext;
  class InstructionContext;
  class Return_stmtContext;
  class Aff_stmtContext;
  class Def_stmtContext;
  class Def_aff_stmtContext; 

  class  AxiomContext : public antlr4::ParserRuleContext {
  public:
    AxiomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProgContext *prog();
    antlr4::tree::TerminalNode *EOF();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  AxiomContext* axiom();

  class  ProgContext : public antlr4::ParserRuleContext {
  public:
    ProgContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TYPE();
    BlocContext *bloc();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProgContext* prog();

  class  BlocContext : public antlr4::ParserRuleContext {
  public:
    BlocContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    std::vector<InstructionContext *> instruction();
    InstructionContext* instruction(size_t i);


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  BlocContext* bloc();

  class  InstructionContext : public antlr4::ParserRuleContext {
  public:
    InstructionContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    InstructionContext() = default;
    void copyFrom(InstructionContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Definition_affectation_statementContext : public InstructionContext {
  public:
    Definition_affectation_statementContext(InstructionContext *ctx);

    Def_aff_stmtContext *def_aff_stmt();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Definition_statementContext : public InstructionContext {
  public:
    Definition_statementContext(InstructionContext *ctx);

    Def_stmtContext *def_stmt();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Affectation_statementContext : public InstructionContext {
  public:
    Affectation_statementContext(InstructionContext *ctx);

    Aff_stmtContext *aff_stmt();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Return_statementContext : public InstructionContext {
  public:
    Return_statementContext(InstructionContext *ctx);

    Return_stmtContext *return_stmt();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  InstructionContext* instruction();

  class  Return_stmtContext : public antlr4::ParserRuleContext {
  public:
    Return_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Return_stmtContext() = default;
    void copyFrom(Return_stmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Return_varContext : public Return_stmtContext {
  public:
    Return_varContext(Return_stmtContext *ctx);

    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *VAR();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Return_constContext : public Return_stmtContext {
  public:
    Return_constContext(Return_stmtContext *ctx);

    antlr4::tree::TerminalNode *RETURN();
    antlr4::tree::TerminalNode *CONST();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Return_stmtContext* return_stmt();

  class  Aff_stmtContext : public antlr4::ParserRuleContext {
  public:
    Aff_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Aff_stmtContext() = default;
    void copyFrom(Aff_stmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Affectation_constContext : public Aff_stmtContext {
  public:
    Affectation_constContext(Aff_stmtContext *ctx);

    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *CONST();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Affectation_varContext : public Aff_stmtContext {
  public:
    Affectation_varContext(Aff_stmtContext *ctx);

    std::vector<antlr4::tree::TerminalNode *> VAR();
    antlr4::tree::TerminalNode* VAR(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Aff_stmtContext* aff_stmt();

  class  Def_stmtContext : public antlr4::ParserRuleContext {
  public:
    Def_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *VAR();


    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  Def_stmtContext* def_stmt();

  class  Def_aff_stmtContext : public antlr4::ParserRuleContext {
  public:
    Def_aff_stmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    Def_aff_stmtContext() = default;
    void copyFrom(Def_aff_stmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  Definition_affectation_constContext : public Def_aff_stmtContext {
  public:
    Definition_affectation_constContext(Def_aff_stmtContext *ctx);

    antlr4::tree::TerminalNode *TYPE();
    antlr4::tree::TerminalNode *VAR();
    antlr4::tree::TerminalNode *CONST();

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  Definition_affectation_varContext : public Def_aff_stmtContext {
  public:
    Definition_affectation_varContext(Def_aff_stmtContext *ctx);

    antlr4::tree::TerminalNode *TYPE();
    std::vector<antlr4::tree::TerminalNode *> VAR();
    antlr4::tree::TerminalNode* VAR(size_t i);

    virtual std::any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  Def_aff_stmtContext* def_aff_stmt();


  // By default the static state used to implement the parser is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:
};

