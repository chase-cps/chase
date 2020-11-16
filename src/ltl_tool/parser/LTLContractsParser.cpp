
// Generated from //wsl$/Ubuntu-20.04/home/lora/software/chase/repo/src/ltl_tool/grammar\LTLContracts.g4 by ANTLR 4.8


#include "LTLContractsVisitor.h"

#include "LTLContractsParser.h"


using namespace antlrcpp;
using namespace antlr4;

LTLContractsParser::LTLContractsParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LTLContractsParser::~LTLContractsParser() {
  delete _interpreter;
}

std::string LTLContractsParser::getGrammarFileName() const {
  return "LTLContracts.g4";
}

const std::vector<std::string>& LTLContractsParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& LTLContractsParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- Bin_logic_opContext ------------------------------------------------------------------

LTLContractsParser::Bin_logic_opContext::Bin_logic_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLContractsParser::Bin_logic_opContext::AND() {
  return getToken(LTLContractsParser::AND, 0);
}

tree::TerminalNode* LTLContractsParser::Bin_logic_opContext::OR() {
  return getToken(LTLContractsParser::OR, 0);
}

tree::TerminalNode* LTLContractsParser::Bin_logic_opContext::IFF() {
  return getToken(LTLContractsParser::IFF, 0);
}

tree::TerminalNode* LTLContractsParser::Bin_logic_opContext::IMPLIES() {
  return getToken(LTLContractsParser::IMPLIES, 0);
}


size_t LTLContractsParser::Bin_logic_opContext::getRuleIndex() const {
  return LTLContractsParser::RuleBin_logic_op;
}


antlrcpp::Any LTLContractsParser::Bin_logic_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitBin_logic_op(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Bin_logic_opContext* LTLContractsParser::bin_logic_op() {
  Bin_logic_opContext *_localctx = _tracker.createInstance<Bin_logic_opContext>(_ctx, getState());
  enterRule(_localctx, 0, LTLContractsParser::RuleBin_logic_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(74);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::AND)
      | (1ULL << LTLContractsParser::OR)
      | (1ULL << LTLContractsParser::IMPLIES)
      | (1ULL << LTLContractsParser::IFF))) != 0))) {
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

LTLContractsParser::Unary_logic_opContext::Unary_logic_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLContractsParser::Unary_logic_opContext::NOT() {
  return getToken(LTLContractsParser::NOT, 0);
}


size_t LTLContractsParser::Unary_logic_opContext::getRuleIndex() const {
  return LTLContractsParser::RuleUnary_logic_op;
}


antlrcpp::Any LTLContractsParser::Unary_logic_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitUnary_logic_op(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Unary_logic_opContext* LTLContractsParser::unary_logic_op() {
  Unary_logic_opContext *_localctx = _tracker.createInstance<Unary_logic_opContext>(_ctx, getState());
  enterRule(_localctx, 2, LTLContractsParser::RuleUnary_logic_op);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(76);
    match(LTLContractsParser::NOT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Unary_temp_opContext ------------------------------------------------------------------

LTLContractsParser::Unary_temp_opContext::Unary_temp_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLContractsParser::Unary_temp_opContext::ALWAYS() {
  return getToken(LTLContractsParser::ALWAYS, 0);
}

tree::TerminalNode* LTLContractsParser::Unary_temp_opContext::EVENTUALLY() {
  return getToken(LTLContractsParser::EVENTUALLY, 0);
}

tree::TerminalNode* LTLContractsParser::Unary_temp_opContext::NEXT() {
  return getToken(LTLContractsParser::NEXT, 0);
}


size_t LTLContractsParser::Unary_temp_opContext::getRuleIndex() const {
  return LTLContractsParser::RuleUnary_temp_op;
}


antlrcpp::Any LTLContractsParser::Unary_temp_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitUnary_temp_op(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Unary_temp_opContext* LTLContractsParser::unary_temp_op() {
  Unary_temp_opContext *_localctx = _tracker.createInstance<Unary_temp_opContext>(_ctx, getState());
  enterRule(_localctx, 4, LTLContractsParser::RuleUnary_temp_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(78);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::ALWAYS)
      | (1ULL << LTLContractsParser::EVENTUALLY)
      | (1ULL << LTLContractsParser::NEXT))) != 0))) {
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

LTLContractsParser::Bin_temp_opContext::Bin_temp_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLContractsParser::Bin_temp_opContext::UNTIL() {
  return getToken(LTLContractsParser::UNTIL, 0);
}


size_t LTLContractsParser::Bin_temp_opContext::getRuleIndex() const {
  return LTLContractsParser::RuleBin_temp_op;
}


antlrcpp::Any LTLContractsParser::Bin_temp_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitBin_temp_op(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Bin_temp_opContext* LTLContractsParser::bin_temp_op() {
  Bin_temp_opContext *_localctx = _tracker.createInstance<Bin_temp_opContext>(_ctx, getState());
  enterRule(_localctx, 6, LTLContractsParser::RuleBin_temp_op);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(80);
    match(LTLContractsParser::UNTIL);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Relation_opContext ------------------------------------------------------------------

LTLContractsParser::Relation_opContext::Relation_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLContractsParser::Relation_opContext::EQ() {
  return getToken(LTLContractsParser::EQ, 0);
}

tree::TerminalNode* LTLContractsParser::Relation_opContext::NEQ() {
  return getToken(LTLContractsParser::NEQ, 0);
}

tree::TerminalNode* LTLContractsParser::Relation_opContext::LT() {
  return getToken(LTLContractsParser::LT, 0);
}

tree::TerminalNode* LTLContractsParser::Relation_opContext::LE() {
  return getToken(LTLContractsParser::LE, 0);
}

tree::TerminalNode* LTLContractsParser::Relation_opContext::GT() {
  return getToken(LTLContractsParser::GT, 0);
}

tree::TerminalNode* LTLContractsParser::Relation_opContext::GE() {
  return getToken(LTLContractsParser::GE, 0);
}


size_t LTLContractsParser::Relation_opContext::getRuleIndex() const {
  return LTLContractsParser::RuleRelation_op;
}


antlrcpp::Any LTLContractsParser::Relation_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitRelation_op(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Relation_opContext* LTLContractsParser::relation_op() {
  Relation_opContext *_localctx = _tracker.createInstance<Relation_opContext>(_ctx, getState());
  enterRule(_localctx, 8, LTLContractsParser::RuleRelation_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(82);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::EQ)
      | (1ULL << LTLContractsParser::NEQ)
      | (1ULL << LTLContractsParser::LT)
      | (1ULL << LTLContractsParser::LE)
      | (1ULL << LTLContractsParser::GT)
      | (1ULL << LTLContractsParser::GE))) != 0))) {
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

LTLContractsParser::Bin_math_opContext::Bin_math_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLContractsParser::Bin_math_opContext::TIMES() {
  return getToken(LTLContractsParser::TIMES, 0);
}

tree::TerminalNode* LTLContractsParser::Bin_math_opContext::DIVIDE() {
  return getToken(LTLContractsParser::DIVIDE, 0);
}

tree::TerminalNode* LTLContractsParser::Bin_math_opContext::PLUS() {
  return getToken(LTLContractsParser::PLUS, 0);
}

tree::TerminalNode* LTLContractsParser::Bin_math_opContext::MINUS() {
  return getToken(LTLContractsParser::MINUS, 0);
}


size_t LTLContractsParser::Bin_math_opContext::getRuleIndex() const {
  return LTLContractsParser::RuleBin_math_op;
}


antlrcpp::Any LTLContractsParser::Bin_math_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitBin_math_op(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Bin_math_opContext* LTLContractsParser::bin_math_op() {
  Bin_math_opContext *_localctx = _tracker.createInstance<Bin_math_opContext>(_ctx, getState());
  enterRule(_localctx, 10, LTLContractsParser::RuleBin_math_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(84);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::PLUS)
      | (1ULL << LTLContractsParser::MINUS)
      | (1ULL << LTLContractsParser::TIMES)
      | (1ULL << LTLContractsParser::DIVIDE))) != 0))) {
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

LTLContractsParser::IntegerKWContext::IntegerKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::IntegerKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleIntegerKW;
}


