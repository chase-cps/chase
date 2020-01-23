
// Generated from LTLContracts.g4 by ANTLR 4.8


#include "LTLContractsListener.h"

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


//----------------- IntegerKWContext ------------------------------------------------------------------

LTLContractsParser::IntegerKWContext::IntegerKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LTLContractsParser::IntegerKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleIntegerKW;
}

void LTLContractsParser::IntegerKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterIntegerKW(this);
}

void LTLContractsParser::IntegerKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitIntegerKW(this);
}

LTLContractsParser::IntegerKWContext* LTLContractsParser::integerKW() {
  IntegerKWContext *_localctx = _tracker.createInstance<IntegerKWContext>(_ctx, getState());
  enterRule(_localctx, 0, LTLContractsParser::RuleIntegerKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(60);
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

void LTLContractsParser::BooleanKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBooleanKW(this);
}

void LTLContractsParser::BooleanKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBooleanKW(this);
}

LTLContractsParser::BooleanKWContext* LTLContractsParser::booleanKW() {
  BooleanKWContext *_localctx = _tracker.createInstance<BooleanKWContext>(_ctx, getState());
  enterRule(_localctx, 2, LTLContractsParser::RuleBooleanKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(62);
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

void LTLContractsParser::VariableKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterVariableKW(this);
}

void LTLContractsParser::VariableKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitVariableKW(this);
}

LTLContractsParser::VariableKWContext* LTLContractsParser::variableKW() {
  VariableKWContext *_localctx = _tracker.createInstance<VariableKWContext>(_ctx, getState());
  enterRule(_localctx, 4, LTLContractsParser::RuleVariableKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    match(LTLContractsParser::T__2);
   
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

void LTLContractsParser::ConstantKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterConstantKW(this);
}

void LTLContractsParser::ConstantKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitConstantKW(this);
}

LTLContractsParser::ConstantKWContext* LTLContractsParser::constantKW() {
  ConstantKWContext *_localctx = _tracker.createInstance<ConstantKWContext>(_ctx, getState());
  enterRule(_localctx, 6, LTLContractsParser::RuleConstantKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(66);
    match(LTLContractsParser::T__3);
   
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

void LTLContractsParser::TrueKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTrueKW(this);
}

void LTLContractsParser::TrueKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTrueKW(this);
}

LTLContractsParser::TrueKWContext* LTLContractsParser::trueKW() {
  TrueKWContext *_localctx = _tracker.createInstance<TrueKWContext>(_ctx, getState());
  enterRule(_localctx, 8, LTLContractsParser::RuleTrueKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(68);
    match(LTLContractsParser::T__4);
   
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

void LTLContractsParser::FalseKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFalseKW(this);
}

void LTLContractsParser::FalseKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFalseKW(this);
}

LTLContractsParser::FalseKWContext* LTLContractsParser::falseKW() {
  FalseKWContext *_localctx = _tracker.createInstance<FalseKWContext>(_ctx, getState());
  enterRule(_localctx, 10, LTLContractsParser::RuleFalseKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(70);
    match(LTLContractsParser::T__5);
   
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

void LTLContractsParser::ContractKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterContractKW(this);
}

void LTLContractsParser::ContractKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitContractKW(this);
}

LTLContractsParser::ContractKWContext* LTLContractsParser::contractKW() {
  ContractKWContext *_localctx = _tracker.createInstance<ContractKWContext>(_ctx, getState());
  enterRule(_localctx, 12, LTLContractsParser::RuleContractKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    match(LTLContractsParser::T__6);
   
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

void LTLContractsParser::AssumptionsKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssumptionsKW(this);
}

void LTLContractsParser::AssumptionsKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssumptionsKW(this);
}

LTLContractsParser::AssumptionsKWContext* LTLContractsParser::assumptionsKW() {
  AssumptionsKWContext *_localctx = _tracker.createInstance<AssumptionsKWContext>(_ctx, getState());
  enterRule(_localctx, 14, LTLContractsParser::RuleAssumptionsKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(74);
    match(LTLContractsParser::T__7);
   
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

void LTLContractsParser::GuaranteesKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGuaranteesKW(this);
}

void LTLContractsParser::GuaranteesKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGuaranteesKW(this);
}

LTLContractsParser::GuaranteesKWContext* LTLContractsParser::guaranteesKW() {
  GuaranteesKWContext *_localctx = _tracker.createInstance<GuaranteesKWContext>(_ctx, getState());
  enterRule(_localctx, 16, LTLContractsParser::RuleGuaranteesKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(76);
    match(LTLContractsParser::T__8);
   
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

void LTLContractsParser::NameKwContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterNameKw(this);
}

void LTLContractsParser::NameKwContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitNameKw(this);
}

LTLContractsParser::NameKwContext* LTLContractsParser::nameKw() {
  NameKwContext *_localctx = _tracker.createInstance<NameKwContext>(_ctx, getState());
  enterRule(_localctx, 18, LTLContractsParser::RuleNameKw);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(78);
    match(LTLContractsParser::T__9);
   
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

void LTLContractsParser::Logic_constantContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterLogic_constant(this);
}

void LTLContractsParser::Logic_constantContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitLogic_constant(this);
}

LTLContractsParser::Logic_constantContext* LTLContractsParser::logic_constant() {
  Logic_constantContext *_localctx = _tracker.createInstance<Logic_constantContext>(_ctx, getState());
  enterRule(_localctx, 20, LTLContractsParser::RuleLogic_constant);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(82);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLContractsParser::T__4: {
        enterOuterAlt(_localctx, 1);
        setState(80);
        trueKW();
        break;
      }

      case LTLContractsParser::T__5: {
        enterOuterAlt(_localctx, 2);
        setState(81);
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

LTLContractsParser::BooleanKWContext* LTLContractsParser::TypeKWContext::booleanKW() {
  return getRuleContext<LTLContractsParser::BooleanKWContext>(0);
}


size_t LTLContractsParser::TypeKWContext::getRuleIndex() const {
  return LTLContractsParser::RuleTypeKW;
}

void LTLContractsParser::TypeKWContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterTypeKW(this);
}

void LTLContractsParser::TypeKWContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitTypeKW(this);
}

