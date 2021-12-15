
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/logics_frontend/grammar\LogicsContracts.g4 by ANTLR 4.9.2


#include "LogicsContractsVisitor.h"

#include "LogicsContractsParser.h"


using namespace antlrcpp;
using namespace antlr4;

LogicsContractsParser::LogicsContractsParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LogicsContractsParser::~LogicsContractsParser() {
  delete _interpreter;
}

std::string LogicsContractsParser::getGrammarFileName() const {
  return "LogicsContracts.g4";
}

const std::vector<std::string>& LogicsContractsParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& LogicsContractsParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- Bin_logic_opContext ------------------------------------------------------------------

LogicsContractsParser::Bin_logic_opContext::Bin_logic_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Bin_logic_opContext::AND() {
  return getToken(LogicsContractsParser::AND, 0);
}

tree::TerminalNode* LogicsContractsParser::Bin_logic_opContext::OR() {
  return getToken(LogicsContractsParser::OR, 0);
}

tree::TerminalNode* LogicsContractsParser::Bin_logic_opContext::IFF() {
  return getToken(LogicsContractsParser::IFF, 0);
}

tree::TerminalNode* LogicsContractsParser::Bin_logic_opContext::IMPLIES() {
  return getToken(LogicsContractsParser::IMPLIES, 0);
}


size_t LogicsContractsParser::Bin_logic_opContext::getRuleIndex() const {
  return LogicsContractsParser::RuleBin_logic_op;
}


antlrcpp::Any LogicsContractsParser::Bin_logic_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitBin_logic_op(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Bin_logic_opContext* LogicsContractsParser::bin_logic_op() {
  Bin_logic_opContext *_localctx = _tracker.createInstance<Bin_logic_opContext>(_ctx, getState());
  enterRule(_localctx, 0, LogicsContractsParser::RuleBin_logic_op);
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
    setState(98);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LogicsContractsParser::AND)
      | (1ULL << LogicsContractsParser::OR)
      | (1ULL << LogicsContractsParser::IMPLIES)
      | (1ULL << LogicsContractsParser::IFF))) != 0))) {
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

//----------------- Unary_logic_opContext ------------------------------------------------------------------

LogicsContractsParser::Unary_logic_opContext::Unary_logic_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Unary_logic_opContext::NOT() {
  return getToken(LogicsContractsParser::NOT, 0);
}


size_t LogicsContractsParser::Unary_logic_opContext::getRuleIndex() const {
  return LogicsContractsParser::RuleUnary_logic_op;
}


antlrcpp::Any LogicsContractsParser::Unary_logic_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitUnary_logic_op(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Unary_logic_opContext* LogicsContractsParser::unary_logic_op() {
  Unary_logic_opContext *_localctx = _tracker.createInstance<Unary_logic_opContext>(_ctx, getState());
  enterRule(_localctx, 2, LogicsContractsParser::RuleUnary_logic_op);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(100);
    match(LogicsContractsParser::NOT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Unary_temp_opContext ------------------------------------------------------------------

LogicsContractsParser::Unary_temp_opContext::Unary_temp_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Unary_temp_opContext::ALWAYS() {
  return getToken(LogicsContractsParser::ALWAYS, 0);
}

tree::TerminalNode* LogicsContractsParser::Unary_temp_opContext::EVENTUALLY() {
  return getToken(LogicsContractsParser::EVENTUALLY, 0);
}

tree::TerminalNode* LogicsContractsParser::Unary_temp_opContext::NEXT() {
  return getToken(LogicsContractsParser::NEXT, 0);
}


size_t LogicsContractsParser::Unary_temp_opContext::getRuleIndex() const {
  return LogicsContractsParser::RuleUnary_temp_op;
}


antlrcpp::Any LogicsContractsParser::Unary_temp_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitUnary_temp_op(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Unary_temp_opContext* LogicsContractsParser::unary_temp_op() {
  Unary_temp_opContext *_localctx = _tracker.createInstance<Unary_temp_opContext>(_ctx, getState());
  enterRule(_localctx, 4, LogicsContractsParser::RuleUnary_temp_op);
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
    setState(102);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LogicsContractsParser::ALWAYS)
      | (1ULL << LogicsContractsParser::EVENTUALLY)
      | (1ULL << LogicsContractsParser::NEXT))) != 0))) {
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

//----------------- Bin_temp_opContext ------------------------------------------------------------------

LogicsContractsParser::Bin_temp_opContext::Bin_temp_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Bin_temp_opContext::UNTIL() {
  return getToken(LogicsContractsParser::UNTIL, 0);
}

tree::TerminalNode* LogicsContractsParser::Bin_temp_opContext::RELEASE() {
  return getToken(LogicsContractsParser::RELEASE, 0);
}


size_t LogicsContractsParser::Bin_temp_opContext::getRuleIndex() const {
  return LogicsContractsParser::RuleBin_temp_op;
}


antlrcpp::Any LogicsContractsParser::Bin_temp_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitBin_temp_op(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Bin_temp_opContext* LogicsContractsParser::bin_temp_op() {
  Bin_temp_opContext *_localctx = _tracker.createInstance<Bin_temp_opContext>(_ctx, getState());
  enterRule(_localctx, 6, LogicsContractsParser::RuleBin_temp_op);
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
    setState(104);
    _la = _input->LA(1);
    if (!(_la == LogicsContractsParser::UNTIL

    || _la == LogicsContractsParser::RELEASE)) {
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

//----------------- Relation_opContext ------------------------------------------------------------------

LogicsContractsParser::Relation_opContext::Relation_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Relation_opContext::EQ() {
  return getToken(LogicsContractsParser::EQ, 0);
}

tree::TerminalNode* LogicsContractsParser::Relation_opContext::NEQ() {
  return getToken(LogicsContractsParser::NEQ, 0);
}

tree::TerminalNode* LogicsContractsParser::Relation_opContext::LT() {
  return getToken(LogicsContractsParser::LT, 0);
}

tree::TerminalNode* LogicsContractsParser::Relation_opContext::LE() {
  return getToken(LogicsContractsParser::LE, 0);
}

tree::TerminalNode* LogicsContractsParser::Relation_opContext::GT() {
  return getToken(LogicsContractsParser::GT, 0);
}

tree::TerminalNode* LogicsContractsParser::Relation_opContext::GE() {
  return getToken(LogicsContractsParser::GE, 0);
}


size_t LogicsContractsParser::Relation_opContext::getRuleIndex() const {
  return LogicsContractsParser::RuleRelation_op;
}


antlrcpp::Any LogicsContractsParser::Relation_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitRelation_op(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Relation_opContext* LogicsContractsParser::relation_op() {
  Relation_opContext *_localctx = _tracker.createInstance<Relation_opContext>(_ctx, getState());
  enterRule(_localctx, 8, LogicsContractsParser::RuleRelation_op);
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
    setState(106);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LogicsContractsParser::EQ)
      | (1ULL << LogicsContractsParser::NEQ)
      | (1ULL << LogicsContractsParser::LT)
      | (1ULL << LogicsContractsParser::LE)
      | (1ULL << LogicsContractsParser::GT)
      | (1ULL << LogicsContractsParser::GE))) != 0))) {
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

//----------------- Bin_math_opContext ------------------------------------------------------------------

LogicsContractsParser::Bin_math_opContext::Bin_math_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Bin_math_opContext::TIMES() {
  return getToken(LogicsContractsParser::TIMES, 0);
}

tree::TerminalNode* LogicsContractsParser::Bin_math_opContext::DIVIDE() {
  return getToken(LogicsContractsParser::DIVIDE, 0);
}

tree::TerminalNode* LogicsContractsParser::Bin_math_opContext::PLUS() {
  return getToken(LogicsContractsParser::PLUS, 0);
}

tree::TerminalNode* LogicsContractsParser::Bin_math_opContext::MINUS() {
  return getToken(LogicsContractsParser::MINUS, 0);
}


size_t LogicsContractsParser::Bin_math_opContext::getRuleIndex() const {
  return LogicsContractsParser::RuleBin_math_op;
}


antlrcpp::Any LogicsContractsParser::Bin_math_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitBin_math_op(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Bin_math_opContext* LogicsContractsParser::bin_math_op() {
  Bin_math_opContext *_localctx = _tracker.createInstance<Bin_math_opContext>(_ctx, getState());
  enterRule(_localctx, 10, LogicsContractsParser::RuleBin_math_op);
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
    setState(108);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LogicsContractsParser::PLUS)
      | (1ULL << LogicsContractsParser::MINUS)
      | (1ULL << LogicsContractsParser::TIMES)
      | (1ULL << LogicsContractsParser::DIVIDE))) != 0))) {
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

//----------------- IntegerKWContext ------------------------------------------------------------------

LogicsContractsParser::IntegerKWContext::IntegerKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::IntegerKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleIntegerKW;
}


