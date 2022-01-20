
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/logistics/grammar\LogisticsLang.g4 by ANTLR 4.9.2


#include "LogisticsLangVisitor.h"

#include "LogisticsLangParser.h"


using namespace antlrcpp;
using namespace antlr4;

LogisticsLangParser::LogisticsLangParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

LogisticsLangParser::~LogisticsLangParser() {
  delete _interpreter;
}

std::string LogisticsLangParser::getGrammarFileName() const {
  return "LogisticsLang.g4";
}

const std::vector<std::string>& LogisticsLangParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& LogisticsLangParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- MapKwContext ------------------------------------------------------------------

LogisticsLangParser::MapKwContext::MapKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::MapKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleMapKw;
}


antlrcpp::Any LogisticsLangParser::MapKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitMapKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::MapKwContext* LogisticsLangParser::mapKw() {
  MapKwContext *_localctx = _tracker.createInstance<MapKwContext>(_ctx, getState());
  enterRule(_localctx, 0, LogisticsLangParser::RuleMapKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(32);
    match(LogisticsLangParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MapContext ------------------------------------------------------------------

LogisticsLangParser::MapContext::MapContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::MapKwContext* LogisticsLangParser::MapContext::mapKw() {
  return getRuleContext<LogisticsLangParser::MapKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::MapContext::LBRACKET() {
  return getToken(LogisticsLangParser::LBRACKET, 0);
}

tree::TerminalNode* LogisticsLangParser::MapContext::RBRACKET() {
  return getToken(LogisticsLangParser::RBRACKET, 0);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::MapContext::MAPLINE() {
  return getTokens(LogisticsLangParser::MAPLINE);
}

tree::TerminalNode* LogisticsLangParser::MapContext::MAPLINE(size_t i) {
  return getToken(LogisticsLangParser::MAPLINE, i);
}


size_t LogisticsLangParser::MapContext::getRuleIndex() const {
  return LogisticsLangParser::RuleMap;
}


antlrcpp::Any LogisticsLangParser::MapContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitMap(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::MapContext* LogisticsLangParser::map() {
  MapContext *_localctx = _tracker.createInstance<MapContext>(_ctx, getState());
  enterRule(_localctx, 2, LogisticsLangParser::RuleMap);
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
    setState(34);
    mapKw();
    setState(35);
    match(LogisticsLangParser::LBRACKET);
    setState(37); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(36);
      match(LogisticsLangParser::MAPLINE);
      setState(39); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LogisticsLangParser::MAPLINE);
    setState(41);
    match(LogisticsLangParser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProductsKwContext ------------------------------------------------------------------

LogisticsLangParser::ProductsKwContext::ProductsKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::ProductsKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleProductsKw;
}


antlrcpp::Any LogisticsLangParser::ProductsKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitProductsKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ProductsKwContext* LogisticsLangParser::productsKw() {
  ProductsKwContext *_localctx = _tracker.createInstance<ProductsKwContext>(_ctx, getState());
  enterRule(_localctx, 4, LogisticsLangParser::RuleProductsKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(43);
    match(LogisticsLangParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CoordxContext ------------------------------------------------------------------

LogisticsLangParser::CoordxContext::CoordxContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::CoordxContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}


size_t LogisticsLangParser::CoordxContext::getRuleIndex() const {
  return LogisticsLangParser::RuleCoordx;
}


antlrcpp::Any LogisticsLangParser::CoordxContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitCoordx(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::CoordxContext* LogisticsLangParser::coordx() {
  CoordxContext *_localctx = _tracker.createInstance<CoordxContext>(_ctx, getState());
  enterRule(_localctx, 6, LogisticsLangParser::RuleCoordx);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(45);
    match(LogisticsLangParser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CoordyContext ------------------------------------------------------------------

LogisticsLangParser::CoordyContext::CoordyContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::CoordyContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}


size_t LogisticsLangParser::CoordyContext::getRuleIndex() const {
  return LogisticsLangParser::RuleCoordy;
}


antlrcpp::Any LogisticsLangParser::CoordyContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitCoordy(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::CoordyContext* LogisticsLangParser::coordy() {
  CoordyContext *_localctx = _tracker.createInstance<CoordyContext>(_ctx, getState());
  enterRule(_localctx, 8, LogisticsLangParser::RuleCoordy);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(47);
    match(LogisticsLangParser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- UnitsContext ------------------------------------------------------------------

LogisticsLangParser::UnitsContext::UnitsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::UnitsContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}


size_t LogisticsLangParser::UnitsContext::getRuleIndex() const {
  return LogisticsLangParser::RuleUnits;
}


antlrcpp::Any LogisticsLangParser::UnitsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitUnits(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::UnitsContext* LogisticsLangParser::units() {
  UnitsContext *_localctx = _tracker.createInstance<UnitsContext>(_ctx, getState());
  enterRule(_localctx, 10, LogisticsLangParser::RuleUnits);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(49);
    match(LogisticsLangParser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TripleContext ------------------------------------------------------------------

LogisticsLangParser::TripleContext::TripleContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::TripleContext::LBRACKET() {
  return getToken(LogisticsLangParser::LBRACKET, 0);
}

LogisticsLangParser::CoordxContext* LogisticsLangParser::TripleContext::coordx() {
  return getRuleContext<LogisticsLangParser::CoordxContext>(0);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::TripleContext::COMMA() {
  return getTokens(LogisticsLangParser::COMMA);
}

tree::TerminalNode* LogisticsLangParser::TripleContext::COMMA(size_t i) {
  return getToken(LogisticsLangParser::COMMA, i);
}

LogisticsLangParser::CoordyContext* LogisticsLangParser::TripleContext::coordy() {
  return getRuleContext<LogisticsLangParser::CoordyContext>(0);
}

LogisticsLangParser::UnitsContext* LogisticsLangParser::TripleContext::units() {
  return getRuleContext<LogisticsLangParser::UnitsContext>(0);
}

tree::TerminalNode* LogisticsLangParser::TripleContext::RBRACKET() {
  return getToken(LogisticsLangParser::RBRACKET, 0);
}


size_t LogisticsLangParser::TripleContext::getRuleIndex() const {
  return LogisticsLangParser::RuleTriple;
}


antlrcpp::Any LogisticsLangParser::TripleContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitTriple(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::TripleContext* LogisticsLangParser::triple() {
  TripleContext *_localctx = _tracker.createInstance<TripleContext>(_ctx, getState());
  enterRule(_localctx, 12, LogisticsLangParser::RuleTriple);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(51);
    match(LogisticsLangParser::LBRACKET);
    setState(52);
    coordx();
    setState(53);
    match(LogisticsLangParser::COMMA);
    setState(54);
    coordy();
    setState(55);
    match(LogisticsLangParser::COMMA);
    setState(56);
    units();
    setState(57);
    match(LogisticsLangParser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProductContext ------------------------------------------------------------------

LogisticsLangParser::ProductContext::ProductContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::ProductContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}

tree::TerminalNode* LogisticsLangParser::ProductContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

std::vector<LogisticsLangParser::TripleContext *> LogisticsLangParser::ProductContext::triple() {
  return getRuleContexts<LogisticsLangParser::TripleContext>();
}

LogisticsLangParser::TripleContext* LogisticsLangParser::ProductContext::triple(size_t i) {
  return getRuleContext<LogisticsLangParser::TripleContext>(i);
}

tree::TerminalNode* LogisticsLangParser::ProductContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::ProductContext::COMMA() {
  return getTokens(LogisticsLangParser::COMMA);
}

tree::TerminalNode* LogisticsLangParser::ProductContext::COMMA(size_t i) {
  return getToken(LogisticsLangParser::COMMA, i);
}


size_t LogisticsLangParser::ProductContext::getRuleIndex() const {
  return LogisticsLangParser::RuleProduct;
}


antlrcpp::Any LogisticsLangParser::ProductContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitProduct(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ProductContext* LogisticsLangParser::product() {
  ProductContext *_localctx = _tracker.createInstance<ProductContext>(_ctx, getState());
  enterRule(_localctx, 14, LogisticsLangParser::RuleProduct);
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
    setState(59);
    match(LogisticsLangParser::ID);
    setState(60);
    match(LogisticsLangParser::COLON);
    setState(61);
    triple();
    setState(66);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LogisticsLangParser::COMMA) {
      setState(62);
      match(LogisticsLangParser::COMMA);
      setState(63);
      triple();
      setState(68);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(69);
    match(LogisticsLangParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProductsContext ------------------------------------------------------------------

LogisticsLangParser::ProductsContext::ProductsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::ProductsKwContext* LogisticsLangParser::ProductsContext::productsKw() {
  return getRuleContext<LogisticsLangParser::ProductsKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::ProductsContext::LBRACKET() {
  return getToken(LogisticsLangParser::LBRACKET, 0);
}

tree::TerminalNode* LogisticsLangParser::ProductsContext::RBRACKET() {
  return getToken(LogisticsLangParser::RBRACKET, 0);
}

std::vector<LogisticsLangParser::ProductContext *> LogisticsLangParser::ProductsContext::product() {
  return getRuleContexts<LogisticsLangParser::ProductContext>();
}

LogisticsLangParser::ProductContext* LogisticsLangParser::ProductsContext::product(size_t i) {
  return getRuleContext<LogisticsLangParser::ProductContext>(i);
}


size_t LogisticsLangParser::ProductsContext::getRuleIndex() const {
  return LogisticsLangParser::RuleProducts;
}


antlrcpp::Any LogisticsLangParser::ProductsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitProducts(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ProductsContext* LogisticsLangParser::products() {
  ProductsContext *_localctx = _tracker.createInstance<ProductsContext>(_ctx, getState());
  enterRule(_localctx, 16, LogisticsLangParser::RuleProducts);
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
    setState(71);
    productsKw();
    setState(72);
    match(LogisticsLangParser::LBRACKET);
    setState(74); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(73);
      product();
      setState(76); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LogisticsLangParser::ID);
    setState(78);
    match(LogisticsLangParser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- VariableKwContext ------------------------------------------------------------------

LogisticsLangParser::VariableKwContext::VariableKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::VariableKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleVariableKw;
}


antlrcpp::Any LogisticsLangParser::VariableKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitVariableKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::VariableKwContext* LogisticsLangParser::variableKw() {
  VariableKwContext *_localctx = _tracker.createInstance<VariableKwContext>(_ctx, getState());
  enterRule(_localctx, 18, LogisticsLangParser::RuleVariableKw);

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
    match(LogisticsLangParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DestinationKwContext ------------------------------------------------------------------

LogisticsLangParser::DestinationKwContext::DestinationKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::DestinationKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleDestinationKw;
}


antlrcpp::Any LogisticsLangParser::DestinationKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitDestinationKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::DestinationKwContext* LogisticsLangParser::destinationKw() {
  DestinationKwContext *_localctx = _tracker.createInstance<DestinationKwContext>(_ctx, getState());
  enterRule(_localctx, 20, LogisticsLangParser::RuleDestinationKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(82);
    match(LogisticsLangParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- TimeContext ------------------------------------------------------------------

LogisticsLangParser::TimeContext::TimeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::TimeContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}

LogisticsLangParser::VariableKwContext* LogisticsLangParser::TimeContext::variableKw() {
  return getRuleContext<LogisticsLangParser::VariableKwContext>(0);
}


size_t LogisticsLangParser::TimeContext::getRuleIndex() const {
  return LogisticsLangParser::RuleTime;
}


antlrcpp::Any LogisticsLangParser::TimeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitTime(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::TimeContext* LogisticsLangParser::time() {
  TimeContext *_localctx = _tracker.createInstance<TimeContext>(_ctx, getState());
  enterRule(_localctx, 22, LogisticsLangParser::RuleTime);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(86);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LogisticsLangParser::NUMBER: {
        enterOuterAlt(_localctx, 1);
        setState(84);
        match(LogisticsLangParser::NUMBER);
        break;
      }

      case LogisticsLangParser::T__2: {
        enterOuterAlt(_localctx, 2);
        setState(85);
        variableKw();
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

//----------------- LocationContext ------------------------------------------------------------------

LogisticsLangParser::LocationContext::LocationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::LocationContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}


size_t LogisticsLangParser::LocationContext::getRuleIndex() const {
  return LogisticsLangParser::RuleLocation;
}


antlrcpp::Any LogisticsLangParser::LocationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitLocation(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::LocationContext* LogisticsLangParser::location() {
  LocationContext *_localctx = _tracker.createInstance<LocationContext>(_ctx, getState());
  enterRule(_localctx, 24, LogisticsLangParser::RuleLocation);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(88);
    match(LogisticsLangParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RequestContext ------------------------------------------------------------------

LogisticsLangParser::RequestContext::RequestContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::RequestContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}

tree::TerminalNode* LogisticsLangParser::RequestContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

tree::TerminalNode* LogisticsLangParser::RequestContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}


size_t LogisticsLangParser::RequestContext::getRuleIndex() const {
  return LogisticsLangParser::RuleRequest;
}


antlrcpp::Any LogisticsLangParser::RequestContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitRequest(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::RequestContext* LogisticsLangParser::request() {
  RequestContext *_localctx = _tracker.createInstance<RequestContext>(_ctx, getState());
  enterRule(_localctx, 26, LogisticsLangParser::RuleRequest);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(90);
    match(LogisticsLangParser::ID);
    setState(91);
    match(LogisticsLangParser::COLON);
    setState(92);
    match(LogisticsLangParser::NUMBER);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DestinationContext ------------------------------------------------------------------

LogisticsLangParser::DestinationContext::DestinationContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::DestinationKwContext* LogisticsLangParser::DestinationContext::destinationKw() {
  return getRuleContext<LogisticsLangParser::DestinationKwContext>(0);
}

LogisticsLangParser::LocationContext* LogisticsLangParser::DestinationContext::location() {
  return getRuleContext<LogisticsLangParser::LocationContext>(0);
}

LogisticsLangParser::TimeContext* LogisticsLangParser::DestinationContext::time() {
  return getRuleContext<LogisticsLangParser::TimeContext>(0);
}

tree::TerminalNode* LogisticsLangParser::DestinationContext::LBRACKET() {
  return getToken(LogisticsLangParser::LBRACKET, 0);
}

std::vector<LogisticsLangParser::RequestContext *> LogisticsLangParser::DestinationContext::request() {
  return getRuleContexts<LogisticsLangParser::RequestContext>();
}

LogisticsLangParser::RequestContext* LogisticsLangParser::DestinationContext::request(size_t i) {
  return getRuleContext<LogisticsLangParser::RequestContext>(i);
}

tree::TerminalNode* LogisticsLangParser::DestinationContext::RBRACKET() {
  return getToken(LogisticsLangParser::RBRACKET, 0);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::DestinationContext::COMMA() {
  return getTokens(LogisticsLangParser::COMMA);
}

tree::TerminalNode* LogisticsLangParser::DestinationContext::COMMA(size_t i) {
  return getToken(LogisticsLangParser::COMMA, i);
}


size_t LogisticsLangParser::DestinationContext::getRuleIndex() const {
  return LogisticsLangParser::RuleDestination;
}


antlrcpp::Any LogisticsLangParser::DestinationContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitDestination(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::DestinationContext* LogisticsLangParser::destination() {
  DestinationContext *_localctx = _tracker.createInstance<DestinationContext>(_ctx, getState());
  enterRule(_localctx, 28, LogisticsLangParser::RuleDestination);
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
    setState(94);
    destinationKw();
    setState(95);
    location();
    setState(96);
    time();
    setState(97);
    match(LogisticsLangParser::LBRACKET);
    setState(98);
    request();
    setState(103);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LogisticsLangParser::COMMA) {
      setState(99);
      match(LogisticsLangParser::COMMA);
      setState(100);
      request();
      setState(105);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(106);
    match(LogisticsLangParser::RBRACKET);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SpecContext ------------------------------------------------------------------

LogisticsLangParser::SpecContext::SpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::MapContext* LogisticsLangParser::SpecContext::map() {
  return getRuleContext<LogisticsLangParser::MapContext>(0);
}

LogisticsLangParser::ProductsContext* LogisticsLangParser::SpecContext::products() {
  return getRuleContext<LogisticsLangParser::ProductsContext>(0);
}

std::vector<LogisticsLangParser::DestinationContext *> LogisticsLangParser::SpecContext::destination() {
  return getRuleContexts<LogisticsLangParser::DestinationContext>();
}

LogisticsLangParser::DestinationContext* LogisticsLangParser::SpecContext::destination(size_t i) {
  return getRuleContext<LogisticsLangParser::DestinationContext>(i);
}


size_t LogisticsLangParser::SpecContext::getRuleIndex() const {
  return LogisticsLangParser::RuleSpec;
}


antlrcpp::Any LogisticsLangParser::SpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitSpec(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::SpecContext* LogisticsLangParser::spec() {
  SpecContext *_localctx = _tracker.createInstance<SpecContext>(_ctx, getState());
  enterRule(_localctx, 30, LogisticsLangParser::RuleSpec);
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
    map();
    setState(109);
    products();
    setState(111); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(110);
      destination();
      setState(113); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LogisticsLangParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

// Static vars and initialization.
std::vector<dfa::DFA> LogisticsLangParser::_decisionToDFA;
atn::PredictionContextCache LogisticsLangParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN LogisticsLangParser::_atn;
std::vector<uint16_t> LogisticsLangParser::_serializedATN;

std::vector<std::string> LogisticsLangParser::_ruleNames = {
  "mapKw", "map", "productsKw", "coordx", "coordy", "units", "triple", "product", 
  "products", "variableKw", "destinationKw", "time", "location", "request", 
  "destination", "spec"
};

std::vector<std::string> LogisticsLangParser::_literalNames = {
  "", "'Map'", "'Products'", "'variable'", "'destination'", "", "", "':'", 
  "';'", "','", "'.'", "'('", "')'"
};

std::vector<std::string> LogisticsLangParser::_symbolicNames = {
  "", "", "", "", "", "WS", "LINE_COMMENT", "COLON", "ENDST", "COMMA", "DOT", 
  "LBRACKET", "RBRACKET", "MAPCHAR", "MAPLINE", "DECIMAL", "NUMBER", "ID"
};

dfa::Vocabulary LogisticsLangParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> LogisticsLangParser::_tokenNames;

LogisticsLangParser::Initializer::Initializer() {
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
       0x3, 0x13, 0x76, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x3, 0x2, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x6, 
       0x3, 0x28, 0xa, 0x3, 0xd, 0x3, 0xe, 0x3, 0x29, 0x3, 0x3, 0x3, 0x3, 
       0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 
       0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 
       0x3, 0x8, 0x3, 0x8, 0x3, 0x8, 0x3, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 
       0x9, 0x3, 0x9, 0x7, 0x9, 0x43, 0xa, 0x9, 0xc, 0x9, 0xe, 0x9, 0x46, 
       0xb, 0x9, 0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xa, 0x6, 
       0xa, 0x4d, 0xa, 0xa, 0xd, 0xa, 0xe, 0xa, 0x4e, 0x3, 0xa, 0x3, 0xa, 
       0x3, 0xb, 0x3, 0xb, 0x3, 0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x5, 
       0xd, 0x59, 0xa, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 
       0xf, 0x3, 0xf, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
       0x3, 0x10, 0x3, 0x10, 0x7, 0x10, 0x68, 0xa, 0x10, 0xc, 0x10, 0xe, 
       0x10, 0x6b, 0xb, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x6, 0x11, 0x72, 0xa, 0x11, 0xd, 0x11, 0xe, 0x11, 0x73, 
       0x3, 0x11, 0x2, 0x2, 0x12, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 
       0x12, 0x14, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x2, 0x2, 0x2, 0x6b, 
       0x2, 0x22, 0x3, 0x2, 0x2, 0x2, 0x4, 0x24, 0x3, 0x2, 0x2, 0x2, 0x6, 
       0x2d, 0x3, 0x2, 0x2, 0x2, 0x8, 0x2f, 0x3, 0x2, 0x2, 0x2, 0xa, 0x31, 
       0x3, 0x2, 0x2, 0x2, 0xc, 0x33, 0x3, 0x2, 0x2, 0x2, 0xe, 0x35, 0x3, 
       0x2, 0x2, 0x2, 0x10, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x12, 0x49, 0x3, 0x2, 
       0x2, 0x2, 0x14, 0x52, 0x3, 0x2, 0x2, 0x2, 0x16, 0x54, 0x3, 0x2, 0x2, 
       0x2, 0x18, 0x58, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x5a, 0x3, 0x2, 0x2, 0x2, 
       0x1c, 0x5c, 0x3, 0x2, 0x2, 0x2, 0x1e, 0x60, 0x3, 0x2, 0x2, 0x2, 0x20, 
       0x6e, 0x3, 0x2, 0x2, 0x2, 0x22, 0x23, 0x7, 0x3, 0x2, 0x2, 0x23, 0x3, 
       0x3, 0x2, 0x2, 0x2, 0x24, 0x25, 0x5, 0x2, 0x2, 0x2, 0x25, 0x27, 0x7, 
       0xd, 0x2, 0x2, 0x26, 0x28, 0x7, 0x10, 0x2, 0x2, 0x27, 0x26, 0x3, 
       0x2, 0x2, 0x2, 0x28, 0x29, 0x3, 0x2, 0x2, 0x2, 0x29, 0x27, 0x3, 0x2, 
       0x2, 0x2, 0x29, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2b, 0x3, 0x2, 0x2, 
       0x2, 0x2b, 0x2c, 0x7, 0xe, 0x2, 0x2, 0x2c, 0x5, 0x3, 0x2, 0x2, 0x2, 
       0x2d, 0x2e, 0x7, 0x4, 0x2, 0x2, 0x2e, 0x7, 0x3, 0x2, 0x2, 0x2, 0x2f, 
       0x30, 0x7, 0x12, 0x2, 0x2, 0x30, 0x9, 0x3, 0x2, 0x2, 0x2, 0x31, 0x32, 
       0x7, 0x12, 0x2, 0x2, 0x32, 0xb, 0x3, 0x2, 0x2, 0x2, 0x33, 0x34, 0x7, 
       0x12, 0x2, 0x2, 0x34, 0xd, 0x3, 0x2, 0x2, 0x2, 0x35, 0x36, 0x7, 0xd, 
       0x2, 0x2, 0x36, 0x37, 0x5, 0x8, 0x5, 0x2, 0x37, 0x38, 0x7, 0xb, 0x2, 
       0x2, 0x38, 0x39, 0x5, 0xa, 0x6, 0x2, 0x39, 0x3a, 0x7, 0xb, 0x2, 0x2, 
       0x3a, 0x3b, 0x5, 0xc, 0x7, 0x2, 0x3b, 0x3c, 0x7, 0xe, 0x2, 0x2, 0x3c, 
       0xf, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x7, 0x13, 0x2, 0x2, 0x3e, 0x3f, 
       0x7, 0x9, 0x2, 0x2, 0x3f, 0x44, 0x5, 0xe, 0x8, 0x2, 0x40, 0x41, 0x7, 
       0xb, 0x2, 0x2, 0x41, 0x43, 0x5, 0xe, 0x8, 0x2, 0x42, 0x40, 0x3, 0x2, 
       0x2, 0x2, 0x43, 0x46, 0x3, 0x2, 0x2, 0x2, 0x44, 0x42, 0x3, 0x2, 0x2, 
       0x2, 0x44, 0x45, 0x3, 0x2, 0x2, 0x2, 0x45, 0x47, 0x3, 0x2, 0x2, 0x2, 
       0x46, 0x44, 0x3, 0x2, 0x2, 0x2, 0x47, 0x48, 0x7, 0xa, 0x2, 0x2, 0x48, 
       0x11, 0x3, 0x2, 0x2, 0x2, 0x49, 0x4a, 0x5, 0x6, 0x4, 0x2, 0x4a, 0x4c, 
       0x7, 0xd, 0x2, 0x2, 0x4b, 0x4d, 0x5, 0x10, 0x9, 0x2, 0x4c, 0x4b, 
       0x3, 0x2, 0x2, 0x2, 0x4d, 0x4e, 0x3, 0x2, 0x2, 0x2, 0x4e, 0x4c, 0x3, 
       0x2, 0x2, 0x2, 0x4e, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x50, 0x3, 0x2, 
       0x2, 0x2, 0x50, 0x51, 0x7, 0xe, 0x2, 0x2, 0x51, 0x13, 0x3, 0x2, 0x2, 
       0x2, 0x52, 0x53, 0x7, 0x5, 0x2, 0x2, 0x53, 0x15, 0x3, 0x2, 0x2, 0x2, 
       0x54, 0x55, 0x7, 0x6, 0x2, 0x2, 0x55, 0x17, 0x3, 0x2, 0x2, 0x2, 0x56, 
       0x59, 0x7, 0x12, 0x2, 0x2, 0x57, 0x59, 0x5, 0x14, 0xb, 0x2, 0x58, 
       0x56, 0x3, 0x2, 0x2, 0x2, 0x58, 0x57, 0x3, 0x2, 0x2, 0x2, 0x59, 0x19, 
       0x3, 0x2, 0x2, 0x2, 0x5a, 0x5b, 0x7, 0x13, 0x2, 0x2, 0x5b, 0x1b, 
       0x3, 0x2, 0x2, 0x2, 0x5c, 0x5d, 0x7, 0x13, 0x2, 0x2, 0x5d, 0x5e, 
       0x7, 0x9, 0x2, 0x2, 0x5e, 0x5f, 0x7, 0x12, 0x2, 0x2, 0x5f, 0x1d, 
       0x3, 0x2, 0x2, 0x2, 0x60, 0x61, 0x5, 0x16, 0xc, 0x2, 0x61, 0x62, 
       0x5, 0x1a, 0xe, 0x2, 0x62, 0x63, 0x5, 0x18, 0xd, 0x2, 0x63, 0x64, 
       0x7, 0xd, 0x2, 0x2, 0x64, 0x69, 0x5, 0x1c, 0xf, 0x2, 0x65, 0x66, 
       0x7, 0xb, 0x2, 0x2, 0x66, 0x68, 0x5, 0x1c, 0xf, 0x2, 0x67, 0x65, 
       0x3, 0x2, 0x2, 0x2, 0x68, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x69, 0x67, 0x3, 
       0x2, 0x2, 0x2, 0x69, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x6a, 0x6c, 0x3, 0x2, 
       0x2, 0x2, 0x6b, 0x69, 0x3, 0x2, 0x2, 0x2, 0x6c, 0x6d, 0x7, 0xe, 0x2, 
       0x2, 0x6d, 0x1f, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6f, 0x5, 0x4, 0x3, 0x2, 
       0x6f, 0x71, 0x5, 0x12, 0xa, 0x2, 0x70, 0x72, 0x5, 0x1e, 0x10, 0x2, 
       0x71, 0x70, 0x3, 0x2, 0x2, 0x2, 0x72, 0x73, 0x3, 0x2, 0x2, 0x2, 0x73, 
       0x71, 0x3, 0x2, 0x2, 0x2, 0x73, 0x74, 0x3, 0x2, 0x2, 0x2, 0x74, 0x21, 
       0x3, 0x2, 0x2, 0x2, 0x8, 0x29, 0x44, 0x4e, 0x58, 0x69, 0x73, 
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

LogisticsLangParser::Initializer LogisticsLangParser::_init;