LTLContractsParser::TypeKWContext* LTLContractsParser::typeKW() {
  TypeKWContext *_localctx = _tracker.createInstance<TypeKWContext>(_ctx, getState());
  enterRule(_localctx, 22, LTLContractsParser::RuleTypeKW);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(86);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLContractsParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(84);
        integerKW();
        break;
      }

      case LTLContractsParser::T__1: {
        enterOuterAlt(_localctx, 2);
        setState(85);
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

void LTLContractsParser::Bin_logic_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBin_logic_op(this);
}

void LTLContractsParser::Bin_logic_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBin_logic_op(this);
}

LTLContractsParser::Bin_logic_opContext* LTLContractsParser::bin_logic_op() {
  Bin_logic_opContext *_localctx = _tracker.createInstance<Bin_logic_opContext>(_ctx, getState());
  enterRule(_localctx, 24, LTLContractsParser::RuleBin_logic_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(88);
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

void LTLContractsParser::Unary_logic_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnary_logic_op(this);
}

void LTLContractsParser::Unary_logic_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnary_logic_op(this);
}

LTLContractsParser::Unary_logic_opContext* LTLContractsParser::unary_logic_op() {
  Unary_logic_opContext *_localctx = _tracker.createInstance<Unary_logic_opContext>(_ctx, getState());
  enterRule(_localctx, 26, LTLContractsParser::RuleUnary_logic_op);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(90);
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

void LTLContractsParser::Unary_temp_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterUnary_temp_op(this);
}

void LTLContractsParser::Unary_temp_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitUnary_temp_op(this);
}

LTLContractsParser::Unary_temp_opContext* LTLContractsParser::unary_temp_op() {
  Unary_temp_opContext *_localctx = _tracker.createInstance<Unary_temp_opContext>(_ctx, getState());
  enterRule(_localctx, 28, LTLContractsParser::RuleUnary_temp_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(92);
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

void LTLContractsParser::Bin_temp_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBin_temp_op(this);
}

void LTLContractsParser::Bin_temp_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBin_temp_op(this);
}

LTLContractsParser::Bin_temp_opContext* LTLContractsParser::bin_temp_op() {
  Bin_temp_opContext *_localctx = _tracker.createInstance<Bin_temp_opContext>(_ctx, getState());
  enterRule(_localctx, 30, LTLContractsParser::RuleBin_temp_op);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(94);
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

void LTLContractsParser::Relation_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelation_op(this);
}

void LTLContractsParser::Relation_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelation_op(this);
}

LTLContractsParser::Relation_opContext* LTLContractsParser::relation_op() {
  Relation_opContext *_localctx = _tracker.createInstance<Relation_opContext>(_ctx, getState());
  enterRule(_localctx, 32, LTLContractsParser::RuleRelation_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
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

void LTLContractsParser::Bin_math_opContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterBin_math_op(this);
}

void LTLContractsParser::Bin_math_opContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitBin_math_op(this);
}

LTLContractsParser::Bin_math_opContext* LTLContractsParser::bin_math_op() {
  Bin_math_opContext *_localctx = _tracker.createInstance<Bin_math_opContext>(_ctx, getState());
  enterRule(_localctx, 34, LTLContractsParser::RuleBin_math_op);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(98);
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

//----------------- ValueContext ------------------------------------------------------------------

LTLContractsParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LTLContractsParser::ValueContext::ID() {
  return getToken(LTLContractsParser::ID, 0);
}

tree::TerminalNode* LTLContractsParser::ValueContext::NUMBER() {
  return getToken(LTLContractsParser::NUMBER, 0);
}

std::vector<LTLContractsParser::ValueContext *> LTLContractsParser::ValueContext::value() {
  return getRuleContexts<LTLContractsParser::ValueContext>();
}

LTLContractsParser::ValueContext* LTLContractsParser::ValueContext::value(size_t i) {
  return getRuleContext<LTLContractsParser::ValueContext>(i);
}

LTLContractsParser::Bin_math_opContext* LTLContractsParser::ValueContext::bin_math_op() {
  return getRuleContext<LTLContractsParser::Bin_math_opContext>(0);
}


size_t LTLContractsParser::ValueContext::getRuleIndex() const {
  return LTLContractsParser::RuleValue;
}

void LTLContractsParser::ValueContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterValue(this);
}

void LTLContractsParser::ValueContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitValue(this);
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
  size_t startState = 36;
  enterRecursionRule(_localctx, 36, LTLContractsParser::RuleValue, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(107);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LTLContractsParser::ID: {
        setState(101);
        match(LTLContractsParser::ID);
        break;
      }

      case LTLContractsParser::NUMBER: {
        setState(102);
        match(LTLContractsParser::NUMBER);
        break;
      }

      case LTLContractsParser::T__10: {
        setState(103);
        match(LTLContractsParser::T__10);
        setState(104);
        value(0);
        setState(105);
        match(LTLContractsParser::T__11);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(115);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<ValueContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleValue);
        setState(109);

        if (!(precpred(_ctx, 2))) throw FailedPredicateException(this, "precpred(_ctx, 2)");
        setState(110);
        bin_math_op();
        setState(111);
        value(3); 
      }
      setState(117);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 3, _ctx);
    }
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

std::vector<LTLContractsParser::ValueContext *> LTLContractsParser::RelationContext::value() {
  return getRuleContexts<LTLContractsParser::ValueContext>();
}

LTLContractsParser::ValueContext* LTLContractsParser::RelationContext::value(size_t i) {
  return getRuleContext<LTLContractsParser::ValueContext>(i);
}

LTLContractsParser::Relation_opContext* LTLContractsParser::RelationContext::relation_op() {
  return getRuleContext<LTLContractsParser::Relation_opContext>(0);
}

LTLContractsParser::RelationContext* LTLContractsParser::RelationContext::relation() {
  return getRuleContext<LTLContractsParser::RelationContext>(0);
}


size_t LTLContractsParser::RelationContext::getRuleIndex() const {
  return LTLContractsParser::RuleRelation;
}

void LTLContractsParser::RelationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterRelation(this);
}

void LTLContractsParser::RelationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitRelation(this);
}

