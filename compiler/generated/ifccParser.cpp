
// Generated from ifcc.g4 by ANTLR 4.13.2


#include "ifccVisitor.h"

#include "ifccParser.h"


using namespace antlrcpp;

using namespace antlr4;

namespace {

struct IfccParserStaticData final {
  IfccParserStaticData(std::vector<std::string> ruleNames,
                        std::vector<std::string> literalNames,
                        std::vector<std::string> symbolicNames)
      : ruleNames(std::move(ruleNames)), literalNames(std::move(literalNames)),
        symbolicNames(std::move(symbolicNames)),
        vocabulary(this->literalNames, this->symbolicNames) {}

  IfccParserStaticData(const IfccParserStaticData&) = delete;
  IfccParserStaticData(IfccParserStaticData&&) = delete;
  IfccParserStaticData& operator=(const IfccParserStaticData&) = delete;
  IfccParserStaticData& operator=(IfccParserStaticData&&) = delete;

  std::vector<antlr4::dfa::DFA> decisionToDFA;
  antlr4::atn::PredictionContextCache sharedContextCache;
  const std::vector<std::string> ruleNames;
  const std::vector<std::string> literalNames;
  const std::vector<std::string> symbolicNames;
  const antlr4::dfa::Vocabulary vocabulary;
  antlr4::atn::SerializedATNView serializedATN;
  std::unique_ptr<antlr4::atn::ATN> atn;
};

::antlr4::internal::OnceFlag ifccParserOnceFlag;
#if ANTLR4_USE_THREAD_LOCAL_CACHE
static thread_local
#endif
std::unique_ptr<IfccParserStaticData> ifccParserStaticData = nullptr;

void ifccParserInitialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  if (ifccParserStaticData != nullptr) {
    return;
  }
#else
  assert(ifccParserStaticData == nullptr);
#endif
  auto staticData = std::make_unique<IfccParserStaticData>(
    std::vector<std::string>{
      "axiom", "prog", "block", "stmt", "declaration_stmt", "assign_stmt", 
      "return_stmt", "expr", "literal"
    },
    std::vector<std::string>{
      "", "'int'", "'main'", "'('", "')'", "'{'", "'}'", "'='", "';'", "'return'", 
      "'-'", "'*'", "'+'"
    },
    std::vector<std::string>{
      "", "", "", "", "", "", "", "", "", "", "", "", "", "CONST", "VAR", 
      "COMMENT", "DIRECTIVE", "WS"
    }
  );
  static const int32_t serializedATNSegment[] = {
  	4,1,17,83,2,0,7,0,2,1,7,1,2,2,7,2,2,3,7,3,2,4,7,4,2,5,7,5,2,6,7,6,2,7,
  	7,7,2,8,7,8,1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,2,5,2,30,8,2,10,
  	2,12,2,33,9,2,1,2,1,2,1,3,1,3,1,3,1,3,3,3,41,8,3,1,4,1,4,1,4,1,4,3,4,
  	47,8,4,1,4,1,4,1,5,1,5,1,5,1,5,1,5,1,6,1,6,1,6,1,6,1,7,1,7,1,7,1,7,1,
  	7,1,7,1,7,1,7,3,7,68,8,7,1,7,1,7,1,7,1,7,1,7,1,7,5,7,76,8,7,10,7,12,7,
  	79,9,7,1,8,1,8,1,8,0,1,14,9,0,2,4,6,8,10,12,14,16,0,2,2,0,10,10,12,12,
  	1,0,13,14,82,0,18,1,0,0,0,2,21,1,0,0,0,4,27,1,0,0,0,6,40,1,0,0,0,8,42,
  	1,0,0,0,10,50,1,0,0,0,12,55,1,0,0,0,14,67,1,0,0,0,16,80,1,0,0,0,18,19,
  	3,2,1,0,19,20,5,0,0,1,20,1,1,0,0,0,21,22,5,1,0,0,22,23,5,2,0,0,23,24,
  	5,3,0,0,24,25,5,4,0,0,25,26,3,4,2,0,26,3,1,0,0,0,27,31,5,5,0,0,28,30,
  	3,6,3,0,29,28,1,0,0,0,30,33,1,0,0,0,31,29,1,0,0,0,31,32,1,0,0,0,32,34,
  	1,0,0,0,33,31,1,0,0,0,34,35,5,6,0,0,35,5,1,0,0,0,36,41,3,8,4,0,37,41,
  	3,10,5,0,38,41,3,12,6,0,39,41,3,14,7,0,40,36,1,0,0,0,40,37,1,0,0,0,40,
  	38,1,0,0,0,40,39,1,0,0,0,41,7,1,0,0,0,42,43,5,1,0,0,43,46,5,14,0,0,44,
  	45,5,7,0,0,45,47,3,14,7,0,46,44,1,0,0,0,46,47,1,0,0,0,47,48,1,0,0,0,48,
  	49,5,8,0,0,49,9,1,0,0,0,50,51,5,14,0,0,51,52,5,7,0,0,52,53,3,14,7,0,53,
  	54,5,8,0,0,54,11,1,0,0,0,55,56,5,9,0,0,56,57,3,14,7,0,57,58,5,8,0,0,58,
  	13,1,0,0,0,59,60,6,7,-1,0,60,61,5,3,0,0,61,62,3,14,7,0,62,63,5,4,0,0,
  	63,68,1,0,0,0,64,65,5,10,0,0,65,68,3,14,7,4,66,68,3,16,8,0,67,59,1,0,
  	0,0,67,64,1,0,0,0,67,66,1,0,0,0,68,77,1,0,0,0,69,70,10,3,0,0,70,71,5,
  	11,0,0,71,76,3,14,7,4,72,73,10,2,0,0,73,74,7,0,0,0,74,76,3,14,7,3,75,
  	69,1,0,0,0,75,72,1,0,0,0,76,79,1,0,0,0,77,75,1,0,0,0,77,78,1,0,0,0,78,
  	15,1,0,0,0,79,77,1,0,0,0,80,81,7,1,0,0,81,17,1,0,0,0,6,31,40,46,67,75,
  	77
  };
  staticData->serializedATN = antlr4::atn::SerializedATNView(serializedATNSegment, sizeof(serializedATNSegment) / sizeof(serializedATNSegment[0]));

