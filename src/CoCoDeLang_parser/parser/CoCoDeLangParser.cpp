
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/CoCoDeLang_parser/grammar\CoCoDeLang.g4 by ANTLR 4.9.2


#include "CoCoDeLangVisitor.h"

#include "CoCoDeLangParser.h"


using namespace antlrcpp;
using namespace antlr4;

CoCoDeLangParser::CoCoDeLangParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

CoCoDeLangParser::~CoCoDeLangParser() {
  delete _interpreter;
}

std::string CoCoDeLangParser::getGrammarFileName() const {
  return "CoCoDeLang.g4";
}

const std::vector<std::string>& CoCoDeLangParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& CoCoDeLangParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- BasetypeContext ------------------------------------------------------------------

CoCoDeLangParser::BasetypeContext::BasetypeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::IntegerKWContext* CoCoDeLangParser::BasetypeContext::integerKW() {
  return getRuleContext<CoCoDeLangParser::IntegerKWContext>(0);
}

CoCoDeLangParser::RealKWContext* CoCoDeLangParser::BasetypeContext::realKW() {
  return getRuleContext<CoCoDeLangParser::RealKWContext>(0);
}

CoCoDeLangParser::BooleanKWContext* CoCoDeLangParser::BasetypeContext::booleanKW() {
  return getRuleContext<CoCoDeLangParser::BooleanKWContext>(0);
}

CoCoDeLangParser::ComplexKWContext* CoCoDeLangParser::BasetypeContext::complexKW() {
  return getRuleContext<CoCoDeLangParser::ComplexKWContext>(0);
}


size_t CoCoDeLangParser::BasetypeContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleBasetype;
}