LTLContractsParser::RelationContext* LTLContractsParser::relation() {
  RelationContext *_localctx = _tracker.createInstance<RelationContext>(_ctx, getState());
  enterRule(_localctx, 38, LTLContractsParser::RuleRelation);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(126);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(118);
      value(0);
      setState(119);
      relation_op();
      setState(120);
      value(0);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(122);
      match(LTLContractsParser::T__10);
      setState(123);
      relation();
      setState(124);
      match(LTLContractsParser::T__11);
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

//----------------- AtomContext ------------------------------------------------------------------

LTLContractsParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::Logic_constantContext* LTLContractsParser::AtomContext::logic_constant() {
  return getRuleContext<LTLContractsParser::Logic_constantContext>(0);
}

LTLContractsParser::RelationContext* LTLContractsParser::AtomContext::relation() {
  return getRuleContext<LTLContractsParser::RelationContext>(0);
}

tree::TerminalNode* LTLContractsParser::AtomContext::ID() {
  return getToken(LTLContractsParser::ID, 0);
}


size_t LTLContractsParser::AtomContext::getRuleIndex() const {
  return LTLContractsParser::RuleAtom;
}

void LTLContractsParser::AtomContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAtom(this);
}

void LTLContractsParser::AtomContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAtom(this);
}

LTLContractsParser::AtomContext* LTLContractsParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 40, LTLContractsParser::RuleAtom);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(131);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 5, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(128);
      logic_constant();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(129);
      relation();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(130);
      match(LTLContractsParser::ID);
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

//----------------- Prop_formulaContext ------------------------------------------------------------------

LTLContractsParser::Prop_formulaContext::Prop_formulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::Unary_logic_opContext* LTLContractsParser::Prop_formulaContext::unary_logic_op() {
  return getRuleContext<LTLContractsParser::Unary_logic_opContext>(0);
}

std::vector<LTLContractsParser::Prop_formulaContext *> LTLContractsParser::Prop_formulaContext::prop_formula() {
  return getRuleContexts<LTLContractsParser::Prop_formulaContext>();
}

LTLContractsParser::Prop_formulaContext* LTLContractsParser::Prop_formulaContext::prop_formula(size_t i) {
  return getRuleContext<LTLContractsParser::Prop_formulaContext>(i);
}

LTLContractsParser::AtomContext* LTLContractsParser::Prop_formulaContext::atom() {
  return getRuleContext<LTLContractsParser::AtomContext>(0);
}

LTLContractsParser::Bin_logic_opContext* LTLContractsParser::Prop_formulaContext::bin_logic_op() {
  return getRuleContext<LTLContractsParser::Bin_logic_opContext>(0);
}


size_t LTLContractsParser::Prop_formulaContext::getRuleIndex() const {
  return LTLContractsParser::RuleProp_formula;
}

void LTLContractsParser::Prop_formulaContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterProp_formula(this);
}

void LTLContractsParser::Prop_formulaContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitProp_formula(this);
}


LTLContractsParser::Prop_formulaContext* LTLContractsParser::prop_formula() {
   return prop_formula(0);
}

LTLContractsParser::Prop_formulaContext* LTLContractsParser::prop_formula(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  LTLContractsParser::Prop_formulaContext *_localctx = _tracker.createInstance<Prop_formulaContext>(_ctx, parentState);
  LTLContractsParser::Prop_formulaContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 42;
  enterRecursionRule(_localctx, 42, LTLContractsParser::RuleProp_formula, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(142);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      setState(134);
      unary_logic_op();
      setState(135);
      prop_formula(4);
      break;
    }

    case 2: {
      setState(137);
      match(LTLContractsParser::T__10);
      setState(138);
      prop_formula(0);
      setState(139);
      match(LTLContractsParser::T__11);
      break;
    }

    case 3: {
      setState(141);
      atom();
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(150);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<Prop_formulaContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleProp_formula);
        setState(144);

        if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
        setState(145);
        bin_logic_op();
        setState(146);
        prop_formula(4); 
      }
      setState(152);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 7, _ctx);
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

LTLContractsParser::FormulaContext::FormulaContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LTLContractsParser::Unary_temp_opContext* LTLContractsParser::FormulaContext::unary_temp_op() {
  return getRuleContext<LTLContractsParser::Unary_temp_opContext>(0);
}

std::vector<LTLContractsParser::FormulaContext *> LTLContractsParser::FormulaContext::formula() {
  return getRuleContexts<LTLContractsParser::FormulaContext>();
}

LTLContractsParser::FormulaContext* LTLContractsParser::FormulaContext::formula(size_t i) {
  return getRuleContext<LTLContractsParser::FormulaContext>(i);
}

LTLContractsParser::Prop_formulaContext* LTLContractsParser::FormulaContext::prop_formula() {
  return getRuleContext<LTLContractsParser::Prop_formulaContext>(0);
}

LTLContractsParser::Bin_temp_opContext* LTLContractsParser::FormulaContext::bin_temp_op() {
  return getRuleContext<LTLContractsParser::Bin_temp_opContext>(0);
}


size_t LTLContractsParser::FormulaContext::getRuleIndex() const {
  return LTLContractsParser::RuleFormula;
}

void LTLContractsParser::FormulaContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterFormula(this);
}

void LTLContractsParser::FormulaContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitFormula(this);
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
  size_t startState = 44;
  enterRecursionRule(_localctx, 44, LTLContractsParser::RuleFormula, precedence);

    

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(164);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      setState(154);
      unary_temp_op();
      setState(155);
      match(LTLContractsParser::T__10);
      setState(156);
      formula(0);
      setState(157);
      match(LTLContractsParser::T__11);
      break;
    }

    case 2: {
      setState(159);
      prop_formula(0);
      break;
    }

    case 3: {
      setState(160);
      match(LTLContractsParser::T__10);
      setState(161);
      formula(0);
      setState(162);
      match(LTLContractsParser::T__11);
      break;
    }

    }
    _ctx->stop = _input->LT(-1);
    setState(172);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<FormulaContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleFormula);
        setState(166);

        if (!(precpred(_ctx, 3))) throw FailedPredicateException(this, "precpred(_ctx, 3)");
        setState(167);
        bin_temp_op();
        setState(168);
        formula(4); 
      }
      setState(174);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 9, _ctx);
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

void LTLContractsParser::Single_formulaContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSingle_formula(this);
}

void LTLContractsParser::Single_formulaContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSingle_formula(this);
}