  antlr4::atn::ATNDeserializer deserializer;
  staticData->atn = deserializer.deserialize(staticData->serializedATN);

  const size_t count = staticData->atn->getNumberOfDecisions();
  staticData->decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    staticData->decisionToDFA.emplace_back(staticData->atn->getDecisionState(i), i);
  }
  ifccParserStaticData = std::move(staticData);
}

}

ifccParser::ifccParser(TokenStream *input) : ifccParser(input, antlr4::atn::ParserATNSimulatorOptions()) {}

ifccParser::ifccParser(TokenStream *input, const antlr4::atn::ParserATNSimulatorOptions &options) : Parser(input) {
  ifccParser::initialize();
  _interpreter = new atn::ParserATNSimulator(this, *ifccParserStaticData->atn, ifccParserStaticData->decisionToDFA, ifccParserStaticData->sharedContextCache, options);
}

ifccParser::~ifccParser() {
  delete _interpreter;
}

const atn::ATN& ifccParser::getATN() const {
  return *ifccParserStaticData->atn;
}

std::string ifccParser::getGrammarFileName() const {
  return "ifcc.g4";
}

const std::vector<std::string>& ifccParser::getRuleNames() const {
  return ifccParserStaticData->ruleNames;
}

const dfa::Vocabulary& ifccParser::getVocabulary() const {
  return ifccParserStaticData->vocabulary;
}

antlr4::atn::SerializedATNView ifccParser::getSerializedATN() const {
  return ifccParserStaticData->serializedATN;
}


//----------------- AxiomContext ------------------------------------------------------------------

ifccParser::AxiomContext::AxiomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::ProgContext* ifccParser::AxiomContext::prog() {
  return getRuleContext<ifccParser::ProgContext>(0);
}

tree::TerminalNode* ifccParser::AxiomContext::EOF() {
  return getToken(ifccParser::EOF, 0);
}


size_t ifccParser::AxiomContext::getRuleIndex() const {
  return ifccParser::RuleAxiom;
}