antlrcpp::Any CoCoDeLangParser::BasetypeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitBasetype(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::BasetypeContext* CoCoDeLangParser::basetype() {
  BasetypeContext *_localctx = _tracker.createInstance<BasetypeContext>(_ctx, getState());
  enterRule(_localctx, 0, CoCoDeLangParser::RuleBasetype);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(114);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CoCoDeLangParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(110);
        integerKW();
        break;
      }

      case CoCoDeLangParser::T__2: {
        enterOuterAlt(_localctx, 2);
        setState(111);
        realKW();
        break;
      }

      case CoCoDeLangParser::T__1: {
        enterOuterAlt(_localctx, 3);
        setState(112);
        booleanKW();
        break;
      }

      case CoCoDeLangParser::T__3: {
        enterOuterAlt(_localctx, 4);
        setState(113);
        complexKW();
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

//----------------- DomainContext ------------------------------------------------------------------

CoCoDeLangParser::DomainContext::DomainContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::IntervalContext* CoCoDeLangParser::DomainContext::interval() {
  return getRuleContext<CoCoDeLangParser::IntervalContext>(0);
}

CoCoDeLangParser::ListContext* CoCoDeLangParser::DomainContext::list() {
  return getRuleContext<CoCoDeLangParser::ListContext>(0);
}


size_t CoCoDeLangParser::DomainContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleDomain;
}


antlrcpp::Any CoCoDeLangParser::DomainContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitDomain(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::DomainContext* CoCoDeLangParser::domain() {
  DomainContext *_localctx = _tracker.createInstance<DomainContext>(_ctx, getState());
  enterRule(_localctx, 2, CoCoDeLangParser::RuleDomain);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(118);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CoCoDeLangParser::LROUND:
      case CoCoDeLangParser::LSQUARE: {
        enterOuterAlt(_localctx, 1);
        setState(116);
        interval();
        break;
      }

      case CoCoDeLangParser::LCURLY: {
        enterOuterAlt(_localctx, 2);
        setState(117);
        list();
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

//----------------- Distribution_typeContext ------------------------------------------------------------------

CoCoDeLangParser::Distribution_typeContext::Distribution_typeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::IntegerKWContext* CoCoDeLangParser::Distribution_typeContext::integerKW() {
  return getRuleContext<CoCoDeLangParser::IntegerKWContext>(0);
}

CoCoDeLangParser::DomainContext* CoCoDeLangParser::Distribution_typeContext::domain() {
  return getRuleContext<CoCoDeLangParser::DomainContext>(0);
}

CoCoDeLangParser::RealKWContext* CoCoDeLangParser::Distribution_typeContext::realKW() {
  return getRuleContext<CoCoDeLangParser::RealKWContext>(0);
}

CoCoDeLangParser::BooleanKWContext* CoCoDeLangParser::Distribution_typeContext::booleanKW() {
  return getRuleContext<CoCoDeLangParser::BooleanKWContext>(0);
}


size_t CoCoDeLangParser::Distribution_typeContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleDistribution_type;
}


antlrcpp::Any CoCoDeLangParser::Distribution_typeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitDistribution_type(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Distribution_typeContext* CoCoDeLangParser::distribution_type() {
  Distribution_typeContext *_localctx = _tracker.createInstance<Distribution_typeContext>(_ctx, getState());
  enterRule(_localctx, 4, CoCoDeLangParser::RuleDistribution_type);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(129);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CoCoDeLangParser::T__0: {
        enterOuterAlt(_localctx, 1);
        setState(120);
        integerKW();
        setState(122);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << CoCoDeLangParser::LROUND)
          | (1ULL << CoCoDeLangParser::LSQUARE)
          | (1ULL << CoCoDeLangParser::LCURLY))) != 0)) {
          setState(121);
          domain();
        }
        break;
      }

      case CoCoDeLangParser::T__2: {
        enterOuterAlt(_localctx, 2);
        setState(124);
        realKW();
        setState(126);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if ((((_la & ~ 0x3fULL) == 0) &&
          ((1ULL << _la) & ((1ULL << CoCoDeLangParser::LROUND)
          | (1ULL << CoCoDeLangParser::LSQUARE)
          | (1ULL << CoCoDeLangParser::LCURLY))) != 0)) {
          setState(125);
          domain();
        }
        break;
      }

      case CoCoDeLangParser::T__1: {
        enterOuterAlt(_localctx, 3);
        setState(128);
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

//----------------- Distribution_featuresContext ------------------------------------------------------------------

CoCoDeLangParser::Distribution_featuresContext::Distribution_featuresContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::GaussianKWContext* CoCoDeLangParser::Distribution_featuresContext::gaussianKW() {
  return getRuleContext<CoCoDeLangParser::GaussianKWContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::Distribution_featuresContext::LROUND() {
  return getToken(CoCoDeLangParser::LROUND, 0);
}

std::vector<CoCoDeLangParser::ValueContext *> CoCoDeLangParser::Distribution_featuresContext::value() {
  return getRuleContexts<CoCoDeLangParser::ValueContext>();
}

CoCoDeLangParser::ValueContext* CoCoDeLangParser::Distribution_featuresContext::value(size_t i) {
  return getRuleContext<CoCoDeLangParser::ValueContext>(i);
}

tree::TerminalNode* CoCoDeLangParser::Distribution_featuresContext::COMMA() {
  return getToken(CoCoDeLangParser::COMMA, 0);
}

tree::TerminalNode* CoCoDeLangParser::Distribution_featuresContext::RROUND() {
  return getToken(CoCoDeLangParser::RROUND, 0);
}

CoCoDeLangParser::MuKWContext* CoCoDeLangParser::Distribution_featuresContext::muKW() {
  return getRuleContext<CoCoDeLangParser::MuKWContext>(0);
}

std::vector<tree::TerminalNode *> CoCoDeLangParser::Distribution_featuresContext::EQ() {
  return getTokens(CoCoDeLangParser::EQ);
}

tree::TerminalNode* CoCoDeLangParser::Distribution_featuresContext::EQ(size_t i) {
  return getToken(CoCoDeLangParser::EQ, i);
}

CoCoDeLangParser::SigmaKWContext* CoCoDeLangParser::Distribution_featuresContext::sigmaKW() {
  return getRuleContext<CoCoDeLangParser::SigmaKWContext>(0);
}

CoCoDeLangParser::HomogeneousKWContext* CoCoDeLangParser::Distribution_featuresContext::homogeneousKW() {
  return getRuleContext<CoCoDeLangParser::HomogeneousKWContext>(0);
}


size_t CoCoDeLangParser::Distribution_featuresContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleDistribution_features;
}


antlrcpp::Any CoCoDeLangParser::Distribution_featuresContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitDistribution_features(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Distribution_featuresContext* CoCoDeLangParser::distribution_features() {
  Distribution_featuresContext *_localctx = _tracker.createInstance<Distribution_featuresContext>(_ctx, getState());
  enterRule(_localctx, 6, CoCoDeLangParser::RuleDistribution_features);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(149);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CoCoDeLangParser::T__22: {
        enterOuterAlt(_localctx, 1);
        setState(131);
        gaussianKW();
        setState(132);
        match(CoCoDeLangParser::LROUND);
        setState(136);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == CoCoDeLangParser::T__23) {
          setState(133);
          muKW();
          setState(134);
          match(CoCoDeLangParser::EQ);
        }
        setState(138);
        value(0);
        setState(139);
        match(CoCoDeLangParser::COMMA);
        setState(143);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == CoCoDeLangParser::T__24) {
          setState(140);
          sigmaKW();
          setState(141);
          match(CoCoDeLangParser::EQ);
        }
        setState(145);
        value(0);
        setState(146);
        match(CoCoDeLangParser::RROUND);
        break;
      }

      case CoCoDeLangParser::T__25: {
        enterOuterAlt(_localctx, 2);
        setState(148);
        homogeneousKW();
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

//----------------- Distribution_definitionContext ------------------------------------------------------------------

CoCoDeLangParser::Distribution_definitionContext::Distribution_definitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::Distribution_typeContext* CoCoDeLangParser::Distribution_definitionContext::distribution_type() {
  return getRuleContext<CoCoDeLangParser::Distribution_typeContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::Distribution_definitionContext::COMMA() {
  return getToken(CoCoDeLangParser::COMMA, 0);
}

CoCoDeLangParser::Distribution_featuresContext* CoCoDeLangParser::Distribution_definitionContext::distribution_features() {
  return getRuleContext<CoCoDeLangParser::Distribution_featuresContext>(0);
}


size_t CoCoDeLangParser::Distribution_definitionContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleDistribution_definition;
}


antlrcpp::Any CoCoDeLangParser::Distribution_definitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitDistribution_definition(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Distribution_definitionContext* CoCoDeLangParser::distribution_definition() {
  Distribution_definitionContext *_localctx = _tracker.createInstance<Distribution_definitionContext>(_ctx, getState());
  enterRule(_localctx, 8, CoCoDeLangParser::RuleDistribution_definition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(151);
    distribution_type();
    setState(152);
    match(CoCoDeLangParser::COMMA);
    setState(153);
    distribution_features();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Distribution_declarationContext ------------------------------------------------------------------

CoCoDeLangParser::Distribution_declarationContext::Distribution_declarationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::DistributionKWContext* CoCoDeLangParser::Distribution_declarationContext::distributionKW() {
  return getRuleContext<CoCoDeLangParser::DistributionKWContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::Distribution_declarationContext::ID() {
  return getToken(CoCoDeLangParser::ID, 0);
}


size_t CoCoDeLangParser::Distribution_declarationContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleDistribution_declaration;
}


antlrcpp::Any CoCoDeLangParser::Distribution_declarationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitDistribution_declaration(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Distribution_declarationContext* CoCoDeLangParser::distribution_declaration() {
  Distribution_declarationContext *_localctx = _tracker.createInstance<Distribution_declarationContext>(_ctx, getState());
  enterRule(_localctx, 10, CoCoDeLangParser::RuleDistribution_declaration);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(155);
    distributionKW();
    setState(156);
    match(CoCoDeLangParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DistributionContext ------------------------------------------------------------------

CoCoDeLangParser::DistributionContext::DistributionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::Distribution_declarationContext* CoCoDeLangParser::DistributionContext::distribution_declaration() {
  return getRuleContext<CoCoDeLangParser::Distribution_declarationContext>(0);
}

CoCoDeLangParser::IsKWContext* CoCoDeLangParser::DistributionContext::isKW() {
  return getRuleContext<CoCoDeLangParser::IsKWContext>(0);
}

CoCoDeLangParser::Distribution_definitionContext* CoCoDeLangParser::DistributionContext::distribution_definition() {
  return getRuleContext<CoCoDeLangParser::Distribution_definitionContext>(0);
}


size_t CoCoDeLangParser::DistributionContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleDistribution;
}


antlrcpp::Any CoCoDeLangParser::DistributionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitDistribution(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::DistributionContext* CoCoDeLangParser::distribution() {
  DistributionContext *_localctx = _tracker.createInstance<DistributionContext>(_ctx, getState());
  enterRule(_localctx, 12, CoCoDeLangParser::RuleDistribution);

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
    distribution_declaration();
    setState(159);
    isKW();
    setState(160);
    distribution_definition();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Distribution_instanceContext ------------------------------------------------------------------

CoCoDeLangParser::Distribution_instanceContext::Distribution_instanceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Distribution_instanceContext::ID() {
  return getToken(CoCoDeLangParser::ID, 0);
}

CoCoDeLangParser::Distribution_definitionContext* CoCoDeLangParser::Distribution_instanceContext::distribution_definition() {
  return getRuleContext<CoCoDeLangParser::Distribution_definitionContext>(0);
}


size_t CoCoDeLangParser::Distribution_instanceContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleDistribution_instance;
}


antlrcpp::Any CoCoDeLangParser::Distribution_instanceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitDistribution_instance(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Distribution_instanceContext* CoCoDeLangParser::distribution_instance() {
  Distribution_instanceContext *_localctx = _tracker.createInstance<Distribution_instanceContext>(_ctx, getState());
  enterRule(_localctx, 14, CoCoDeLangParser::RuleDistribution_instance);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(164);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CoCoDeLangParser::ID: {
        enterOuterAlt(_localctx, 1);
        setState(162);
        match(CoCoDeLangParser::ID);
        break;
      }

      case CoCoDeLangParser::T__0:
      case CoCoDeLangParser::T__1:
      case CoCoDeLangParser::T__2: {
        enterOuterAlt(_localctx, 2);
        setState(163);
        distribution_definition();
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

//----------------- ControllabilityContext ------------------------------------------------------------------

CoCoDeLangParser::ControllabilityContext::ControllabilityContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::ControlledKWContext* CoCoDeLangParser::ControllabilityContext::controlledKW() {
  return getRuleContext<CoCoDeLangParser::ControlledKWContext>(0);
}

CoCoDeLangParser::UncontrolledKWContext* CoCoDeLangParser::ControllabilityContext::uncontrolledKW() {
  return getRuleContext<CoCoDeLangParser::UncontrolledKWContext>(0);
}


size_t CoCoDeLangParser::ControllabilityContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleControllability;
}


antlrcpp::Any CoCoDeLangParser::ControllabilityContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitControllability(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ControllabilityContext* CoCoDeLangParser::controllability() {
  ControllabilityContext *_localctx = _tracker.createInstance<ControllabilityContext>(_ctx, getState());
  enterRule(_localctx, 16, CoCoDeLangParser::RuleControllability);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(168);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CoCoDeLangParser::T__8: {
        enterOuterAlt(_localctx, 1);
        setState(166);
        controlledKW();
        break;
      }

      case CoCoDeLangParser::T__9: {
        enterOuterAlt(_localctx, 2);
        setState(167);
        uncontrolledKW();
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

//----------------- VariableContext ------------------------------------------------------------------

CoCoDeLangParser::VariableContext::VariableContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::ControllabilityContext* CoCoDeLangParser::VariableContext::controllability() {
  return getRuleContext<CoCoDeLangParser::ControllabilityContext>(0);
}

CoCoDeLangParser::VariableKWContext* CoCoDeLangParser::VariableContext::variableKW() {
  return getRuleContext<CoCoDeLangParser::VariableKWContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::VariableContext::ID() {
  return getToken(CoCoDeLangParser::ID, 0);
}

CoCoDeLangParser::IsKWContext* CoCoDeLangParser::VariableContext::isKW() {
  return getRuleContext<CoCoDeLangParser::IsKWContext>(0);
}

std::vector<tree::TerminalNode *> CoCoDeLangParser::VariableContext::COLON() {
  return getTokens(CoCoDeLangParser::COLON);
}

tree::TerminalNode* CoCoDeLangParser::VariableContext::COLON(size_t i) {
  return getToken(CoCoDeLangParser::COLON, i);
}

CoCoDeLangParser::BasetypeContext* CoCoDeLangParser::VariableContext::basetype() {
  return getRuleContext<CoCoDeLangParser::BasetypeContext>(0);
}

std::vector<tree::TerminalNode *> CoCoDeLangParser::VariableContext::COMMA() {
  return getTokens(CoCoDeLangParser::COMMA);
}

tree::TerminalNode* CoCoDeLangParser::VariableContext::COMMA(size_t i) {
  return getToken(CoCoDeLangParser::COMMA, i);
}

CoCoDeLangParser::ParametricKWContext* CoCoDeLangParser::VariableContext::parametricKW() {
  return getRuleContext<CoCoDeLangParser::ParametricKWContext>(0);
}

std::vector<CoCoDeLangParser::DomainContext *> CoCoDeLangParser::VariableContext::domain() {
  return getRuleContexts<CoCoDeLangParser::DomainContext>();
}

CoCoDeLangParser::DomainContext* CoCoDeLangParser::VariableContext::domain(size_t i) {
  return getRuleContext<CoCoDeLangParser::DomainContext>(i);
}

CoCoDeLangParser::StochasticKWContext* CoCoDeLangParser::VariableContext::stochasticKW() {
  return getRuleContext<CoCoDeLangParser::StochasticKWContext>(0);
}

CoCoDeLangParser::Distribution_instanceContext* CoCoDeLangParser::VariableContext::distribution_instance() {
  return getRuleContext<CoCoDeLangParser::Distribution_instanceContext>(0);
}


size_t CoCoDeLangParser::VariableContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleVariable;
}


antlrcpp::Any CoCoDeLangParser::VariableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitVariable(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::VariableContext* CoCoDeLangParser::variable() {
  VariableContext *_localctx = _tracker.createInstance<VariableContext>(_ctx, getState());
  enterRule(_localctx, 18, CoCoDeLangParser::RuleVariable);
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
    setState(170);
    controllability();
    setState(171);
    variableKW();
    setState(172);
    match(CoCoDeLangParser::ID);
    setState(173);
    isKW();
    setState(174);
    match(CoCoDeLangParser::COLON);
    setState(175);
    basetype();
    setState(177);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if ((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << CoCoDeLangParser::LROUND)
      | (1ULL << CoCoDeLangParser::LSQUARE)
      | (1ULL << CoCoDeLangParser::LCURLY))) != 0)) {
      setState(176);
      domain();
    }
    setState(184);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 11, _ctx)) {
    case 1: {
      setState(179);
      match(CoCoDeLangParser::COMMA);
      setState(180);
      stochasticKW();
      setState(181);
      match(CoCoDeLangParser::COLON);
      setState(182);
      distribution_instance();
      break;
    }

    default:
      break;
    }

    setState(186);
    match(CoCoDeLangParser::COMMA);
    setState(187);
    parametricKW();
    setState(188);
    match(CoCoDeLangParser::COLON);
    setState(189);
    domain();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Constant_definitionContext ------------------------------------------------------------------

CoCoDeLangParser::Constant_definitionContext::Constant_definitionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::NumberContext* CoCoDeLangParser::Constant_definitionContext::number() {
  return getRuleContext<CoCoDeLangParser::NumberContext>(0);
}

CoCoDeLangParser::TrueKWContext* CoCoDeLangParser::Constant_definitionContext::trueKW() {
  return getRuleContext<CoCoDeLangParser::TrueKWContext>(0);
}

CoCoDeLangParser::FalseKWContext* CoCoDeLangParser::Constant_definitionContext::falseKW() {
  return getRuleContext<CoCoDeLangParser::FalseKWContext>(0);
}


size_t CoCoDeLangParser::Constant_definitionContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleConstant_definition;
}


antlrcpp::Any CoCoDeLangParser::Constant_definitionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitConstant_definition(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Constant_definitionContext* CoCoDeLangParser::constant_definition() {
  Constant_definitionContext *_localctx = _tracker.createInstance<Constant_definitionContext>(_ctx, getState());
  enterRule(_localctx, 20, CoCoDeLangParser::RuleConstant_definition);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(194);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case CoCoDeLangParser::INTEGER:
      case CoCoDeLangParser::MINUS: {
        enterOuterAlt(_localctx, 1);
        setState(191);
        number();
        break;
      }

      case CoCoDeLangParser::T__17: {
        enterOuterAlt(_localctx, 2);
        setState(192);
        trueKW();
        break;
      }

      case CoCoDeLangParser::T__18: {
        enterOuterAlt(_localctx, 3);
        setState(193);
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

//----------------- ConstantContext ------------------------------------------------------------------

CoCoDeLangParser::ConstantContext::ConstantContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::ConstantKWContext* CoCoDeLangParser::ConstantContext::constantKW() {
  return getRuleContext<CoCoDeLangParser::ConstantKWContext>(0);
}

CoCoDeLangParser::BasetypeContext* CoCoDeLangParser::ConstantContext::basetype() {
  return getRuleContext<CoCoDeLangParser::BasetypeContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::ConstantContext::ID() {
  return getToken(CoCoDeLangParser::ID, 0);
}

tree::TerminalNode* CoCoDeLangParser::ConstantContext::COLON() {
  return getToken(CoCoDeLangParser::COLON, 0);
}

CoCoDeLangParser::Constant_definitionContext* CoCoDeLangParser::ConstantContext::constant_definition() {
  return getRuleContext<CoCoDeLangParser::Constant_definitionContext>(0);
}


size_t CoCoDeLangParser::ConstantContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleConstant;
}


antlrcpp::Any CoCoDeLangParser::ConstantContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitConstant(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ConstantContext* CoCoDeLangParser::constant() {
  ConstantContext *_localctx = _tracker.createInstance<ConstantContext>(_ctx, getState());
  enterRule(_localctx, 22, CoCoDeLangParser::RuleConstant);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(196);
    constantKW();
    setState(197);
    basetype();
    setState(198);
    match(CoCoDeLangParser::ID);
    setState(199);
    match(CoCoDeLangParser::COLON);
    setState(200);
    constant_definition();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Bin_logic_opContext ------------------------------------------------------------------

CoCoDeLangParser::Bin_logic_opContext::Bin_logic_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Bin_logic_opContext::AND() {
  return getToken(CoCoDeLangParser::AND, 0);
}

tree::TerminalNode* CoCoDeLangParser::Bin_logic_opContext::OR() {
  return getToken(CoCoDeLangParser::OR, 0);
}

tree::TerminalNode* CoCoDeLangParser::Bin_logic_opContext::IFF() {
  return getToken(CoCoDeLangParser::IFF, 0);
}

tree::TerminalNode* CoCoDeLangParser::Bin_logic_opContext::IMPLIES() {
  return getToken(CoCoDeLangParser::IMPLIES, 0);
}


size_t CoCoDeLangParser::Bin_logic_opContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleBin_logic_op;
}


antlrcpp::Any CoCoDeLangParser::Bin_logic_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitBin_logic_op(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Bin_logic_opContext* CoCoDeLangParser::bin_logic_op() {
  Bin_logic_opContext *_localctx = _tracker.createInstance<Bin_logic_opContext>(_ctx, getState());
  enterRule(_localctx, 24, CoCoDeLangParser::RuleBin_logic_op);
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
    setState(202);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << CoCoDeLangParser::AND)
      | (1ULL << CoCoDeLangParser::OR)
      | (1ULL << CoCoDeLangParser::IMPLIES)
      | (1ULL << CoCoDeLangParser::IFF))) != 0))) {
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

CoCoDeLangParser::Unary_logic_opContext::Unary_logic_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Unary_logic_opContext::NOT() {
  return getToken(CoCoDeLangParser::NOT, 0);
}


size_t CoCoDeLangParser::Unary_logic_opContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleUnary_logic_op;
}


antlrcpp::Any CoCoDeLangParser::Unary_logic_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitUnary_logic_op(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Unary_logic_opContext* CoCoDeLangParser::unary_logic_op() {
  Unary_logic_opContext *_localctx = _tracker.createInstance<Unary_logic_opContext>(_ctx, getState());
  enterRule(_localctx, 26, CoCoDeLangParser::RuleUnary_logic_op);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(204);
    match(CoCoDeLangParser::NOT);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Unary_temp_opContext ------------------------------------------------------------------

CoCoDeLangParser::Unary_temp_opContext::Unary_temp_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Unary_temp_opContext::ALWAYS() {
  return getToken(CoCoDeLangParser::ALWAYS, 0);
}

tree::TerminalNode* CoCoDeLangParser::Unary_temp_opContext::EVENTUALLY() {
  return getToken(CoCoDeLangParser::EVENTUALLY, 0);
}

tree::TerminalNode* CoCoDeLangParser::Unary_temp_opContext::NEXT() {
  return getToken(CoCoDeLangParser::NEXT, 0);
}


size_t CoCoDeLangParser::Unary_temp_opContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleUnary_temp_op;
}


antlrcpp::Any CoCoDeLangParser::Unary_temp_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitUnary_temp_op(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Unary_temp_opContext* CoCoDeLangParser::unary_temp_op() {
  Unary_temp_opContext *_localctx = _tracker.createInstance<Unary_temp_opContext>(_ctx, getState());
  enterRule(_localctx, 28, CoCoDeLangParser::RuleUnary_temp_op);
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
    setState(206);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << CoCoDeLangParser::ALWAYS)
      | (1ULL << CoCoDeLangParser::EVENTUALLY)
      | (1ULL << CoCoDeLangParser::NEXT))) != 0))) {
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

CoCoDeLangParser::Bin_temp_opContext::Bin_temp_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Bin_temp_opContext::UNTIL() {
  return getToken(CoCoDeLangParser::UNTIL, 0);
}

tree::TerminalNode* CoCoDeLangParser::Bin_temp_opContext::RELEASE() {
  return getToken(CoCoDeLangParser::RELEASE, 0);
}


size_t CoCoDeLangParser::Bin_temp_opContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleBin_temp_op;
}


antlrcpp::Any CoCoDeLangParser::Bin_temp_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitBin_temp_op(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Bin_temp_opContext* CoCoDeLangParser::bin_temp_op() {
  Bin_temp_opContext *_localctx = _tracker.createInstance<Bin_temp_opContext>(_ctx, getState());
  enterRule(_localctx, 30, CoCoDeLangParser::RuleBin_temp_op);
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
    setState(208);
    _la = _input->LA(1);
    if (!(_la == CoCoDeLangParser::UNTIL

    || _la == CoCoDeLangParser::RELEASE)) {
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

CoCoDeLangParser::Relation_opContext::Relation_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Relation_opContext::EQ() {
  return getToken(CoCoDeLangParser::EQ, 0);
}

tree::TerminalNode* CoCoDeLangParser::Relation_opContext::NEQ() {
  return getToken(CoCoDeLangParser::NEQ, 0);
}

tree::TerminalNode* CoCoDeLangParser::Relation_opContext::LT() {
  return getToken(CoCoDeLangParser::LT, 0);
}

tree::TerminalNode* CoCoDeLangParser::Relation_opContext::LE() {
  return getToken(CoCoDeLangParser::LE, 0);
}

tree::TerminalNode* CoCoDeLangParser::Relation_opContext::GT() {
  return getToken(CoCoDeLangParser::GT, 0);
}

tree::TerminalNode* CoCoDeLangParser::Relation_opContext::GE() {
  return getToken(CoCoDeLangParser::GE, 0);
}


size_t CoCoDeLangParser::Relation_opContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleRelation_op;
}


antlrcpp::Any CoCoDeLangParser::Relation_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitRelation_op(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Relation_opContext* CoCoDeLangParser::relation_op() {
  Relation_opContext *_localctx = _tracker.createInstance<Relation_opContext>(_ctx, getState());
  enterRule(_localctx, 32, CoCoDeLangParser::RuleRelation_op);
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
    setState(210);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << CoCoDeLangParser::EQ)
      | (1ULL << CoCoDeLangParser::NEQ)
      | (1ULL << CoCoDeLangParser::LT)
      | (1ULL << CoCoDeLangParser::LE)
      | (1ULL << CoCoDeLangParser::GT)
      | (1ULL << CoCoDeLangParser::GE))) != 0))) {
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

CoCoDeLangParser::Bin_math_opContext::Bin_math_opContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Bin_math_opContext::TIMES() {
  return getToken(CoCoDeLangParser::TIMES, 0);
}

tree::TerminalNode* CoCoDeLangParser::Bin_math_opContext::DIVIDE() {
  return getToken(CoCoDeLangParser::DIVIDE, 0);
}

tree::TerminalNode* CoCoDeLangParser::Bin_math_opContext::PLUS() {
  return getToken(CoCoDeLangParser::PLUS, 0);
}

tree::TerminalNode* CoCoDeLangParser::Bin_math_opContext::MINUS() {
  return getToken(CoCoDeLangParser::MINUS, 0);
}


size_t CoCoDeLangParser::Bin_math_opContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleBin_math_op;
}


antlrcpp::Any CoCoDeLangParser::Bin_math_opContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitBin_math_op(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Bin_math_opContext* CoCoDeLangParser::bin_math_op() {
  Bin_math_opContext *_localctx = _tracker.createInstance<Bin_math_opContext>(_ctx, getState());
  enterRule(_localctx, 34, CoCoDeLangParser::RuleBin_math_op);
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
    setState(212);
    _la = _input->LA(1);
    if (!((((_la & ~ 0x3fULL) == 0) &&
      ((1ULL << _la) & ((1ULL << CoCoDeLangParser::PLUS)
      | (1ULL << CoCoDeLangParser::MINUS)
      | (1ULL << CoCoDeLangParser::TIMES)
      | (1ULL << CoCoDeLangParser::DIVIDE))) != 0))) {
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

//----------------- NumberContext ------------------------------------------------------------------

CoCoDeLangParser::NumberContext::NumberContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::NumberContext::INTEGER() {
  return getToken(CoCoDeLangParser::INTEGER, 0);
}

tree::TerminalNode* CoCoDeLangParser::NumberContext::MINUS() {
  return getToken(CoCoDeLangParser::MINUS, 0);
}

tree::TerminalNode* CoCoDeLangParser::NumberContext::DECIMAL() {
  return getToken(CoCoDeLangParser::DECIMAL, 0);
}


size_t CoCoDeLangParser::NumberContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleNumber;
}


antlrcpp::Any CoCoDeLangParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::NumberContext* CoCoDeLangParser::number() {
  NumberContext *_localctx = _tracker.createInstance<NumberContext>(_ctx, getState());
  enterRule(_localctx, 36, CoCoDeLangParser::RuleNumber);
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
    setState(215);
    _errHandler->sync(this);

    _la = _input->LA(1);
    if (_la == CoCoDeLangParser::MINUS) {
      setState(214);
      match(CoCoDeLangParser::MINUS);
    }
    setState(217);
    match(CoCoDeLangParser::INTEGER);
    setState(219);
    _errHandler->sync(this);

    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 14, _ctx)) {
    case 1: {
      setState(218);
      match(CoCoDeLangParser::DECIMAL);
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

//----------------- IntegerKWContext ------------------------------------------------------------------

CoCoDeLangParser::IntegerKWContext::IntegerKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::IntegerKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleIntegerKW;
}


antlrcpp::Any CoCoDeLangParser::IntegerKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitIntegerKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::IntegerKWContext* CoCoDeLangParser::integerKW() {
  IntegerKWContext *_localctx = _tracker.createInstance<IntegerKWContext>(_ctx, getState());
  enterRule(_localctx, 38, CoCoDeLangParser::RuleIntegerKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(221);
    match(CoCoDeLangParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BooleanKWContext ------------------------------------------------------------------

CoCoDeLangParser::BooleanKWContext::BooleanKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::BooleanKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleBooleanKW;
}


antlrcpp::Any CoCoDeLangParser::BooleanKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitBooleanKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::BooleanKWContext* CoCoDeLangParser::booleanKW() {
  BooleanKWContext *_localctx = _tracker.createInstance<BooleanKWContext>(_ctx, getState());
  enterRule(_localctx, 40, CoCoDeLangParser::RuleBooleanKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(223);
    match(CoCoDeLangParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RealKWContext ------------------------------------------------------------------

CoCoDeLangParser::RealKWContext::RealKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::RealKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleRealKW;
}


antlrcpp::Any CoCoDeLangParser::RealKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitRealKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::RealKWContext* CoCoDeLangParser::realKW() {
  RealKWContext *_localctx = _tracker.createInstance<RealKWContext>(_ctx, getState());
  enterRule(_localctx, 42, CoCoDeLangParser::RuleRealKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(225);
    match(CoCoDeLangParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComplexKWContext ------------------------------------------------------------------

CoCoDeLangParser::ComplexKWContext::ComplexKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::ComplexKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleComplexKW;
}


antlrcpp::Any CoCoDeLangParser::ComplexKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitComplexKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ComplexKWContext* CoCoDeLangParser::complexKW() {
  ComplexKWContext *_localctx = _tracker.createInstance<ComplexKWContext>(_ctx, getState());
  enterRule(_localctx, 44, CoCoDeLangParser::RuleComplexKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(227);
    match(CoCoDeLangParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ContractKWContext ------------------------------------------------------------------

CoCoDeLangParser::ContractKWContext::ContractKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::ContractKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleContractKW;
}


antlrcpp::Any CoCoDeLangParser::ContractKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitContractKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ContractKWContext* CoCoDeLangParser::contractKW() {
  ContractKWContext *_localctx = _tracker.createInstance<ContractKWContext>(_ctx, getState());
  enterRule(_localctx, 46, CoCoDeLangParser::RuleContractKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(229);
    match(CoCoDeLangParser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableKWContext ------------------------------------------------------------------

CoCoDeLangParser::VariableKWContext::VariableKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::VariableKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleVariableKW;
}


antlrcpp::Any CoCoDeLangParser::VariableKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitVariableKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::VariableKWContext* CoCoDeLangParser::variableKW() {
  VariableKWContext *_localctx = _tracker.createInstance<VariableKWContext>(_ctx, getState());
  enterRule(_localctx, 48, CoCoDeLangParser::RuleVariableKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(231);
    match(CoCoDeLangParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConstantKWContext ------------------------------------------------------------------

CoCoDeLangParser::ConstantKWContext::ConstantKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::ConstantKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleConstantKW;
}


antlrcpp::Any CoCoDeLangParser::ConstantKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitConstantKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ConstantKWContext* CoCoDeLangParser::constantKW() {
  ConstantKWContext *_localctx = _tracker.createInstance<ConstantKWContext>(_ctx, getState());
  enterRule(_localctx, 50, CoCoDeLangParser::RuleConstantKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(233);
    match(CoCoDeLangParser::T__6);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ParametricKWContext ------------------------------------------------------------------

CoCoDeLangParser::ParametricKWContext::ParametricKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::ParametricKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleParametricKW;
}


antlrcpp::Any CoCoDeLangParser::ParametricKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitParametricKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ParametricKWContext* CoCoDeLangParser::parametricKW() {
  ParametricKWContext *_localctx = _tracker.createInstance<ParametricKWContext>(_ctx, getState());
  enterRule(_localctx, 52, CoCoDeLangParser::RuleParametricKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(235);
    match(CoCoDeLangParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ControlledKWContext ------------------------------------------------------------------

CoCoDeLangParser::ControlledKWContext::ControlledKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::ControlledKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleControlledKW;
}


antlrcpp::Any CoCoDeLangParser::ControlledKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitControlledKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ControlledKWContext* CoCoDeLangParser::controlledKW() {
  ControlledKWContext *_localctx = _tracker.createInstance<ControlledKWContext>(_ctx, getState());
  enterRule(_localctx, 54, CoCoDeLangParser::RuleControlledKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(237);
    match(CoCoDeLangParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UncontrolledKWContext ------------------------------------------------------------------

CoCoDeLangParser::UncontrolledKWContext::UncontrolledKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::UncontrolledKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleUncontrolledKW;
}


antlrcpp::Any CoCoDeLangParser::UncontrolledKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitUncontrolledKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::UncontrolledKWContext* CoCoDeLangParser::uncontrolledKW() {
  UncontrolledKWContext *_localctx = _tracker.createInstance<UncontrolledKWContext>(_ctx, getState());
  enterRule(_localctx, 56, CoCoDeLangParser::RuleUncontrolledKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(239);
    match(CoCoDeLangParser::T__9);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IsKWContext ------------------------------------------------------------------

CoCoDeLangParser::IsKWContext::IsKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::IsKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleIsKW;
}


antlrcpp::Any CoCoDeLangParser::IsKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitIsKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::IsKWContext* CoCoDeLangParser::isKW() {
  IsKWContext *_localctx = _tracker.createInstance<IsKWContext>(_ctx, getState());
  enterRule(_localctx, 58, CoCoDeLangParser::RuleIsKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(241);
    match(CoCoDeLangParser::T__10);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- StochasticKWContext ------------------------------------------------------------------

CoCoDeLangParser::StochasticKWContext::StochasticKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::StochasticKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleStochasticKW;
}


antlrcpp::Any CoCoDeLangParser::StochasticKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitStochasticKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::StochasticKWContext* CoCoDeLangParser::stochasticKW() {
  StochasticKWContext *_localctx = _tracker.createInstance<StochasticKWContext>(_ctx, getState());
  enterRule(_localctx, 60, CoCoDeLangParser::RuleStochasticKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(243);
    match(CoCoDeLangParser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DistributionKWContext ------------------------------------------------------------------

CoCoDeLangParser::DistributionKWContext::DistributionKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::DistributionKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleDistributionKW;
}


antlrcpp::Any CoCoDeLangParser::DistributionKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitDistributionKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::DistributionKWContext* CoCoDeLangParser::distributionKW() {
  DistributionKWContext *_localctx = _tracker.createInstance<DistributionKWContext>(_ctx, getState());
  enterRule(_localctx, 62, CoCoDeLangParser::RuleDistributionKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(245);
    match(CoCoDeLangParser::T__12);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComponentKWContext ------------------------------------------------------------------

CoCoDeLangParser::ComponentKWContext::ComponentKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::ComponentKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleComponentKW;
}


antlrcpp::Any CoCoDeLangParser::ComponentKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitComponentKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ComponentKWContext* CoCoDeLangParser::componentKW() {
  ComponentKWContext *_localctx = _tracker.createInstance<ComponentKWContext>(_ctx, getState());
  enterRule(_localctx, 64, CoCoDeLangParser::RuleComponentKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(247);
    match(CoCoDeLangParser::T__13);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- InputKWContext ------------------------------------------------------------------

CoCoDeLangParser::InputKWContext::InputKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::InputKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleInputKW;
}


antlrcpp::Any CoCoDeLangParser::InputKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitInputKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::InputKWContext* CoCoDeLangParser::inputKW() {
  InputKWContext *_localctx = _tracker.createInstance<InputKWContext>(_ctx, getState());
  enterRule(_localctx, 66, CoCoDeLangParser::RuleInputKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(249);
    match(CoCoDeLangParser::T__14);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- OutputKWContext ------------------------------------------------------------------

CoCoDeLangParser::OutputKWContext::OutputKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::OutputKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleOutputKW;
}


antlrcpp::Any CoCoDeLangParser::OutputKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitOutputKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::OutputKWContext* CoCoDeLangParser::outputKW() {
  OutputKWContext *_localctx = _tracker.createInstance<OutputKWContext>(_ctx, getState());
  enterRule(_localctx, 68, CoCoDeLangParser::RuleOutputKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(251);
    match(CoCoDeLangParser::T__15);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- PropositionKWContext ------------------------------------------------------------------

CoCoDeLangParser::PropositionKWContext::PropositionKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::PropositionKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RulePropositionKW;
}


antlrcpp::Any CoCoDeLangParser::PropositionKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitPropositionKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::PropositionKWContext* CoCoDeLangParser::propositionKW() {
  PropositionKWContext *_localctx = _tracker.createInstance<PropositionKWContext>(_ctx, getState());
  enterRule(_localctx, 70, CoCoDeLangParser::RulePropositionKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(253);
    match(CoCoDeLangParser::T__16);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TrueKWContext ------------------------------------------------------------------

CoCoDeLangParser::TrueKWContext::TrueKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::TrueKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleTrueKW;
}


antlrcpp::Any CoCoDeLangParser::TrueKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitTrueKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::TrueKWContext* CoCoDeLangParser::trueKW() {
  TrueKWContext *_localctx = _tracker.createInstance<TrueKWContext>(_ctx, getState());
  enterRule(_localctx, 72, CoCoDeLangParser::RuleTrueKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(255);
    match(CoCoDeLangParser::T__17);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- FalseKWContext ------------------------------------------------------------------

CoCoDeLangParser::FalseKWContext::FalseKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::FalseKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleFalseKW;
}


antlrcpp::Any CoCoDeLangParser::FalseKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitFalseKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::FalseKWContext* CoCoDeLangParser::falseKW() {
  FalseKWContext *_localctx = _tracker.createInstance<FalseKWContext>(_ctx, getState());
  enterRule(_localctx, 74, CoCoDeLangParser::RuleFalseKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(257);
    match(CoCoDeLangParser::T__18);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AssumptionsKWContext ------------------------------------------------------------------

CoCoDeLangParser::AssumptionsKWContext::AssumptionsKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::AssumptionsKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleAssumptionsKW;
}


antlrcpp::Any CoCoDeLangParser::AssumptionsKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitAssumptionsKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::AssumptionsKWContext* CoCoDeLangParser::assumptionsKW() {
  AssumptionsKWContext *_localctx = _tracker.createInstance<AssumptionsKWContext>(_ctx, getState());
  enterRule(_localctx, 76, CoCoDeLangParser::RuleAssumptionsKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(259);
    match(CoCoDeLangParser::T__19);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GuaranteesKWContext ------------------------------------------------------------------

CoCoDeLangParser::GuaranteesKWContext::GuaranteesKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::GuaranteesKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleGuaranteesKW;
}


antlrcpp::Any CoCoDeLangParser::GuaranteesKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitGuaranteesKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::GuaranteesKWContext* CoCoDeLangParser::guaranteesKW() {
  GuaranteesKWContext *_localctx = _tracker.createInstance<GuaranteesKWContext>(_ctx, getState());
  enterRule(_localctx, 78, CoCoDeLangParser::RuleGuaranteesKW);

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
    match(CoCoDeLangParser::T__20);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SystemKWContext ------------------------------------------------------------------

CoCoDeLangParser::SystemKWContext::SystemKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::SystemKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleSystemKW;
}


antlrcpp::Any CoCoDeLangParser::SystemKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitSystemKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::SystemKWContext* CoCoDeLangParser::systemKW() {
  SystemKWContext *_localctx = _tracker.createInstance<SystemKWContext>(_ctx, getState());
  enterRule(_localctx, 80, CoCoDeLangParser::RuleSystemKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(263);
    match(CoCoDeLangParser::T__21);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- GaussianKWContext ------------------------------------------------------------------

CoCoDeLangParser::GaussianKWContext::GaussianKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::GaussianKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleGaussianKW;
}


antlrcpp::Any CoCoDeLangParser::GaussianKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitGaussianKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::GaussianKWContext* CoCoDeLangParser::gaussianKW() {
  GaussianKWContext *_localctx = _tracker.createInstance<GaussianKWContext>(_ctx, getState());
  enterRule(_localctx, 82, CoCoDeLangParser::RuleGaussianKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(265);
    match(CoCoDeLangParser::T__22);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MuKWContext ------------------------------------------------------------------

CoCoDeLangParser::MuKWContext::MuKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::MuKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleMuKW;
}


antlrcpp::Any CoCoDeLangParser::MuKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitMuKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::MuKWContext* CoCoDeLangParser::muKW() {
  MuKWContext *_localctx = _tracker.createInstance<MuKWContext>(_ctx, getState());
  enterRule(_localctx, 84, CoCoDeLangParser::RuleMuKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(267);
    match(CoCoDeLangParser::T__23);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SigmaKWContext ------------------------------------------------------------------

CoCoDeLangParser::SigmaKWContext::SigmaKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::SigmaKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleSigmaKW;
}


antlrcpp::Any CoCoDeLangParser::SigmaKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitSigmaKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::SigmaKWContext* CoCoDeLangParser::sigmaKW() {
  SigmaKWContext *_localctx = _tracker.createInstance<SigmaKWContext>(_ctx, getState());
  enterRule(_localctx, 86, CoCoDeLangParser::RuleSigmaKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(269);
    match(CoCoDeLangParser::T__24);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- HomogeneousKWContext ------------------------------------------------------------------

CoCoDeLangParser::HomogeneousKWContext::HomogeneousKWContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t CoCoDeLangParser::HomogeneousKWContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleHomogeneousKW;
}


antlrcpp::Any CoCoDeLangParser::HomogeneousKWContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitHomogeneousKW(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::HomogeneousKWContext* CoCoDeLangParser::homogeneousKW() {
  HomogeneousKWContext *_localctx = _tracker.createInstance<HomogeneousKWContext>(_ctx, getState());
  enterRule(_localctx, 88, CoCoDeLangParser::RuleHomogeneousKW);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(271);
    match(CoCoDeLangParser::T__25);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Primed_IDContext ------------------------------------------------------------------

CoCoDeLangParser::Primed_IDContext::Primed_IDContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Primed_IDContext::ID() {
  return getToken(CoCoDeLangParser::ID, 0);
}


size_t CoCoDeLangParser::Primed_IDContext::getRuleIndex() const {
  return CoCoDeLangParser::RulePrimed_ID;
}


antlrcpp::Any CoCoDeLangParser::Primed_IDContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitPrimed_ID(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Primed_IDContext* CoCoDeLangParser::primed_ID() {
  Primed_IDContext *_localctx = _tracker.createInstance<Primed_IDContext>(_ctx, getState());
  enterRule(_localctx, 90, CoCoDeLangParser::RulePrimed_ID);

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
    match(CoCoDeLangParser::ID);
    setState(274);
    match(CoCoDeLangParser::T__26);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Minus_IDContext ------------------------------------------------------------------

CoCoDeLangParser::Minus_IDContext::Minus_IDContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Minus_IDContext::MINUS() {
  return getToken(CoCoDeLangParser::MINUS, 0);
}

tree::TerminalNode* CoCoDeLangParser::Minus_IDContext::ID() {
  return getToken(CoCoDeLangParser::ID, 0);
}


size_t CoCoDeLangParser::Minus_IDContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleMinus_ID;
}


antlrcpp::Any CoCoDeLangParser::Minus_IDContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitMinus_ID(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Minus_IDContext* CoCoDeLangParser::minus_ID() {
  Minus_IDContext *_localctx = _tracker.createInstance<Minus_IDContext>(_ctx, getState());
  enterRule(_localctx, 92, CoCoDeLangParser::RuleMinus_ID);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(276);
    match(CoCoDeLangParser::MINUS);
    setState(277);
    match(CoCoDeLangParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ValueContext ------------------------------------------------------------------

CoCoDeLangParser::ValueContext::ValueContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::Primed_IDContext* CoCoDeLangParser::ValueContext::primed_ID() {
  return getRuleContext<CoCoDeLangParser::Primed_IDContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::ValueContext::ID() {
  return getToken(CoCoDeLangParser::ID, 0);
}

CoCoDeLangParser::Minus_IDContext* CoCoDeLangParser::ValueContext::minus_ID() {
  return getRuleContext<CoCoDeLangParser::Minus_IDContext>(0);
}

CoCoDeLangParser::NumberContext* CoCoDeLangParser::ValueContext::number() {
  return getRuleContext<CoCoDeLangParser::NumberContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::ValueContext::LROUND() {
  return getToken(CoCoDeLangParser::LROUND, 0);
}

std::vector<CoCoDeLangParser::ValueContext *> CoCoDeLangParser::ValueContext::value() {
  return getRuleContexts<CoCoDeLangParser::ValueContext>();
}

CoCoDeLangParser::ValueContext* CoCoDeLangParser::ValueContext::value(size_t i) {
  return getRuleContext<CoCoDeLangParser::ValueContext>(i);
}

tree::TerminalNode* CoCoDeLangParser::ValueContext::RROUND() {
  return getToken(CoCoDeLangParser::RROUND, 0);
}

CoCoDeLangParser::TrueKWContext* CoCoDeLangParser::ValueContext::trueKW() {
  return getRuleContext<CoCoDeLangParser::TrueKWContext>(0);
}

CoCoDeLangParser::FalseKWContext* CoCoDeLangParser::ValueContext::falseKW() {
  return getRuleContext<CoCoDeLangParser::FalseKWContext>(0);
}

CoCoDeLangParser::Bin_math_opContext* CoCoDeLangParser::ValueContext::bin_math_op() {
  return getRuleContext<CoCoDeLangParser::Bin_math_opContext>(0);
}


size_t CoCoDeLangParser::ValueContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleValue;
}


antlrcpp::Any CoCoDeLangParser::ValueContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitValue(this);
  else
    return visitor->visitChildren(this);
}


CoCoDeLangParser::ValueContext* CoCoDeLangParser::value() {
   return value(0);
}

CoCoDeLangParser::ValueContext* CoCoDeLangParser::value(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  CoCoDeLangParser::ValueContext *_localctx = _tracker.createInstance<ValueContext>(_ctx, parentState);
  CoCoDeLangParser::ValueContext *previousContext = _localctx;
  (void)previousContext; // Silence compiler, in case the context is not used by generated code.
  size_t startState = 94;
  enterRecursionRule(_localctx, 94, CoCoDeLangParser::RuleValue, precedence);

    

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
    setState(290);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      setState(280);
      primed_ID();
      break;
    }

    case 2: {
      setState(281);
      match(CoCoDeLangParser::ID);
      break;
    }

    case 3: {
      setState(282);
      minus_ID();
      break;
    }

    case 4: {
      setState(283);
      number();
      break;
    }

    case 5: {
      setState(284);
      match(CoCoDeLangParser::LROUND);
      setState(285);
      value(0);
      setState(286);
      match(CoCoDeLangParser::RROUND);
      break;
    }

    case 6: {
      setState(288);
      trueKW();
      break;
    }

    case 7: {
      setState(289);
      falseKW();
      break;
    }

    default:
      break;
    }
    _ctx->stop = _input->LT(-1);
    setState(298);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        _localctx = _tracker.createInstance<ValueContext>(parentContext, parentState);
        pushNewRecursionContext(_localctx, startState, RuleValue);
        setState(292);

        if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");
        setState(293);
        bin_math_op();
        setState(294);
        value(9); 
      }
      setState(300);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 16, _ctx);
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

CoCoDeLangParser::PairContext::PairContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<CoCoDeLangParser::ValueContext *> CoCoDeLangParser::PairContext::value() {
  return getRuleContexts<CoCoDeLangParser::ValueContext>();
}

CoCoDeLangParser::ValueContext* CoCoDeLangParser::PairContext::value(size_t i) {
  return getRuleContext<CoCoDeLangParser::ValueContext>(i);
}

tree::TerminalNode* CoCoDeLangParser::PairContext::COMMA() {
  return getToken(CoCoDeLangParser::COMMA, 0);
}


size_t CoCoDeLangParser::PairContext::getRuleIndex() const {
  return CoCoDeLangParser::RulePair;
}


antlrcpp::Any CoCoDeLangParser::PairContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitPair(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::PairContext* CoCoDeLangParser::pair() {
  PairContext *_localctx = _tracker.createInstance<PairContext>(_ctx, getState());
  enterRule(_localctx, 96, CoCoDeLangParser::RulePair);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(301);
    value(0);
    setState(302);
    match(CoCoDeLangParser::COMMA);
    setState(303);
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

CoCoDeLangParser::Interval_leftopenContext::Interval_leftopenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Interval_leftopenContext::LROUND() {
  return getToken(CoCoDeLangParser::LROUND, 0);
}

CoCoDeLangParser::PairContext* CoCoDeLangParser::Interval_leftopenContext::pair() {
  return getRuleContext<CoCoDeLangParser::PairContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::Interval_leftopenContext::RSQUARE() {
  return getToken(CoCoDeLangParser::RSQUARE, 0);
}


size_t CoCoDeLangParser::Interval_leftopenContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleInterval_leftopen;
}


antlrcpp::Any CoCoDeLangParser::Interval_leftopenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitInterval_leftopen(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Interval_leftopenContext* CoCoDeLangParser::interval_leftopen() {
  Interval_leftopenContext *_localctx = _tracker.createInstance<Interval_leftopenContext>(_ctx, getState());
  enterRule(_localctx, 98, CoCoDeLangParser::RuleInterval_leftopen);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(305);
    match(CoCoDeLangParser::LROUND);
    setState(306);
    pair();
    setState(307);
    match(CoCoDeLangParser::RSQUARE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Interval_rightopenContext ------------------------------------------------------------------

CoCoDeLangParser::Interval_rightopenContext::Interval_rightopenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Interval_rightopenContext::LSQUARE() {
  return getToken(CoCoDeLangParser::LSQUARE, 0);
}

CoCoDeLangParser::PairContext* CoCoDeLangParser::Interval_rightopenContext::pair() {
  return getRuleContext<CoCoDeLangParser::PairContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::Interval_rightopenContext::RROUND() {
  return getToken(CoCoDeLangParser::RROUND, 0);
}


size_t CoCoDeLangParser::Interval_rightopenContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleInterval_rightopen;
}


antlrcpp::Any CoCoDeLangParser::Interval_rightopenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitInterval_rightopen(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Interval_rightopenContext* CoCoDeLangParser::interval_rightopen() {
  Interval_rightopenContext *_localctx = _tracker.createInstance<Interval_rightopenContext>(_ctx, getState());
  enterRule(_localctx, 100, CoCoDeLangParser::RuleInterval_rightopen);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(309);
    match(CoCoDeLangParser::LSQUARE);
    setState(310);
    pair();
    setState(311);
    match(CoCoDeLangParser::RROUND);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Interval_fullopenContext ------------------------------------------------------------------

CoCoDeLangParser::Interval_fullopenContext::Interval_fullopenContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Interval_fullopenContext::LROUND() {
  return getToken(CoCoDeLangParser::LROUND, 0);
}

CoCoDeLangParser::PairContext* CoCoDeLangParser::Interval_fullopenContext::pair() {
  return getRuleContext<CoCoDeLangParser::PairContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::Interval_fullopenContext::RROUND() {
  return getToken(CoCoDeLangParser::RROUND, 0);
}


size_t CoCoDeLangParser::Interval_fullopenContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleInterval_fullopen;
}


antlrcpp::Any CoCoDeLangParser::Interval_fullopenContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitInterval_fullopen(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Interval_fullopenContext* CoCoDeLangParser::interval_fullopen() {
  Interval_fullopenContext *_localctx = _tracker.createInstance<Interval_fullopenContext>(_ctx, getState());
  enterRule(_localctx, 102, CoCoDeLangParser::RuleInterval_fullopen);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(313);
    match(CoCoDeLangParser::LROUND);
    setState(314);
    pair();
    setState(315);
    match(CoCoDeLangParser::RROUND);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- Interval_closedContext ------------------------------------------------------------------

CoCoDeLangParser::Interval_closedContext::Interval_closedContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::Interval_closedContext::LSQUARE() {
  return getToken(CoCoDeLangParser::LSQUARE, 0);
}

CoCoDeLangParser::PairContext* CoCoDeLangParser::Interval_closedContext::pair() {
  return getRuleContext<CoCoDeLangParser::PairContext>(0);
}

tree::TerminalNode* CoCoDeLangParser::Interval_closedContext::RSQUARE() {
  return getToken(CoCoDeLangParser::RSQUARE, 0);
}


size_t CoCoDeLangParser::Interval_closedContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleInterval_closed;
}


antlrcpp::Any CoCoDeLangParser::Interval_closedContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitInterval_closed(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::Interval_closedContext* CoCoDeLangParser::interval_closed() {
  Interval_closedContext *_localctx = _tracker.createInstance<Interval_closedContext>(_ctx, getState());
  enterRule(_localctx, 104, CoCoDeLangParser::RuleInterval_closed);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(317);
    match(CoCoDeLangParser::LSQUARE);
    setState(318);
    pair();
    setState(319);
    match(CoCoDeLangParser::RSQUARE);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IntervalContext ------------------------------------------------------------------

CoCoDeLangParser::IntervalContext::IntervalContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

CoCoDeLangParser::Interval_closedContext* CoCoDeLangParser::IntervalContext::interval_closed() {
  return getRuleContext<CoCoDeLangParser::Interval_closedContext>(0);
}

CoCoDeLangParser::Interval_fullopenContext* CoCoDeLangParser::IntervalContext::interval_fullopen() {
  return getRuleContext<CoCoDeLangParser::Interval_fullopenContext>(0);
}

CoCoDeLangParser::Interval_leftopenContext* CoCoDeLangParser::IntervalContext::interval_leftopen() {
  return getRuleContext<CoCoDeLangParser::Interval_leftopenContext>(0);
}

CoCoDeLangParser::Interval_rightopenContext* CoCoDeLangParser::IntervalContext::interval_rightopen() {
  return getRuleContext<CoCoDeLangParser::Interval_rightopenContext>(0);
}


size_t CoCoDeLangParser::IntervalContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleInterval;
}


antlrcpp::Any CoCoDeLangParser::IntervalContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitInterval(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::IntervalContext* CoCoDeLangParser::interval() {
  IntervalContext *_localctx = _tracker.createInstance<IntervalContext>(_ctx, getState());
  enterRule(_localctx, 106, CoCoDeLangParser::RuleInterval);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(325);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 17, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(321);
      interval_closed();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(322);
      interval_fullopen();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(323);
      interval_leftopen();
      break;
    }

    case 4: {
      enterOuterAlt(_localctx, 4);
      setState(324);
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

//----------------- ListContext ------------------------------------------------------------------

CoCoDeLangParser::ListContext::ListContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* CoCoDeLangParser::ListContext::LCURLY() {
  return getToken(CoCoDeLangParser::LCURLY, 0);
}

std::vector<CoCoDeLangParser::ValueContext *> CoCoDeLangParser::ListContext::value() {
  return getRuleContexts<CoCoDeLangParser::ValueContext>();
}

CoCoDeLangParser::ValueContext* CoCoDeLangParser::ListContext::value(size_t i) {
  return getRuleContext<CoCoDeLangParser::ValueContext>(i);
}

tree::TerminalNode* CoCoDeLangParser::ListContext::RCURLY() {
  return getToken(CoCoDeLangParser::RCURLY, 0);
}

std::vector<tree::TerminalNode *> CoCoDeLangParser::ListContext::COMMA() {
  return getTokens(CoCoDeLangParser::COMMA);
}

tree::TerminalNode* CoCoDeLangParser::ListContext::COMMA(size_t i) {
  return getToken(CoCoDeLangParser::COMMA, i);
}


size_t CoCoDeLangParser::ListContext::getRuleIndex() const {
  return CoCoDeLangParser::RuleList;
}


antlrcpp::Any CoCoDeLangParser::ListContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<CoCoDeLangVisitor*>(visitor))
    return parserVisitor->visitList(this);
  else
    return visitor->visitChildren(this);
}

CoCoDeLangParser::ListContext* CoCoDeLangParser::list() {
  ListContext *_localctx = _tracker.createInstance<ListContext>(_ctx, getState());
  enterRule(_localctx, 108, CoCoDeLangParser::RuleList);
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
    setState(327);
    match(CoCoDeLangParser::LCURLY);
    setState(328);
    value(0);
    setState(333);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == CoCoDeLangParser::COMMA) {
      setState(329);
      match(CoCoDeLangParser::COMMA);
      setState(330);
      value(0);
      setState(335);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(336);
    match(CoCoDeLangParser::RCURLY);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool CoCoDeLangParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 47: return valueSempred(dynamic_cast<ValueContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool CoCoDeLangParser::valueSempred(ValueContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 8);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> CoCoDeLangParser::_decisionToDFA;
atn::PredictionContextCache CoCoDeLangParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN CoCoDeLangParser::_atn;
std::vector<uint16_t> CoCoDeLangParser::_serializedATN;

std::vector<std::string> CoCoDeLangParser::_ruleNames = {
  "basetype", "domain", "distribution_type", "distribution_features", "distribution_definition", 
  "distribution_declaration", "distribution", "distribution_instance", "controllability", 
  "variable", "constant_definition", "constant", "bin_logic_op", "unary_logic_op", 
  "unary_temp_op", "bin_temp_op", "relation_op", "bin_math_op", "number", 
  "integerKW", "booleanKW", "realKW", "complexKW", "contractKW", "variableKW", 
  "constantKW", "parametricKW", "controlledKW", "uncontrolledKW", "isKW", 
  "stochasticKW", "distributionKW", "componentKW", "inputKW", "outputKW", 
  "propositionKW", "trueKW", "falseKW", "assumptionsKW", "guaranteesKW", 
  "systemKW", "gaussianKW", "muKW", "sigmaKW", "homogeneousKW", "primed_ID", 
  "minus_ID", "value", "pair", "interval_leftopen", "interval_rightopen", 
  "interval_fullopen", "interval_closed", "interval", "list"
};

std::vector<std::string> CoCoDeLangParser::_literalNames = {
  "", "'integer'", "'boolean'", "'real'", "'complex'", "'Contract'", "'variable'", 
  "'constant'", "'parametric'", "'controlled'", "'uncontrolled'", "'is'", 
  "'stochastic'", "'distribution'", "'Component'", "'input'", "'output'", 
  "'proposition'", "'true'", "'false'", "'Assumptions'", "'Guarantees'", 
  "'System'", "'gaussian'", "'mu'", "'sigma'", "'homogeneous'", "'''", "", 
  "", "", "", "", "", "", "", "", "'->'", "'<->'", "':'", "'G'", "'F'", 
  "'X'", "'U'", "'R'", "'='", "'!='", "'<'", "'<='", "'>'", "'>='", "'+'", 
  "'-'", "'*'", "'/'", "'('", "')'", "'['", "']'", "'{'", "'}'", "','", 
  "'.'", "';'"
};

std::vector<std::string> CoCoDeLangParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "", "", "ID", "INTEGER", "DECIMAL", "WS", 
  "LINE_COMMENT", "MULTILINE_COMMENT", "AND", "OR", "NOT", "IMPLIES", "IFF", 
  "COLON", "ALWAYS", "EVENTUALLY", "NEXT", "UNTIL", "RELEASE", "EQ", "NEQ", 
  "LT", "LE", "GT", "GE", "PLUS", "MINUS", "TIMES", "DIVIDE", "LROUND", 
  "RROUND", "LSQUARE", "RSQUARE", "LCURLY", "RCURLY", "COMMA", "DOT", "ENDST"
};

dfa::Vocabulary CoCoDeLangParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> CoCoDeLangParser::_tokenNames;

CoCoDeLangParser::Initializer::Initializer() {
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
       0x3, 0x41, 0x155, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
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
       0x9, 0x32, 0x4, 0x33, 0x9, 0x33, 0x4, 0x34, 0x9, 0x34, 0x4, 0x35, 
       0x9, 0x35, 0x4, 0x36, 0x9, 0x36, 0x4, 0x37, 0x9, 0x37, 0x4, 0x38, 
       0x9, 0x38, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x3, 0x2, 0x5, 0x2, 0x75, 
       0xa, 0x2, 0x3, 0x3, 0x3, 0x3, 0x5, 0x3, 0x79, 0xa, 0x3, 0x3, 0x4, 
       0x3, 0x4, 0x5, 0x4, 0x7d, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 
       0x81, 0xa, 0x4, 0x3, 0x4, 0x5, 0x4, 0x84, 0xa, 0x4, 0x3, 0x5, 0x3, 
       0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x8b, 0xa, 0x5, 0x3, 
       0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x92, 0xa, 
       0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x98, 0xa, 
       0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 
       0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 
       0x9, 0x5, 0x9, 0xa7, 0xa, 0x9, 0x3, 0xa, 0x3, 0xa, 0x5, 0xa, 0xab, 
       0xa, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 
       0xb, 0x3, 0xb, 0x5, 0xb, 0xb4, 0xa, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 
       0xb, 0x3, 0xb, 0x3, 0xb, 0x5, 0xb, 0xbb, 0xa, 0xb, 0x3, 0xb, 0x3, 
       0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xc, 
       0x5, 0xc, 0xc5, 0xa, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 0x3, 0xd, 
       0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 
       0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 
       0x13, 0x3, 0x13, 0x3, 0x14, 0x5, 0x14, 0xda, 0xa, 0x14, 0x3, 0x14, 
       0x3, 0x14, 0x5, 0x14, 0xde, 0xa, 0x14, 0x3, 0x15, 0x3, 0x15, 0x3, 
       0x16, 0x3, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x18, 0x3, 0x18, 0x3, 
       0x19, 0x3, 0x19, 0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 
       0x1c, 0x3, 0x1c, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 
       0x1f, 0x3, 0x1f, 0x3, 0x20, 0x3, 0x20, 0x3, 0x21, 0x3, 0x21, 0x3, 
       0x22, 0x3, 0x22, 0x3, 0x23, 0x3, 0x23, 0x3, 0x24, 0x3, 0x24, 0x3, 
       0x25, 0x3, 0x25, 0x3, 0x26, 0x3, 0x26, 0x3, 0x27, 0x3, 0x27, 0x3, 
       0x28, 0x3, 0x28, 0x3, 0x29, 0x3, 0x29, 0x3, 0x2a, 0x3, 0x2a, 0x3, 
       0x2b, 0x3, 0x2b, 0x3, 0x2c, 0x3, 0x2c, 0x3, 0x2d, 0x3, 0x2d, 0x3, 
       0x2e, 0x3, 0x2e, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x2f, 0x3, 0x30, 0x3, 
       0x30, 0x3, 0x30, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 
       0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 
       0x31, 0x5, 0x31, 0x125, 0xa, 0x31, 0x3, 0x31, 0x3, 0x31, 0x3, 0x31, 
       0x3, 0x31, 0x7, 0x31, 0x12b, 0xa, 0x31, 0xc, 0x31, 0xe, 0x31, 0x12e, 
       0xb, 0x31, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x32, 0x3, 0x33, 
       0x3, 0x33, 0x3, 0x33, 0x3, 0x33, 0x3, 0x34, 0x3, 0x34, 0x3, 0x34, 
       0x3, 0x34, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x3, 0x35, 0x3, 0x36, 
       0x3, 0x36, 0x3, 0x36, 0x3, 0x36, 0x3, 0x37, 0x3, 0x37, 0x3, 0x37, 
       0x3, 0x37, 0x5, 0x37, 0x148, 0xa, 0x37, 0x3, 0x38, 0x3, 0x38, 0x3, 
       0x38, 0x3, 0x38, 0x7, 0x38, 0x14e, 0xa, 0x38, 0xc, 0x38, 0xe, 0x38, 
       0x151, 0xb, 0x38, 0x3, 0x38, 0x3, 0x38, 0x3, 0x38, 0x2, 0x3, 0x60, 
       0x39, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 
       0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 
       0x2e, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x3e, 0x40, 0x42, 
       0x44, 0x46, 0x48, 0x4a, 0x4c, 0x4e, 0x50, 0x52, 0x54, 0x56, 0x58, 
       0x5a, 0x5c, 0x5e, 0x60, 0x62, 0x64, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 
       0x2, 0x7, 0x4, 0x2, 0x24, 0x25, 0x27, 0x28, 0x3, 0x2, 0x2a, 0x2c, 
       0x3, 0x2, 0x2d, 0x2e, 0x3, 0x2, 0x2f, 0x34, 0x3, 0x2, 0x35, 0x38, 
       0x2, 0x13b, 0x2, 0x74, 0x3, 0x2, 0x2, 0x2, 0x4, 0x78, 0x3, 0x2, 0x2, 
       0x2, 0x6, 0x83, 0x3, 0x2, 0x2, 0x2, 0x8, 0x97, 0x3, 0x2, 0x2, 0x2, 
       0xa, 0x99, 0x3, 0x2, 0x2, 0x2, 0xc, 0x9d, 0x3, 0x2, 0x2, 0x2, 0xe, 
       0xa0, 0x3, 0x2, 0x2, 0x2, 0x10, 0xa6, 0x3, 0x2, 0x2, 0x2, 0x12, 0xaa, 
       0x3, 0x2, 0x2, 0x2, 0x14, 0xac, 0x3, 0x2, 0x2, 0x2, 0x16, 0xc4, 0x3, 
       0x2, 0x2, 0x2, 0x18, 0xc6, 0x3, 0x2, 0x2, 0x2, 0x1a, 0xcc, 0x3, 0x2, 
       0x2, 0x2, 0x1c, 0xce, 0x3, 0x2, 0x2, 0x2, 0x1e, 0xd0, 0x3, 0x2, 0x2, 
       0x2, 0x20, 0xd2, 0x3, 0x2, 0x2, 0x2, 0x22, 0xd4, 0x3, 0x2, 0x2, 0x2, 
       0x24, 0xd6, 0x3, 0x2, 0x2, 0x2, 0x26, 0xd9, 0x3, 0x2, 0x2, 0x2, 0x28, 
       0xdf, 0x3, 0x2, 0x2, 0x2, 0x2a, 0xe1, 0x3, 0x2, 0x2, 0x2, 0x2c, 0xe3, 
       0x3, 0x2, 0x2, 0x2, 0x2e, 0xe5, 0x3, 0x2, 0x2, 0x2, 0x30, 0xe7, 0x3, 
       0x2, 0x2, 0x2, 0x32, 0xe9, 0x3, 0x2, 0x2, 0x2, 0x34, 0xeb, 0x3, 0x2, 
       0x2, 0x2, 0x36, 0xed, 0x3, 0x2, 0x2, 0x2, 0x38, 0xef, 0x3, 0x2, 0x2, 
       0x2, 0x3a, 0xf1, 0x3, 0x2, 0x2, 0x2, 0x3c, 0xf3, 0x3, 0x2, 0x2, 0x2, 
       0x3e, 0xf5, 0x3, 0x2, 0x2, 0x2, 0x40, 0xf7, 0x3, 0x2, 0x2, 0x2, 0x42, 
       0xf9, 0x3, 0x2, 0x2, 0x2, 0x44, 0xfb, 0x3, 0x2, 0x2, 0x2, 0x46, 0xfd, 
       0x3, 0x2, 0x2, 0x2, 0x48, 0xff, 0x3, 0x2, 0x2, 0x2, 0x4a, 0x101, 
       0x3, 0x2, 0x2, 0x2, 0x4c, 0x103, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x105, 
       0x3, 0x2, 0x2, 0x2, 0x50, 0x107, 0x3, 0x2, 0x2, 0x2, 0x52, 0x109, 
       0x3, 0x2, 0x2, 0x2, 0x54, 0x10b, 0x3, 0x2, 0x2, 0x2, 0x56, 0x10d, 
       0x3, 0x2, 0x2, 0x2, 0x58, 0x10f, 0x3, 0x2, 0x2, 0x2, 0x5a, 0x111, 
       0x3, 0x2, 0x2, 0x2, 0x5c, 0x113, 0x3, 0x2, 0x2, 0x2, 0x5e, 0x116, 
       0x3, 0x2, 0x2, 0x2, 0x60, 0x124, 0x3, 0x2, 0x2, 0x2, 0x62, 0x12f, 
       0x3, 0x2, 0x2, 0x2, 0x64, 0x133, 0x3, 0x2, 0x2, 0x2, 0x66, 0x137, 
       0x3, 0x2, 0x2, 0x2, 0x68, 0x13b, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x13f, 
       0x3, 0x2, 0x2, 0x2, 0x6c, 0x147, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x149, 
       0x3, 0x2, 0x2, 0x2, 0x70, 0x75, 0x5, 0x28, 0x15, 0x2, 0x71, 0x75, 
       0x5, 0x2c, 0x17, 0x2, 0x72, 0x75, 0x5, 0x2a, 0x16, 0x2, 0x73, 0x75, 
       0x5, 0x2e, 0x18, 0x2, 0x74, 0x70, 0x3, 0x2, 0x2, 0x2, 0x74, 0x71, 
       0x3, 0x2, 0x2, 0x2, 0x74, 0x72, 0x3, 0x2, 0x2, 0x2, 0x74, 0x73, 0x3, 
       0x2, 0x2, 0x2, 0x75, 0x3, 0x3, 0x2, 0x2, 0x2, 0x76, 0x79, 0x5, 0x6c, 
       0x37, 0x2, 0x77, 0x79, 0x5, 0x6e, 0x38, 0x2, 0x78, 0x76, 0x3, 0x2, 
       0x2, 0x2, 0x78, 0x77, 0x3, 0x2, 0x2, 0x2, 0x79, 0x5, 0x3, 0x2, 0x2, 
       0x2, 0x7a, 0x7c, 0x5, 0x28, 0x15, 0x2, 0x7b, 0x7d, 0x5, 0x4, 0x3, 
       0x2, 0x7c, 0x7b, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7d, 0x3, 0x2, 0x2, 0x2, 
       0x7d, 0x84, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x80, 0x5, 0x2c, 0x17, 0x2, 
       0x7f, 0x81, 0x5, 0x4, 0x3, 0x2, 0x80, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x80, 
       0x81, 0x3, 0x2, 0x2, 0x2, 0x81, 0x84, 0x3, 0x2, 0x2, 0x2, 0x82, 0x84, 
       0x5, 0x2a, 0x16, 0x2, 0x83, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x83, 0x7e, 
       0x3, 0x2, 0x2, 0x2, 0x83, 0x82, 0x3, 0x2, 0x2, 0x2, 0x84, 0x7, 0x3, 
       0x2, 0x2, 0x2, 0x85, 0x86, 0x5, 0x54, 0x2b, 0x2, 0x86, 0x8a, 0x7, 
       0x39, 0x2, 0x2, 0x87, 0x88, 0x5, 0x56, 0x2c, 0x2, 0x88, 0x89, 0x7, 
       0x2f, 0x2, 0x2, 0x89, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x87, 0x3, 
       0x2, 0x2, 0x2, 0x8a, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x8c, 0x3, 0x2, 
       0x2, 0x2, 0x8c, 0x8d, 0x5, 0x60, 0x31, 0x2, 0x8d, 0x91, 0x7, 0x3f, 
       0x2, 0x2, 0x8e, 0x8f, 0x5, 0x58, 0x2d, 0x2, 0x8f, 0x90, 0x7, 0x2f, 
       0x2, 0x2, 0x90, 0x92, 0x3, 0x2, 0x2, 0x2, 0x91, 0x8e, 0x3, 0x2, 0x2, 
       0x2, 0x91, 0x92, 0x3, 0x2, 0x2, 0x2, 0x92, 0x93, 0x3, 0x2, 0x2, 0x2, 
       0x93, 0x94, 0x5, 0x60, 0x31, 0x2, 0x94, 0x95, 0x7, 0x3a, 0x2, 0x2, 
       0x95, 0x98, 0x3, 0x2, 0x2, 0x2, 0x96, 0x98, 0x5, 0x5a, 0x2e, 0x2, 
       0x97, 0x85, 0x3, 0x2, 0x2, 0x2, 0x97, 0x96, 0x3, 0x2, 0x2, 0x2, 0x98, 
       0x9, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9a, 0x5, 0x6, 0x4, 0x2, 0x9a, 0x9b, 
       0x7, 0x3f, 0x2, 0x2, 0x9b, 0x9c, 0x5, 0x8, 0x5, 0x2, 0x9c, 0xb, 0x3, 
       0x2, 0x2, 0x2, 0x9d, 0x9e, 0x5, 0x40, 0x21, 0x2, 0x9e, 0x9f, 0x7, 
       0x1e, 0x2, 0x2, 0x9f, 0xd, 0x3, 0x2, 0x2, 0x2, 0xa0, 0xa1, 0x5, 0xc, 
       0x7, 0x2, 0xa1, 0xa2, 0x5, 0x3c, 0x1f, 0x2, 0xa2, 0xa3, 0x5, 0xa, 
       0x6, 0x2, 0xa3, 0xf, 0x3, 0x2, 0x2, 0x2, 0xa4, 0xa7, 0x7, 0x1e, 0x2, 
       0x2, 0xa5, 0xa7, 0x5, 0xa, 0x6, 0x2, 0xa6, 0xa4, 0x3, 0x2, 0x2, 0x2, 
       0xa6, 0xa5, 0x3, 0x2, 0x2, 0x2, 0xa7, 0x11, 0x3, 0x2, 0x2, 0x2, 0xa8, 
       0xab, 0x5, 0x38, 0x1d, 0x2, 0xa9, 0xab, 0x5, 0x3a, 0x1e, 0x2, 0xaa, 
       0xa8, 0x3, 0x2, 0x2, 0x2, 0xaa, 0xa9, 0x3, 0x2, 0x2, 0x2, 0xab, 0x13, 
       0x3, 0x2, 0x2, 0x2, 0xac, 0xad, 0x5, 0x12, 0xa, 0x2, 0xad, 0xae, 
       0x5, 0x32, 0x1a, 0x2, 0xae, 0xaf, 0x7, 0x1e, 0x2, 0x2, 0xaf, 0xb0, 
       0x5, 0x3c, 0x1f, 0x2, 0xb0, 0xb1, 0x7, 0x29, 0x2, 0x2, 0xb1, 0xb3, 
       0x5, 0x2, 0x2, 0x2, 0xb2, 0xb4, 0x5, 0x4, 0x3, 0x2, 0xb3, 0xb2, 0x3, 
       0x2, 0x2, 0x2, 0xb3, 0xb4, 0x3, 0x2, 0x2, 0x2, 0xb4, 0xba, 0x3, 0x2, 
       0x2, 0x2, 0xb5, 0xb6, 0x7, 0x3f, 0x2, 0x2, 0xb6, 0xb7, 0x5, 0x3e, 
       0x20, 0x2, 0xb7, 0xb8, 0x7, 0x29, 0x2, 0x2, 0xb8, 0xb9, 0x5, 0x10, 
       0x9, 0x2, 0xb9, 0xbb, 0x3, 0x2, 0x2, 0x2, 0xba, 0xb5, 0x3, 0x2, 0x2, 
       0x2, 0xba, 0xbb, 0x3, 0x2, 0x2, 0x2, 0xbb, 0xbc, 0x3, 0x2, 0x2, 0x2, 
       0xbc, 0xbd, 0x7, 0x3f, 0x2, 0x2, 0xbd, 0xbe, 0x5, 0x36, 0x1c, 0x2, 
       0xbe, 0xbf, 0x7, 0x29, 0x2, 0x2, 0xbf, 0xc0, 0x5, 0x4, 0x3, 0x2, 
       0xc0, 0x15, 0x3, 0x2, 0x2, 0x2, 0xc1, 0xc5, 0x5, 0x26, 0x14, 0x2, 
       0xc2, 0xc5, 0x5, 0x4a, 0x26, 0x2, 0xc3, 0xc5, 0x5, 0x4c, 0x27, 0x2, 
       0xc4, 0xc1, 0x3, 0x2, 0x2, 0x2, 0xc4, 0xc2, 0x3, 0x2, 0x2, 0x2, 0xc4, 
       0xc3, 0x3, 0x2, 0x2, 0x2, 0xc5, 0x17, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc7, 
       0x5, 0x34, 0x1b, 0x2, 0xc7, 0xc8, 0x5, 0x2, 0x2, 0x2, 0xc8, 0xc9, 
       0x7, 0x1e, 0x2, 0x2, 0xc9, 0xca, 0x7, 0x29, 0x2, 0x2, 0xca, 0xcb, 
       0x5, 0x16, 0xc, 0x2, 0xcb, 0x19, 0x3, 0x2, 0x2, 0x2, 0xcc, 0xcd, 
       0x9, 0x2, 0x2, 0x2, 0xcd, 0x1b, 0x3, 0x2, 0x2, 0x2, 0xce, 0xcf, 0x7, 
       0x26, 0x2, 0x2, 0xcf, 0x1d, 0x3, 0x2, 0x2, 0x2, 0xd0, 0xd1, 0x9, 
       0x3, 0x2, 0x2, 0xd1, 0x1f, 0x3, 0x2, 0x2, 0x2, 0xd2, 0xd3, 0x9, 0x4, 
       0x2, 0x2, 0xd3, 0x21, 0x3, 0x2, 0x2, 0x2, 0xd4, 0xd5, 0x9, 0x5, 0x2, 
       0x2, 0xd5, 0x23, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd7, 0x9, 0x6, 0x2, 0x2, 
       0xd7, 0x25, 0x3, 0x2, 0x2, 0x2, 0xd8, 0xda, 0x7, 0x36, 0x2, 0x2, 
       0xd9, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xd9, 0xda, 0x3, 0x2, 0x2, 0x2, 0xda, 
       0xdb, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdd, 0x7, 0x1f, 0x2, 0x2, 0xdc, 
       0xde, 0x7, 0x20, 0x2, 0x2, 0xdd, 0xdc, 0x3, 0x2, 0x2, 0x2, 0xdd, 
       0xde, 0x3, 0x2, 0x2, 0x2, 0xde, 0x27, 0x3, 0x2, 0x2, 0x2, 0xdf, 0xe0, 
       0x7, 0x3, 0x2, 0x2, 0xe0, 0x29, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe2, 0x7, 
       0x4, 0x2, 0x2, 0xe2, 0x2b, 0x3, 0x2, 0x2, 0x2, 0xe3, 0xe4, 0x7, 0x5, 
       0x2, 0x2, 0xe4, 0x2d, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe6, 0x7, 0x6, 0x2, 
       0x2, 0xe6, 0x2f, 0x3, 0x2, 0x2, 0x2, 0xe7, 0xe8, 0x7, 0x7, 0x2, 0x2, 
       0xe8, 0x31, 0x3, 0x2, 0x2, 0x2, 0xe9, 0xea, 0x7, 0x8, 0x2, 0x2, 0xea, 
       0x33, 0x3, 0x2, 0x2, 0x2, 0xeb, 0xec, 0x7, 0x9, 0x2, 0x2, 0xec, 0x35, 
       0x3, 0x2, 0x2, 0x2, 0xed, 0xee, 0x7, 0xa, 0x2, 0x2, 0xee, 0x37, 0x3, 
       0x2, 0x2, 0x2, 0xef, 0xf0, 0x7, 0xb, 0x2, 0x2, 0xf0, 0x39, 0x3, 0x2, 
       0x2, 0x2, 0xf1, 0xf2, 0x7, 0xc, 0x2, 0x2, 0xf2, 0x3b, 0x3, 0x2, 0x2, 
       0x2, 0xf3, 0xf4, 0x7, 0xd, 0x2, 0x2, 0xf4, 0x3d, 0x3, 0x2, 0x2, 0x2, 
       0xf5, 0xf6, 0x7, 0xe, 0x2, 0x2, 0xf6, 0x3f, 0x3, 0x2, 0x2, 0x2, 0xf7, 
       0xf8, 0x7, 0xf, 0x2, 0x2, 0xf8, 0x41, 0x3, 0x2, 0x2, 0x2, 0xf9, 0xfa, 
       0x7, 0x10, 0x2, 0x2, 0xfa, 0x43, 0x3, 0x2, 0x2, 0x2, 0xfb, 0xfc, 
       0x7, 0x11, 0x2, 0x2, 0xfc, 0x45, 0x3, 0x2, 0x2, 0x2, 0xfd, 0xfe, 
       0x7, 0x12, 0x2, 0x2, 0xfe, 0x47, 0x3, 0x2, 0x2, 0x2, 0xff, 0x100, 
       0x7, 0x13, 0x2, 0x2, 0x100, 0x49, 0x3, 0x2, 0x2, 0x2, 0x101, 0x102, 
       0x7, 0x14, 0x2, 0x2, 0x102, 0x4b, 0x3, 0x2, 0x2, 0x2, 0x103, 0x104, 
       0x7, 0x15, 0x2, 0x2, 0x104, 0x4d, 0x3, 0x2, 0x2, 0x2, 0x105, 0x106, 
       0x7, 0x16, 0x2, 0x2, 0x106, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x107, 0x108, 
       0x7, 0x17, 0x2, 0x2, 0x108, 0x51, 0x3, 0x2, 0x2, 0x2, 0x109, 0x10a, 
       0x7, 0x18, 0x2, 0x2, 0x10a, 0x53, 0x3, 0x2, 0x2, 0x2, 0x10b, 0x10c, 
       0x7, 0x19, 0x2, 0x2, 0x10c, 0x55, 0x3, 0x2, 0x2, 0x2, 0x10d, 0x10e, 
       0x7, 0x1a, 0x2, 0x2, 0x10e, 0x57, 0x3, 0x2, 0x2, 0x2, 0x10f, 0x110, 
       0x7, 0x1b, 0x2, 0x2, 0x110, 0x59, 0x3, 0x2, 0x2, 0x2, 0x111, 0x112, 
       0x7, 0x1c, 0x2, 0x2, 0x112, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x113, 0x114, 
       0x7, 0x1e, 0x2, 0x2, 0x114, 0x115, 0x7, 0x1d, 0x2, 0x2, 0x115, 0x5d, 
       0x3, 0x2, 0x2, 0x2, 0x116, 0x117, 0x7, 0x36, 0x2, 0x2, 0x117, 0x118, 
       0x7, 0x1e, 0x2, 0x2, 0x118, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x119, 0x11a, 
       0x8, 0x31, 0x1, 0x2, 0x11a, 0x125, 0x5, 0x5c, 0x2f, 0x2, 0x11b, 0x125, 
       0x7, 0x1e, 0x2, 0x2, 0x11c, 0x125, 0x5, 0x5e, 0x30, 0x2, 0x11d, 0x125, 
       0x5, 0x26, 0x14, 0x2, 0x11e, 0x11f, 0x7, 0x39, 0x2, 0x2, 0x11f, 0x120, 
       0x5, 0x60, 0x31, 0x2, 0x120, 0x121, 0x7, 0x3a, 0x2, 0x2, 0x121, 0x125, 
       0x3, 0x2, 0x2, 0x2, 0x122, 0x125, 0x5, 0x4a, 0x26, 0x2, 0x123, 0x125, 
       0x5, 0x4c, 0x27, 0x2, 0x124, 0x119, 0x3, 0x2, 0x2, 0x2, 0x124, 0x11b, 
       0x3, 0x2, 0x2, 0x2, 0x124, 0x11c, 0x3, 0x2, 0x2, 0x2, 0x124, 0x11d, 
       0x3, 0x2, 0x2, 0x2, 0x124, 0x11e, 0x3, 0x2, 0x2, 0x2, 0x124, 0x122, 
       0x3, 0x2, 0x2, 0x2, 0x124, 0x123, 0x3, 0x2, 0x2, 0x2, 0x125, 0x12c, 
       0x3, 0x2, 0x2, 0x2, 0x126, 0x127, 0xc, 0xa, 0x2, 0x2, 0x127, 0x128, 
       0x5, 0x24, 0x13, 0x2, 0x128, 0x129, 0x5, 0x60, 0x31, 0xb, 0x129, 
       0x12b, 0x3, 0x2, 0x2, 0x2, 0x12a, 0x126, 0x3, 0x2, 0x2, 0x2, 0x12b, 
       0x12e, 0x3, 0x2, 0x2, 0x2, 0x12c, 0x12a, 0x3, 0x2, 0x2, 0x2, 0x12c, 
       0x12d, 0x3, 0x2, 0x2, 0x2, 0x12d, 0x61, 0x3, 0x2, 0x2, 0x2, 0x12e, 
       0x12c, 0x3, 0x2, 0x2, 0x2, 0x12f, 0x130, 0x5, 0x60, 0x31, 0x2, 0x130, 
       0x131, 0x7, 0x3f, 0x2, 0x2, 0x131, 0x132, 0x5, 0x60, 0x31, 0x2, 0x132, 
       0x63, 0x3, 0x2, 0x2, 0x2, 0x133, 0x134, 0x7, 0x39, 0x2, 0x2, 0x134, 
       0x135, 0x5, 0x62, 0x32, 0x2, 0x135, 0x136, 0x7, 0x3c, 0x2, 0x2, 0x136, 
       0x65, 0x3, 0x2, 0x2, 0x2, 0x137, 0x138, 0x7, 0x3b, 0x2, 0x2, 0x138, 
       0x139, 0x5, 0x62, 0x32, 0x2, 0x139, 0x13a, 0x7, 0x3a, 0x2, 0x2, 0x13a, 
       0x67, 0x3, 0x2, 0x2, 0x2, 0x13b, 0x13c, 0x7, 0x39, 0x2, 0x2, 0x13c, 
       0x13d, 0x5, 0x62, 0x32, 0x2, 0x13d, 0x13e, 0x7, 0x3a, 0x2, 0x2, 0x13e, 
       0x69, 0x3, 0x2, 0x2, 0x2, 0x13f, 0x140, 0x7, 0x3b, 0x2, 0x2, 0x140, 
       0x141, 0x5, 0x62, 0x32, 0x2, 0x141, 0x142, 0x7, 0x3c, 0x2, 0x2, 0x142, 
       0x6b, 0x3, 0x2, 0x2, 0x2, 0x143, 0x148, 0x5, 0x6a, 0x36, 0x2, 0x144, 
       0x148, 0x5, 0x68, 0x35, 0x2, 0x145, 0x148, 0x5, 0x64, 0x33, 0x2, 
       0x146, 0x148, 0x5, 0x66, 0x34, 0x2, 0x147, 0x143, 0x3, 0x2, 0x2, 
       0x2, 0x147, 0x144, 0x3, 0x2, 0x2, 0x2, 0x147, 0x145, 0x3, 0x2, 0x2, 
       0x2, 0x147, 0x146, 0x3, 0x2, 0x2, 0x2, 0x148, 0x6d, 0x3, 0x2, 0x2, 
       0x2, 0x149, 0x14a, 0x7, 0x3d, 0x2, 0x2, 0x14a, 0x14f, 0x5, 0x60, 
       0x31, 0x2, 0x14b, 0x14c, 0x7, 0x3f, 0x2, 0x2, 0x14c, 0x14e, 0x5, 
       0x60, 0x31, 0x2, 0x14d, 0x14b, 0x3, 0x2, 0x2, 0x2, 0x14e, 0x151, 
       0x3, 0x2, 0x2, 0x2, 0x14f, 0x14d, 0x3, 0x2, 0x2, 0x2, 0x14f, 0x150, 
       0x3, 0x2, 0x2, 0x2, 0x150, 0x152, 0x3, 0x2, 0x2, 0x2, 0x151, 0x14f, 
       0x3, 0x2, 0x2, 0x2, 0x152, 0x153, 0x7, 0x3e, 0x2, 0x2, 0x153, 0x6f, 
       0x3, 0x2, 0x2, 0x2, 0x15, 0x74, 0x78, 0x7c, 0x80, 0x83, 0x8a, 0x91, 
       0x97, 0xa6, 0xaa, 0xb3, 0xba, 0xc4, 0xd9, 0xdd, 0x124, 0x12c, 0x147, 
       0x14f, 
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

CoCoDeLangParser::Initializer CoCoDeLangParser::_init;