LTLContractsParser::Single_formulaContext* LTLContractsParser::single_formula() {
  Single_formulaContext *_localctx = _tracker.createInstance<Single_formulaContext>(_ctx, getState());
  enterRule(_localctx, 46, LTLContractsParser::RuleSingle_formula);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(175);
    formula(0);
    setState(176);
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

std::vector<LTLContractsParser::Single_formulaContext *> LTLContractsParser::AssumptionsContext::single_formula() {
  return getRuleContexts<LTLContractsParser::Single_formulaContext>();
}

LTLContractsParser::Single_formulaContext* LTLContractsParser::AssumptionsContext::single_formula(size_t i) {
  return getRuleContext<LTLContractsParser::Single_formulaContext>(i);
}


size_t LTLContractsParser::AssumptionsContext::getRuleIndex() const {
  return LTLContractsParser::RuleAssumptions;
}

void LTLContractsParser::AssumptionsContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterAssumptions(this);
}

void LTLContractsParser::AssumptionsContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitAssumptions(this);
}

LTLContractsParser::AssumptionsContext* LTLContractsParser::assumptions() {
  AssumptionsContext *_localctx = _tracker.createInstance<AssumptionsContext>(_ctx, getState());
  enterRule(_localctx, 48, LTLContractsParser::RuleAssumptions);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(178);
    assumptionsKW();
    setState(179);
    match(LTLContractsParser::T__12);
    setState(181); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(180);
      single_formula();
      setState(183); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::T__4)
      | (1ULL << LTLContractsParser::T__5)
      | (1ULL << LTLContractsParser::T__10)
      | (1ULL << LTLContractsParser::ID)
      | (1ULL << LTLContractsParser::NUMBER)
      | (1ULL << LTLContractsParser::NOT)
      | (1ULL << LTLContractsParser::ALWAYS)
      | (1ULL << LTLContractsParser::EVENTUALLY)
      | (1ULL << LTLContractsParser::NEXT))) != 0));
   
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

std::vector<LTLContractsParser::Single_formulaContext *> LTLContractsParser::GuaranteesContext::single_formula() {
  return getRuleContexts<LTLContractsParser::Single_formulaContext>();
}

LTLContractsParser::Single_formulaContext* LTLContractsParser::GuaranteesContext::single_formula(size_t i) {
  return getRuleContext<LTLContractsParser::Single_formulaContext>(i);
}


size_t LTLContractsParser::GuaranteesContext::getRuleIndex() const {
  return LTLContractsParser::RuleGuarantees;
}

void LTLContractsParser::GuaranteesContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterGuarantees(this);
}

void LTLContractsParser::GuaranteesContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitGuarantees(this);
}

LTLContractsParser::GuaranteesContext* LTLContractsParser::guarantees() {
  GuaranteesContext *_localctx = _tracker.createInstance<GuaranteesContext>(_ctx, getState());
  enterRule(_localctx, 50, LTLContractsParser::RuleGuarantees);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(185);
    guaranteesKW();
    setState(186);
    match(LTLContractsParser::T__12);
    setState(188); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(187);
      single_formula();
      setState(190); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << LTLContractsParser::T__4)
      | (1ULL << LTLContractsParser::T__5)
      | (1ULL << LTLContractsParser::T__10)
      | (1ULL << LTLContractsParser::ID)
      | (1ULL << LTLContractsParser::NUMBER)
      | (1ULL << LTLContractsParser::NOT)
      | (1ULL << LTLContractsParser::ALWAYS)
      | (1ULL << LTLContractsParser::EVENTUALLY)
      | (1ULL << LTLContractsParser::NEXT))) != 0));
   
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

LTLContractsParser::VariableKWContext* LTLContractsParser::DeclarationContext::variableKW() {
  return getRuleContext<LTLContractsParser::VariableKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::DeclarationContext::ID() {
  return getToken(LTLContractsParser::ID, 0);
}

LTLContractsParser::TypeKWContext* LTLContractsParser::DeclarationContext::typeKW() {
  return getRuleContext<LTLContractsParser::TypeKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::DeclarationContext::ENDST() {
  return getToken(LTLContractsParser::ENDST, 0);
}

LTLContractsParser::ConstantKWContext* LTLContractsParser::DeclarationContext::constantKW() {
  return getRuleContext<LTLContractsParser::ConstantKWContext>(0);
}

LTLContractsParser::IntegerKWContext* LTLContractsParser::DeclarationContext::integerKW() {
  return getRuleContext<LTLContractsParser::IntegerKWContext>(0);
}

tree::TerminalNode* LTLContractsParser::DeclarationContext::NUMBER() {
  return getToken(LTLContractsParser::NUMBER, 0);
}

LTLContractsParser::BooleanKWContext* LTLContractsParser::DeclarationContext::booleanKW() {
  return getRuleContext<LTLContractsParser::BooleanKWContext>(0);
}

LTLContractsParser::Logic_constantContext* LTLContractsParser::DeclarationContext::logic_constant() {
  return getRuleContext<LTLContractsParser::Logic_constantContext>(0);
}


size_t LTLContractsParser::DeclarationContext::getRuleIndex() const {
  return LTLContractsParser::RuleDeclaration;
}

void LTLContractsParser::DeclarationContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterDeclaration(this);
}

void LTLContractsParser::DeclarationContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitDeclaration(this);
}

LTLContractsParser::DeclarationContext* LTLContractsParser::declaration() {
  DeclarationContext *_localctx = _tracker.createInstance<DeclarationContext>(_ctx, getState());
  enterRule(_localctx, 52, LTLContractsParser::RuleDeclaration);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(209);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(192);
      variableKW();
      setState(193);
      match(LTLContractsParser::ID);
      setState(194);
      typeKW();
      setState(195);
      match(LTLContractsParser::ENDST);
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(197);
      constantKW();
      setState(198);
      match(LTLContractsParser::ID);
      setState(199);
      integerKW();
      setState(200);
      match(LTLContractsParser::NUMBER);
      setState(201);
      match(LTLContractsParser::ENDST);
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(203);
      constantKW();
      setState(204);
      match(LTLContractsParser::ID);
      setState(205);
      booleanKW();
      setState(206);
      logic_constant();
      setState(207);
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

LTLContractsParser::AssumptionsContext* LTLContractsParser::ContractContext::assumptions() {
  return getRuleContext<LTLContractsParser::AssumptionsContext>(0);
}

LTLContractsParser::GuaranteesContext* LTLContractsParser::ContractContext::guarantees() {
  return getRuleContext<LTLContractsParser::GuaranteesContext>(0);
}

std::vector<LTLContractsParser::DeclarationContext *> LTLContractsParser::ContractContext::declaration() {
  return getRuleContexts<LTLContractsParser::DeclarationContext>();
}

LTLContractsParser::DeclarationContext* LTLContractsParser::ContractContext::declaration(size_t i) {
  return getRuleContext<LTLContractsParser::DeclarationContext>(i);
}


size_t LTLContractsParser::ContractContext::getRuleIndex() const {
  return LTLContractsParser::RuleContract;
}

void LTLContractsParser::ContractContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterContract(this);
}