antlrcpp::Any LogicsContractsParser::IntegerKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitIntegerKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::IntegerKWContext* LogicsContractsParser::integerKW() {
  IntegerKWContext *_localctx = _tracker.createInstance<IntegerKWContext>(_ctx, getState());
  enterRule(_localctx, 12, LogicsContractsParser::RuleIntegerKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(110);
    match(LogicsContractsParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanKWContext ------------------------------------------------------------------

LogicsContractsParser::BooleanKWContext::BooleanKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::BooleanKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleBooleanKW;
}


antlrcpp::Any LogicsContractsParser::BooleanKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitBooleanKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::BooleanKWContext* LogicsContractsParser::booleanKW() {
  BooleanKWContext *_localctx = _tracker.createInstance<BooleanKWContext>(_ctx, getState());
  enterRule(_localctx, 14, LogicsContractsParser::RuleBooleanKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(112);
    match(LogicsContractsParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RealKWContext ------------------------------------------------------------------

LogicsContractsParser::RealKWContext::RealKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::RealKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleRealKW;
}


antlrcpp::Any LogicsContractsParser::RealKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitRealKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::RealKWContext* LogicsContractsParser::realKW() {
  RealKWContext *_localctx = _tracker.createInstance<RealKWContext>(_ctx, getState());
  enterRule(_localctx, 16, LogicsContractsParser::RuleRealKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(114);
    match(LogicsContractsParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableKWContext ------------------------------------------------------------------

LogicsContractsParser::VariableKWContext::VariableKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::VariableKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleVariableKW;
}


antlrcpp::Any LogicsContractsParser::VariableKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitVariableKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::VariableKWContext* LogicsContractsParser::variableKW() {
  VariableKWContext *_localctx = _tracker.createInstance<VariableKWContext>(_ctx, getState());
  enterRule(_localctx, 18, LogicsContractsParser::RuleVariableKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(116);
    match(LogicsContractsParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InputKWContext ------------------------------------------------------------------

LogicsContractsParser::InputKWContext::InputKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::InputKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleInputKW;
}


antlrcpp::Any LogicsContractsParser::InputKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitInputKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::InputKWContext* LogicsContractsParser::inputKW() {
  InputKWContext *_localctx = _tracker.createInstance<InputKWContext>(_ctx, getState());
  enterRule(_localctx, 20, LogicsContractsParser::RuleInputKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(118);
    match(LogicsContractsParser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OutputKWContext ------------------------------------------------------------------

LogicsContractsParser::OutputKWContext::OutputKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::OutputKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleOutputKW;
}


antlrcpp::Any LogicsContractsParser::OutputKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitOutputKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::OutputKWContext* LogicsContractsParser::outputKW() {
  OutputKWContext *_localctx = _tracker.createInstance<OutputKWContext>(_ctx, getState());
  enterRule(_localctx, 22, LogicsContractsParser::RuleOutputKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(120);
    match(LogicsContractsParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstantKWContext ------------------------------------------------------------------

LogicsContractsParser::ConstantKWContext::ConstantKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::ConstantKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleConstantKW;
}


antlrcpp::Any LogicsContractsParser::ConstantKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitConstantKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::ConstantKWContext* LogicsContractsParser::constantKW() {
  ConstantKWContext *_localctx = _tracker.createInstance<ConstantKWContext>(_ctx, getState());
  enterRule(_localctx, 24, LogicsContractsParser::RuleConstantKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(122);
    match(LogicsContractsParser::T__6);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropositionKwContext ------------------------------------------------------------------

LogicsContractsParser::PropositionKwContext::PropositionKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::PropositionKwContext::getRuleIndex() const {
  return LogicsContractsParser::RulePropositionKw;
}


antlrcpp::Any LogicsContractsParser::PropositionKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitPropositionKw(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::PropositionKwContext* LogicsContractsParser::propositionKw() {
  PropositionKwContext *_localctx = _tracker.createInstance<PropositionKwContext>(_ctx, getState());
  enterRule(_localctx, 26, LogicsContractsParser::RulePropositionKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(124);
    match(LogicsContractsParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IsKwContext ------------------------------------------------------------------

LogicsContractsParser::IsKwContext::IsKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::IsKwContext::getRuleIndex() const {
  return LogicsContractsParser::RuleIsKw;
}


antlrcpp::Any LogicsContractsParser::IsKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitIsKw(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::IsKwContext* LogicsContractsParser::isKw() {
  IsKwContext *_localctx = _tracker.createInstance<IsKwContext>(_ctx, getState());
  enterRule(_localctx, 28, LogicsContractsParser::RuleIsKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(126);
    match(LogicsContractsParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TrueKWContext ------------------------------------------------------------------

LogicsContractsParser::TrueKWContext::TrueKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::TrueKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleTrueKW;
}


antlrcpp::Any LogicsContractsParser::TrueKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitTrueKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::TrueKWContext* LogicsContractsParser::trueKW() {
  TrueKWContext *_localctx = _tracker.createInstance<TrueKWContext>(_ctx, getState());
  enterRule(_localctx, 30, LogicsContractsParser::RuleTrueKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(128);
    match(LogicsContractsParser::T__9);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FalseKWContext ------------------------------------------------------------------

LogicsContractsParser::FalseKWContext::FalseKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::FalseKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleFalseKW;
}


antlrcpp::Any LogicsContractsParser::FalseKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitFalseKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::FalseKWContext* LogicsContractsParser::falseKW() {
  FalseKWContext *_localctx = _tracker.createInstance<FalseKWContext>(_ctx, getState());
  enterRule(_localctx, 32, LogicsContractsParser::RuleFalseKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(130);
    match(LogicsContractsParser::T__10);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ContractKWContext ------------------------------------------------------------------

LogicsContractsParser::ContractKWContext::ContractKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::ContractKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleContractKW;
}


antlrcpp::Any LogicsContractsParser::ContractKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitContractKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::ContractKWContext* LogicsContractsParser::contractKW() {
  ContractKWContext *_localctx = _tracker.createInstance<ContractKWContext>(_ctx, getState());
  enterRule(_localctx, 34, LogicsContractsParser::RuleContractKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(132);
    match(LogicsContractsParser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssumptionsKWContext ------------------------------------------------------------------

LogicsContractsParser::AssumptionsKWContext::AssumptionsKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::AssumptionsKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleAssumptionsKW;
}


antlrcpp::Any LogicsContractsParser::AssumptionsKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitAssumptionsKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::AssumptionsKWContext* LogicsContractsParser::assumptionsKW() {
  AssumptionsKWContext *_localctx = _tracker.createInstance<AssumptionsKWContext>(_ctx, getState());
  enterRule(_localctx, 36, LogicsContractsParser::RuleAssumptionsKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(134);
    match(LogicsContractsParser::T__12);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GuaranteesKWContext ------------------------------------------------------------------

LogicsContractsParser::GuaranteesKWContext::GuaranteesKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::GuaranteesKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleGuaranteesKW;
}


antlrcpp::Any LogicsContractsParser::GuaranteesKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitGuaranteesKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::GuaranteesKWContext* LogicsContractsParser::guaranteesKW() {
  GuaranteesKWContext *_localctx = _tracker.createInstance<GuaranteesKWContext>(_ctx, getState());
  enterRule(_localctx, 38, LogicsContractsParser::RuleGuaranteesKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    match(LogicsContractsParser::T__13);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NameKwContext ------------------------------------------------------------------

LogicsContractsParser::NameKwContext::NameKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogicsContractsParser::NameKwContext::getRuleIndex() const {
  return LogicsContractsParser::RuleNameKw;
}


antlrcpp::Any LogicsContractsParser::NameKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitNameKw(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::NameKwContext* LogicsContractsParser::nameKw() {
  NameKwContext *_localctx = _tracker.createInstance<NameKwContext>(_ctx, getState());
  enterRule(_localctx, 40, LogicsContractsParser::RuleNameKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(138);
    match(LogicsContractsParser::T__14);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CausalityContext ------------------------------------------------------------------

LogicsContractsParser::CausalityContext::CausalityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::InputKWContext* LogicsContractsParser::CausalityContext::inputKW() {
  return getRuleContext<LogicsContractsParser::InputKWContext>(0);
}

LogicsContractsParser::OutputKWContext* LogicsContractsParser::CausalityContext::outputKW() {
  return getRuleContext<LogicsContractsParser::OutputKWContext>(0);
}


size_t LogicsContractsParser::CausalityContext::getRuleIndex() const {
  return LogicsContractsParser::RuleCausality;
}


antlrcpp::Any LogicsContractsParser::CausalityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitCausality(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::CausalityContext* LogicsContractsParser::causality() {
  CausalityContext *_localctx = _tracker.createInstance<CausalityContext>(_ctx, getState());
  enterRule(_localctx, 42, LogicsContractsParser::RuleCausality);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(142);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LogicsContractsParser::T__4: {
        enterOuterAlt(_localctx, 1);
        setState(140);
        inputKW();
        break;
      }

      case LogicsContractsParser::T__5: {
        enterOuterAlt(_localctx, 2);
        setState(141);
        outputKW();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Logic_constantContext ------------------------------------------------------------------

LogicsContractsParser::Logic_constantContext::Logic_constantContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::TrueKWContext* LogicsContractsParser::Logic_constantContext::trueKW() {
  return getRuleContext<LogicsContractsParser::TrueKWContext>(0);
}

LogicsContractsParser::FalseKWContext* LogicsContractsParser::Logic_constantContext::falseKW() {
  return getRuleContext<LogicsContractsParser::FalseKWContext>(0);
}


size_t LogicsContractsParser::Logic_constantContext::getRuleIndex() const {
  return LogicsContractsParser::RuleLogic_constant;
}


antlrcpp::Any LogicsContractsParser::Logic_constantContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitLogic_constant(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Logic_constantContext* LogicsContractsParser::logic_constant() {
  Logic_constantContext *_localctx = _tracker.createInstance<Logic_constantContext>(_ctx, getState());
  enterRule(_localctx, 44, LogicsContractsParser::RuleLogic_constant);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(146);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LogicsContractsParser::T__9: {
        enterOuterAlt(_localctx, 1);
        setState(144);
        trueKW();
        break;
      }

      case LogicsContractsParser::T__10: {
        enterOuterAlt(_localctx, 2);
        setState(145);
        falseKW();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RangeContext ------------------------------------------------------------------

LogicsContractsParser::RangeContext::RangeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::RangeContext::LBRACKET() {
  return getToken(LogicsContractsParser::LBRACKET, 0);
}

std::vector<tree::TerminalNode *> LogicsContractsParser::RangeContext::NUMBER() {
  return getTokens(LogicsContractsParser::NUMBER);
}

tree::TerminalNode* LogicsContractsParser::RangeContext::NUMBER(size_t i) {
  return getToken(LogicsContractsParser::NUMBER, i);
}

tree::TerminalNode* LogicsContractsParser::RangeContext::COLON() {
  return getToken(LogicsContractsParser::COLON, 0);
}

tree::TerminalNode* LogicsContractsParser::RangeContext::RBRACKET() {
  return getToken(LogicsContractsParser::RBRACKET, 0);
}


size_t LogicsContractsParser::RangeContext::getRuleIndex() const {
  return LogicsContractsParser::RuleRange;
}


antlrcpp::Any LogicsContractsParser::RangeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitRange(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::RangeContext* LogicsContractsParser::range() {
  RangeContext *_localctx = _tracker.createInstance<RangeContext>(_ctx, getState());
  enterRule(_localctx, 46, LogicsContractsParser::RuleRange);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(148);
    match(LogicsContractsParser::LBRACKET);
    setState(149);
    match(LogicsContractsParser::NUMBER);
    setState(150);
    match(LogicsContractsParser::COLON);
    setState(151);
    match(LogicsContractsParser::NUMBER);
    setState(152);
    match(LogicsContractsParser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntegerContext ------------------------------------------------------------------

LogicsContractsParser::IntegerContext::IntegerContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::IntegerKWContext* LogicsContractsParser::IntegerContext::integerKW() {
  return getRuleContext<LogicsContractsParser::IntegerKWContext>(0);
}

LogicsContractsParser::RangeContext* LogicsContractsParser::IntegerContext::range() {
  return getRuleContext<LogicsContractsParser::RangeContext>(0);
}


size_t LogicsContractsParser::IntegerContext::getRuleIndex() const {
  return LogicsContractsParser::RuleInteger;
}


antlrcpp::Any LogicsContractsParser::IntegerContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitInteger(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::IntegerContext* LogicsContractsParser::integer() {
  IntegerContext *_localctx = _tracker.createInstance<IntegerContext>(_ctx, getState());
  enterRule(_localctx, 48, LogicsContractsParser::RuleInteger);
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
    setState(154);
    integerKW();
    setState(156);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LogicsContractsParser::LBRACKET) {
      setState(155);
      range();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RealContext ------------------------------------------------------------------

LogicsContractsParser::RealContext::RealContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::RealKWContext* LogicsContractsParser::RealContext::realKW() {
  return getRuleContext<LogicsContractsParser::RealKWContext>(0);
}

LogicsContractsParser::RangeContext* LogicsContractsParser::RealContext::range() {
  return getRuleContext<LogicsContractsParser::RangeContext>(0);
}


size_t LogicsContractsParser::RealContext::getRuleIndex() const {
  return LogicsContractsParser::RuleReal;
}


antlrcpp::Any LogicsContractsParser::RealContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitReal(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::RealContext* LogicsContractsParser::real() {
  RealContext *_localctx = _tracker.createInstance<RealContext>(_ctx, getState());
  enterRule(_localctx, 50, LogicsContractsParser::RuleReal);
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
    setState(158);
    realKW();
    setState(160);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LogicsContractsParser::LBRACKET) {
      setState(159);
      range();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TypeKWContext ------------------------------------------------------------------

LogicsContractsParser::TypeKWContext::TypeKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::IntegerContext* LogicsContractsParser::TypeKWContext::integer() {
  return getRuleContext<LogicsContractsParser::IntegerContext>(0);
}

LogicsContractsParser::RealContext* LogicsContractsParser::TypeKWContext::real() {
  return getRuleContext<LogicsContractsParser::RealContext>(0);
}

LogicsContractsParser::BooleanKWContext* LogicsContractsParser::TypeKWContext::booleanKW() {
  return getRuleContext<LogicsContractsParser::BooleanKWContext>(0);
}


size_t LogicsContractsParser::TypeKWContext::getRuleIndex() const {
  return LogicsContractsParser::RuleTypeKW;
}


antlrcpp::Any LogicsContractsParser::TypeKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitTypeKW(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::TypeKWContext* LogicsContractsParser::typeKW() {
  TypeKWContext *_localctx = _tracker.createInstance<TypeKWContext>(_ctx, getState());
  enterRule(_localctx, 52, LogicsContractsParser::RuleTypeKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(165);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LogicsContractsParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(162);
        integer();
        break;
      }

      case LogicsContractsParser::T__2: {
        enterOuterAlt(_localctx, 2);
        setState(163);
        real();
        break;
      }

      case LogicsContractsParser::T__1: {
        enterOuterAlt(_localctx, 3);
        setState(164);
        booleanKW();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LvalueContext ------------------------------------------------------------------

LogicsContractsParser::LvalueContext::LvalueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::ValueContext* LogicsContractsParser::LvalueContext::value() {
  return getRuleContext<LogicsContractsParser::ValueContext>(0);
}


size_t LogicsContractsParser::LvalueContext::getRuleIndex() const {
  return LogicsContractsParser::RuleLvalue;
}


antlrcpp::Any LogicsContractsParser::LvalueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitLvalue(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::LvalueContext* LogicsContractsParser::lvalue() {
  LvalueContext *_localctx = _tracker.createInstance<LvalueContext>(_ctx, getState());
  enterRule(_localctx, 54, LogicsContractsParser::RuleLvalue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(167);
    value(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RvalueContext ------------------------------------------------------------------

LogicsContractsParser::RvalueContext::RvalueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::ValueContext* LogicsContractsParser::RvalueContext::value() {
  return getRuleContext<LogicsContractsParser::ValueContext>(0);
}


size_t LogicsContractsParser::RvalueContext::getRuleIndex() const {
  return LogicsContractsParser::RuleRvalue;
}


antlrcpp::Any LogicsContractsParser::RvalueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitRvalue(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::RvalueContext* LogicsContractsParser::rvalue() {
  RvalueContext *_localctx = _tracker.createInstance<RvalueContext>(_ctx, getState());
  enterRule(_localctx, 56, LogicsContractsParser::RuleRvalue);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(169);
    value(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Primed_IDContext ------------------------------------------------------------------

LogicsContractsParser::Primed_IDContext::Primed_IDContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Primed_IDContext::ID() {
  return getToken(LogicsContractsParser::ID, 0);
}


size_t LogicsContractsParser::Primed_IDContext::getRuleIndex() const {
  return LogicsContractsParser::RulePrimed_ID;
}


antlrcpp::Any LogicsContractsParser::Primed_IDContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitPrimed_ID(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Primed_IDContext* LogicsContractsParser::primed_ID() {
  Primed_IDContext *_localctx = _tracker.createInstance<Primed_IDContext>(_ctx, getState());
  enterRule(_localctx, 58, LogicsContractsParser::RulePrimed_ID);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(171);
    match(LogicsContractsParser::ID);
    setState(172);
    match(LogicsContractsParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationContext ------------------------------------------------------------------

LogicsContractsParser::RelationContext::RelationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::LvalueContext* LogicsContractsParser::RelationContext::lvalue() {
  return getRuleContext<LogicsContractsParser::LvalueContext>(0);
}

LogicsContractsParser::Relation_opContext* LogicsContractsParser::RelationContext::relation_op() {
  return getRuleContext<LogicsContractsParser::Relation_opContext>(0);
}

LogicsContractsParser::RvalueContext* LogicsContractsParser::RelationContext::rvalue() {
  return getRuleContext<LogicsContractsParser::RvalueContext>(0);
}


size_t LogicsContractsParser::RelationContext::getRuleIndex() const {
  return LogicsContractsParser::RuleRelation;
}


antlrcpp::Any LogicsContractsParser::RelationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitRelation(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::RelationContext* LogicsContractsParser::relation() {
  RelationContext *_localctx = _tracker.createInstance<RelationContext>(_ctx, getState());
  enterRule(_localctx, 60, LogicsContractsParser::RuleRelation);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(174);
    lvalue();
    setState(175);
    relation_op();
    setState(176);
    rvalue();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

LogicsContractsParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::Primed_IDContext* LogicsContractsParser::ValueContext::primed_ID() {
  return getRuleContext<LogicsContractsParser::Primed_IDContext>(0);
}

tree::TerminalNode* LogicsContractsParser::ValueContext::ID() {
  return getToken(LogicsContractsParser::ID, 0);
}

LogicsContractsParser::Minus_IDContext* LogicsContractsParser::ValueContext::minus_ID() {
  return getRuleContext<LogicsContractsParser::Minus_IDContext>(0);
}

LogicsContractsParser::Minus_numberContext* LogicsContractsParser::ValueContext::minus_number() {
  return getRuleContext<LogicsContractsParser::Minus_numberContext>(0);
}

tree::TerminalNode* LogicsContractsParser::ValueContext::NUMBER() {
  return getToken(LogicsContractsParser::NUMBER, 0);
}

tree::TerminalNode* LogicsContractsParser::ValueContext::LBRACKET() {
  return getToken(LogicsContractsParser::LBRACKET, 0);
}

std::vector<LogicsContractsParser::ValueContext *> LogicsContractsParser::ValueContext::value() {
  return getRuleContexts<LogicsContractsParser::ValueContext>();
}

LogicsContractsParser::ValueContext* LogicsContractsParser::ValueContext::value(size_t i) {
  return getRuleContext<LogicsContractsParser::ValueContext>(i);
}

tree::TerminalNode* LogicsContractsParser::ValueContext::RBRACKET() {
  return getToken(LogicsContractsParser::RBRACKET, 0);
}

LogicsContractsParser::Bin_math_opContext* LogicsContractsParser::ValueContext::bin_math_op() {
  return getRuleContext<LogicsContractsParser::Bin_math_opContext>(0);
}


size_t LogicsContractsParser::ValueContext::getRuleIndex() const {
  return LogicsContractsParser::RuleValue;
}


antlrcpp::Any LogicsContractsParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}


LogicsContractsParser::ValueContext* LogicsContractsParser::value() {
   return value(0);
}

LogicsContractsParser::ValueContext* LogicsContractsParser::value(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LogicsContractsParser::ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, parentState);
  LogicsContractsParser::ValueContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 62;
  enterRecursionRule(_localctx, 62, LogicsContractsParser::RuleValue, precedence);

    

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
    setState(188);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      setState(179);
      primed_ID();
      break;
    }

    case 2: {
      setState(180);
      match(LogicsContractsParser::ID);
      break;
    }

    case 3: {
      setState(181);
      minus_ID();
      break;
    }

    case 4: {
      setState(182);
      minus_number();
      break;
    }

    case 5: {
      setState(183);
      match(LogicsContractsParser::NUMBER);
      break;
    }

    case 6: {
      setState(184);
      match(LogicsContractsParser::LBRACKET);
      setState(185);
      value(0);
      setState(186);
      match(LogicsContractsParser::RBRACKET);
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(196);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<ValueContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleValue);
        setState(190);

        if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
        setState(191);
        bin_math_op();
        setState(192);
        value(8); 
      }
      setState(198);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- PairContext ------------------------------------------------------------------

LogicsContractsParser::PairContext::PairContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<LogicsContractsParser::ValueContext *> LogicsContractsParser::PairContext::value() {
  return getRuleContexts<LogicsContractsParser::ValueContext>();
}

LogicsContractsParser::ValueContext* LogicsContractsParser::PairContext::value(size_t i) {
  return getRuleContext<LogicsContractsParser::ValueContext>(i);
}

tree::TerminalNode* LogicsContractsParser::PairContext::COMMA() {
  return getToken(LogicsContractsParser::COMMA, 0);
}


size_t LogicsContractsParser::PairContext::getRuleIndex() const {
  return LogicsContractsParser::RulePair;
}


antlrcpp::Any LogicsContractsParser::PairContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitPair(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::PairContext* LogicsContractsParser::pair() {
  PairContext *_localctx = _tracker.createInstance<PairContext>(_ctx, getState());
  enterRule(_localctx, 64, LogicsContractsParser::RulePair);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(199);
    value(0);
    setState(200);
    match(LogicsContractsParser::COMMA);
    setState(201);
    value(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Interval_leftopenContext ------------------------------------------------------------------

LogicsContractsParser::Interval_leftopenContext::Interval_leftopenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> LogicsContractsParser::Interval_leftopenContext::RSQUARE() {
  return getTokens(LogicsContractsParser::RSQUARE);
}

tree::TerminalNode* LogicsContractsParser::Interval_leftopenContext::RSQUARE(size_t i) {
  return getToken(LogicsContractsParser::RSQUARE, i);
}

LogicsContractsParser::PairContext* LogicsContractsParser::Interval_leftopenContext::pair() {
  return getRuleContext<LogicsContractsParser::PairContext>(0);
}


size_t LogicsContractsParser::Interval_leftopenContext::getRuleIndex() const {
  return LogicsContractsParser::RuleInterval_leftopen;
}


antlrcpp::Any LogicsContractsParser::Interval_leftopenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitInterval_leftopen(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Interval_leftopenContext* LogicsContractsParser::interval_leftopen() {
  Interval_leftopenContext *_localctx = _tracker.createInstance<Interval_leftopenContext>(_ctx, getState());
  enterRule(_localctx, 66, LogicsContractsParser::RuleInterval_leftopen);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(203);
    match(LogicsContractsParser::RSQUARE);
    setState(204);
    pair();
    setState(205);
    match(LogicsContractsParser::RSQUARE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Interval_rightopenContext ------------------------------------------------------------------

LogicsContractsParser::Interval_rightopenContext::Interval_rightopenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> LogicsContractsParser::Interval_rightopenContext::LSQUARE() {
  return getTokens(LogicsContractsParser::LSQUARE);
}

tree::TerminalNode* LogicsContractsParser::Interval_rightopenContext::LSQUARE(size_t i) {
  return getToken(LogicsContractsParser::LSQUARE, i);
}

LogicsContractsParser::PairContext* LogicsContractsParser::Interval_rightopenContext::pair() {
  return getRuleContext<LogicsContractsParser::PairContext>(0);
}


size_t LogicsContractsParser::Interval_rightopenContext::getRuleIndex() const {
  return LogicsContractsParser::RuleInterval_rightopen;
}


antlrcpp::Any LogicsContractsParser::Interval_rightopenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitInterval_rightopen(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Interval_rightopenContext* LogicsContractsParser::interval_rightopen() {
  Interval_rightopenContext *_localctx = _tracker.createInstance<Interval_rightopenContext>(_ctx, getState());
  enterRule(_localctx, 68, LogicsContractsParser::RuleInterval_rightopen);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(207);
    match(LogicsContractsParser::LSQUARE);
    setState(208);
    pair();
    setState(209);
    match(LogicsContractsParser::LSQUARE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Interval_fullopenContext ------------------------------------------------------------------

LogicsContractsParser::Interval_fullopenContext::Interval_fullopenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Interval_fullopenContext::RSQUARE() {
  return getToken(LogicsContractsParser::RSQUARE, 0);
}

LogicsContractsParser::PairContext* LogicsContractsParser::Interval_fullopenContext::pair() {
  return getRuleContext<LogicsContractsParser::PairContext>(0);
}

tree::TerminalNode* LogicsContractsParser::Interval_fullopenContext::LSQUARE() {
  return getToken(LogicsContractsParser::LSQUARE, 0);
}


size_t LogicsContractsParser::Interval_fullopenContext::getRuleIndex() const {
  return LogicsContractsParser::RuleInterval_fullopen;
}


antlrcpp::Any LogicsContractsParser::Interval_fullopenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitInterval_fullopen(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Interval_fullopenContext* LogicsContractsParser::interval_fullopen() {
  Interval_fullopenContext *_localctx = _tracker.createInstance<Interval_fullopenContext>(_ctx, getState());
  enterRule(_localctx, 70, LogicsContractsParser::RuleInterval_fullopen);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(211);
    match(LogicsContractsParser::RSQUARE);
    setState(212);
    pair();
    setState(213);
    match(LogicsContractsParser::LSQUARE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Interval_closedContext ------------------------------------------------------------------

LogicsContractsParser::Interval_closedContext::Interval_closedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Interval_closedContext::LSQUARE() {
  return getToken(LogicsContractsParser::LSQUARE, 0);
}

LogicsContractsParser::PairContext* LogicsContractsParser::Interval_closedContext::pair() {
  return getRuleContext<LogicsContractsParser::PairContext>(0);
}

tree::TerminalNode* LogicsContractsParser::Interval_closedContext::RSQUARE() {
  return getToken(LogicsContractsParser::RSQUARE, 0);
}


size_t LogicsContractsParser::Interval_closedContext::getRuleIndex() const {
  return LogicsContractsParser::RuleInterval_closed;
}


antlrcpp::Any LogicsContractsParser::Interval_closedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitInterval_closed(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Interval_closedContext* LogicsContractsParser::interval_closed() {
  Interval_closedContext *_localctx = _tracker.createInstance<Interval_closedContext>(_ctx, getState());
  enterRule(_localctx, 72, LogicsContractsParser::RuleInterval_closed);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(215);
    match(LogicsContractsParser::LSQUARE);
    setState(216);
    pair();
    setState(217);
    match(LogicsContractsParser::RSQUARE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntervalContext ------------------------------------------------------------------

LogicsContractsParser::IntervalContext::IntervalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::Interval_closedContext* LogicsContractsParser::IntervalContext::interval_closed() {
  return getRuleContext<LogicsContractsParser::Interval_closedContext>(0);
}

LogicsContractsParser::Interval_fullopenContext* LogicsContractsParser::IntervalContext::interval_fullopen() {
  return getRuleContext<LogicsContractsParser::Interval_fullopenContext>(0);
}

LogicsContractsParser::Interval_leftopenContext* LogicsContractsParser::IntervalContext::interval_leftopen() {
  return getRuleContext<LogicsContractsParser::Interval_leftopenContext>(0);
}

LogicsContractsParser::Interval_rightopenContext* LogicsContractsParser::IntervalContext::interval_rightopen() {
  return getRuleContext<LogicsContractsParser::Interval_rightopenContext>(0);
}


size_t LogicsContractsParser::IntervalContext::getRuleIndex() const {
  return LogicsContractsParser::RuleInterval;
}


antlrcpp::Any LogicsContractsParser::IntervalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitInterval(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::IntervalContext* LogicsContractsParser::interval() {
  IntervalContext *_localctx = _tracker.createInstance<IntervalContext>(_ctx, getState());
  enterRule(_localctx, 74, LogicsContractsParser::RuleInterval);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(223);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(219);
      interval_closed();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(220);
      interval_fullopen();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(221);
      interval_leftopen();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(222);
      interval_rightopen();
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

//----------------- FormulaContext ------------------------------------------------------------------

LogicsContractsParser::FormulaContext::FormulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::FormulaContext::LBRACKET() {
  return getToken(LogicsContractsParser::LBRACKET, 0);
}

std::vector<LogicsContractsParser::FormulaContext *> LogicsContractsParser::FormulaContext::formula() {
  return getRuleContexts<LogicsContractsParser::FormulaContext>();
}

LogicsContractsParser::FormulaContext* LogicsContractsParser::FormulaContext::formula(size_t i) {
  return getRuleContext<LogicsContractsParser::FormulaContext>(i);
}

tree::TerminalNode* LogicsContractsParser::FormulaContext::RBRACKET() {
  return getToken(LogicsContractsParser::RBRACKET, 0);
}

LogicsContractsParser::Unary_logic_opContext* LogicsContractsParser::FormulaContext::unary_logic_op() {
  return getRuleContext<LogicsContractsParser::Unary_logic_opContext>(0);
}

LogicsContractsParser::Unary_temp_opContext* LogicsContractsParser::FormulaContext::unary_temp_op() {
  return getRuleContext<LogicsContractsParser::Unary_temp_opContext>(0);
}

LogicsContractsParser::IntervalContext* LogicsContractsParser::FormulaContext::interval() {
  return getRuleContext<LogicsContractsParser::IntervalContext>(0);
}

LogicsContractsParser::AtomContext* LogicsContractsParser::FormulaContext::atom() {
  return getRuleContext<LogicsContractsParser::AtomContext>(0);
}

LogicsContractsParser::Bin_logic_opContext* LogicsContractsParser::FormulaContext::bin_logic_op() {
  return getRuleContext<LogicsContractsParser::Bin_logic_opContext>(0);
}

LogicsContractsParser::Bin_temp_opContext* LogicsContractsParser::FormulaContext::bin_temp_op() {
  return getRuleContext<LogicsContractsParser::Bin_temp_opContext>(0);
}


size_t LogicsContractsParser::FormulaContext::getRuleIndex() const {
  return LogicsContractsParser::RuleFormula;
}


antlrcpp::Any LogicsContractsParser::FormulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitFormula(this);
  else
    return visitor->visitChildren(this);
}


LogicsContractsParser::FormulaContext* LogicsContractsParser::formula() {
   return formula(0);
}

LogicsContractsParser::FormulaContext* LogicsContractsParser::formula(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LogicsContractsParser::FormulaContext *_localctx = _tracker.createInstance<FormulaContext>(_ctx, parentState);
  LogicsContractsParser::FormulaContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 76;
  enterRecursionRule(_localctx, 76, LogicsContractsParser::RuleFormula, precedence);

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
    setState(240);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx)) {
    case 1: {
      setState(226);
      match(LogicsContractsParser::LBRACKET);
      setState(227);
      formula(0);
      setState(228);
      match(LogicsContractsParser::RBRACKET);
      break;
    }

    case 2: {
      setState(230);
      unary_logic_op();
      setState(231);
      formula(5);
      break;
    }

    case 3: {
      setState(233);
      unary_temp_op();
      setState(235);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == LogicsContractsParser::LSQUARE

      || _la == LogicsContractsParser::RSQUARE) {
        setState(234);
        interval();
      }
      setState(237);
      formula(3);
      break;
    }

    case 4: {
      setState(239);
      atom();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(255);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(253);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<FormulaContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleFormula);
          setState(242);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(243);
          bin_logic_op();
          setState(244);
          formula(5);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<FormulaContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleFormula);
          setState(246);

          if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
          setState(247);
          bin_temp_op();
          setState(249);
          _errHandler->sync(this);

          _la = _input->LA(1);
          if (_la == LogicsContractsParser::LSQUARE

          || _la == LogicsContractsParser::RSQUARE) {
            setState(248);
            interval();
          }
          setState(251);
          formula(3);
          break;
        }

        default:
          break;
        } 
      }
      setState(257);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- Minus_numberContext ------------------------------------------------------------------

LogicsContractsParser::Minus_numberContext::Minus_numberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Minus_numberContext::MINUS() {
  return getToken(LogicsContractsParser::MINUS, 0);
}

tree::TerminalNode* LogicsContractsParser::Minus_numberContext::NUMBER() {
  return getToken(LogicsContractsParser::NUMBER, 0);
}


size_t LogicsContractsParser::Minus_numberContext::getRuleIndex() const {
  return LogicsContractsParser::RuleMinus_number;
}


antlrcpp::Any LogicsContractsParser::Minus_numberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitMinus_number(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Minus_numberContext* LogicsContractsParser::minus_number() {
  Minus_numberContext *_localctx = _tracker.createInstance<Minus_numberContext>(_ctx, getState());
  enterRule(_localctx, 78, LogicsContractsParser::RuleMinus_number);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(258);
    match(LogicsContractsParser::MINUS);
    setState(259);
    match(LogicsContractsParser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Minus_IDContext ------------------------------------------------------------------

LogicsContractsParser::Minus_IDContext::Minus_IDContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogicsContractsParser::Minus_IDContext::MINUS() {
  return getToken(LogicsContractsParser::MINUS, 0);
}

tree::TerminalNode* LogicsContractsParser::Minus_IDContext::ID() {
  return getToken(LogicsContractsParser::ID, 0);
}


size_t LogicsContractsParser::Minus_IDContext::getRuleIndex() const {
  return LogicsContractsParser::RuleMinus_ID;
}


antlrcpp::Any LogicsContractsParser::Minus_IDContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitMinus_ID(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Minus_IDContext* LogicsContractsParser::minus_ID() {
  Minus_IDContext *_localctx = _tracker.createInstance<Minus_IDContext>(_ctx, getState());
  enterRule(_localctx, 80, LogicsContractsParser::RuleMinus_ID);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(261);
    match(LogicsContractsParser::MINUS);
    setState(262);
    match(LogicsContractsParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

LogicsContractsParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::Logic_constantContext* LogicsContractsParser::AtomContext::logic_constant() {
  return getRuleContext<LogicsContractsParser::Logic_constantContext>(0);
}

LogicsContractsParser::RelationContext* LogicsContractsParser::AtomContext::relation() {
  return getRuleContext<LogicsContractsParser::RelationContext>(0);
}

tree::TerminalNode* LogicsContractsParser::AtomContext::ID() {
  return getToken(LogicsContractsParser::ID, 0);
}

LogicsContractsParser::Primed_IDContext* LogicsContractsParser::AtomContext::primed_ID() {
  return getRuleContext<LogicsContractsParser::Primed_IDContext>(0);
}


size_t LogicsContractsParser::AtomContext::getRuleIndex() const {
  return LogicsContractsParser::RuleAtom;
}


antlrcpp::Any LogicsContractsParser::AtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitAtom(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::AtomContext* LogicsContractsParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 82, LogicsContractsParser::RuleAtom);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(268);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(264);
      logic_constant();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(265);
      relation();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(266);
      match(LogicsContractsParser::ID);
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(267);
      primed_ID();
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

//----------------- Single_formulaContext ------------------------------------------------------------------

LogicsContractsParser::Single_formulaContext::Single_formulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::FormulaContext* LogicsContractsParser::Single_formulaContext::formula() {
  return getRuleContext<LogicsContractsParser::FormulaContext>(0);
}

tree::TerminalNode* LogicsContractsParser::Single_formulaContext::ENDST() {
  return getToken(LogicsContractsParser::ENDST, 0);
}


size_t LogicsContractsParser::Single_formulaContext::getRuleIndex() const {
  return LogicsContractsParser::RuleSingle_formula;
}


antlrcpp::Any LogicsContractsParser::Single_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitSingle_formula(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::Single_formulaContext* LogicsContractsParser::single_formula() {
  Single_formulaContext *_localctx = _tracker.createInstance<Single_formulaContext>(_ctx, getState());
  enterRule(_localctx, 84, LogicsContractsParser::RuleSingle_formula);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(270);
    formula(0);
    setState(271);
    match(LogicsContractsParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssumptionsContext ------------------------------------------------------------------

LogicsContractsParser::AssumptionsContext::AssumptionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::AssumptionsKWContext* LogicsContractsParser::AssumptionsContext::assumptionsKW() {
  return getRuleContext<LogicsContractsParser::AssumptionsKWContext>(0);
}

tree::TerminalNode* LogicsContractsParser::AssumptionsContext::COLON() {
  return getToken(LogicsContractsParser::COLON, 0);
}

std::vector<LogicsContractsParser::Single_formulaContext *> LogicsContractsParser::AssumptionsContext::single_formula() {
  return getRuleContexts<LogicsContractsParser::Single_formulaContext>();
}

LogicsContractsParser::Single_formulaContext* LogicsContractsParser::AssumptionsContext::single_formula(size_t i) {
  return getRuleContext<LogicsContractsParser::Single_formulaContext>(i);
}


size_t LogicsContractsParser::AssumptionsContext::getRuleIndex() const {
  return LogicsContractsParser::RuleAssumptions;
}


antlrcpp::Any LogicsContractsParser::AssumptionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitAssumptions(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::AssumptionsContext* LogicsContractsParser::assumptions() {
  AssumptionsContext *_localctx = _tracker.createInstance<AssumptionsContext>(_ctx, getState());
  enterRule(_localctx, 86, LogicsContractsParser::RuleAssumptions);
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
    setState(273);
    assumptionsKW();
    setState(274);
    match(LogicsContractsParser::COLON);
    setState(276); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(275);
      single_formula();
      setState(278); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LogicsContractsParser::T__9)
      | (1ULL << LogicsContractsParser::T__10)
      | (1ULL << LogicsContractsParser::NOT)
      | (1ULL << LogicsContractsParser::ALWAYS)
      | (1ULL << LogicsContractsParser::EVENTUALLY)
      | (1ULL << LogicsContractsParser::NEXT)
      | (1ULL << LogicsContractsParser::MINUS)
      | (1ULL << LogicsContractsParser::LBRACKET)
      | (1ULL << LogicsContractsParser::ID)
      | (1ULL << LogicsContractsParser::NUMBER))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GuaranteesContext ------------------------------------------------------------------

LogicsContractsParser::GuaranteesContext::GuaranteesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::GuaranteesKWContext* LogicsContractsParser::GuaranteesContext::guaranteesKW() {
  return getRuleContext<LogicsContractsParser::GuaranteesKWContext>(0);
}

tree::TerminalNode* LogicsContractsParser::GuaranteesContext::COLON() {
  return getToken(LogicsContractsParser::COLON, 0);
}

std::vector<LogicsContractsParser::Single_formulaContext *> LogicsContractsParser::GuaranteesContext::single_formula() {
  return getRuleContexts<LogicsContractsParser::Single_formulaContext>();
}

LogicsContractsParser::Single_formulaContext* LogicsContractsParser::GuaranteesContext::single_formula(size_t i) {
  return getRuleContext<LogicsContractsParser::Single_formulaContext>(i);
}


size_t LogicsContractsParser::GuaranteesContext::getRuleIndex() const {
  return LogicsContractsParser::RuleGuarantees;
}


antlrcpp::Any LogicsContractsParser::GuaranteesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitGuarantees(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::GuaranteesContext* LogicsContractsParser::guarantees() {
  GuaranteesContext *_localctx = _tracker.createInstance<GuaranteesContext>(_ctx, getState());
  enterRule(_localctx, 88, LogicsContractsParser::RuleGuarantees);
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
    setState(280);
    guaranteesKW();
    setState(281);
    match(LogicsContractsParser::COLON);
    setState(283); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(282);
      single_formula();
      setState(285); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LogicsContractsParser::T__9)
      | (1ULL << LogicsContractsParser::T__10)
      | (1ULL << LogicsContractsParser::NOT)
      | (1ULL << LogicsContractsParser::ALWAYS)
      | (1ULL << LogicsContractsParser::EVENTUALLY)
      | (1ULL << LogicsContractsParser::NEXT)
      | (1ULL << LogicsContractsParser::MINUS)
      | (1ULL << LogicsContractsParser::LBRACKET)
      | (1ULL << LogicsContractsParser::ID)
      | (1ULL << LogicsContractsParser::NUMBER))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

LogicsContractsParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::TypeKWContext* LogicsContractsParser::DeclarationContext::typeKW() {
  return getRuleContext<LogicsContractsParser::TypeKWContext>(0);
}

LogicsContractsParser::VariableKWContext* LogicsContractsParser::DeclarationContext::variableKW() {
  return getRuleContext<LogicsContractsParser::VariableKWContext>(0);
}

tree::TerminalNode* LogicsContractsParser::DeclarationContext::ID() {
  return getToken(LogicsContractsParser::ID, 0);
}

tree::TerminalNode* LogicsContractsParser::DeclarationContext::ENDST() {
  return getToken(LogicsContractsParser::ENDST, 0);
}

LogicsContractsParser::CausalityContext* LogicsContractsParser::DeclarationContext::causality() {
  return getRuleContext<LogicsContractsParser::CausalityContext>(0);
}

LogicsContractsParser::ConstantKWContext* LogicsContractsParser::DeclarationContext::constantKW() {
  return getRuleContext<LogicsContractsParser::ConstantKWContext>(0);
}

tree::TerminalNode* LogicsContractsParser::DeclarationContext::NUMBER() {
  return getToken(LogicsContractsParser::NUMBER, 0);
}

LogicsContractsParser::PropositionKwContext* LogicsContractsParser::DeclarationContext::propositionKw() {
  return getRuleContext<LogicsContractsParser::PropositionKwContext>(0);
}

LogicsContractsParser::IsKwContext* LogicsContractsParser::DeclarationContext::isKw() {
  return getRuleContext<LogicsContractsParser::IsKwContext>(0);
}

LogicsContractsParser::RelationContext* LogicsContractsParser::DeclarationContext::relation() {
  return getRuleContext<LogicsContractsParser::RelationContext>(0);
}


size_t LogicsContractsParser::DeclarationContext::getRuleIndex() const {
  return LogicsContractsParser::RuleDeclaration;
}


antlrcpp::Any LogicsContractsParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::DeclarationContext* LogicsContractsParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 90, LogicsContractsParser::RuleDeclaration);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(310);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 18, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(288);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == LogicsContractsParser::T__4

      || _la == LogicsContractsParser::T__5) {
        setState(287);
        causality();
      }
      setState(290);
      typeKW();
      setState(291);
      variableKW();
      setState(292);
      match(LogicsContractsParser::ID);
      setState(293);
      match(LogicsContractsParser::ENDST);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(295);
      typeKW();
      setState(296);
      constantKW();
      setState(297);
      match(LogicsContractsParser::ID);
      setState(298);
      match(LogicsContractsParser::NUMBER);
      setState(299);
      match(LogicsContractsParser::ENDST);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(301);
      propositionKw();
      setState(302);
      match(LogicsContractsParser::ID);
      setState(306);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == LogicsContractsParser::T__8) {
        setState(303);
        isKw();
        setState(304);
        relation();
      }
      setState(308);
      match(LogicsContractsParser::ENDST);
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

//----------------- ContractContext ------------------------------------------------------------------

LogicsContractsParser::ContractContext::ContractContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::ContractKWContext* LogicsContractsParser::ContractContext::contractKW() {
  return getRuleContext<LogicsContractsParser::ContractKWContext>(0);
}

tree::TerminalNode* LogicsContractsParser::ContractContext::ID() {
  return getToken(LogicsContractsParser::ID, 0);
}

tree::TerminalNode* LogicsContractsParser::ContractContext::COLON() {
  return getToken(LogicsContractsParser::COLON, 0);
}

std::vector<LogicsContractsParser::DeclarationContext *> LogicsContractsParser::ContractContext::declaration() {
  return getRuleContexts<LogicsContractsParser::DeclarationContext>();
}

LogicsContractsParser::DeclarationContext* LogicsContractsParser::ContractContext::declaration(size_t i) {
  return getRuleContext<LogicsContractsParser::DeclarationContext>(i);
}

LogicsContractsParser::AssumptionsContext* LogicsContractsParser::ContractContext::assumptions() {
  return getRuleContext<LogicsContractsParser::AssumptionsContext>(0);
}

LogicsContractsParser::GuaranteesContext* LogicsContractsParser::ContractContext::guarantees() {
  return getRuleContext<LogicsContractsParser::GuaranteesContext>(0);
}


size_t LogicsContractsParser::ContractContext::getRuleIndex() const {
  return LogicsContractsParser::RuleContract;
}


antlrcpp::Any LogicsContractsParser::ContractContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitContract(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::ContractContext* LogicsContractsParser::contract() {
  ContractContext *_localctx = _tracker.createInstance<ContractContext>(_ctx, getState());
  enterRule(_localctx, 92, LogicsContractsParser::RuleContract);
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
    setState(312);
    contractKW();
    setState(313);
    match(LogicsContractsParser::ID);
    setState(314);
    match(LogicsContractsParser::COLON);
    setState(318);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LogicsContractsParser::T__0)
      | (1ULL << LogicsContractsParser::T__1)
      | (1ULL << LogicsContractsParser::T__2)
      | (1ULL << LogicsContractsParser::T__4)
      | (1ULL << LogicsContractsParser::T__5)
      | (1ULL << LogicsContractsParser::T__7))) != 0)) {
      setState(315);
      declaration();
      setState(320);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(322);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LogicsContractsParser::T__12) {
      setState(321);
      assumptions();
    }
    setState(325);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LogicsContractsParser::T__13) {
      setState(324);
      guarantees();
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NameContext ------------------------------------------------------------------

LogicsContractsParser::NameContext::NameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::NameKwContext* LogicsContractsParser::NameContext::nameKw() {
  return getRuleContext<LogicsContractsParser::NameKwContext>(0);
}

tree::TerminalNode* LogicsContractsParser::NameContext::COLON() {
  return getToken(LogicsContractsParser::COLON, 0);
}

tree::TerminalNode* LogicsContractsParser::NameContext::ID() {
  return getToken(LogicsContractsParser::ID, 0);
}

tree::TerminalNode* LogicsContractsParser::NameContext::ENDST() {
  return getToken(LogicsContractsParser::ENDST, 0);
}


size_t LogicsContractsParser::NameContext::getRuleIndex() const {
  return LogicsContractsParser::RuleName;
}


antlrcpp::Any LogicsContractsParser::NameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitName(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::NameContext* LogicsContractsParser::name() {
  NameContext *_localctx = _tracker.createInstance<NameContext>(_ctx, getState());
  enterRule(_localctx, 94, LogicsContractsParser::RuleName);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(327);
    nameKw();
    setState(328);
    match(LogicsContractsParser::COLON);
    setState(329);
    match(LogicsContractsParser::ID);
    setState(330);
    match(LogicsContractsParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SystemSpecContext ------------------------------------------------------------------

LogicsContractsParser::SystemSpecContext::SystemSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogicsContractsParser::NameContext* LogicsContractsParser::SystemSpecContext::name() {
  return getRuleContext<LogicsContractsParser::NameContext>(0);
}

std::vector<LogicsContractsParser::DeclarationContext *> LogicsContractsParser::SystemSpecContext::declaration() {
  return getRuleContexts<LogicsContractsParser::DeclarationContext>();
}

LogicsContractsParser::DeclarationContext* LogicsContractsParser::SystemSpecContext::declaration(size_t i) {
  return getRuleContext<LogicsContractsParser::DeclarationContext>(i);
}

std::vector<LogicsContractsParser::ContractContext *> LogicsContractsParser::SystemSpecContext::contract() {
  return getRuleContexts<LogicsContractsParser::ContractContext>();
}

LogicsContractsParser::ContractContext* LogicsContractsParser::SystemSpecContext::contract(size_t i) {
  return getRuleContext<LogicsContractsParser::ContractContext>(i);
}


size_t LogicsContractsParser::SystemSpecContext::getRuleIndex() const {
  return LogicsContractsParser::RuleSystemSpec;
}


antlrcpp::Any LogicsContractsParser::SystemSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogicsContractsVisitor*>(visitor))
    return parserVisitor->visitSystemSpec(this);
  else
    return visitor->visitChildren(this);
}

LogicsContractsParser::SystemSpecContext* LogicsContractsParser::systemSpec() {
  SystemSpecContext *_localctx = _tracker.createInstance<SystemSpecContext>(_ctx, getState());
  enterRule(_localctx, 96, LogicsContractsParser::RuleSystemSpec);
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
    setState(333);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LogicsContractsParser::T__14) {
      setState(332);
      name();
    }
    setState(338);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LogicsContractsParser::T__0)
      | (1ULL << LogicsContractsParser::T__1)
      | (1ULL << LogicsContractsParser::T__2)
      | (1ULL << LogicsContractsParser::T__4)
      | (1ULL << LogicsContractsParser::T__5)
      | (1ULL << LogicsContractsParser::T__7))) != 0)) {
      setState(335);
      declaration();
      setState(340);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(342); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(341);
      contract();
      setState(344); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LogicsContractsParser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool LogicsContractsParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 31: return valueSempred(dynamic_cast<ValueContext *>(context), predicateIndex);
    case 38: return formulaSempred(dynamic_cast<FormulaContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool LogicsContractsParser::valueSempred(ValueContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 7);

  default:
    break;
  }
  return true;
}

bool LogicsContractsParser::formulaSempred(FormulaContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return precpred(_ctx, 4);
    case 2: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> LogicsContractsParser::_decisionToDFA;
atn::PredictionContextCache LogicsContractsParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LogicsContractsParser::_atn;
std::vector<uint16_t> LogicsContractsParser::_serializedATN;

std::vector<std::string> LogicsContractsParser::_ruleNames = {
  "bin_logic_op", "unary_logic_op", "unary_temp_op", "bin_temp_op", "relation_op", 
  "bin_math_op", "integerKW", "booleanKW", "realKW", "variableKW", "inputKW", 
  "outputKW", "constantKW", "propositionKw", "isKw", "trueKW", "falseKW", 
  "contractKW", "assumptionsKW", "guaranteesKW", "nameKw", "causality", 
  "logic_constant", "range", "integer", "real", "typeKW", "lvalue", "rvalue", 
  "primed_ID", "relation", "value", "pair", "interval_leftopen", "interval_rightopen", 
  "interval_fullopen", "interval_closed", "interval", "formula", "minus_number", 
  "minus_ID", "atom", "single_formula", "assumptions", "guarantees", "declaration", 
  "contract", "name", "systemSpec"
};

std::vector<std::string> LogicsContractsParser::_literalNames = {
  "", "'integer'", "'boolean'", "'real'", "'variable'", "'input'", "'output'", 
  "'constant'", "'proposition'", "'is'", "'true'", "'false'", "'CONTRACT'", 
  "'Assumptions'", "'Guarantees'", "'NAME'", "'''", "", "", "", "", "", 
  "'->'", "'<->'", "':'", "'G'", "'F'", "'X'", "'U'", "'R'", "'='", "'!='", 
  "'<'", "'<='", "'>'", "'>='", "'+'", "'-'", "'*'", "'/'", "'('", "')'", 
  "'['", "']'", "','", "'.'", "", "", "", "';'"
};

std::vector<std::string> LogicsContractsParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "WS", 
  "LINE_COMMENT", "AND", "OR", "NOT", "IMPLIES", "IFF", "COLON", "ALWAYS", 
  "EVENTUALLY", "NEXT", "UNTIL", "RELEASE", "EQ", "NEQ", "LT", "LE", "GT", 
  "GE", "PLUS", "MINUS", "TIMES", "DIVIDE", "LBRACKET", "RBRACKET", "LSQUARE", 
  "RSQUARE", "COMMA", "DOT", "ID", "DECIMAL", "NUMBER", "ENDST"
};

dfa::Vocabulary LogicsContractsParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LogicsContractsParser::_tokenNames;

LogicsContractsParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  static const uint16_t serializedATNSegment0[] = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
       0x3, 0x33, 0x15d, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 
       0x9, 0x14, 0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 
       0x9, 0x17, 0x4, 0x18, 0x9, 0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 
       0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 
       0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 0x1f, 0x9, 0x1f, 0x4, 0x20, 
       0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 0x9, 0x22, 0x4, 0x23, 
       0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 0x25, 0x4, 0x26, 
       0x9, 0x26, 0x4, 0x27, 0x9, 0x27, 0x4, 0x28, 0x9, 0x28, 0x4, 0x29, 
       0x9, 0x29, 0x4, 0x2a, 0x9, 0x2a, 0x4, 0x2b, 0x9, 0x2b, 0x4, 0x2c, 
       0x9, 0x2c, 0x4, 0x2d, 0x9, 0x2d, 0x4, 0x2e, 0x9, 0x2e, 0x4, 0x2f, 
       0x9, 0x2f, 0x4, 0x30, 0x9, 0x30, 0x4, 0x31, 0x9, 0x31, 0x4, 0x32, 
       0x9, 0x32, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 
       0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 
       0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 
       0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 
       0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 
       0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 
       0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 
       0x17, 0x5, 0x17, 0x91, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 
       0x95, 0xa, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 
       0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x5, 0x1a, 0x9f, 0xa, 0x1a, 
       0x3, 0x1b, 0x3, 0x1b, 0x5, 0x1b, 0xa3, 0xa, 0x1b, 0x3, 0x1c, 0x3, 
       0x1c, 0x3, 0x1c, 0x5, 0x1c, 0xa8, 0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 
       0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x20, 
       0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 
       0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 
       0x3, 0x21, 0x5, 0x21, 0xbf, 0xa, 0x21, 0x3, 0x21, 0x3, 0x21, 0x3, 
       0x21, 0x3, 0x21, 0x7, 0x21, 0xc5, 0xa, 0x21, 0xc, 0x21, 0xe, 0x21, 
       0xc8, 0xb, 0x21, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 0x22, 0x3, 
       0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 
       0x24, 0x3, 0x24, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 
       0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 0x3, 0x27, 0x3, 
       0x27, 0x3, 0x27, 0x5, 0x27, 0xe2, 0xa, 0x27, 0x3, 0x28, 0x3, 0x28, 
       0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 
       0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 0xee, 0xa, 0x28, 0x3, 0x28, 0x3, 
       0x28, 0x3, 0x28, 0x5, 0x28, 0xf3, 0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 
       0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x3, 0x28, 0x5, 0x28, 
       0xfc, 0xa, 0x28, 0x3, 0x28, 0x3, 0x28, 0x7, 0x28, 0x100, 0xa, 0x28, 
       0xc, 0x28, 0xe, 0x28, 0x103, 0xb, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 
       0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2a, 0x3, 0x2b, 0x3, 0x2b, 0x3, 
       0x2b, 0x3, 0x2b, 0x5, 0x2b, 0x10f, 0xa, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 
       0x3, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 0x2d, 0x6, 0x2d, 0x117, 0xa, 
       0x2d, 0xd, 0x2d, 0xe, 0x2d, 0x118, 0x3, 0x2e, 0x3, 0x2e, 0x3, 0x2e, 
       0x6, 0x2e, 0x11e, 0xa, 0x2e, 0xd, 0x2e, 0xe, 0x2e, 0x11f, 0x3, 0x2f, 
       0x5, 0x2f, 0x123, 0xa, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 
       0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 
       0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 
       0x2f, 0x5, 0x2f, 0x135, 0xa, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x5, 0x2f, 
       0x139, 0xa, 0x2f, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x3, 0x30, 0x7, 
       0x30, 0x13f, 0xa, 0x30, 0xc, 0x30, 0xe, 0x30, 0x142, 0xb, 0x30, 0x3, 
       0x30, 0x5, 0x30, 0x145, 0xa, 0x30, 0x3, 0x30, 0x5, 0x30, 0x148, 0xa, 
       0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 
       0x32, 0x5, 0x32, 0x150, 0xa, 0x32, 0x3, 0x32, 0x7, 0x32, 0x153, 0xa, 
       0x32, 0xc, 0x32, 0xe, 0x32, 0x156, 0xb, 0x32, 0x3, 0x32, 0x6, 0x32, 
       0x159, 0xa, 0x32, 0xd, 0x32, 0xe, 0x32, 0x15a, 0x3, 0x32, 0x2, 0x4, 
       0x40, 0x4e, 0x33, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 
       0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 
       0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 
       0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 
       0x56, 0x58, 0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x2, 0x7, 0x4, 0x2, 0x15, 
       0x16, 0x18, 0x19, 0x3, 0x2, 0x1b, 0x1d, 0x3, 0x2, 0x1e, 0x1f, 0x3, 
       0x2, 0x20, 0x25, 0x3, 0x2, 0x26, 0x29, 0x2, 0x150, 0x2, 0x64, 0x3, 
       0x2, 0x2, 0x2, 0x4, 0x66, 0x3, 0x2, 0x2, 0x2, 0x6, 0x68, 0x3, 0x2, 
       0x2, 0x2, 0x8, 0x6a, 0x3, 0x2, 0x2, 0x2, 0xa, 0x6c, 0x3, 0x2, 0x2, 
       0x2, 0xc, 0x6e, 0x3, 0x2, 0x2, 0x2, 0xe, 0x70, 0x3, 0x2, 0x2, 0x2, 
       0x10, 0x72, 0x3, 0x2, 0x2, 0x2, 0x12, 0x74, 0x3, 0x2, 0x2, 0x2, 0x14, 
       0x76, 0x3, 0x2, 0x2, 0x2, 0x16, 0x78, 0x3, 0x2, 0x2, 0x2, 0x18, 0x7a, 
       0x3, 0x2, 0x2, 0x2, 0x1a, 0x7c, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x7e, 0x3, 
       0x2, 0x2, 0x2, 0x1e, 0x80, 0x3, 0x2, 0x2, 0x2, 0x20, 0x82, 0x3, 0x2, 
       0x2, 0x2, 0x22, 0x84, 0x3, 0x2, 0x2, 0x2, 0x24, 0x86, 0x3, 0x2, 0x2, 
       0x2, 0x26, 0x88, 0x3, 0x2, 0x2, 0x2, 0x28, 0x8a, 0x3, 0x2, 0x2, 0x2, 
       0x2a, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x2c, 0x90, 0x3, 0x2, 0x2, 0x2, 0x2e, 
       0x94, 0x3, 0x2, 0x2, 0x2, 0x30, 0x96, 0x3, 0x2, 0x2, 0x2, 0x32, 0x9c, 
       0x3, 0x2, 0x2, 0x2, 0x34, 0xa0, 0x3, 0x2, 0x2, 0x2, 0x36, 0xa7, 0x3, 
       0x2, 0x2, 0x2, 0x38, 0xa9, 0x3, 0x2, 0x2, 0x2, 0x3a, 0xab, 0x3, 0x2, 
       0x2, 0x2, 0x3c, 0xad, 0x3, 0x2, 0x2, 0x2, 0x3e, 0xb0, 0x3, 0x2, 0x2, 
       0x2, 0x40, 0xbe, 0x3, 0x2, 0x2, 0x2, 0x42, 0xc9, 0x3, 0x2, 0x2, 0x2, 
       0x44, 0xcd, 0x3, 0x2, 0x2, 0x2, 0x46, 0xd1, 0x3, 0x2, 0x2, 0x2, 0x48, 
       0xd5, 0x3, 0x2, 0x2, 0x2, 0x4a, 0xd9, 0x3, 0x2, 0x2, 0x2, 0x4c, 0xe1, 
       0x3, 0x2, 0x2, 0x2, 0x4e, 0xf2, 0x3, 0x2, 0x2, 0x2, 0x50, 0x104, 
       0x3, 0x2, 0x2, 0x2, 0x52, 0x107, 0x3, 0x2, 0x2, 0x2, 0x54, 0x10e, 
       0x3, 0x2, 0x2, 0x2, 0x56, 0x110, 0x3, 0x2, 0x2, 0x2, 0x58, 0x113, 
       0x3, 0x2, 0x2, 0x2, 0x5a, 0x11a, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x138, 
       0x3, 0x2, 0x2, 0x2, 0x5e, 0x13a, 0x3, 0x2, 0x2, 0x2, 0x60, 0x149, 
       0x3, 0x2, 0x2, 0x2, 0x62, 0x14f, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 
       0x9, 0x2, 0x2, 0x2, 0x65, 0x3, 0x3, 0x2, 0x2, 0x2, 0x66, 0x67, 0x7, 
       0x17, 0x2, 0x2, 0x67, 0x5, 0x3, 0x2, 0x2, 0x2, 0x68, 0x69, 0x9, 0x3, 
       0x2, 0x2, 0x69, 0x7, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x6b, 0x9, 0x4, 0x2, 
       0x2, 0x6b, 0x9, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x6d, 0x9, 0x5, 0x2, 0x2, 
       0x6d, 0xb, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6f, 0x9, 0x6, 0x2, 0x2, 0x6f, 
       0xd, 0x3, 0x2, 0x2, 0x2, 0x70, 0x71, 0x7, 0x3, 0x2, 0x2, 0x71, 0xf, 
       0x3, 0x2, 0x2, 0x2, 0x72, 0x73, 0x7, 0x4, 0x2, 0x2, 0x73, 0x11, 0x3, 
       0x2, 0x2, 0x2, 0x74, 0x75, 0x7, 0x5, 0x2, 0x2, 0x75, 0x13, 0x3, 0x2, 
       0x2, 0x2, 0x76, 0x77, 0x7, 0x6, 0x2, 0x2, 0x77, 0x15, 0x3, 0x2, 0x2, 
       0x2, 0x78, 0x79, 0x7, 0x7, 0x2, 0x2, 0x79, 0x17, 0x3, 0x2, 0x2, 0x2, 
       0x7a, 0x7b, 0x7, 0x8, 0x2, 0x2, 0x7b, 0x19, 0x3, 0x2, 0x2, 0x2, 0x7c, 
       0x7d, 0x7, 0x9, 0x2, 0x2, 0x7d, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x7f, 
       0x7, 0xa, 0x2, 0x2, 0x7f, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x80, 0x81, 0x7, 
       0xb, 0x2, 0x2, 0x81, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x82, 0x83, 0x7, 0xc, 
       0x2, 0x2, 0x83, 0x21, 0x3, 0x2, 0x2, 0x2, 0x84, 0x85, 0x7, 0xd, 0x2, 
       0x2, 0x85, 0x23, 0x3, 0x2, 0x2, 0x2, 0x86, 0x87, 0x7, 0xe, 0x2, 0x2, 
       0x87, 0x25, 0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x7, 0xf, 0x2, 0x2, 0x89, 
       0x27, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8b, 0x7, 0x10, 0x2, 0x2, 0x8b, 
       0x29, 0x3, 0x2, 0x2, 0x2, 0x8c, 0x8d, 0x7, 0x11, 0x2, 0x2, 0x8d, 
       0x2b, 0x3, 0x2, 0x2, 0x2, 0x8e, 0x91, 0x5, 0x16, 0xc, 0x2, 0x8f, 
       0x91, 0x5, 0x18, 0xd, 0x2, 0x90, 0x8e, 0x3, 0x2, 0x2, 0x2, 0x90, 
       0x8f, 0x3, 0x2, 0x2, 0x2, 0x91, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x92, 0x95, 
       0x5, 0x20, 0x11, 0x2, 0x93, 0x95, 0x5, 0x22, 0x12, 0x2, 0x94, 0x92, 
       0x3, 0x2, 0x2, 0x2, 0x94, 0x93, 0x3, 0x2, 0x2, 0x2, 0x95, 0x2f, 0x3, 
       0x2, 0x2, 0x2, 0x96, 0x97, 0x7, 0x2a, 0x2, 0x2, 0x97, 0x98, 0x7, 
       0x32, 0x2, 0x2, 0x98, 0x99, 0x7, 0x1a, 0x2, 0x2, 0x99, 0x9a, 0x7, 
       0x32, 0x2, 0x2, 0x9a, 0x9b, 0x7, 0x2b, 0x2, 0x2, 0x9b, 0x31, 0x3, 
       0x2, 0x2, 0x2, 0x9c, 0x9e, 0x5, 0xe, 0x8, 0x2, 0x9d, 0x9f, 0x5, 0x30, 
       0x19, 0x2, 0x9e, 0x9d, 0x3, 0x2, 0x2, 0x2, 0x9e, 0x9f, 0x3, 0x2, 
       0x2, 0x2, 0x9f, 0x33, 0x3, 0x2, 0x2, 0x2, 0xa0, 0xa2, 0x5, 0x12, 
       0xa, 0x2, 0xa1, 0xa3, 0x5, 0x30, 0x19, 0x2, 0xa2, 0xa1, 0x3, 0x2, 
       0x2, 0x2, 0xa2, 0xa3, 0x3, 0x2, 0x2, 0x2, 0xa3, 0x35, 0x3, 0x2, 0x2, 
       0x2, 0xa4, 0xa8, 0x5, 0x32, 0x1a, 0x2, 0xa5, 0xa8, 0x5, 0x34, 0x1b, 
       0x2, 0xa6, 0xa8, 0x5, 0x10, 0x9, 0x2, 0xa7, 0xa4, 0x3, 0x2, 0x2, 
       0x2, 0xa7, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa6, 0x3, 0x2, 0x2, 0x2, 
       0xa8, 0x37, 0x3, 0x2, 0x2, 0x2, 0xa9, 0xaa, 0x5, 0x40, 0x21, 0x2, 
       0xaa, 0x39, 0x3, 0x2, 0x2, 0x2, 0xab, 0xac, 0x5, 0x40, 0x21, 0x2, 
       0xac, 0x3b, 0x3, 0x2, 0x2, 0x2, 0xad, 0xae, 0x7, 0x30, 0x2, 0x2, 
       0xae, 0xaf, 0x7, 0x12, 0x2, 0x2, 0xaf, 0x3d, 0x3, 0x2, 0x2, 0x2, 
       0xb0, 0xb1, 0x5, 0x38, 0x1d, 0x2, 0xb1, 0xb2, 0x5, 0xa, 0x6, 0x2, 
       0xb2, 0xb3, 0x5, 0x3a, 0x1e, 0x2, 0xb3, 0x3f, 0x3, 0x2, 0x2, 0x2, 
       0xb4, 0xb5, 0x8, 0x21, 0x1, 0x2, 0xb5, 0xbf, 0x5, 0x3c, 0x1f, 0x2, 
       0xb6, 0xbf, 0x7, 0x30, 0x2, 0x2, 0xb7, 0xbf, 0x5, 0x52, 0x2a, 0x2, 
       0xb8, 0xbf, 0x5, 0x50, 0x29, 0x2, 0xb9, 0xbf, 0x7, 0x32, 0x2, 0x2, 
       0xba, 0xbb, 0x7, 0x2a, 0x2, 0x2, 0xbb, 0xbc, 0x5, 0x40, 0x21, 0x2, 
       0xbc, 0xbd, 0x7, 0x2b, 0x2, 0x2, 0xbd, 0xbf, 0x3, 0x2, 0x2, 0x2, 
       0xbe, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xbe, 
       0xb7, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xb8, 0x3, 0x2, 0x2, 0x2, 0xbe, 0xb9, 
       0x3, 0x2, 0x2, 0x2, 0xbe, 0xba, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xc6, 0x3, 
       0x2, 0x2, 0x2, 0xc0, 0xc1, 0xc, 0x9, 0x2, 0x2, 0xc1, 0xc2, 0x5, 0xc, 
       0x7, 0x2, 0xc2, 0xc3, 0x5, 0x40, 0x21, 0xa, 0xc3, 0xc5, 0x3, 0x2, 
       0x2, 0x2, 0xc4, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xc5, 0xc8, 0x3, 0x2, 0x2, 
       0x2, 0xc6, 0xc4, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc7, 0x3, 0x2, 0x2, 0x2, 
       0xc7, 0x41, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xc9, 
       0xca, 0x5, 0x40, 0x21, 0x2, 0xca, 0xcb, 0x7, 0x2e, 0x2, 0x2, 0xcb, 
       0xcc, 0x5, 0x40, 0x21, 0x2, 0xcc, 0x43, 0x3, 0x2, 0x2, 0x2, 0xcd, 
       0xce, 0x7, 0x2d, 0x2, 0x2, 0xce, 0xcf, 0x5, 0x42, 0x22, 0x2, 0xcf, 
       0xd0, 0x7, 0x2d, 0x2, 0x2, 0xd0, 0x45, 0x3, 0x2, 0x2, 0x2, 0xd1, 
       0xd2, 0x7, 0x2c, 0x2, 0x2, 0xd2, 0xd3, 0x5, 0x42, 0x22, 0x2, 0xd3, 
       0xd4, 0x7, 0x2c, 0x2, 0x2, 0xd4, 0x47, 0x3, 0x2, 0x2, 0x2, 0xd5, 
       0xd6, 0x7, 0x2d, 0x2, 0x2, 0xd6, 0xd7, 0x5, 0x42, 0x22, 0x2, 0xd7, 
       0xd8, 0x7, 0x2c, 0x2, 0x2, 0xd8, 0x49, 0x3, 0x2, 0x2, 0x2, 0xd9, 
       0xda, 0x7, 0x2c, 0x2, 0x2, 0xda, 0xdb, 0x5, 0x42, 0x22, 0x2, 0xdb, 
       0xdc, 0x7, 0x2d, 0x2, 0x2, 0xdc, 0x4b, 0x3, 0x2, 0x2, 0x2, 0xdd, 
       0xe2, 0x5, 0x4a, 0x26, 0x2, 0xde, 0xe2, 0x5, 0x48, 0x25, 0x2, 0xdf, 
       0xe2, 0x5, 0x44, 0x23, 0x2, 0xe0, 0xe2, 0x5, 0x46, 0x24, 0x2, 0xe1, 
       0xdd, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xde, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xdf, 
       0x3, 0x2, 0x2, 0x2, 0xe1, 0xe0, 0x3, 0x2, 0x2, 0x2, 0xe2, 0x4d, 0x3, 
       0x2, 0x2, 0x2, 0xe3, 0xe4, 0x8, 0x28, 0x1, 0x2, 0xe4, 0xe5, 0x7, 
       0x2a, 0x2, 0x2, 0xe5, 0xe6, 0x5, 0x4e, 0x28, 0x2, 0xe6, 0xe7, 0x7, 
       0x2b, 0x2, 0x2, 0xe7, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xe9, 0x5, 
       0x4, 0x3, 0x2, 0xe9, 0xea, 0x5, 0x4e, 0x28, 0x7, 0xea, 0xf3, 0x3, 
       0x2, 0x2, 0x2, 0xeb, 0xed, 0x5, 0x6, 0x4, 0x2, 0xec, 0xee, 0x5, 0x4c, 
       0x27, 0x2, 0xed, 0xec, 0x3, 0x2, 0x2, 0x2, 0xed, 0xee, 0x3, 0x2, 
       0x2, 0x2, 0xee, 0xef, 0x3, 0x2, 0x2, 0x2, 0xef, 0xf0, 0x5, 0x4e, 
       0x28, 0x5, 0xf0, 0xf3, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xf3, 0x5, 0x54, 
       0x2b, 0x2, 0xf2, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xe8, 0x3, 0x2, 
       0x2, 0x2, 0xf2, 0xeb, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xf1, 0x3, 0x2, 0x2, 
       0x2, 0xf3, 0x101, 0x3, 0x2, 0x2, 0x2, 0xf4, 0xf5, 0xc, 0x6, 0x2, 
       0x2, 0xf5, 0xf6, 0x5, 0x2, 0x2, 0x2, 0xf6, 0xf7, 0x5, 0x4e, 0x28, 
       0x7, 0xf7, 0x100, 0x3, 0x2, 0x2, 0x2, 0xf8, 0xf9, 0xc, 0x4, 0x2, 
       0x2, 0xf9, 0xfb, 0x5, 0x8, 0x5, 0x2, 0xfa, 0xfc, 0x5, 0x4c, 0x27, 
       0x2, 0xfb, 0xfa, 0x3, 0x2, 0x2, 0x2, 0xfb, 0xfc, 0x3, 0x2, 0x2, 0x2, 
       0xfc, 0xfd, 0x3, 0x2, 0x2, 0x2, 0xfd, 0xfe, 0x5, 0x4e, 0x28, 0x5, 
       0xfe, 0x100, 0x3, 0x2, 0x2, 0x2, 0xff, 0xf4, 0x3, 0x2, 0x2, 0x2, 
       0xff, 0xf8, 0x3, 0x2, 0x2, 0x2, 0x100, 0x103, 0x3, 0x2, 0x2, 0x2, 
       0x101, 0xff, 0x3, 0x2, 0x2, 0x2, 0x101, 0x102, 0x3, 0x2, 0x2, 0x2, 
       0x102, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x103, 0x101, 0x3, 0x2, 0x2, 0x2, 
       0x104, 0x105, 0x7, 0x27, 0x2, 0x2, 0x105, 0x106, 0x7, 0x32, 0x2, 
       0x2, 0x106, 0x51, 0x3, 0x2, 0x2, 0x2, 0x107, 0x108, 0x7, 0x27, 0x2, 
       0x2, 0x108, 0x109, 0x7, 0x30, 0x2, 0x2, 0x109, 0x53, 0x3, 0x2, 0x2, 
       0x2, 0x10a, 0x10f, 0x5, 0x2e, 0x18, 0x2, 0x10b, 0x10f, 0x5, 0x3e, 
       0x20, 0x2, 0x10c, 0x10f, 0x7, 0x30, 0x2, 0x2, 0x10d, 0x10f, 0x5, 
       0x3c, 0x1f, 0x2, 0x10e, 0x10a, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10b, 
       0x3, 0x2, 0x2, 0x2, 0x10e, 0x10c, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10d, 
       0x3, 0x2, 0x2, 0x2, 0x10f, 0x55, 0x3, 0x2, 0x2, 0x2, 0x110, 0x111, 
       0x5, 0x4e, 0x28, 0x2, 0x111, 0x112, 0x7, 0x33, 0x2, 0x2, 0x112, 0x57, 
       0x3, 0x2, 0x2, 0x2, 0x113, 0x114, 0x5, 0x26, 0x14, 0x2, 0x114, 0x116, 
       0x7, 0x1a, 0x2, 0x2, 0x115, 0x117, 0x5, 0x56, 0x2c, 0x2, 0x116, 0x115, 
       0x3, 0x2, 0x2, 0x2, 0x117, 0x118, 0x3, 0x2, 0x2, 0x2, 0x118, 0x116, 
       0x3, 0x2, 0x2, 0x2, 0x118, 0x119, 0x3, 0x2, 0x2, 0x2, 0x119, 0x59, 
       0x3, 0x2, 0x2, 0x2, 0x11a, 0x11b, 0x5, 0x28, 0x15, 0x2, 0x11b, 0x11d, 
       0x7, 0x1a, 0x2, 0x2, 0x11c, 0x11e, 0x5, 0x56, 0x2c, 0x2, 0x11d, 0x11c, 
       0x3, 0x2, 0x2, 0x2, 0x11e, 0x11f, 0x3, 0x2, 0x2, 0x2, 0x11f, 0x11d, 
       0x3, 0x2, 0x2, 0x2, 0x11f, 0x120, 0x3, 0x2, 0x2, 0x2, 0x120, 0x5b, 
       0x3, 0x2, 0x2, 0x2, 0x121, 0x123, 0x5, 0x2c, 0x17, 0x2, 0x122, 0x121, 
       0x3, 0x2, 0x2, 0x2, 0x122, 0x123, 0x3, 0x2, 0x2, 0x2, 0x123, 0x124, 
       0x3, 0x2, 0x2, 0x2, 0x124, 0x125, 0x5, 0x36, 0x1c, 0x2, 0x125, 0x126, 
       0x5, 0x14, 0xb, 0x2, 0x126, 0x127, 0x7, 0x30, 0x2, 0x2, 0x127, 0x128, 
       0x7, 0x33, 0x2, 0x2, 0x128, 0x139, 0x3, 0x2, 0x2, 0x2, 0x129, 0x12a, 
       0x5, 0x36, 0x1c, 0x2, 0x12a, 0x12b, 0x5, 0x1a, 0xe, 0x2, 0x12b, 0x12c, 
       0x7, 0x30, 0x2, 0x2, 0x12c, 0x12d, 0x7, 0x32, 0x2, 0x2, 0x12d, 0x12e, 
       0x7, 0x33, 0x2, 0x2, 0x12e, 0x139, 0x3, 0x2, 0x2, 0x2, 0x12f, 0x130, 
       0x5, 0x1c, 0xf, 0x2, 0x130, 0x134, 0x7, 0x30, 0x2, 0x2, 0x131, 0x132, 
       0x5, 0x1e, 0x10, 0x2, 0x132, 0x133, 0x5, 0x3e, 0x20, 0x2, 0x133, 
       0x135, 0x3, 0x2, 0x2, 0x2, 0x134, 0x131, 0x3, 0x2, 0x2, 0x2, 0x134, 
       0x135, 0x3, 0x2, 0x2, 0x2, 0x135, 0x136, 0x3, 0x2, 0x2, 0x2, 0x136, 
       0x137, 0x7, 0x33, 0x2, 0x2, 0x137, 0x139, 0x3, 0x2, 0x2, 0x2, 0x138, 
       0x122, 0x3, 0x2, 0x2, 0x2, 0x138, 0x129, 0x3, 0x2, 0x2, 0x2, 0x138, 
       0x12f, 0x3, 0x2, 0x2, 0x2, 0x139, 0x5d, 0x3, 0x2, 0x2, 0x2, 0x13a, 
       0x13b, 0x5, 0x24, 0x13, 0x2, 0x13b, 0x13c, 0x7, 0x30, 0x2, 0x2, 0x13c, 
       0x140, 0x7, 0x1a, 0x2, 0x2, 0x13d, 0x13f, 0x5, 0x5c, 0x2f, 0x2, 0x13e, 
       0x13d, 0x3, 0x2, 0x2, 0x2, 0x13f, 0x142, 0x3, 0x2, 0x2, 0x2, 0x140, 
       0x13e, 0x3, 0x2, 0x2, 0x2, 0x140, 0x141, 0x3, 0x2, 0x2, 0x2, 0x141, 
       0x144, 0x3, 0x2, 0x2, 0x2, 0x142, 0x140, 0x3, 0x2, 0x2, 0x2, 0x143, 
       0x145, 0x5, 0x58, 0x2d, 0x2, 0x144, 0x143, 0x3, 0x2, 0x2, 0x2, 0x144, 
       0x145, 0x3, 0x2, 0x2, 0x2, 0x145, 0x147, 0x3, 0x2, 0x2, 0x2, 0x146, 
       0x148, 0x5, 0x5a, 0x2e, 0x2, 0x147, 0x146, 0x3, 0x2, 0x2, 0x2, 0x147, 
       0x148, 0x3, 0x2, 0x2, 0x2, 0x148, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x149, 
       0x14a, 0x5, 0x2a, 0x16, 0x2, 0x14a, 0x14b, 0x7, 0x1a, 0x2, 0x2, 0x14b, 
       0x14c, 0x7, 0x30, 0x2, 0x2, 0x14c, 0x14d, 0x7, 0x33, 0x2, 0x2, 0x14d, 
       0x61, 0x3, 0x2, 0x2, 0x2, 0x14e, 0x150, 0x5, 0x60, 0x31, 0x2, 0x14f, 
       0x14e, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x150, 0x3, 0x2, 0x2, 0x2, 0x150, 
       0x154, 0x3, 0x2, 0x2, 0x2, 0x151, 0x153, 0x5, 0x5c, 0x2f, 0x2, 0x152, 
       0x151, 0x3, 0x2, 0x2, 0x2, 0x153, 0x156, 0x3, 0x2, 0x2, 0x2, 0x154, 
       0x152, 0x3, 0x2, 0x2, 0x2, 0x154, 0x155, 0x3, 0x2, 0x2, 0x2, 0x155, 
       0x158, 0x3, 0x2, 0x2, 0x2, 0x156, 0x154, 0x3, 0x2, 0x2, 0x2, 0x157, 
       0x159, 0x5, 0x5e, 0x30, 0x2, 0x158, 0x157, 0x3, 0x2, 0x2, 0x2, 0x159, 
       0x15a, 0x3, 0x2, 0x2, 0x2, 0x15a, 0x158, 0x3, 0x2, 0x2, 0x2, 0x15a, 
       0x15b, 0x3, 0x2, 0x2, 0x2, 0x15b, 0x63, 0x3, 0x2, 0x2, 0x2, 0x1b, 
       0x90, 0x94, 0x9e, 0xa2, 0xa7, 0xbe, 0xc6, 0xe1, 0xed, 0xf2, 0xfb, 
       0xff, 0x101, 0x10e, 0x118, 0x11f, 0x122, 0x134, 0x138, 0x140, 0x144, 
       0x147, 0x14f, 0x154, 0x15a, 
  };

  _serializedATN.insert(_serializedATN.end(), serializedATNSegment0,
    serializedATNSegment0 + sizeof(serializedATNSegment0) / sizeof(serializedATNSegment0[0]));


  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

LogicsContractsParser::Initializer LogicsContractsParser::_init;