antlrcpp::Any LTLContractsParser::IntegerKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitIntegerKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::IntegerKWContext* LTLContractsParser::integerKW() {
  IntegerKWContext *_localctx = _tracker.createInstance<IntegerKWContext>(_ctx, getState());
  enterRule(_localctx, 12, LTLContractsParser::RuleIntegerKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(86);
    match(LTLContractsParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanKWContext ------------------------------------------------------------------

LTLContractsParser::BooleanKWContext::BooleanKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::BooleanKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleBooleanKW;
}


antlrcpp::Any LTLContractsParser::BooleanKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitBooleanKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::BooleanKWContext* LTLContractsParser::booleanKW() {
  BooleanKWContext *_localctx = _tracker.createInstance<BooleanKWContext>(_ctx, getState());
  enterRule(_localctx, 14, LTLContractsParser::RuleBooleanKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(88);
    match(LTLContractsParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableKWContext ------------------------------------------------------------------

LTLContractsParser::VariableKWContext::VariableKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::VariableKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleVariableKW;
}


antlrcpp::Any LTLContractsParser::VariableKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitVariableKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::VariableKWContext* LTLContractsParser::variableKW() {
  VariableKWContext *_localctx = _tracker.createInstance<VariableKWContext>(_ctx, getState());
  enterRule(_localctx, 16, LTLContractsParser::RuleVariableKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(90);
    match(LTLContractsParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InputKWContext ------------------------------------------------------------------

LTLContractsParser::InputKWContext::InputKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::InputKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleInputKW;
}


antlrcpp::Any LTLContractsParser::InputKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitInputKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::InputKWContext* LTLContractsParser::inputKW() {
  InputKWContext *_localctx = _tracker.createInstance<InputKWContext>(_ctx, getState());
  enterRule(_localctx, 18, LTLContractsParser::RuleInputKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(92);
    match(LTLContractsParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OutputKWContext ------------------------------------------------------------------

LTLContractsParser::OutputKWContext::OutputKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::OutputKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleOutputKW;
}


antlrcpp::Any LTLContractsParser::OutputKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitOutputKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::OutputKWContext* LTLContractsParser::outputKW() {
  OutputKWContext *_localctx = _tracker.createInstance<OutputKWContext>(_ctx, getState());
  enterRule(_localctx, 20, LTLContractsParser::RuleOutputKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(94);
    match(LTLContractsParser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstantKWContext ------------------------------------------------------------------

LTLContractsParser::ConstantKWContext::ConstantKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::ConstantKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleConstantKW;
}


antlrcpp::Any LTLContractsParser::ConstantKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitConstantKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::ConstantKWContext* LTLContractsParser::constantKW() {
  ConstantKWContext *_localctx = _tracker.createInstance<ConstantKWContext>(_ctx, getState());
  enterRule(_localctx, 22, LTLContractsParser::RuleConstantKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
    match(LTLContractsParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropositionKwContext ------------------------------------------------------------------

LTLContractsParser::PropositionKwContext::PropositionKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::PropositionKwContext::getRuleIndex() const {
  return LTLContractsParser::RulePropositionKw;
}


antlrcpp::Any LTLContractsParser::PropositionKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitPropositionKw(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::PropositionKwContext* LTLContractsParser::propositionKw() {
  PropositionKwContext *_localctx = _tracker.createInstance<PropositionKwContext>(_ctx, getState());
  enterRule(_localctx, 24, LTLContractsParser::RulePropositionKw);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
    match(LTLContractsParser::T__6);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IsKwContext ------------------------------------------------------------------

LTLContractsParser::IsKwContext::IsKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::IsKwContext::getRuleIndex() const {
  return LTLContractsParser::RuleIsKw;
}


antlrcpp::Any LTLContractsParser::IsKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitIsKw(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::IsKwContext* LTLContractsParser::isKw() {
  IsKwContext *_localctx = _tracker.createInstance<IsKwContext>(_ctx, getState());
  enterRule(_localctx, 26, LTLContractsParser::RuleIsKw);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(100);
    match(LTLContractsParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TrueKWContext ------------------------------------------------------------------

LTLContractsParser::TrueKWContext::TrueKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::TrueKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleTrueKW;
}


antlrcpp::Any LTLContractsParser::TrueKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitTrueKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::TrueKWContext* LTLContractsParser::trueKW() {
  TrueKWContext *_localctx = _tracker.createInstance<TrueKWContext>(_ctx, getState());
  enterRule(_localctx, 28, LTLContractsParser::RuleTrueKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(102);
    match(LTLContractsParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FalseKWContext ------------------------------------------------------------------

LTLContractsParser::FalseKWContext::FalseKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::FalseKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleFalseKW;
}


antlrcpp::Any LTLContractsParser::FalseKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitFalseKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::FalseKWContext* LTLContractsParser::falseKW() {
  FalseKWContext *_localctx = _tracker.createInstance<FalseKWContext>(_ctx, getState());
  enterRule(_localctx, 30, LTLContractsParser::RuleFalseKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(104);
    match(LTLContractsParser::T__9);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ContractKWContext ------------------------------------------------------------------

LTLContractsParser::ContractKWContext::ContractKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::ContractKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleContractKW;
}


antlrcpp::Any LTLContractsParser::ContractKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitContractKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::ContractKWContext* LTLContractsParser::contractKW() {
  ContractKWContext *_localctx = _tracker.createInstance<ContractKWContext>(_ctx, getState());
  enterRule(_localctx, 32, LTLContractsParser::RuleContractKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(106);
    match(LTLContractsParser::T__10);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssumptionsKWContext ------------------------------------------------------------------

LTLContractsParser::AssumptionsKWContext::AssumptionsKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::AssumptionsKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleAssumptionsKW;
}


antlrcpp::Any LTLContractsParser::AssumptionsKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitAssumptionsKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::AssumptionsKWContext* LTLContractsParser::assumptionsKW() {
  AssumptionsKWContext *_localctx = _tracker.createInstance<AssumptionsKWContext>(_ctx, getState());
  enterRule(_localctx, 34, LTLContractsParser::RuleAssumptionsKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(108);
    match(LTLContractsParser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GuaranteesKWContext ------------------------------------------------------------------

LTLContractsParser::GuaranteesKWContext::GuaranteesKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::GuaranteesKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleGuaranteesKW;
}


antlrcpp::Any LTLContractsParser::GuaranteesKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitGuaranteesKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::GuaranteesKWContext* LTLContractsParser::guaranteesKW() {
  GuaranteesKWContext *_localctx = _tracker.createInstance<GuaranteesKWContext>(_ctx, getState());
  enterRule(_localctx, 36, LTLContractsParser::RuleGuaranteesKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(110);
    match(LTLContractsParser::T__12);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NameKwContext ------------------------------------------------------------------

LTLContractsParser::NameKwContext::NameKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::NameKwContext::getRuleIndex() const {
  return LTLContractsParser::RuleNameKw;
}


antlrcpp::Any LTLContractsParser::NameKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitNameKw(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::NameKwContext* LTLContractsParser::nameKw() {
  NameKwContext *_localctx = _tracker.createInstance<NameKwContext>(_ctx, getState());
  enterRule(_localctx, 38, LTLContractsParser::RuleNameKw);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(112);
    match(LTLContractsParser::T__13);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CausalityContext ------------------------------------------------------------------

LTLContractsParser::CausalityContext::CausalityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::InputKWContext* LTLContractsParser::CausalityContext::inputKW() {
  return getRuleContext<LTLContractsParser::InputKWContext>(0);
}

LTLContractsParser::OutputKWContext* LTLContractsParser::CausalityContext::outputKW() {
  return getRuleContext<LTLContractsParser::OutputKWContext>(0);
}


size_t LTLContractsParser::CausalityContext::getRuleIndex() const {
  return LTLContractsParser::RuleCausality;
}


antlrcpp::Any LTLContractsParser::CausalityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitCausality(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::CausalityContext* LTLContractsParser::causality() {
  CausalityContext *_localctx = _tracker.createInstance<CausalityContext>(_ctx, getState());
  enterRule(_localctx, 40, LTLContractsParser::RuleCausality);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(116);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLContractsParser::T__3: {
        enterOuterAlt(_localctx, 1);
        setState(114);
        inputKW();
        break;
      }

      case LTLContractsParser::T__4: {
        enterOuterAlt(_localctx, 2);
        setState(115);
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

LTLContractsParser::Logic_constantContext::Logic_constantContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::TrueKWContext* LTLContractsParser::Logic_constantContext::trueKW() {
  return getRuleContext<LTLContractsParser::TrueKWContext>(0);
}

LTLContractsParser::FalseKWContext* LTLContractsParser::Logic_constantContext::falseKW() {
  return getRuleContext<LTLContractsParser::FalseKWContext>(0);
}


size_t LTLContractsParser::Logic_constantContext::getRuleIndex() const {
  return LTLContractsParser::RuleLogic_constant;
}


antlrcpp::Any LTLContractsParser::Logic_constantContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitLogic_constant(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Logic_constantContext* LTLContractsParser::logic_constant() {
  Logic_constantContext *_localctx = _tracker.createInstance<Logic_constantContext>(_ctx, getState());
  enterRule(_localctx, 42, LTLContractsParser::RuleLogic_constant);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(120);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLContractsParser::T__8: {
        enterOuterAlt(_localctx, 1);
        setState(118);
        trueKW();
        break;
      }

      case LTLContractsParser::T__9: {
        enterOuterAlt(_localctx, 2);
        setState(119);
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

//----------------- TypeKWContext ------------------------------------------------------------------

LTLContractsParser::TypeKWContext::TypeKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::IntegerKWContext* LTLContractsParser::TypeKWContext::integerKW() {
  return getRuleContext<LTLContractsParser::IntegerKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::TypeKWContext::LBRACKET() {
  return getToken(LTLContractsParser::LBRACKET, 0);
}

std::vector<tree::TerminalNode *> LTLContractsParser::TypeKWContext::NUMBER() {
  return getTokens(LTLContractsParser::NUMBER);
}

tree::TerminalNode* LTLContractsParser::TypeKWContext::NUMBER(size_t i) {
  return getToken(LTLContractsParser::NUMBER, i);
}

tree::TerminalNode* LTLContractsParser::TypeKWContext::COLON() {
  return getToken(LTLContractsParser::COLON, 0);
}

tree::TerminalNode* LTLContractsParser::TypeKWContext::RBRACKET() {
  return getToken(LTLContractsParser::RBRACKET, 0);
}

LTLContractsParser::BooleanKWContext* LTLContractsParser::TypeKWContext::booleanKW() {
  return getRuleContext<LTLContractsParser::BooleanKWContext>(0);
}


size_t LTLContractsParser::TypeKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleTypeKW;
}


antlrcpp::Any LTLContractsParser::TypeKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitTypeKW(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::TypeKWContext* LTLContractsParser::typeKW() {
  TypeKWContext *_localctx = _tracker.createInstance<TypeKWContext>(_ctx, getState());
  enterRule(_localctx, 44, LTLContractsParser::RuleTypeKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(130);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLContractsParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(122);
        integerKW();
        setState(123);
        match(LTLContractsParser::LBRACKET);
        setState(124);
        match(LTLContractsParser::NUMBER);
        setState(125);
        match(LTLContractsParser::COLON);
        setState(126);
        match(LTLContractsParser::NUMBER);
        setState(127);
        match(LTLContractsParser::RBRACKET);
        break;
      }

      case LTLContractsParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(129);
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

LTLContractsParser::LvalueContext::LvalueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::ValueContext* LTLContractsParser::LvalueContext::value() {
  return getRuleContext<LTLContractsParser::ValueContext>(0);
}


size_t LTLContractsParser::LvalueContext::getRuleIndex() const {
  return LTLContractsParser::RuleLvalue;
}


antlrcpp::Any LTLContractsParser::LvalueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitLvalue(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::LvalueContext* LTLContractsParser::lvalue() {
  LvalueContext *_localctx = _tracker.createInstance<LvalueContext>(_ctx, getState());
  enterRule(_localctx, 46, LTLContractsParser::RuleLvalue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(132);
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

LTLContractsParser::RvalueContext::RvalueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::ValueContext* LTLContractsParser::RvalueContext::value() {
  return getRuleContext<LTLContractsParser::ValueContext>(0);
}


size_t LTLContractsParser::RvalueContext::getRuleIndex() const {
  return LTLContractsParser::RuleRvalue;
}


antlrcpp::Any LTLContractsParser::RvalueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitRvalue(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::RvalueContext* LTLContractsParser::rvalue() {
  RvalueContext *_localctx = _tracker.createInstance<RvalueContext>(_ctx, getState());
  enterRule(_localctx, 48, LTLContractsParser::RuleRvalue);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(134);
    value(0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RelationContext ------------------------------------------------------------------

LTLContractsParser::RelationContext::RelationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::LvalueContext* LTLContractsParser::RelationContext::lvalue() {
  return getRuleContext<LTLContractsParser::LvalueContext>(0);
}

LTLContractsParser::Relation_opContext* LTLContractsParser::RelationContext::relation_op() {
  return getRuleContext<LTLContractsParser::Relation_opContext>(0);
}

LTLContractsParser::RvalueContext* LTLContractsParser::RelationContext::rvalue() {
  return getRuleContext<LTLContractsParser::RvalueContext>(0);
}


size_t LTLContractsParser::RelationContext::getRuleIndex() const {
  return LTLContractsParser::RuleRelation;
}


antlrcpp::Any LTLContractsParser::RelationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitRelation(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::RelationContext* LTLContractsParser::relation() {
  RelationContext *_localctx = _tracker.createInstance<RelationContext>(_ctx, getState());
  enterRule(_localctx, 50, LTLContractsParser::RuleRelation);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(136);
    lvalue();
    setState(137);
    relation_op();
    setState(138);
    rvalue();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FormulaContext ------------------------------------------------------------------

LTLContractsParser::FormulaContext::FormulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::Unary_logic_opContext* LTLContractsParser::FormulaContext::unary_logic_op() {
  return getRuleContext<LTLContractsParser::Unary_logic_opContext>(0);
}

std::vector<LTLContractsParser::FormulaContext *> LTLContractsParser::FormulaContext::formula() {
  return getRuleContexts<LTLContractsParser::FormulaContext>();
}

LTLContractsParser::FormulaContext* LTLContractsParser::FormulaContext::formula(size_t i) {
  return getRuleContext<LTLContractsParser::FormulaContext>(i);
}

LTLContractsParser::Unary_temp_opContext* LTLContractsParser::FormulaContext::unary_temp_op() {
  return getRuleContext<LTLContractsParser::Unary_temp_opContext>(0);
}

tree::TerminalNode* LTLContractsParser::FormulaContext::LBRACKET() {
  return getToken(LTLContractsParser::LBRACKET, 0);
}

tree::TerminalNode* LTLContractsParser::FormulaContext::RBRACKET() {
  return getToken(LTLContractsParser::RBRACKET, 0);
}

LTLContractsParser::AtomContext* LTLContractsParser::FormulaContext::atom() {
  return getRuleContext<LTLContractsParser::AtomContext>(0);
}

LTLContractsParser::Bin_logic_opContext* LTLContractsParser::FormulaContext::bin_logic_op() {
  return getRuleContext<LTLContractsParser::Bin_logic_opContext>(0);
}

LTLContractsParser::Bin_temp_opContext* LTLContractsParser::FormulaContext::bin_temp_op() {
  return getRuleContext<LTLContractsParser::Bin_temp_opContext>(0);
}


size_t LTLContractsParser::FormulaContext::getRuleIndex() const {
  return LTLContractsParser::RuleFormula;
}


antlrcpp::Any LTLContractsParser::FormulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitFormula(this);
  else
    return visitor->visitChildren(this);
}


LTLContractsParser::FormulaContext* LTLContractsParser::formula() {
   return formula(0);
}

LTLContractsParser::FormulaContext* LTLContractsParser::formula(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LTLContractsParser::FormulaContext *_localctx = _tracker.createInstance<FormulaContext>(_ctx, parentState);
  LTLContractsParser::FormulaContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 52;
  enterRecursionRule(_localctx, 52, LTLContractsParser::RuleFormula, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(152);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLContractsParser::NOT: {
        setState(141);
        unary_logic_op();
        setState(142);
        formula(6);
        break;
      }

      case LTLContractsParser::ALWAYS:
      case LTLContractsParser::EVENTUALLY:
      case LTLContractsParser::NEXT: {
        setState(144);
        unary_temp_op();
        setState(145);
        formula(4);
        break;
      }

      case LTLContractsParser::LBRACKET: {
        setState(147);
        match(LTLContractsParser::LBRACKET);
        setState(148);
        formula(0);
        setState(149);
        match(LTLContractsParser::RBRACKET);
        break;
      }

      case LTLContractsParser::T__8:
      case LTLContractsParser::T__9:
      case LTLContractsParser::ID: {
        setState(151);
        atom();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(164);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(162);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<FormulaContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleFormula);
          setState(154);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(155);
          bin_logic_op();
          setState(156);
          formula(6);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<FormulaContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleFormula);
          setState(158);

          if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
          setState(159);
          bin_temp_op();
          setState(160);
          formula(4);
          break;
        }

        } 
      }
      setState(166);
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

//----------------- Minus_numberContext ------------------------------------------------------------------

LTLContractsParser::Minus_numberContext::Minus_numberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLContractsParser::Minus_numberContext::MINUS() {
  return getToken(LTLContractsParser::MINUS, 0);
}

tree::TerminalNode* LTLContractsParser::Minus_numberContext::NUMBER() {
  return getToken(LTLContractsParser::NUMBER, 0);
}


size_t LTLContractsParser::Minus_numberContext::getRuleIndex() const {
  return LTLContractsParser::RuleMinus_number;
}


antlrcpp::Any LTLContractsParser::Minus_numberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitMinus_number(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Minus_numberContext* LTLContractsParser::minus_number() {
  Minus_numberContext *_localctx = _tracker.createInstance<Minus_numberContext>(_ctx, getState());
  enterRule(_localctx, 54, LTLContractsParser::RuleMinus_number);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(167);
    match(LTLContractsParser::MINUS);
    setState(168);
    match(LTLContractsParser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

LTLContractsParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> LTLContractsParser::ValueContext::ID() {
  return getTokens(LTLContractsParser::ID);
}

tree::TerminalNode* LTLContractsParser::ValueContext::ID(size_t i) {
  return getToken(LTLContractsParser::ID, i);
}

LTLContractsParser::Bin_math_opContext* LTLContractsParser::ValueContext::bin_math_op() {
  return getRuleContext<LTLContractsParser::Bin_math_opContext>(0);
}

std::vector<LTLContractsParser::ValueContext *> LTLContractsParser::ValueContext::value() {
  return getRuleContexts<LTLContractsParser::ValueContext>();
}

LTLContractsParser::ValueContext* LTLContractsParser::ValueContext::value(size_t i) {
  return getRuleContext<LTLContractsParser::ValueContext>(i);
}

LTLContractsParser::Minus_numberContext* LTLContractsParser::ValueContext::minus_number() {
  return getRuleContext<LTLContractsParser::Minus_numberContext>(0);
}

tree::TerminalNode* LTLContractsParser::ValueContext::NUMBER() {
  return getToken(LTLContractsParser::NUMBER, 0);
}

tree::TerminalNode* LTLContractsParser::ValueContext::LBRACKET() {
  return getToken(LTLContractsParser::LBRACKET, 0);
}

tree::TerminalNode* LTLContractsParser::ValueContext::RBRACKET() {
  return getToken(LTLContractsParser::RBRACKET, 0);
}


size_t LTLContractsParser::ValueContext::getRuleIndex() const {
  return LTLContractsParser::RuleValue;
}


antlrcpp::Any LTLContractsParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}


LTLContractsParser::ValueContext* LTLContractsParser::value() {
   return value(0);
}

LTLContractsParser::ValueContext* LTLContractsParser::value(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LTLContractsParser::ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, parentState);
  LTLContractsParser::ValueContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 56;
  enterRecursionRule(_localctx, 56, LTLContractsParser::RuleValue, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(186);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      setState(171);
      match(LTLContractsParser::ID);
      setState(172);
      bin_math_op();
      setState(173);
      value(7);
      break;
    }

    case 2: {
      setState(175);
      match(LTLContractsParser::ID);
      setState(176);
      bin_math_op();
      setState(177);
      match(LTLContractsParser::ID);
      break;
    }

    case 3: {
      setState(179);
      match(LTLContractsParser::ID);
      break;
    }

    case 4: {
      setState(180);
      minus_number();
      break;
    }

    case 5: {
      setState(181);
      match(LTLContractsParser::NUMBER);
      break;
    }

    case 6: {
      setState(182);
      match(LTLContractsParser::LBRACKET);
      setState(183);
      value(0);
      setState(184);
      match(LTLContractsParser::RBRACKET);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(198);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(196);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx)) {
        case 1: {
          _localctx = _tracker.createInstance<ValueContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleValue);
          setState(188);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
          setState(189);
          bin_math_op();
          setState(190);
          value(9);
          break;
        }

        case 2: {
          _localctx = _tracker.createInstance<ValueContext>(parentContext, parentState);
          pushNewRecursionContext(_localctx, startState, RuleValue);
          setState(192);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(193);
          bin_math_op();
          setState(194);
          match(LTLContractsParser::ID);
          break;
        }

        } 
      }
      setState(200);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

LTLContractsParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::Logic_constantContext* LTLContractsParser::AtomContext::logic_constant() {
  return getRuleContext<LTLContractsParser::Logic_constantContext>(0);
}

tree::TerminalNode* LTLContractsParser::AtomContext::ID() {
  return getToken(LTLContractsParser::ID, 0);
}


size_t LTLContractsParser::AtomContext::getRuleIndex() const {
  return LTLContractsParser::RuleAtom;
}


antlrcpp::Any LTLContractsParser::AtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitAtom(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::AtomContext* LTLContractsParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 58, LTLContractsParser::RuleAtom);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(203);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLContractsParser::T__8:
      case LTLContractsParser::T__9: {
        enterOuterAlt(_localctx, 1);
        setState(201);
        logic_constant();
        break;
      }

      case LTLContractsParser::ID: {
        enterOuterAlt(_localctx, 2);
        setState(202);
        match(LTLContractsParser::ID);
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

//----------------- Single_formulaContext ------------------------------------------------------------------

LTLContractsParser::Single_formulaContext::Single_formulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::FormulaContext* LTLContractsParser::Single_formulaContext::formula() {
  return getRuleContext<LTLContractsParser::FormulaContext>(0);
}

tree::TerminalNode* LTLContractsParser::Single_formulaContext::ENDST() {
  return getToken(LTLContractsParser::ENDST, 0);
}


size_t LTLContractsParser::Single_formulaContext::getRuleIndex() const {
  return LTLContractsParser::RuleSingle_formula;
}


antlrcpp::Any LTLContractsParser::Single_formulaContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitSingle_formula(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::Single_formulaContext* LTLContractsParser::single_formula() {
  Single_formulaContext *_localctx = _tracker.createInstance<Single_formulaContext>(_ctx, getState());
  enterRule(_localctx, 60, LTLContractsParser::RuleSingle_formula);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(205);
    formula(0);
    setState(206);
    match(LTLContractsParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssumptionsContext ------------------------------------------------------------------

LTLContractsParser::AssumptionsContext::AssumptionsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::AssumptionsKWContext* LTLContractsParser::AssumptionsContext::assumptionsKW() {
  return getRuleContext<LTLContractsParser::AssumptionsKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::AssumptionsContext::COLON() {
  return getToken(LTLContractsParser::COLON, 0);
}

std::vector<LTLContractsParser::Single_formulaContext *> LTLContractsParser::AssumptionsContext::single_formula() {
  return getRuleContexts<LTLContractsParser::Single_formulaContext>();
}

LTLContractsParser::Single_formulaContext* LTLContractsParser::AssumptionsContext::single_formula(size_t i) {
  return getRuleContext<LTLContractsParser::Single_formulaContext>(i);
}


size_t LTLContractsParser::AssumptionsContext::getRuleIndex() const {
  return LTLContractsParser::RuleAssumptions;
}


antlrcpp::Any LTLContractsParser::AssumptionsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitAssumptions(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::AssumptionsContext* LTLContractsParser::assumptions() {
  AssumptionsContext *_localctx = _tracker.createInstance<AssumptionsContext>(_ctx, getState());
  enterRule(_localctx, 62, LTLContractsParser::RuleAssumptions);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(208);
    assumptionsKW();
    setState(209);
    match(LTLContractsParser::COLON);
    setState(211); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(210);
      single_formula();
      setState(213); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::T__8)
      | (1ULL << LTLContractsParser::T__9)
      | (1ULL << LTLContractsParser::NOT)
      | (1ULL << LTLContractsParser::ALWAYS)
      | (1ULL << LTLContractsParser::EVENTUALLY)
      | (1ULL << LTLContractsParser::NEXT)
      | (1ULL << LTLContractsParser::LBRACKET)
      | (1ULL << LTLContractsParser::ID))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GuaranteesContext ------------------------------------------------------------------

LTLContractsParser::GuaranteesContext::GuaranteesContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::GuaranteesKWContext* LTLContractsParser::GuaranteesContext::guaranteesKW() {
  return getRuleContext<LTLContractsParser::GuaranteesKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::GuaranteesContext::COLON() {
  return getToken(LTLContractsParser::COLON, 0);
}

std::vector<LTLContractsParser::Single_formulaContext *> LTLContractsParser::GuaranteesContext::single_formula() {
  return getRuleContexts<LTLContractsParser::Single_formulaContext>();
}

LTLContractsParser::Single_formulaContext* LTLContractsParser::GuaranteesContext::single_formula(size_t i) {
  return getRuleContext<LTLContractsParser::Single_formulaContext>(i);
}


size_t LTLContractsParser::GuaranteesContext::getRuleIndex() const {
  return LTLContractsParser::RuleGuarantees;
}


antlrcpp::Any LTLContractsParser::GuaranteesContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitGuarantees(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::GuaranteesContext* LTLContractsParser::guarantees() {
  GuaranteesContext *_localctx = _tracker.createInstance<GuaranteesContext>(_ctx, getState());
  enterRule(_localctx, 64, LTLContractsParser::RuleGuarantees);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(215);
    guaranteesKW();
    setState(216);
    match(LTLContractsParser::COLON);
    setState(218); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(217);
      single_formula();
      setState(220); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::T__8)
      | (1ULL << LTLContractsParser::T__9)
      | (1ULL << LTLContractsParser::NOT)
      | (1ULL << LTLContractsParser::ALWAYS)
      | (1ULL << LTLContractsParser::EVENTUALLY)
      | (1ULL << LTLContractsParser::NEXT)
      | (1ULL << LTLContractsParser::LBRACKET)
      | (1ULL << LTLContractsParser::ID))) != 0));
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DeclarationContext ------------------------------------------------------------------

LTLContractsParser::DeclarationContext::DeclarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::TypeKWContext* LTLContractsParser::DeclarationContext::typeKW() {
  return getRuleContext<LTLContractsParser::TypeKWContext>(0);
}

LTLContractsParser::VariableKWContext* LTLContractsParser::DeclarationContext::variableKW() {
  return getRuleContext<LTLContractsParser::VariableKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::DeclarationContext::ID() {
  return getToken(LTLContractsParser::ID, 0);
}

tree::TerminalNode* LTLContractsParser::DeclarationContext::ENDST() {
  return getToken(LTLContractsParser::ENDST, 0);
}

LTLContractsParser::CausalityContext* LTLContractsParser::DeclarationContext::causality() {
  return getRuleContext<LTLContractsParser::CausalityContext>(0);
}

LTLContractsParser::ConstantKWContext* LTLContractsParser::DeclarationContext::constantKW() {
  return getRuleContext<LTLContractsParser::ConstantKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::DeclarationContext::NUMBER() {
  return getToken(LTLContractsParser::NUMBER, 0);
}

LTLContractsParser::PropositionKwContext* LTLContractsParser::DeclarationContext::propositionKw() {
  return getRuleContext<LTLContractsParser::PropositionKwContext>(0);
}

LTLContractsParser::IsKwContext* LTLContractsParser::DeclarationContext::isKw() {
  return getRuleContext<LTLContractsParser::IsKwContext>(0);
}

LTLContractsParser::RelationContext* LTLContractsParser::DeclarationContext::relation() {
  return getRuleContext<LTLContractsParser::RelationContext>(0);
}


size_t LTLContractsParser::DeclarationContext::getRuleIndex() const {
  return LTLContractsParser::RuleDeclaration;
}


antlrcpp::Any LTLContractsParser::DeclarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitDeclaration(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::DeclarationContext* LTLContractsParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 66, LTLContractsParser::RuleDeclaration);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(245);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(223);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == LTLContractsParser::T__3

      || _la == LTLContractsParser::T__4) {
        setState(222);
        causality();
      }
      setState(225);
      typeKW();
      setState(226);
      variableKW();
      setState(227);
      match(LTLContractsParser::ID);
      setState(228);
      match(LTLContractsParser::ENDST);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(230);
      typeKW();
      setState(231);
      constantKW();
      setState(232);
      match(LTLContractsParser::ID);
      setState(233);
      match(LTLContractsParser::NUMBER);
      setState(234);
      match(LTLContractsParser::ENDST);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(236);
      propositionKw();
      setState(237);
      match(LTLContractsParser::ID);
      setState(241);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == LTLContractsParser::T__7) {
        setState(238);
        isKw();
        setState(239);
        relation();
      }
      setState(243);
      match(LTLContractsParser::ENDST);
      break;
    }

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

LTLContractsParser::ContractContext::ContractContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::ContractKWContext* LTLContractsParser::ContractContext::contractKW() {
  return getRuleContext<LTLContractsParser::ContractKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::ContractContext::ID() {
  return getToken(LTLContractsParser::ID, 0);
}

tree::TerminalNode* LTLContractsParser::ContractContext::COLON() {
  return getToken(LTLContractsParser::COLON, 0);
}

std::vector<LTLContractsParser::DeclarationContext *> LTLContractsParser::ContractContext::declaration() {
  return getRuleContexts<LTLContractsParser::DeclarationContext>();
}

LTLContractsParser::DeclarationContext* LTLContractsParser::ContractContext::declaration(size_t i) {
  return getRuleContext<LTLContractsParser::DeclarationContext>(i);
}

LTLContractsParser::AssumptionsContext* LTLContractsParser::ContractContext::assumptions() {
  return getRuleContext<LTLContractsParser::AssumptionsContext>(0);
}

LTLContractsParser::GuaranteesContext* LTLContractsParser::ContractContext::guarantees() {
  return getRuleContext<LTLContractsParser::GuaranteesContext>(0);
}


size_t LTLContractsParser::ContractContext::getRuleIndex() const {
  return LTLContractsParser::RuleContract;
}


antlrcpp::Any LTLContractsParser::ContractContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitContract(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::ContractContext* LTLContractsParser::contract() {
  ContractContext *_localctx = _tracker.createInstance<ContractContext>(_ctx, getState());
  enterRule(_localctx, 68, LTLContractsParser::RuleContract);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(247);
    contractKW();
    setState(248);
    match(LTLContractsParser::ID);
    setState(249);
    match(LTLContractsParser::COLON);
    setState(253);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::T__0)
      | (1ULL << LTLContractsParser::T__1)
      | (1ULL << LTLContractsParser::T__3)
      | (1ULL << LTLContractsParser::T__4)
      | (1ULL << LTLContractsParser::T__6))) != 0)) {
      setState(250);
      declaration();
      setState(255);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(257);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LTLContractsParser::T__11) {
      setState(256);
      assumptions();
    }
    setState(260);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LTLContractsParser::T__12) {
      setState(259);
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

LTLContractsParser::NameContext::NameContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::NameKwContext* LTLContractsParser::NameContext::nameKw() {
  return getRuleContext<LTLContractsParser::NameKwContext>(0);
}

tree::TerminalNode* LTLContractsParser::NameContext::COLON() {
  return getToken(LTLContractsParser::COLON, 0);
}

tree::TerminalNode* LTLContractsParser::NameContext::ID() {
  return getToken(LTLContractsParser::ID, 0);
}

tree::TerminalNode* LTLContractsParser::NameContext::ENDST() {
  return getToken(LTLContractsParser::ENDST, 0);
}


size_t LTLContractsParser::NameContext::getRuleIndex() const {
  return LTLContractsParser::RuleName;
}


antlrcpp::Any LTLContractsParser::NameContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitName(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::NameContext* LTLContractsParser::name() {
  NameContext *_localctx = _tracker.createInstance<NameContext>(_ctx, getState());
  enterRule(_localctx, 70, LTLContractsParser::RuleName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(262);
    nameKw();
    setState(263);
    match(LTLContractsParser::COLON);
    setState(264);
    match(LTLContractsParser::ID);
    setState(265);
    match(LTLContractsParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SystemSpecContext ------------------------------------------------------------------

LTLContractsParser::SystemSpecContext::SystemSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::NameContext* LTLContractsParser::SystemSpecContext::name() {
  return getRuleContext<LTLContractsParser::NameContext>(0);
}

std::vector<LTLContractsParser::DeclarationContext *> LTLContractsParser::SystemSpecContext::declaration() {
  return getRuleContexts<LTLContractsParser::DeclarationContext>();
}

LTLContractsParser::DeclarationContext* LTLContractsParser::SystemSpecContext::declaration(size_t i) {
  return getRuleContext<LTLContractsParser::DeclarationContext>(i);
}

std::vector<LTLContractsParser::ContractContext *> LTLContractsParser::SystemSpecContext::contract() {
  return getRuleContexts<LTLContractsParser::ContractContext>();
}

LTLContractsParser::ContractContext* LTLContractsParser::SystemSpecContext::contract(size_t i) {
  return getRuleContext<LTLContractsParser::ContractContext>(i);
}


size_t LTLContractsParser::SystemSpecContext::getRuleIndex() const {
  return LTLContractsParser::RuleSystemSpec;
}


antlrcpp::Any LTLContractsParser::SystemSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LTLContractsVisitor*>(visitor))
    return parserVisitor->visitSystemSpec(this);
  else
    return visitor->visitChildren(this);
}

LTLContractsParser::SystemSpecContext* LTLContractsParser::systemSpec() {
  SystemSpecContext *_localctx = _tracker.createInstance<SystemSpecContext>(_ctx, getState());
  enterRule(_localctx, 72, LTLContractsParser::RuleSystemSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(268);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LTLContractsParser::T__13) {
      setState(267);
      name();
    }
    setState(273);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::T__0)
      | (1ULL << LTLContractsParser::T__1)
      | (1ULL << LTLContractsParser::T__3)
      | (1ULL << LTLContractsParser::T__4)
      | (1ULL << LTLContractsParser::T__6))) != 0)) {
      setState(270);
      declaration();
      setState(275);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(277); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(276);
      contract();
      setState(279); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LTLContractsParser::T__10);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool LTLContractsParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 26: return formulaSempred(dynamic_cast<FormulaContext *>(context), predicateIndex);
    case 28: return valueSempred(dynamic_cast<ValueContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool LTLContractsParser::formulaSempred(FormulaContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 5);
    case 1: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

bool LTLContractsParser::valueSempred(ValueContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 2: return precpred(_ctx, 8);
    case 3: return precpred(_ctx, 5);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> LTLContractsParser::_decisionToDFA;
atn::PredictionContextCache LTLContractsParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LTLContractsParser::_atn;
std::vector<uint16_t> LTLContractsParser::_serializedATN;

std::vector<std::string> LTLContractsParser::_ruleNames = {
  "bin_logic_op", "unary_logic_op", "unary_temp_op", "bin_temp_op", "relation_op", 
  "bin_math_op", "integerKW", "booleanKW", "variableKW", "inputKW", "outputKW", 
  "constantKW", "propositionKw", "isKw", "trueKW", "falseKW", "contractKW", 
  "assumptionsKW", "guaranteesKW", "nameKw", "causality", "logic_constant", 
  "typeKW", "lvalue", "rvalue", "relation", "formula", "minus_number", "value", 
  "atom", "single_formula", "assumptions", "guarantees", "declaration", 
  "contract", "name", "systemSpec"
};

std::vector<std::string> LTLContractsParser::_literalNames = {
  "", "'integer'", "'boolean'", "'variable'", "'input'", "'output'", "'constant'", 
  "'proposition'", "'is'", "'true'", "'false'", "'CONTRACT'", "'Assumptions'", 
  "'Guarantees'", "'NAME'", "", "", "", "", "", "'->'", "'<->'", "':'", 
  "'[]'", "'<>'", "'X'", "'U'", "'='", "'!='", "'<'", "'<='", "'>'", "'>='", 
  "'+'", "'-'", "'*'", "'/'", "'('", "')'", "", "", "';'"
};

std::vector<std::string> LTLContractsParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "WS", "LINE_COMMENT", 
  "AND", "OR", "NOT", "IMPLIES", "IFF", "COLON", "ALWAYS", "EVENTUALLY", 
  "NEXT", "UNTIL", "EQ", "NEQ", "LT", "LE", "GT", "GE", "PLUS", "MINUS", 
  "TIMES", "DIVIDE", "LBRACKET", "RBRACKET", "ID", "NUMBER", "ENDST"
};

dfa::Vocabulary LTLContractsParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LTLContractsParser::_tokenNames;

LTLContractsParser::Initializer::Initializer() {
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

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x2b, 0x11c, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
    0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 
    0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 
    0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 
    0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 
    0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 
    0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 
    0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 
    0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 
    0x1f, 0x9, 0x1f, 0x4, 0x20, 0x9, 0x20, 0x4, 0x21, 0x9, 0x21, 0x4, 0x22, 
    0x9, 0x22, 0x4, 0x23, 0x9, 0x23, 0x4, 0x24, 0x9, 0x24, 0x4, 0x25, 0x9, 
    0x25, 0x4, 0x26, 0x9, 0x26, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 
    0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 
    0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 
    0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 
    0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x15, 0x3, 0x15, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0x77, 0xa, 0x16, 
    0x3, 0x17, 0x3, 0x17, 0x5, 0x17, 0x7b, 0xa, 0x17, 0x3, 0x18, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 
    0x18, 0x85, 0xa, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 
    0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 
    0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x5, 0x1c, 0x9b, 0xa, 0x1c, 0x3, 0x1c, 0x3, 
    0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x7, 0x1c, 0xa5, 0xa, 0x1c, 0xc, 0x1c, 0xe, 0x1c, 0xa8, 0xb, 0x1c, 0x3, 
    0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 
    0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x5, 0x1e, 
    0xbd, 0xa, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x7, 0x1e, 0xc7, 0xa, 0x1e, 0xc, 0x1e, 
    0xe, 0x1e, 0xca, 0xb, 0x1e, 0x3, 0x1f, 0x3, 0x1f, 0x5, 0x1f, 0xce, 0xa, 
    0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 0x21, 
    0x6, 0x21, 0xd6, 0xa, 0x21, 0xd, 0x21, 0xe, 0x21, 0xd7, 0x3, 0x22, 0x3, 
    0x22, 0x3, 0x22, 0x6, 0x22, 0xdd, 0xa, 0x22, 0xd, 0x22, 0xe, 0x22, 0xde, 
    0x3, 0x23, 0x5, 0x23, 0xe2, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 
    0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 
    0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 0x3, 0x23, 
    0x5, 0x23, 0xf4, 0xa, 0x23, 0x3, 0x23, 0x3, 0x23, 0x5, 0x23, 0xf8, 0xa, 
    0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x3, 0x24, 0x7, 0x24, 0xfe, 0xa, 
    0x24, 0xc, 0x24, 0xe, 0x24, 0x101, 0xb, 0x24, 0x3, 0x24, 0x5, 0x24, 
    0x104, 0xa, 0x24, 0x3, 0x24, 0x5, 0x24, 0x107, 0xa, 0x24, 0x3, 0x25, 
    0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x25, 0x3, 0x26, 0x5, 0x26, 0x10f, 
    0xa, 0x26, 0x3, 0x26, 0x7, 0x26, 0x112, 0xa, 0x26, 0xc, 0x26, 0xe, 0x26, 
    0x115, 0xb, 0x26, 0x3, 0x26, 0x6, 0x26, 0x118, 0xa, 0x26, 0xd, 0x26, 
    0xe, 0x26, 0x119, 0x3, 0x26, 0x2, 0x4, 0x36, 0x3a, 0x27, 0x2, 0x4, 0x6, 
    0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 
    0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 
    0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 0x44, 0x46, 0x48, 0x4a, 0x2, 0x6, 
    0x4, 0x2, 0x13, 0x14, 0x16, 0x17, 0x3, 0x2, 0x19, 0x1b, 0x3, 0x2, 0x1d, 
    0x22, 0x3, 0x2, 0x23, 0x26, 0x2, 0x112, 0x2, 0x4c, 0x3, 0x2, 0x2, 0x2, 
    0x4, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x6, 0x50, 0x3, 0x2, 0x2, 0x2, 0x8, 0x52, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0x54, 0x3, 0x2, 0x2, 0x2, 0xc, 0x56, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0x58, 0x3, 0x2, 0x2, 0x2, 0x10, 0x5a, 0x3, 0x2, 0x2, 
    0x2, 0x12, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x14, 0x5e, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0x60, 0x3, 0x2, 0x2, 0x2, 0x18, 0x62, 0x3, 0x2, 0x2, 0x2, 0x1a, 
    0x64, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x66, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x68, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x22, 0x6c, 0x3, 
    0x2, 0x2, 0x2, 0x24, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x26, 0x70, 0x3, 0x2, 
    0x2, 0x2, 0x28, 0x72, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x76, 0x3, 0x2, 0x2, 
    0x2, 0x2c, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x2e, 0x84, 0x3, 0x2, 0x2, 0x2, 
    0x30, 0x86, 0x3, 0x2, 0x2, 0x2, 0x32, 0x88, 0x3, 0x2, 0x2, 0x2, 0x34, 
    0x8a, 0x3, 0x2, 0x2, 0x2, 0x36, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x38, 0xa9, 
    0x3, 0x2, 0x2, 0x2, 0x3a, 0xbc, 0x3, 0x2, 0x2, 0x2, 0x3c, 0xcd, 0x3, 
    0x2, 0x2, 0x2, 0x3e, 0xcf, 0x3, 0x2, 0x2, 0x2, 0x40, 0xd2, 0x3, 0x2, 
    0x2, 0x2, 0x42, 0xd9, 0x3, 0x2, 0x2, 0x2, 0x44, 0xf7, 0x3, 0x2, 0x2, 
    0x2, 0x46, 0xf9, 0x3, 0x2, 0x2, 0x2, 0x48, 0x108, 0x3, 0x2, 0x2, 0x2, 
    0x4a, 0x10e, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4d, 0x9, 0x2, 0x2, 0x2, 0x4d, 
    0x3, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x4f, 0x7, 0x15, 0x2, 0x2, 0x4f, 0x5, 
    0x3, 0x2, 0x2, 0x2, 0x50, 0x51, 0x9, 0x3, 0x2, 0x2, 0x51, 0x7, 0x3, 
    0x2, 0x2, 0x2, 0x52, 0x53, 0x7, 0x1c, 0x2, 0x2, 0x53, 0x9, 0x3, 0x2, 
    0x2, 0x2, 0x54, 0x55, 0x9, 0x4, 0x2, 0x2, 0x55, 0xb, 0x3, 0x2, 0x2, 
    0x2, 0x56, 0x57, 0x9, 0x5, 0x2, 0x2, 0x57, 0xd, 0x3, 0x2, 0x2, 0x2, 
    0x58, 0x59, 0x7, 0x3, 0x2, 0x2, 0x59, 0xf, 0x3, 0x2, 0x2, 0x2, 0x5a, 
    0x5b, 0x7, 0x4, 0x2, 0x2, 0x5b, 0x11, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5d, 
    0x7, 0x5, 0x2, 0x2, 0x5d, 0x13, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x5f, 0x7, 
    0x6, 0x2, 0x2, 0x5f, 0x15, 0x3, 0x2, 0x2, 0x2, 0x60, 0x61, 0x7, 0x7, 
    0x2, 0x2, 0x61, 0x17, 0x3, 0x2, 0x2, 0x2, 0x62, 0x63, 0x7, 0x8, 0x2, 
    0x2, 0x63, 0x19, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 0x7, 0x9, 0x2, 0x2, 
    0x65, 0x1b, 0x3, 0x2, 0x2, 0x2, 0x66, 0x67, 0x7, 0xa, 0x2, 0x2, 0x67, 
    0x1d, 0x3, 0x2, 0x2, 0x2, 0x68, 0x69, 0x7, 0xb, 0x2, 0x2, 0x69, 0x1f, 
    0x3, 0x2, 0x2, 0x2, 0x6a, 0x6b, 0x7, 0xc, 0x2, 0x2, 0x6b, 0x21, 0x3, 
    0x2, 0x2, 0x2, 0x6c, 0x6d, 0x7, 0xd, 0x2, 0x2, 0x6d, 0x23, 0x3, 0x2, 
    0x2, 0x2, 0x6e, 0x6f, 0x7, 0xe, 0x2, 0x2, 0x6f, 0x25, 0x3, 0x2, 0x2, 
    0x2, 0x70, 0x71, 0x7, 0xf, 0x2, 0x2, 0x71, 0x27, 0x3, 0x2, 0x2, 0x2, 
    0x72, 0x73, 0x7, 0x10, 0x2, 0x2, 0x73, 0x29, 0x3, 0x2, 0x2, 0x2, 0x74, 
    0x77, 0x5, 0x14, 0xb, 0x2, 0x75, 0x77, 0x5, 0x16, 0xc, 0x2, 0x76, 0x74, 
    0x3, 0x2, 0x2, 0x2, 0x76, 0x75, 0x3, 0x2, 0x2, 0x2, 0x77, 0x2b, 0x3, 
    0x2, 0x2, 0x2, 0x78, 0x7b, 0x5, 0x1e, 0x10, 0x2, 0x79, 0x7b, 0x5, 0x20, 
    0x11, 0x2, 0x7a, 0x78, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x79, 0x3, 0x2, 0x2, 
    0x2, 0x7b, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7d, 0x5, 0xe, 0x8, 0x2, 
    0x7d, 0x7e, 0x7, 0x27, 0x2, 0x2, 0x7e, 0x7f, 0x7, 0x2a, 0x2, 0x2, 0x7f, 
    0x80, 0x7, 0x18, 0x2, 0x2, 0x80, 0x81, 0x7, 0x2a, 0x2, 0x2, 0x81, 0x82, 
    0x7, 0x28, 0x2, 0x2, 0x82, 0x85, 0x3, 0x2, 0x2, 0x2, 0x83, 0x85, 0x5, 
    0x10, 0x9, 0x2, 0x84, 0x7c, 0x3, 0x2, 0x2, 0x2, 0x84, 0x83, 0x3, 0x2, 
    0x2, 0x2, 0x85, 0x2f, 0x3, 0x2, 0x2, 0x2, 0x86, 0x87, 0x5, 0x3a, 0x1e, 
    0x2, 0x87, 0x31, 0x3, 0x2, 0x2, 0x2, 0x88, 0x89, 0x5, 0x3a, 0x1e, 0x2, 
    0x89, 0x33, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x8b, 0x5, 0x30, 0x19, 0x2, 0x8b, 
    0x8c, 0x5, 0xa, 0x6, 0x2, 0x8c, 0x8d, 0x5, 0x32, 0x1a, 0x2, 0x8d, 0x35, 
    0x3, 0x2, 0x2, 0x2, 0x8e, 0x8f, 0x8, 0x1c, 0x1, 0x2, 0x8f, 0x90, 0x5, 
    0x4, 0x3, 0x2, 0x90, 0x91, 0x5, 0x36, 0x1c, 0x8, 0x91, 0x9b, 0x3, 0x2, 
    0x2, 0x2, 0x92, 0x93, 0x5, 0x6, 0x4, 0x2, 0x93, 0x94, 0x5, 0x36, 0x1c, 
    0x6, 0x94, 0x9b, 0x3, 0x2, 0x2, 0x2, 0x95, 0x96, 0x7, 0x27, 0x2, 0x2, 
    0x96, 0x97, 0x5, 0x36, 0x1c, 0x2, 0x97, 0x98, 0x7, 0x28, 0x2, 0x2, 0x98, 
    0x9b, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9b, 0x5, 0x3c, 0x1f, 0x2, 0x9a, 0x8e, 
    0x3, 0x2, 0x2, 0x2, 0x9a, 0x92, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x95, 0x3, 
    0x2, 0x2, 0x2, 0x9a, 0x99, 0x3, 0x2, 0x2, 0x2, 0x9b, 0xa6, 0x3, 0x2, 
    0x2, 0x2, 0x9c, 0x9d, 0xc, 0x7, 0x2, 0x2, 0x9d, 0x9e, 0x5, 0x2, 0x2, 
    0x2, 0x9e, 0x9f, 0x5, 0x36, 0x1c, 0x8, 0x9f, 0xa5, 0x3, 0x2, 0x2, 0x2, 
    0xa0, 0xa1, 0xc, 0x5, 0x2, 0x2, 0xa1, 0xa2, 0x5, 0x8, 0x5, 0x2, 0xa2, 
    0xa3, 0x5, 0x36, 0x1c, 0x6, 0xa3, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa4, 0x9c, 
    0x3, 0x2, 0x2, 0x2, 0xa4, 0xa0, 0x3, 0x2, 0x2, 0x2, 0xa5, 0xa8, 0x3, 
    0x2, 0x2, 0x2, 0xa6, 0xa4, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa7, 0x3, 0x2, 
    0x2, 0x2, 0xa7, 0x37, 0x3, 0x2, 0x2, 0x2, 0xa8, 0xa6, 0x3, 0x2, 0x2, 
    0x2, 0xa9, 0xaa, 0x7, 0x24, 0x2, 0x2, 0xaa, 0xab, 0x7, 0x2a, 0x2, 0x2, 
    0xab, 0x39, 0x3, 0x2, 0x2, 0x2, 0xac, 0xad, 0x8, 0x1e, 0x1, 0x2, 0xad, 
    0xae, 0x7, 0x29, 0x2, 0x2, 0xae, 0xaf, 0x5, 0xc, 0x7, 0x2, 0xaf, 0xb0, 
    0x5, 0x3a, 0x1e, 0x9, 0xb0, 0xbd, 0x3, 0x2, 0x2, 0x2, 0xb1, 0xb2, 0x7, 
    0x29, 0x2, 0x2, 0xb2, 0xb3, 0x5, 0xc, 0x7, 0x2, 0xb3, 0xb4, 0x7, 0x29, 
    0x2, 0x2, 0xb4, 0xbd, 0x3, 0x2, 0x2, 0x2, 0xb5, 0xbd, 0x7, 0x29, 0x2, 
    0x2, 0xb6, 0xbd, 0x5, 0x38, 0x1d, 0x2, 0xb7, 0xbd, 0x7, 0x2a, 0x2, 0x2, 
    0xb8, 0xb9, 0x7, 0x27, 0x2, 0x2, 0xb9, 0xba, 0x5, 0x3a, 0x1e, 0x2, 0xba, 
    0xbb, 0x7, 0x28, 0x2, 0x2, 0xbb, 0xbd, 0x3, 0x2, 0x2, 0x2, 0xbc, 0xac, 
    0x3, 0x2, 0x2, 0x2, 0xbc, 0xb1, 0x3, 0x2, 0x2, 0x2, 0xbc, 0xb5, 0x3, 
    0x2, 0x2, 0x2, 0xbc, 0xb6, 0x3, 0x2, 0x2, 0x2, 0xbc, 0xb7, 0x3, 0x2, 
    0x2, 0x2, 0xbc, 0xb8, 0x3, 0x2, 0x2, 0x2, 0xbd, 0xc8, 0x3, 0x2, 0x2, 
    0x2, 0xbe, 0xbf, 0xc, 0xa, 0x2, 0x2, 0xbf, 0xc0, 0x5, 0xc, 0x7, 0x2, 
    0xc0, 0xc1, 0x5, 0x3a, 0x1e, 0xb, 0xc1, 0xc7, 0x3, 0x2, 0x2, 0x2, 0xc2, 
    0xc3, 0xc, 0x7, 0x2, 0x2, 0xc3, 0xc4, 0x5, 0xc, 0x7, 0x2, 0xc4, 0xc5, 
    0x7, 0x29, 0x2, 0x2, 0xc5, 0xc7, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xbe, 0x3, 
    0x2, 0x2, 0x2, 0xc6, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc7, 0xca, 0x3, 0x2, 
    0x2, 0x2, 0xc8, 0xc6, 0x3, 0x2, 0x2, 0x2, 0xc8, 0xc9, 0x3, 0x2, 0x2, 
    0x2, 0xc9, 0x3b, 0x3, 0x2, 0x2, 0x2, 0xca, 0xc8, 0x3, 0x2, 0x2, 0x2, 
    0xcb, 0xce, 0x5, 0x2c, 0x17, 0x2, 0xcc, 0xce, 0x7, 0x29, 0x2, 0x2, 0xcd, 
    0xcb, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xce, 0x3d, 
    0x3, 0x2, 0x2, 0x2, 0xcf, 0xd0, 0x5, 0x36, 0x1c, 0x2, 0xd0, 0xd1, 0x7, 
    0x2b, 0x2, 0x2, 0xd1, 0x3f, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd3, 0x5, 0x24, 
    0x13, 0x2, 0xd3, 0xd5, 0x7, 0x18, 0x2, 0x2, 0xd4, 0xd6, 0x5, 0x3e, 0x20, 
    0x2, 0xd5, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd7, 0x3, 0x2, 0x2, 0x2, 
    0xd7, 0xd5, 0x3, 0x2, 0x2, 0x2, 0xd7, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xd8, 
    0x41, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xda, 0x5, 0x26, 0x14, 0x2, 0xda, 0xdc, 
    0x7, 0x18, 0x2, 0x2, 0xdb, 0xdd, 0x5, 0x3e, 0x20, 0x2, 0xdc, 0xdb, 0x3, 
    0x2, 0x2, 0x2, 0xdd, 0xde, 0x3, 0x2, 0x2, 0x2, 0xde, 0xdc, 0x3, 0x2, 
    0x2, 0x2, 0xde, 0xdf, 0x3, 0x2, 0x2, 0x2, 0xdf, 0x43, 0x3, 0x2, 0x2, 
    0x2, 0xe0, 0xe2, 0x5, 0x2a, 0x16, 0x2, 0xe1, 0xe0, 0x3, 0x2, 0x2, 0x2, 
    0xe1, 0xe2, 0x3, 0x2, 0x2, 0x2, 0xe2, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe3, 
    0xe4, 0x5, 0x2e, 0x18, 0x2, 0xe4, 0xe5, 0x5, 0x12, 0xa, 0x2, 0xe5, 0xe6, 
    0x7, 0x29, 0x2, 0x2, 0xe6, 0xe7, 0x7, 0x2b, 0x2, 0x2, 0xe7, 0xf8, 0x3, 
    0x2, 0x2, 0x2, 0xe8, 0xe9, 0x5, 0x2e, 0x18, 0x2, 0xe9, 0xea, 0x5, 0x18, 
    0xd, 0x2, 0xea, 0xeb, 0x7, 0x29, 0x2, 0x2, 0xeb, 0xec, 0x7, 0x2a, 0x2, 
    0x2, 0xec, 0xed, 0x7, 0x2b, 0x2, 0x2, 0xed, 0xf8, 0x3, 0x2, 0x2, 0x2, 
    0xee, 0xef, 0x5, 0x1a, 0xe, 0x2, 0xef, 0xf3, 0x7, 0x29, 0x2, 0x2, 0xf0, 
    0xf1, 0x5, 0x1c, 0xf, 0x2, 0xf1, 0xf2, 0x5, 0x34, 0x1b, 0x2, 0xf2, 0xf4, 
    0x3, 0x2, 0x2, 0x2, 0xf3, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xf3, 0xf4, 0x3, 
    0x2, 0x2, 0x2, 0xf4, 0xf5, 0x3, 0x2, 0x2, 0x2, 0xf5, 0xf6, 0x7, 0x2b, 
    0x2, 0x2, 0xf6, 0xf8, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xe1, 0x3, 0x2, 0x2, 
    0x2, 0xf7, 0xe8, 0x3, 0x2, 0x2, 0x2, 0xf7, 0xee, 0x3, 0x2, 0x2, 0x2, 
    0xf8, 0x45, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xfa, 0x5, 0x22, 0x12, 0x2, 0xfa, 
    0xfb, 0x7, 0x29, 0x2, 0x2, 0xfb, 0xff, 0x7, 0x18, 0x2, 0x2, 0xfc, 0xfe, 
    0x5, 0x44, 0x23, 0x2, 0xfd, 0xfc, 0x3, 0x2, 0x2, 0x2, 0xfe, 0x101, 0x3, 
    0x2, 0x2, 0x2, 0xff, 0xfd, 0x3, 0x2, 0x2, 0x2, 0xff, 0x100, 0x3, 0x2, 
    0x2, 0x2, 0x100, 0x103, 0x3, 0x2, 0x2, 0x2, 0x101, 0xff, 0x3, 0x2, 0x2, 
    0x2, 0x102, 0x104, 0x5, 0x40, 0x21, 0x2, 0x103, 0x102, 0x3, 0x2, 0x2, 
    0x2, 0x103, 0x104, 0x3, 0x2, 0x2, 0x2, 0x104, 0x106, 0x3, 0x2, 0x2, 
    0x2, 0x105, 0x107, 0x5, 0x42, 0x22, 0x2, 0x106, 0x105, 0x3, 0x2, 0x2, 
    0x2, 0x106, 0x107, 0x3, 0x2, 0x2, 0x2, 0x107, 0x47, 0x3, 0x2, 0x2, 0x2, 
    0x108, 0x109, 0x5, 0x28, 0x15, 0x2, 0x109, 0x10a, 0x7, 0x18, 0x2, 0x2, 
    0x10a, 0x10b, 0x7, 0x29, 0x2, 0x2, 0x10b, 0x10c, 0x7, 0x2b, 0x2, 0x2, 
    0x10c, 0x49, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x10f, 0x5, 0x48, 0x25, 0x2, 
    0x10e, 0x10d, 0x3, 0x2, 0x2, 0x2, 0x10e, 0x10f, 0x3, 0x2, 0x2, 0x2, 
    0x10f, 0x113, 0x3, 0x2, 0x2, 0x2, 0x110, 0x112, 0x5, 0x44, 0x23, 0x2, 
    0x111, 0x110, 0x3, 0x2, 0x2, 0x2, 0x112, 0x115, 0x3, 0x2, 0x2, 0x2, 
    0x113, 0x111, 0x3, 0x2, 0x2, 0x2, 0x113, 0x114, 0x3, 0x2, 0x2, 0x2, 
    0x114, 0x117, 0x3, 0x2, 0x2, 0x2, 0x115, 0x113, 0x3, 0x2, 0x2, 0x2, 
    0x116, 0x118, 0x5, 0x46, 0x24, 0x2, 0x117, 0x116, 0x3, 0x2, 0x2, 0x2, 
    0x118, 0x119, 0x3, 0x2, 0x2, 0x2, 0x119, 0x117, 0x3, 0x2, 0x2, 0x2, 
    0x119, 0x11a, 0x3, 0x2, 0x2, 0x2, 0x11a, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x17, 
    0x76, 0x7a, 0x84, 0x9a, 0xa4, 0xa6, 0xbc, 0xc6, 0xc8, 0xcd, 0xd7, 0xde, 
    0xe1, 0xf3, 0xf7, 0xff, 0x103, 0x106, 0x10e, 0x113, 0x119, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

LTLContractsParser::Initializer LTLContractsParser::_init;