void LTLContractsParser::ContractContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitContract(this);
}

LTLContractsParser::ContractContext* LTLContractsParser::contract() {
  ContractContext *_localctx = _tracker.createInstance<ContractContext>(_ctx, getState());
  enterRule(_localctx, 54, LTLContractsParser::RuleContract);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(211);
    contractKW();
    setState(212);
    match(LTLContractsParser::ID);
    setState(213);
    match(LTLContractsParser::T__12);
    setState(217);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LTLContractsParser::T__2

    || _la == LTLContractsParser::T__3) {
      setState(214);
      declaration();
      setState(219);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(220);
    assumptions();
    setState(221);
    guarantees();
   
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

tree::TerminalNode* LTLContractsParser::NameContext::ID() {
  return getToken(LTLContractsParser::ID, 0);
}

tree::TerminalNode* LTLContractsParser::NameContext::ENDST() {
  return getToken(LTLContractsParser::ENDST, 0);
}


size_t LTLContractsParser::NameContext::getRuleIndex() const {
  return LTLContractsParser::RuleName;
}

void LTLContractsParser::NameContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterName(this);
}

void LTLContractsParser::NameContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitName(this);
}

LTLContractsParser::NameContext* LTLContractsParser::name() {
  NameContext *_localctx = _tracker.createInstance<NameContext>(_ctx, getState());
  enterRule(_localctx, 56, LTLContractsParser::RuleName);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(223);
    nameKw();
    setState(224);
    match(LTLContractsParser::T__12);
    setState(225);
    match(LTLContractsParser::ID);
    setState(226);
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

void LTLContractsParser::SystemSpecContext::enterRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->enterSystemSpec(this);
}

void LTLContractsParser::SystemSpecContext::exitRule(tree::ParseTreeListener *listener) {
  auto parserListener = dynamic_cast<LTLContractsListener *>(listener);
  if (parserListener != nullptr)
    parserListener->exitSystemSpec(this);
}

LTLContractsParser::SystemSpecContext* LTLContractsParser::systemSpec() {
  SystemSpecContext *_localctx = _tracker.createInstance<SystemSpecContext>(_ctx, getState());
  enterRule(_localctx, 58, LTLContractsParser::RuleSystemSpec);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(229);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == LTLContractsParser::T__9) {
      setState(228);
      name();
    }
    setState(234);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LTLContractsParser::T__2

    || _la == LTLContractsParser::T__3) {
      setState(231);
      declaration();
      setState(236);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(238); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(237);
      contract();
      setState(240); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LTLContractsParser::T__6);
   
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
    case 18: return valueSempred(dynamic_cast<ValueContext *>(context), predicateIndex);
    case 21: return prop_formulaSempred(dynamic_cast<Prop_formulaContext *>(context), predicateIndex);
    case 22: return formulaSempred(dynamic_cast<FormulaContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool LTLContractsParser::valueSempred(ValueContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 2);

  default:
    break;
  }
  return true;
}

bool LTLContractsParser::prop_formulaSempred(Prop_formulaContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 1: return precpred(_ctx, 3);

  default:
    break;
  }
  return true;
}

bool LTLContractsParser::formulaSempred(FormulaContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 2: return precpred(_ctx, 3);

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
  "integerKW", "booleanKW", "variableKW", "constantKW", "trueKW", "falseKW", 
  "contractKW", "assumptionsKW", "guaranteesKW", "nameKw", "logic_constant", 
  "typeKW", "bin_logic_op", "unary_logic_op", "unary_temp_op", "bin_temp_op", 
  "relation_op", "bin_math_op", "value", "relation", "atom", "prop_formula", 
  "formula", "single_formula", "assumptions", "guarantees", "declaration", 
  "contract", "name", "systemSpec"
};

std::vector<std::string> LTLContractsParser::_literalNames = {
  "", "'integer'", "'boolean'", "'variable'", "'constant'", "'true'", "'false'", 
  "'CONTRACT'", "'Assumptions'", "'Guarantees'", "'NAME'", "'('", "')'", 
  "':'", "", "", "", "", "';'", "", "", "", "'->'", "'<->'", "'[]'", "'<>'", 
  "'X'", "'U'", "'='", "'!='", "'<'", "'<='", "'>'", "'>='", "'+'", "'-'", 
  "'*'", "'/'"
};