std::any ifccParser::AxiomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAxiom(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::AxiomContext* ifccParser::axiom() {
  AxiomContext *_localctx = _tracker.createInstance<AxiomContext>(_ctx, getState());
  enterRule(_localctx, 0, ifccParser::RuleAxiom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(18);
    prog();
    setState(19);
    match(ifccParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProgContext ------------------------------------------------------------------

ifccParser::ProgContext::ProgContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::BlockContext* ifccParser::ProgContext::block() {
  return getRuleContext<ifccParser::BlockContext>(0);
}


size_t ifccParser::ProgContext::getRuleIndex() const {
  return ifccParser::RuleProg;
}


std::any ifccParser::ProgContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitProg(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::ProgContext* ifccParser::prog() {
  ProgContext *_localctx = _tracker.createInstance<ProgContext>(_ctx, getState());
  enterRule(_localctx, 2, ifccParser::RuleProg);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(21);
    match(ifccParser::T__0);
    setState(22);
    match(ifccParser::T__1);
    setState(23);
    match(ifccParser::T__2);
    setState(24);
    match(ifccParser::T__3);
    setState(25);
    block();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BlockContext ------------------------------------------------------------------

ifccParser::BlockContext::BlockContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<ifccParser::StmtContext *> ifccParser::BlockContext::stmt() {
  return getRuleContexts<ifccParser::StmtContext>();
}

ifccParser::StmtContext* ifccParser::BlockContext::stmt(size_t i) {
  return getRuleContext<ifccParser::StmtContext>(i);
}


size_t ifccParser::BlockContext::getRuleIndex() const {
  return ifccParser::RuleBlock;
}


std::any ifccParser::BlockContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitBlock(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::BlockContext* ifccParser::block() {
  BlockContext *_localctx = _tracker.createInstance<BlockContext>(_ctx, getState());
  enterRule(_localctx, 4, ifccParser::RuleBlock);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(27);
    match(ifccParser::T__4);
    setState(31);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & 26122) != 0)) {
      setState(28);
      stmt();
      setState(33);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(34);
    match(ifccParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StmtContext ------------------------------------------------------------------

ifccParser::StmtContext::StmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::Declaration_stmtContext* ifccParser::StmtContext::declaration_stmt() {
  return getRuleContext<ifccParser::Declaration_stmtContext>(0);
}

ifccParser::Assign_stmtContext* ifccParser::StmtContext::assign_stmt() {
  return getRuleContext<ifccParser::Assign_stmtContext>(0);
}

ifccParser::Return_stmtContext* ifccParser::StmtContext::return_stmt() {
  return getRuleContext<ifccParser::Return_stmtContext>(0);
}

ifccParser::ExprContext* ifccParser::StmtContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}


size_t ifccParser::StmtContext::getRuleIndex() const {
  return ifccParser::RuleStmt;
}


std::any ifccParser::StmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitStmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::StmtContext* ifccParser::stmt() {
  StmtContext *_localctx = _tracker.createInstance<StmtContext>(_ctx, getState());
  enterRule(_localctx, 6, ifccParser::RuleStmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(40);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 1, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(36);
      declaration_stmt();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(37);
      assign_stmt();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(38);
      return_stmt();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(39);
      expr(0);
      break;
    }

    default:
      break;
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Declaration_stmtContext ------------------------------------------------------------------

ifccParser::Declaration_stmtContext::Declaration_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::Declaration_stmtContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

ifccParser::ExprContext* ifccParser::Declaration_stmtContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}


size_t ifccParser::Declaration_stmtContext::getRuleIndex() const {
  return ifccParser::RuleDeclaration_stmt;
}


std::any ifccParser::Declaration_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitDeclaration_stmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Declaration_stmtContext* ifccParser::declaration_stmt() {
  Declaration_stmtContext *_localctx = _tracker.createInstance<Declaration_stmtContext>(_ctx, getState());
  enterRule(_localctx, 8, ifccParser::RuleDeclaration_stmt);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(42);
    match(ifccParser::T__0);
    setState(43);
    match(ifccParser::VAR);
    setState(46);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == ifccParser::T__6) {
      setState(44);
      match(ifccParser::T__6);
      setState(45);
      expr(0);
    }
    setState(48);
    match(ifccParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Assign_stmtContext ------------------------------------------------------------------

ifccParser::Assign_stmtContext::Assign_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::Assign_stmtContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}

ifccParser::ExprContext* ifccParser::Assign_stmtContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}


size_t ifccParser::Assign_stmtContext::getRuleIndex() const {
  return ifccParser::RuleAssign_stmt;
}


std::any ifccParser::Assign_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAssign_stmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Assign_stmtContext* ifccParser::assign_stmt() {
  Assign_stmtContext *_localctx = _tracker.createInstance<Assign_stmtContext>(_ctx, getState());
  enterRule(_localctx, 10, ifccParser::RuleAssign_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(50);
    match(ifccParser::VAR);
    setState(51);
    match(ifccParser::T__6);
    setState(52);
    expr(0);
    setState(53);
    match(ifccParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Return_stmtContext ------------------------------------------------------------------

ifccParser::Return_stmtContext::Return_stmtContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

ifccParser::ExprContext* ifccParser::Return_stmtContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}


size_t ifccParser::Return_stmtContext::getRuleIndex() const {
  return ifccParser::RuleReturn_stmt;
}


std::any ifccParser::Return_stmtContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitReturn_stmt(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::Return_stmtContext* ifccParser::return_stmt() {
  Return_stmtContext *_localctx = _tracker.createInstance<Return_stmtContext>(_ctx, getState());
  enterRule(_localctx, 12, ifccParser::RuleReturn_stmt);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(55);
    match(ifccParser::T__8);
    setState(56);
    expr(0);
    setState(57);
    match(ifccParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

ifccParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t ifccParser::ExprContext::getRuleIndex() const {
  return ifccParser::RuleExpr;
}

void ifccParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MinusContext ------------------------------------------------------------------

ifccParser::ExprContext* ifccParser::MinusContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}

ifccParser::MinusContext::MinusContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::MinusContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitMinus(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParentheseContext ------------------------------------------------------------------

ifccParser::ExprContext* ifccParser::ParentheseContext::expr() {
  return getRuleContext<ifccParser::ExprContext>(0);
}

ifccParser::ParentheseContext::ParentheseContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::ParentheseContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitParenthese(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AddSubContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::AddSubContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::AddSubContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::AddSubContext::AddSubContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::AddSubContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitAddSub(this);
  else
    return visitor->visitChildren(this);
}
//----------------- Literal_exprContext ------------------------------------------------------------------

ifccParser::LiteralContext* ifccParser::Literal_exprContext::literal() {
  return getRuleContext<ifccParser::LiteralContext>(0);
}

ifccParser::Literal_exprContext::Literal_exprContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::Literal_exprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitLiteral_expr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- MultiplicationContext ------------------------------------------------------------------

std::vector<ifccParser::ExprContext *> ifccParser::MultiplicationContext::expr() {
  return getRuleContexts<ifccParser::ExprContext>();
}

ifccParser::ExprContext* ifccParser::MultiplicationContext::expr(size_t i) {
  return getRuleContext<ifccParser::ExprContext>(i);
}

ifccParser::MultiplicationContext::MultiplicationContext(ExprContext *ctx) { copyFrom(ctx); }


std::any ifccParser::MultiplicationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitMultiplication(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::ExprContext* ifccParser::expr() {
   return expr(0);
}

ifccParser::ExprContext* ifccParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  ifccParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  ifccParser::ExprContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 14;
  enterRecursionRule(_localctx, 14, ifccParser::RuleExpr, precedence);

    size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(67);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case ifccParser::T__2: {
        _localctx = _tracker.createInstance<ParentheseContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(60);
        match(ifccParser::T__2);
        setState(61);
        expr(0);
        setState(62);
        match(ifccParser::T__3);
        break;
      }

      case ifccParser::T__9: {
        _localctx = _tracker.createInstance<MinusContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(64);
        match(ifccParser::T__9);
        setState(65);
        expr(4);
        break;
      }

      case ifccParser::CONST:
      case ifccParser::VAR: {
        _localctx = _tracker.createInstance<Literal_exprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(66);
        literal();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(77);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(75);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<MultiplicationContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(69);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(70);
          match(ifccParser::T__10);
          setState(71);
          expr(4);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<AddSubContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(72);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(73);
          _la = _input->LA(1);
          if (!(_la == ifccParser::T__9

          || _la == ifccParser::T__11)) {
          _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(74);
          expr(3);
          break;
        }

        default:
          break;
        } 
      }
      setState(79);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- LiteralContext ------------------------------------------------------------------

ifccParser::LiteralContext::LiteralContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* ifccParser::LiteralContext::CONST() {
  return getToken(ifccParser::CONST, 0);
}

tree::TerminalNode* ifccParser::LiteralContext::VAR() {
  return getToken(ifccParser::VAR, 0);
}


size_t ifccParser::LiteralContext::getRuleIndex() const {
  return ifccParser::RuleLiteral;
}


std::any ifccParser::LiteralContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<ifccVisitor*>(visitor))
    return parserVisitor->visitLiteral(this);
  else
    return visitor->visitChildren(this);
}

ifccParser::LiteralContext* ifccParser::literal() {
  LiteralContext *_localctx = _tracker.createInstance<LiteralContext>(_ctx, getState());
  enterRule(_localctx, 16, ifccParser::RuleLiteral);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(80);
    _la = _input->LA(1);
    if (!(_la == ifccParser::CONST

    || _la == ifccParser::VAR)) {
    _errHandler->recoverInline(this);
    }
    else {
      _errHandler->reportMatch(this);
      consume();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool ifccParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 7: return exprSempred(antlrcpp::downCast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool ifccParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 3);
    case 1: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

void ifccParser::initialize() {
#if ANTLR4_USE_THREAD_LOCAL_CACHE
  ifccParserInitialize();
#else
  ::antlr4::internal::call_once(ifccParserOnceFlag, ifccParserInitialize);
#endif
}