std::vector<std::string> LTLContractsParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "WS", "LINE_COMMENT", 
  "ID", "NUMBER", "ENDST", "AND", "OR", "NOT", "IMPLIES", "IFF", "ALWAYS", 
  "EVENTUALLY", "NEXT", "UNTIL", "EQ", "NEQ", "LT", "LE", "GT", "GE", "PLUS", 
  "MINUS", "TIMES", "DIVIDE"
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
    0x3, 0x27, 0xf5, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x4, 
    0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 0x4, 0xb, 0x9, 
    0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 0xe, 0x9, 0xe, 0x4, 
    0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 0x9, 0x11, 0x4, 0x12, 
    0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 0x9, 0x14, 0x4, 0x15, 0x9, 
    0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 0x9, 0x17, 0x4, 0x18, 0x9, 0x18, 
    0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 0x4, 
    0x1c, 0x9, 0x1c, 0x4, 0x1d, 0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 0x1f, 
    0x9, 0x1f, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 
    0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 
    0x3, 0xc, 0x3, 0xc, 0x5, 0xc, 0x55, 0xa, 0xc, 0x3, 0xd, 0x3, 0xd, 0x5, 
    0xd, 0x59, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0x10, 
    0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x13, 0x3, 
    0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x5, 0x14, 0x6e, 0xa, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
    0x3, 0x14, 0x7, 0x14, 0x74, 0xa, 0x14, 0xc, 0x14, 0xe, 0x14, 0x77, 0xb, 
    0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 
    0x3, 0x15, 0x3, 0x15, 0x5, 0x15, 0x81, 0xa, 0x15, 0x3, 0x16, 0x3, 0x16, 
    0x3, 0x16, 0x5, 0x16, 0x86, 0xa, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 
    0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x5, 
    0x17, 0x91, 0xa, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x7, 
    0x17, 0x97, 0xa, 0x17, 0xc, 0x17, 0xe, 0x17, 0x9a, 0xb, 0x17, 0x3, 0x18, 
    0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 
    0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x5, 0x18, 0xa7, 0xa, 0x18, 0x3, 
    0x18, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 0x7, 0x18, 0xad, 0xa, 0x18, 0xc, 
    0x18, 0xe, 0x18, 0xb0, 0xb, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x3, 
    0x1a, 0x3, 0x1a, 0x3, 0x1a, 0x6, 0x1a, 0xb8, 0xa, 0x1a, 0xd, 0x1a, 0xe, 
    0x1a, 0xb9, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x6, 0x1b, 0xbf, 0xa, 0x1b, 
    0xd, 0x1b, 0xe, 0x1b, 0xc0, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 
    0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 
    0x5, 0x1c, 0xd4, 0xa, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 
    0x7, 0x1d, 0xda, 0xa, 0x1d, 0xc, 0x1d, 0xe, 0x1d, 0xdd, 0xb, 0x1d, 0x3, 
    0x1d, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 
    0x3, 0x1e, 0x3, 0x1f, 0x5, 0x1f, 0xe8, 0xa, 0x1f, 0x3, 0x1f, 0x7, 0x1f, 
    0xeb, 0xa, 0x1f, 0xc, 0x1f, 0xe, 0x1f, 0xee, 0xb, 0x1f, 0x3, 0x1f, 0x6, 
    0x1f, 0xf1, 0xa, 0x1f, 0xd, 0x1f, 0xe, 0x1f, 0xf2, 0x3, 0x1f, 0x2, 0x5, 
    0x26, 0x2c, 0x2e, 0x20, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 
    0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 
    0x2c, 0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x2, 0x6, 0x4, 
    0x2, 0x15, 0x16, 0x18, 0x19, 0x3, 0x2, 0x1a, 0x1c, 0x3, 0x2, 0x1e, 0x23, 
    0x3, 0x2, 0x24, 0x27, 0x2, 0xec, 0x2, 0x3e, 0x3, 0x2, 0x2, 0x2, 0x4, 
    0x40, 0x3, 0x2, 0x2, 0x2, 0x6, 0x42, 0x3, 0x2, 0x2, 0x2, 0x8, 0x44, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0x46, 0x3, 0x2, 0x2, 0x2, 0xc, 0x48, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0x4a, 0x3, 0x2, 0x2, 0x2, 0x10, 0x4c, 0x3, 0x2, 0x2, 
    0x2, 0x12, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x14, 0x50, 0x3, 0x2, 0x2, 0x2, 
    0x16, 0x54, 0x3, 0x2, 0x2, 0x2, 0x18, 0x58, 0x3, 0x2, 0x2, 0x2, 0x1a, 
    0x5a, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x5e, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0x60, 0x3, 0x2, 0x2, 0x2, 0x22, 0x62, 0x3, 
    0x2, 0x2, 0x2, 0x24, 0x64, 0x3, 0x2, 0x2, 0x2, 0x26, 0x6d, 0x3, 0x2, 
    0x2, 0x2, 0x28, 0x80, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x85, 0x3, 0x2, 0x2, 
    0x2, 0x2c, 0x90, 0x3, 0x2, 0x2, 0x2, 0x2e, 0xa6, 0x3, 0x2, 0x2, 0x2, 
    0x30, 0xb1, 0x3, 0x2, 0x2, 0x2, 0x32, 0xb4, 0x3, 0x2, 0x2, 0x2, 0x34, 
    0xbb, 0x3, 0x2, 0x2, 0x2, 0x36, 0xd3, 0x3, 0x2, 0x2, 0x2, 0x38, 0xd5, 
    0x3, 0x2, 0x2, 0x2, 0x3a, 0xe1, 0x3, 0x2, 0x2, 0x2, 0x3c, 0xe7, 0x3, 
    0x2, 0x2, 0x2, 0x3e, 0x3f, 0x7, 0x3, 0x2, 0x2, 0x3f, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x40, 0x41, 0x7, 0x4, 0x2, 0x2, 0x41, 0x5, 0x3, 0x2, 0x2, 
    0x2, 0x42, 0x43, 0x7, 0x5, 0x2, 0x2, 0x43, 0x7, 0x3, 0x2, 0x2, 0x2, 
    0x44, 0x45, 0x7, 0x6, 0x2, 0x2, 0x45, 0x9, 0x3, 0x2, 0x2, 0x2, 0x46, 
    0x47, 0x7, 0x7, 0x2, 0x2, 0x47, 0xb, 0x3, 0x2, 0x2, 0x2, 0x48, 0x49, 
    0x7, 0x8, 0x2, 0x2, 0x49, 0xd, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x4b, 0x7, 
    0x9, 0x2, 0x2, 0x4b, 0xf, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4d, 0x7, 0xa, 
    0x2, 0x2, 0x4d, 0x11, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x4f, 0x7, 0xb, 0x2, 
    0x2, 0x4f, 0x13, 0x3, 0x2, 0x2, 0x2, 0x50, 0x51, 0x7, 0xc, 0x2, 0x2, 
    0x51, 0x15, 0x3, 0x2, 0x2, 0x2, 0x52, 0x55, 0x5, 0xa, 0x6, 0x2, 0x53, 
    0x55, 0x5, 0xc, 0x7, 0x2, 0x54, 0x52, 0x3, 0x2, 0x2, 0x2, 0x54, 0x53, 
    0x3, 0x2, 0x2, 0x2, 0x55, 0x17, 0x3, 0x2, 0x2, 0x2, 0x56, 0x59, 0x5, 
    0x2, 0x2, 0x2, 0x57, 0x59, 0x5, 0x4, 0x3, 0x2, 0x58, 0x56, 0x3, 0x2, 
    0x2, 0x2, 0x58, 0x57, 0x3, 0x2, 0x2, 0x2, 0x59, 0x19, 0x3, 0x2, 0x2, 
    0x2, 0x5a, 0x5b, 0x9, 0x2, 0x2, 0x2, 0x5b, 0x1b, 0x3, 0x2, 0x2, 0x2, 
    0x5c, 0x5d, 0x7, 0x17, 0x2, 0x2, 0x5d, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x5e, 
    0x5f, 0x9, 0x3, 0x2, 0x2, 0x5f, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x60, 0x61, 
    0x7, 0x1d, 0x2, 0x2, 0x61, 0x21, 0x3, 0x2, 0x2, 0x2, 0x62, 0x63, 0x9, 
    0x4, 0x2, 0x2, 0x63, 0x23, 0x3, 0x2, 0x2, 0x2, 0x64, 0x65, 0x9, 0x5, 
    0x2, 0x2, 0x65, 0x25, 0x3, 0x2, 0x2, 0x2, 0x66, 0x67, 0x8, 0x14, 0x1, 
    0x2, 0x67, 0x6e, 0x7, 0x12, 0x2, 0x2, 0x68, 0x6e, 0x7, 0x13, 0x2, 0x2, 
    0x69, 0x6a, 0x7, 0xd, 0x2, 0x2, 0x6a, 0x6b, 0x5, 0x26, 0x14, 0x2, 0x6b, 
    0x6c, 0x7, 0xe, 0x2, 0x2, 0x6c, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x66, 
    0x3, 0x2, 0x2, 0x2, 0x6d, 0x68, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x69, 0x3, 
    0x2, 0x2, 0x2, 0x6e, 0x75, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x70, 0xc, 0x4, 
    0x2, 0x2, 0x70, 0x71, 0x5, 0x24, 0x13, 0x2, 0x71, 0x72, 0x5, 0x26, 0x14, 
    0x5, 0x72, 0x74, 0x3, 0x2, 0x2, 0x2, 0x73, 0x6f, 0x3, 0x2, 0x2, 0x2, 
    0x74, 0x77, 0x3, 0x2, 0x2, 0x2, 0x75, 0x73, 0x3, 0x2, 0x2, 0x2, 0x75, 
    0x76, 0x3, 0x2, 0x2, 0x2, 0x76, 0x27, 0x3, 0x2, 0x2, 0x2, 0x77, 0x75, 
    0x3, 0x2, 0x2, 0x2, 0x78, 0x79, 0x5, 0x26, 0x14, 0x2, 0x79, 0x7a, 0x5, 
    0x22, 0x12, 0x2, 0x7a, 0x7b, 0x5, 0x26, 0x14, 0x2, 0x7b, 0x81, 0x3, 
    0x2, 0x2, 0x2, 0x7c, 0x7d, 0x7, 0xd, 0x2, 0x2, 0x7d, 0x7e, 0x5, 0x28, 
    0x15, 0x2, 0x7e, 0x7f, 0x7, 0xe, 0x2, 0x2, 0x7f, 0x81, 0x3, 0x2, 0x2, 
    0x2, 0x80, 0x78, 0x3, 0x2, 0x2, 0x2, 0x80, 0x7c, 0x3, 0x2, 0x2, 0x2, 
    0x81, 0x29, 0x3, 0x2, 0x2, 0x2, 0x82, 0x86, 0x5, 0x16, 0xc, 0x2, 0x83, 
    0x86, 0x5, 0x28, 0x15, 0x2, 0x84, 0x86, 0x7, 0x12, 0x2, 0x2, 0x85, 0x82, 
    0x3, 0x2, 0x2, 0x2, 0x85, 0x83, 0x3, 0x2, 0x2, 0x2, 0x85, 0x84, 0x3, 
    0x2, 0x2, 0x2, 0x86, 0x2b, 0x3, 0x2, 0x2, 0x2, 0x87, 0x88, 0x8, 0x17, 
    0x1, 0x2, 0x88, 0x89, 0x5, 0x1c, 0xf, 0x2, 0x89, 0x8a, 0x5, 0x2c, 0x17, 
    0x6, 0x8a, 0x91, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x8c, 0x7, 0xd, 0x2, 0x2, 
    0x8c, 0x8d, 0x5, 0x2c, 0x17, 0x2, 0x8d, 0x8e, 0x7, 0xe, 0x2, 0x2, 0x8e, 
    0x91, 0x3, 0x2, 0x2, 0x2, 0x8f, 0x91, 0x5, 0x2a, 0x16, 0x2, 0x90, 0x87, 
    0x3, 0x2, 0x2, 0x2, 0x90, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x90, 0x8f, 0x3, 
    0x2, 0x2, 0x2, 0x91, 0x98, 0x3, 0x2, 0x2, 0x2, 0x92, 0x93, 0xc, 0x5, 
    0x2, 0x2, 0x93, 0x94, 0x5, 0x1a, 0xe, 0x2, 0x94, 0x95, 0x5, 0x2c, 0x17, 
    0x6, 0x95, 0x97, 0x3, 0x2, 0x2, 0x2, 0x96, 0x92, 0x3, 0x2, 0x2, 0x2, 
    0x97, 0x9a, 0x3, 0x2, 0x2, 0x2, 0x98, 0x96, 0x3, 0x2, 0x2, 0x2, 0x98, 
    0x99, 0x3, 0x2, 0x2, 0x2, 0x99, 0x2d, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x98, 
    0x3, 0x2, 0x2, 0x2, 0x9b, 0x9c, 0x8, 0x18, 0x1, 0x2, 0x9c, 0x9d, 0x5, 
    0x1e, 0x10, 0x2, 0x9d, 0x9e, 0x7, 0xd, 0x2, 0x2, 0x9e, 0x9f, 0x5, 0x2e, 
    0x18, 0x2, 0x9f, 0xa0, 0x7, 0xe, 0x2, 0x2, 0xa0, 0xa7, 0x3, 0x2, 0x2, 
    0x2, 0xa1, 0xa7, 0x5, 0x2c, 0x17, 0x2, 0xa2, 0xa3, 0x7, 0xd, 0x2, 0x2, 
    0xa3, 0xa4, 0x5, 0x2e, 0x18, 0x2, 0xa4, 0xa5, 0x7, 0xe, 0x2, 0x2, 0xa5, 
    0xa7, 0x3, 0x2, 0x2, 0x2, 0xa6, 0x9b, 0x3, 0x2, 0x2, 0x2, 0xa6, 0xa1, 
    0x3, 0x2, 0x2, 0x2, 0xa6, 0xa2, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xae, 0x3, 
    0x2, 0x2, 0x2, 0xa8, 0xa9, 0xc, 0x5, 0x2, 0x2, 0xa9, 0xaa, 0x5, 0x20, 
    0x11, 0x2, 0xaa, 0xab, 0x5, 0x2e, 0x18, 0x6, 0xab, 0xad, 0x3, 0x2, 0x2, 
    0x2, 0xac, 0xa8, 0x3, 0x2, 0x2, 0x2, 0xad, 0xb0, 0x3, 0x2, 0x2, 0x2, 
    0xae, 0xac, 0x3, 0x2, 0x2, 0x2, 0xae, 0xaf, 0x3, 0x2, 0x2, 0x2, 0xaf, 
    0x2f, 0x3, 0x2, 0x2, 0x2, 0xb0, 0xae, 0x3, 0x2, 0x2, 0x2, 0xb1, 0xb2, 
    0x5, 0x2e, 0x18, 0x2, 0xb2, 0xb3, 0x7, 0x14, 0x2, 0x2, 0xb3, 0x31, 0x3, 
    0x2, 0x2, 0x2, 0xb4, 0xb5, 0x5, 0x10, 0x9, 0x2, 0xb5, 0xb7, 0x7, 0xf, 
    0x2, 0x2, 0xb6, 0xb8, 0x5, 0x30, 0x19, 0x2, 0xb7, 0xb6, 0x3, 0x2, 0x2, 
    0x2, 0xb8, 0xb9, 0x3, 0x2, 0x2, 0x2, 0xb9, 0xb7, 0x3, 0x2, 0x2, 0x2, 
    0xb9, 0xba, 0x3, 0x2, 0x2, 0x2, 0xba, 0x33, 0x3, 0x2, 0x2, 0x2, 0xbb, 
    0xbc, 0x5, 0x12, 0xa, 0x2, 0xbc, 0xbe, 0x7, 0xf, 0x2, 0x2, 0xbd, 0xbf, 
    0x5, 0x30, 0x19, 0x2, 0xbe, 0xbd, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xc0, 0x3, 
    0x2, 0x2, 0x2, 0xc0, 0xbe, 0x3, 0x2, 0x2, 0x2, 0xc0, 0xc1, 0x3, 0x2, 
    0x2, 0x2, 0xc1, 0x35, 0x3, 0x2, 0x2, 0x2, 0xc2, 0xc3, 0x5, 0x6, 0x4, 
    0x2, 0xc3, 0xc4, 0x7, 0x12, 0x2, 0x2, 0xc4, 0xc5, 0x5, 0x18, 0xd, 0x2, 
    0xc5, 0xc6, 0x7, 0x14, 0x2, 0x2, 0xc6, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xc7, 
    0xc8, 0x5, 0x8, 0x5, 0x2, 0xc8, 0xc9, 0x7, 0x12, 0x2, 0x2, 0xc9, 0xca, 
    0x5, 0x2, 0x2, 0x2, 0xca, 0xcb, 0x7, 0x13, 0x2, 0x2, 0xcb, 0xcc, 0x7, 
    0x14, 0x2, 0x2, 0xcc, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xcd, 0xce, 0x5, 0x8, 
    0x5, 0x2, 0xce, 0xcf, 0x7, 0x12, 0x2, 0x2, 0xcf, 0xd0, 0x5, 0x4, 0x3, 
    0x2, 0xd0, 0xd1, 0x5, 0x16, 0xc, 0x2, 0xd1, 0xd2, 0x7, 0x14, 0x2, 0x2, 
    0xd2, 0xd4, 0x3, 0x2, 0x2, 0x2, 0xd3, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xd3, 
    0xc7, 0x3, 0x2, 0x2, 0x2, 0xd3, 0xcd, 0x3, 0x2, 0x2, 0x2, 0xd4, 0x37, 
    0x3, 0x2, 0x2, 0x2, 0xd5, 0xd6, 0x5, 0xe, 0x8, 0x2, 0xd6, 0xd7, 0x7, 
    0x12, 0x2, 0x2, 0xd7, 0xdb, 0x7, 0xf, 0x2, 0x2, 0xd8, 0xda, 0x5, 0x36, 
    0x1c, 0x2, 0xd9, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xda, 0xdd, 0x3, 0x2, 0x2, 
    0x2, 0xdb, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdc, 0x3, 0x2, 0x2, 0x2, 
    0xdc, 0xde, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xdb, 0x3, 0x2, 0x2, 0x2, 0xde, 
    0xdf, 0x5, 0x32, 0x1a, 0x2, 0xdf, 0xe0, 0x5, 0x34, 0x1b, 0x2, 0xe0, 
    0x39, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe2, 0x5, 0x14, 0xb, 0x2, 0xe2, 0xe3, 
    0x7, 0xf, 0x2, 0x2, 0xe3, 0xe4, 0x7, 0x12, 0x2, 0x2, 0xe4, 0xe5, 0x7, 
    0x14, 0x2, 0x2, 0xe5, 0x3b, 0x3, 0x2, 0x2, 0x2, 0xe6, 0xe8, 0x5, 0x3a, 
    0x1e, 0x2, 0xe7, 0xe6, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe8, 0x3, 0x2, 0x2, 
    0x2, 0xe8, 0xec, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xeb, 0x5, 0x36, 0x1c, 0x2, 
    0xea, 0xe9, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xee, 0x3, 0x2, 0x2, 0x2, 0xec, 
    0xea, 0x3, 0x2, 0x2, 0x2, 0xec, 0xed, 0x3, 0x2, 0x2, 0x2, 0xed, 0xf0, 
    0x3, 0x2, 0x2, 0x2, 0xee, 0xec, 0x3, 0x2, 0x2, 0x2, 0xef, 0xf1, 0x5, 
    0x38, 0x1d, 0x2, 0xf0, 0xef, 0x3, 0x2, 0x2, 0x2, 0xf1, 0xf2, 0x3, 0x2, 
    0x2, 0x2, 0xf2, 0xf0, 0x3, 0x2, 0x2, 0x2, 0xf2, 0xf3, 0x3, 0x2, 0x2, 
    0x2, 0xf3, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x13, 0x54, 0x58, 0x6d, 0x75, 0x80, 
    0x85, 0x90, 0x98, 0xa6, 0xae, 0xb9, 0xc0, 0xd3, 0xdb, 0xe7, 0xec, 0xf2, 
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
