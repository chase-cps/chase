
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


//----------------- WidgetsKwContext ------------------------------------------------------------------

LogisticsLangParser::WidgetsKwContext::WidgetsKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::WidgetsKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleWidgetsKw;
}


antlrcpp::Any LogisticsLangParser::WidgetsKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitWidgetsKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::WidgetsKwContext* LogisticsLangParser::widgetsKw() {
  WidgetsKwContext *_localctx = _tracker.createInstance<WidgetsKwContext>(_ctx, getState());
  enterRule(_localctx, 0, LogisticsLangParser::RuleWidgetsKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(60);
    match(LogisticsLangParser::T__0);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComponentsKwContext ------------------------------------------------------------------

LogisticsLangParser::ComponentsKwContext::ComponentsKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::ComponentsKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleComponentsKw;
}


antlrcpp::Any LogisticsLangParser::ComponentsKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitComponentsKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ComponentsKwContext* LogisticsLangParser::componentsKw() {
  ComponentsKwContext *_localctx = _tracker.createInstance<ComponentsKwContext>(_ctx, getState());
  enterRule(_localctx, 2, LogisticsLangParser::RuleComponentsKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(62);
    match(LogisticsLangParser::T__1);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CrossroadsKwContext ------------------------------------------------------------------

LogisticsLangParser::CrossroadsKwContext::CrossroadsKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::CrossroadsKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleCrossroadsKw;
}


antlrcpp::Any LogisticsLangParser::CrossroadsKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitCrossroadsKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::CrossroadsKwContext* LogisticsLangParser::crossroadsKw() {
  CrossroadsKwContext *_localctx = _tracker.createInstance<CrossroadsKwContext>(_ctx, getState());
  enterRule(_localctx, 4, LogisticsLangParser::RuleCrossroadsKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(64);
    match(LogisticsLangParser::T__2);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArchitectureKwContext ------------------------------------------------------------------

LogisticsLangParser::ArchitectureKwContext::ArchitectureKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::ArchitectureKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleArchitectureKw;
}


antlrcpp::Any LogisticsLangParser::ArchitectureKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitArchitectureKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ArchitectureKwContext* LogisticsLangParser::architectureKw() {
  ArchitectureKwContext *_localctx = _tracker.createInstance<ArchitectureKwContext>(_ctx, getState());
  enterRule(_localctx, 6, LogisticsLangParser::RuleArchitectureKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(66);
    match(LogisticsLangParser::T__3);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- IsKwContext ------------------------------------------------------------------

LogisticsLangParser::IsKwContext::IsKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::IsKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleIsKw;
}


antlrcpp::Any LogisticsLangParser::IsKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitIsKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::IsKwContext* LogisticsLangParser::isKw() {
  IsKwContext *_localctx = _tracker.createInstance<IsKwContext>(_ctx, getState());
  enterRule(_localctx, 8, LogisticsLangParser::RuleIsKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(68);
    match(LogisticsLangParser::T__4);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RequirementsKwContext ------------------------------------------------------------------

LogisticsLangParser::RequirementsKwContext::RequirementsKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::RequirementsKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleRequirementsKw;
}


antlrcpp::Any LogisticsLangParser::RequirementsKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitRequirementsKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::RequirementsKwContext* LogisticsLangParser::requirementsKw() {
  RequirementsKwContext *_localctx = _tracker.createInstance<RequirementsKwContext>(_ctx, getState());
  enterRule(_localctx, 10, LogisticsLangParser::RuleRequirementsKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(70);
    match(LogisticsLangParser::T__5);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ProducesKwContext ------------------------------------------------------------------

LogisticsLangParser::ProducesKwContext::ProducesKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::ProducesKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleProducesKw;
}


antlrcpp::Any LogisticsLangParser::ProducesKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitProducesKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ProducesKwContext* LogisticsLangParser::producesKw() {
  ProducesKwContext *_localctx = _tracker.createInstance<ProducesKwContext>(_ctx, getState());
  enterRule(_localctx, 12, LogisticsLangParser::RuleProducesKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(72);
    match(LogisticsLangParser::T__6);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RequiresKwContext ------------------------------------------------------------------

LogisticsLangParser::RequiresKwContext::RequiresKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::RequiresKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleRequiresKw;
}


antlrcpp::Any LogisticsLangParser::RequiresKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitRequiresKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::RequiresKwContext* LogisticsLangParser::requiresKw() {
  RequiresKwContext *_localctx = _tracker.createInstance<RequiresKwContext>(_ctx, getState());
  enterRule(_localctx, 14, LogisticsLangParser::RuleRequiresKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(74);
    match(LogisticsLangParser::T__7);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BinKwContext ------------------------------------------------------------------

LogisticsLangParser::BinKwContext::BinKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::BinKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleBinKw;
}


antlrcpp::Any LogisticsLangParser::BinKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitBinKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::BinKwContext* LogisticsLangParser::binKw() {
  BinKwContext *_localctx = _tracker.createInstance<BinKwContext>(_ctx, getState());
  enterRule(_localctx, 16, LogisticsLangParser::RuleBinKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(76);
    match(LogisticsLangParser::T__8);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- MachineKwContext ------------------------------------------------------------------

LogisticsLangParser::MachineKwContext::MachineKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::MachineKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleMachineKw;
}


antlrcpp::Any LogisticsLangParser::MachineKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitMachineKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::MachineKwContext* LogisticsLangParser::machineKw() {
  MachineKwContext *_localctx = _tracker.createInstance<MachineKwContext>(_ctx, getState());
  enterRule(_localctx, 18, LogisticsLangParser::RuleMachineKw);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(78);
    match(LogisticsLangParser::T__9);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SinkKwContext ------------------------------------------------------------------

LogisticsLangParser::SinkKwContext::SinkKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::SinkKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleSinkKw;
}


antlrcpp::Any LogisticsLangParser::SinkKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitSinkKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::SinkKwContext* LogisticsLangParser::sinkKw() {
  SinkKwContext *_localctx = _tracker.createInstance<SinkKwContext>(_ctx, getState());
  enterRule(_localctx, 20, LogisticsLangParser::RuleSinkKw);

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
    match(LogisticsLangParser::T__10);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- NothingKwContext ------------------------------------------------------------------

LogisticsLangParser::NothingKwContext::NothingKwContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t LogisticsLangParser::NothingKwContext::getRuleIndex() const {
  return LogisticsLangParser::RuleNothingKw;
}


antlrcpp::Any LogisticsLangParser::NothingKwContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitNothingKw(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::NothingKwContext* LogisticsLangParser::nothingKw() {
  NothingKwContext *_localctx = _tracker.createInstance<NothingKwContext>(_ctx, getState());
  enterRule(_localctx, 22, LogisticsLangParser::RuleNothingKw);

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
    match(LogisticsLangParser::T__11);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- WidgetsContext ------------------------------------------------------------------

LogisticsLangParser::WidgetsContext::WidgetsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::WidgetsKwContext* LogisticsLangParser::WidgetsContext::widgetsKw() {
  return getRuleContext<LogisticsLangParser::WidgetsKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::WidgetsContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::WidgetsContext::ID() {
  return getTokens(LogisticsLangParser::ID);
}

tree::TerminalNode* LogisticsLangParser::WidgetsContext::ID(size_t i) {
  return getToken(LogisticsLangParser::ID, i);
}

tree::TerminalNode* LogisticsLangParser::WidgetsContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::WidgetsContext::COMMA() {
  return getTokens(LogisticsLangParser::COMMA);
}

tree::TerminalNode* LogisticsLangParser::WidgetsContext::COMMA(size_t i) {
  return getToken(LogisticsLangParser::COMMA, i);
}


size_t LogisticsLangParser::WidgetsContext::getRuleIndex() const {
  return LogisticsLangParser::RuleWidgets;
}


antlrcpp::Any LogisticsLangParser::WidgetsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitWidgets(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::WidgetsContext* LogisticsLangParser::widgets() {
  WidgetsContext *_localctx = _tracker.createInstance<WidgetsContext>(_ctx, getState());
  enterRule(_localctx, 24, LogisticsLangParser::RuleWidgets);
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
    setState(84);
    widgetsKw();
    setState(85);
    match(LogisticsLangParser::COLON);
    setState(86);
    match(LogisticsLangParser::ID);
    setState(91);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LogisticsLangParser::COMMA) {
      setState(87);
      match(LogisticsLangParser::COMMA);
      setState(88);
      match(LogisticsLangParser::ID);
      setState(93);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(94);
    match(LogisticsLangParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ItemContext ------------------------------------------------------------------

LogisticsLangParser::ItemContext::ItemContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::ItemContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}

tree::TerminalNode* LogisticsLangParser::ItemContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}


size_t LogisticsLangParser::ItemContext::getRuleIndex() const {
  return LogisticsLangParser::RuleItem;
}


antlrcpp::Any LogisticsLangParser::ItemContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitItem(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ItemContext* LogisticsLangParser::item() {
  ItemContext *_localctx = _tracker.createInstance<ItemContext>(_ctx, getState());
  enterRule(_localctx, 26, LogisticsLangParser::RuleItem);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(96);
    match(LogisticsLangParser::NUMBER);
    setState(97);
    match(LogisticsLangParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- BinContext ------------------------------------------------------------------

LogisticsLangParser::BinContext::BinContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::BinContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}

LogisticsLangParser::IsKwContext* LogisticsLangParser::BinContext::isKw() {
  return getRuleContext<LogisticsLangParser::IsKwContext>(0);
}

LogisticsLangParser::BinKwContext* LogisticsLangParser::BinContext::binKw() {
  return getRuleContext<LogisticsLangParser::BinKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::BinContext::LBRACKET() {
  return getToken(LogisticsLangParser::LBRACKET, 0);
}

std::vector<LogisticsLangParser::ItemContext *> LogisticsLangParser::BinContext::item() {
  return getRuleContexts<LogisticsLangParser::ItemContext>();
}

LogisticsLangParser::ItemContext* LogisticsLangParser::BinContext::item(size_t i) {
  return getRuleContext<LogisticsLangParser::ItemContext>(i);
}

tree::TerminalNode* LogisticsLangParser::BinContext::RBRACKET() {
  return getToken(LogisticsLangParser::RBRACKET, 0);
}

tree::TerminalNode* LogisticsLangParser::BinContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::BinContext::COMMA() {
  return getTokens(LogisticsLangParser::COMMA);
}

tree::TerminalNode* LogisticsLangParser::BinContext::COMMA(size_t i) {
  return getToken(LogisticsLangParser::COMMA, i);
}


size_t LogisticsLangParser::BinContext::getRuleIndex() const {
  return LogisticsLangParser::RuleBin;
}


antlrcpp::Any LogisticsLangParser::BinContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitBin(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::BinContext* LogisticsLangParser::bin() {
  BinContext *_localctx = _tracker.createInstance<BinContext>(_ctx, getState());
  enterRule(_localctx, 28, LogisticsLangParser::RuleBin);
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
    setState(99);
    match(LogisticsLangParser::ID);
    setState(100);
    isKw();
    setState(101);
    binKw();
    setState(102);
    match(LogisticsLangParser::LBRACKET);
    setState(103);
    item();
    setState(108);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LogisticsLangParser::COMMA) {
      setState(104);
      match(LogisticsLangParser::COMMA);
      setState(105);
      item();
      setState(110);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(111);
    match(LogisticsLangParser::RBRACKET);
    setState(112);
    match(LogisticsLangParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- SinkContext ------------------------------------------------------------------

LogisticsLangParser::SinkContext::SinkContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::SinkContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}

LogisticsLangParser::IsKwContext* LogisticsLangParser::SinkContext::isKw() {
  return getRuleContext<LogisticsLangParser::IsKwContext>(0);
}

LogisticsLangParser::SinkKwContext* LogisticsLangParser::SinkContext::sinkKw() {
  return getRuleContext<LogisticsLangParser::SinkKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::SinkContext::LBRACKET() {
  return getToken(LogisticsLangParser::LBRACKET, 0);
}

std::vector<LogisticsLangParser::ItemContext *> LogisticsLangParser::SinkContext::item() {
  return getRuleContexts<LogisticsLangParser::ItemContext>();
}

LogisticsLangParser::ItemContext* LogisticsLangParser::SinkContext::item(size_t i) {
  return getRuleContext<LogisticsLangParser::ItemContext>(i);
}

tree::TerminalNode* LogisticsLangParser::SinkContext::RBRACKET() {
  return getToken(LogisticsLangParser::RBRACKET, 0);
}

tree::TerminalNode* LogisticsLangParser::SinkContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::SinkContext::COMMA() {
  return getTokens(LogisticsLangParser::COMMA);
}

tree::TerminalNode* LogisticsLangParser::SinkContext::COMMA(size_t i) {
  return getToken(LogisticsLangParser::COMMA, i);
}


size_t LogisticsLangParser::SinkContext::getRuleIndex() const {
  return LogisticsLangParser::RuleSink;
}


antlrcpp::Any LogisticsLangParser::SinkContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitSink(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::SinkContext* LogisticsLangParser::sink() {
  SinkContext *_localctx = _tracker.createInstance<SinkContext>(_ctx, getState());
  enterRule(_localctx, 30, LogisticsLangParser::RuleSink);
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
    setState(114);
    match(LogisticsLangParser::ID);
    setState(115);
    isKw();
    setState(116);
    sinkKw();
    setState(117);
    match(LogisticsLangParser::LBRACKET);
    setState(118);
    item();
    setState(123);
    _errHandler->sync(this);
    _la = _input->LA(1);
    while (_la == LogisticsLangParser::COMMA) {
      setState(119);
      match(LogisticsLangParser::COMMA);
      setState(120);
      item();
      setState(125);
      _errHandler->sync(this);
      _la = _input->LA(1);
    }
    setState(126);
    match(LogisticsLangParser::RBRACKET);
    setState(127);
    match(LogisticsLangParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RequireContext ------------------------------------------------------------------

LogisticsLangParser::RequireContext::RequireContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::RequiresKwContext* LogisticsLangParser::RequireContext::requiresKw() {
  return getRuleContext<LogisticsLangParser::RequiresKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::RequireContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

std::vector<LogisticsLangParser::ItemContext *> LogisticsLangParser::RequireContext::item() {
  return getRuleContexts<LogisticsLangParser::ItemContext>();
}

LogisticsLangParser::ItemContext* LogisticsLangParser::RequireContext::item(size_t i) {
  return getRuleContext<LogisticsLangParser::ItemContext>(i);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::RequireContext::COMMA() {
  return getTokens(LogisticsLangParser::COMMA);
}

tree::TerminalNode* LogisticsLangParser::RequireContext::COMMA(size_t i) {
  return getToken(LogisticsLangParser::COMMA, i);
}

LogisticsLangParser::NothingKwContext* LogisticsLangParser::RequireContext::nothingKw() {
  return getRuleContext<LogisticsLangParser::NothingKwContext>(0);
}


size_t LogisticsLangParser::RequireContext::getRuleIndex() const {
  return LogisticsLangParser::RuleRequire;
}


antlrcpp::Any LogisticsLangParser::RequireContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitRequire(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::RequireContext* LogisticsLangParser::require() {
  RequireContext *_localctx = _tracker.createInstance<RequireContext>(_ctx, getState());
  enterRule(_localctx, 32, LogisticsLangParser::RuleRequire);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(143);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 4, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(129);
      requiresKw();
      setState(130);
      match(LogisticsLangParser::COLON);
      setState(131);
      item();
      setState(136);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == LogisticsLangParser::COMMA) {
        setState(132);
        match(LogisticsLangParser::COMMA);
        setState(133);
        item();
        setState(138);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(139);
      requiresKw();
      setState(140);
      match(LogisticsLangParser::COLON);
      setState(141);
      nothingKw();
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

//----------------- ProduceContext ------------------------------------------------------------------

LogisticsLangParser::ProduceContext::ProduceContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::ProducesKwContext* LogisticsLangParser::ProduceContext::producesKw() {
  return getRuleContext<LogisticsLangParser::ProducesKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::ProduceContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

std::vector<LogisticsLangParser::ItemContext *> LogisticsLangParser::ProduceContext::item() {
  return getRuleContexts<LogisticsLangParser::ItemContext>();
}

LogisticsLangParser::ItemContext* LogisticsLangParser::ProduceContext::item(size_t i) {
  return getRuleContext<LogisticsLangParser::ItemContext>(i);
}

std::vector<tree::TerminalNode *> LogisticsLangParser::ProduceContext::COMMA() {
  return getTokens(LogisticsLangParser::COMMA);
}

tree::TerminalNode* LogisticsLangParser::ProduceContext::COMMA(size_t i) {
  return getToken(LogisticsLangParser::COMMA, i);
}

LogisticsLangParser::NothingKwContext* LogisticsLangParser::ProduceContext::nothingKw() {
  return getRuleContext<LogisticsLangParser::NothingKwContext>(0);
}


size_t LogisticsLangParser::ProduceContext::getRuleIndex() const {
  return LogisticsLangParser::RuleProduce;
}


antlrcpp::Any LogisticsLangParser::ProduceContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitProduce(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ProduceContext* LogisticsLangParser::produce() {
  ProduceContext *_localctx = _tracker.createInstance<ProduceContext>(_ctx, getState());
  enterRule(_localctx, 34, LogisticsLangParser::RuleProduce);
  size_t _la = 0;

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(159);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(145);
      producesKw();
      setState(146);
      match(LogisticsLangParser::COLON);
      setState(147);
      item();
      setState(152);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == LogisticsLangParser::COMMA) {
        setState(148);
        match(LogisticsLangParser::COMMA);
        setState(149);
        item();
        setState(154);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(155);
      producesKw();
      setState(156);
      match(LogisticsLangParser::COLON);
      setState(157);
      nothingKw();
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

//----------------- MachineSpecContext ------------------------------------------------------------------

LogisticsLangParser::MachineSpecContext::MachineSpecContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::ProduceContext* LogisticsLangParser::MachineSpecContext::produce() {
  return getRuleContext<LogisticsLangParser::ProduceContext>(0);
}

tree::TerminalNode* LogisticsLangParser::MachineSpecContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}

LogisticsLangParser::RequireContext* LogisticsLangParser::MachineSpecContext::require() {
  return getRuleContext<LogisticsLangParser::RequireContext>(0);
}


size_t LogisticsLangParser::MachineSpecContext::getRuleIndex() const {
  return LogisticsLangParser::RuleMachineSpec;
}


antlrcpp::Any LogisticsLangParser::MachineSpecContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitMachineSpec(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::MachineSpecContext* LogisticsLangParser::machineSpec() {
  MachineSpecContext *_localctx = _tracker.createInstance<MachineSpecContext>(_ctx, getState());
  enterRule(_localctx, 36, LogisticsLangParser::RuleMachineSpec);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(169);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case LogisticsLangParser::T__6: {
        enterOuterAlt(_localctx, 1);
        setState(161);
        produce();
        setState(162);
        match(LogisticsLangParser::ENDST);
        setState(163);
        require();
        break;
      }

      case LogisticsLangParser::T__7: {
        enterOuterAlt(_localctx, 2);
        setState(165);
        require();
        setState(166);
        match(LogisticsLangParser::ENDST);
        setState(167);
        produce();
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

//----------------- MachineContext ------------------------------------------------------------------

LogisticsLangParser::MachineContext::MachineContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::MachineContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}

LogisticsLangParser::IsKwContext* LogisticsLangParser::MachineContext::isKw() {
  return getRuleContext<LogisticsLangParser::IsKwContext>(0);
}

LogisticsLangParser::MachineKwContext* LogisticsLangParser::MachineContext::machineKw() {
  return getRuleContext<LogisticsLangParser::MachineKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::MachineContext::LBRACKET() {
  return getToken(LogisticsLangParser::LBRACKET, 0);
}

LogisticsLangParser::MachineSpecContext* LogisticsLangParser::MachineContext::machineSpec() {
  return getRuleContext<LogisticsLangParser::MachineSpecContext>(0);
}

tree::TerminalNode* LogisticsLangParser::MachineContext::RBRACKET() {
  return getToken(LogisticsLangParser::RBRACKET, 0);
}

tree::TerminalNode* LogisticsLangParser::MachineContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}


size_t LogisticsLangParser::MachineContext::getRuleIndex() const {
  return LogisticsLangParser::RuleMachine;
}


antlrcpp::Any LogisticsLangParser::MachineContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitMachine(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::MachineContext* LogisticsLangParser::machine() {
  MachineContext *_localctx = _tracker.createInstance<MachineContext>(_ctx, getState());
  enterRule(_localctx, 38, LogisticsLangParser::RuleMachine);

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
    match(LogisticsLangParser::ID);
    setState(172);
    isKw();
    setState(173);
    machineKw();
    setState(174);
    match(LogisticsLangParser::LBRACKET);
    setState(175);
    machineSpec();
    setState(176);
    match(LogisticsLangParser::RBRACKET);
    setState(177);
    match(LogisticsLangParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ComponentContext ------------------------------------------------------------------

LogisticsLangParser::ComponentContext::ComponentContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::MachineContext* LogisticsLangParser::ComponentContext::machine() {
  return getRuleContext<LogisticsLangParser::MachineContext>(0);
}

LogisticsLangParser::SinkContext* LogisticsLangParser::ComponentContext::sink() {
  return getRuleContext<LogisticsLangParser::SinkContext>(0);
}

LogisticsLangParser::BinContext* LogisticsLangParser::ComponentContext::bin() {
  return getRuleContext<LogisticsLangParser::BinContext>(0);
}


size_t LogisticsLangParser::ComponentContext::getRuleIndex() const {
  return LogisticsLangParser::RuleComponent;
}


antlrcpp::Any LogisticsLangParser::ComponentContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitComponent(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ComponentContext* LogisticsLangParser::component() {
  ComponentContext *_localctx = _tracker.createInstance<ComponentContext>(_ctx, getState());
  enterRule(_localctx, 40, LogisticsLangParser::RuleComponent);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    setState(182);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 8, _ctx)) {
    case 1: {
      enterOuterAlt(_localctx, 1);
      setState(179);
      machine();
      break;
    }

    case 2: {
      enterOuterAlt(_localctx, 2);
      setState(180);
      sink();
      break;
    }

    case 3: {
      enterOuterAlt(_localctx, 3);
      setState(181);
      bin();
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

//----------------- ComponentsContext ------------------------------------------------------------------

LogisticsLangParser::ComponentsContext::ComponentsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::ComponentsKwContext* LogisticsLangParser::ComponentsContext::componentsKw() {
  return getRuleContext<LogisticsLangParser::ComponentsKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::ComponentsContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

std::vector<LogisticsLangParser::ComponentContext *> LogisticsLangParser::ComponentsContext::component() {
  return getRuleContexts<LogisticsLangParser::ComponentContext>();
}

LogisticsLangParser::ComponentContext* LogisticsLangParser::ComponentsContext::component(size_t i) {
  return getRuleContext<LogisticsLangParser::ComponentContext>(i);
}


size_t LogisticsLangParser::ComponentsContext::getRuleIndex() const {
  return LogisticsLangParser::RuleComponents;
}


antlrcpp::Any LogisticsLangParser::ComponentsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitComponents(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ComponentsContext* LogisticsLangParser::components() {
  ComponentsContext *_localctx = _tracker.createInstance<ComponentsContext>(_ctx, getState());
  enterRule(_localctx, 42, LogisticsLangParser::RuleComponents);
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
    setState(184);
    componentsKw();
    setState(185);
    match(LogisticsLangParser::COLON);
    setState(187); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(186);
      component();
      setState(189); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LogisticsLangParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CrossroadContext ------------------------------------------------------------------

LogisticsLangParser::CrossroadContext::CrossroadContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::CrossroadContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}

tree::TerminalNode* LogisticsLangParser::CrossroadContext::COMMA() {
  return getToken(LogisticsLangParser::COMMA, 0);
}

tree::TerminalNode* LogisticsLangParser::CrossroadContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}

tree::TerminalNode* LogisticsLangParser::CrossroadContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}


size_t LogisticsLangParser::CrossroadContext::getRuleIndex() const {
  return LogisticsLangParser::RuleCrossroad;
}


antlrcpp::Any LogisticsLangParser::CrossroadContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitCrossroad(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::CrossroadContext* LogisticsLangParser::crossroad() {
  CrossroadContext *_localctx = _tracker.createInstance<CrossroadContext>(_ctx, getState());
  enterRule(_localctx, 44, LogisticsLangParser::RuleCrossroad);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(191);
    match(LogisticsLangParser::ID);
    setState(192);
    match(LogisticsLangParser::COMMA);
    setState(193);
    match(LogisticsLangParser::NUMBER);
    setState(194);
    match(LogisticsLangParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- CrossroadsContext ------------------------------------------------------------------

LogisticsLangParser::CrossroadsContext::CrossroadsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::CrossroadsKwContext* LogisticsLangParser::CrossroadsContext::crossroadsKw() {
  return getRuleContext<LogisticsLangParser::CrossroadsKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::CrossroadsContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

std::vector<LogisticsLangParser::CrossroadContext *> LogisticsLangParser::CrossroadsContext::crossroad() {
  return getRuleContexts<LogisticsLangParser::CrossroadContext>();
}

LogisticsLangParser::CrossroadContext* LogisticsLangParser::CrossroadsContext::crossroad(size_t i) {
  return getRuleContext<LogisticsLangParser::CrossroadContext>(i);
}


size_t LogisticsLangParser::CrossroadsContext::getRuleIndex() const {
  return LogisticsLangParser::RuleCrossroads;
}


antlrcpp::Any LogisticsLangParser::CrossroadsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitCrossroads(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::CrossroadsContext* LogisticsLangParser::crossroads() {
  CrossroadsContext *_localctx = _tracker.createInstance<CrossroadsContext>(_ctx, getState());
  enterRule(_localctx, 46, LogisticsLangParser::RuleCrossroads);
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
    setState(196);
    crossroadsKw();
    setState(197);
    match(LogisticsLangParser::COLON);
    setState(199); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(198);
      crossroad();
      setState(201); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LogisticsLangParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RoadContext ------------------------------------------------------------------

LogisticsLangParser::RoadContext::RoadContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::RoadContext::ARROW_PRE() {
  return getToken(LogisticsLangParser::ARROW_PRE, 0);
}

tree::TerminalNode* LogisticsLangParser::RoadContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}

tree::TerminalNode* LogisticsLangParser::RoadContext::ARROW_POST() {
  return getToken(LogisticsLangParser::ARROW_POST, 0);
}


size_t LogisticsLangParser::RoadContext::getRuleIndex() const {
  return LogisticsLangParser::RuleRoad;
}


antlrcpp::Any LogisticsLangParser::RoadContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitRoad(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::RoadContext* LogisticsLangParser::road() {
  RoadContext *_localctx = _tracker.createInstance<RoadContext>(_ctx, getState());
  enterRule(_localctx, 48, LogisticsLangParser::RuleRoad);

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
    match(LogisticsLangParser::ARROW_PRE);
    setState(204);
    match(LogisticsLangParser::NUMBER);
    setState(205);
    match(LogisticsLangParser::ARROW_POST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ConnectionContext ------------------------------------------------------------------

LogisticsLangParser::ConnectionContext::ConnectionContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

std::vector<tree::TerminalNode *> LogisticsLangParser::ConnectionContext::ID() {
  return getTokens(LogisticsLangParser::ID);
}

tree::TerminalNode* LogisticsLangParser::ConnectionContext::ID(size_t i) {
  return getToken(LogisticsLangParser::ID, i);
}

LogisticsLangParser::RoadContext* LogisticsLangParser::ConnectionContext::road() {
  return getRuleContext<LogisticsLangParser::RoadContext>(0);
}

tree::TerminalNode* LogisticsLangParser::ConnectionContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}


size_t LogisticsLangParser::ConnectionContext::getRuleIndex() const {
  return LogisticsLangParser::RuleConnection;
}


antlrcpp::Any LogisticsLangParser::ConnectionContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitConnection(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ConnectionContext* LogisticsLangParser::connection() {
  ConnectionContext *_localctx = _tracker.createInstance<ConnectionContext>(_ctx, getState());
  enterRule(_localctx, 50, LogisticsLangParser::RuleConnection);

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
    match(LogisticsLangParser::ID);
    setState(208);
    road();
    setState(209);
    match(LogisticsLangParser::ID);
    setState(210);
    match(LogisticsLangParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ArchitectureContext ------------------------------------------------------------------

LogisticsLangParser::ArchitectureContext::ArchitectureContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::ArchitectureKwContext* LogisticsLangParser::ArchitectureContext::architectureKw() {
  return getRuleContext<LogisticsLangParser::ArchitectureKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::ArchitectureContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

std::vector<LogisticsLangParser::ConnectionContext *> LogisticsLangParser::ArchitectureContext::connection() {
  return getRuleContexts<LogisticsLangParser::ConnectionContext>();
}

LogisticsLangParser::ConnectionContext* LogisticsLangParser::ArchitectureContext::connection(size_t i) {
  return getRuleContext<LogisticsLangParser::ConnectionContext>(i);
}


size_t LogisticsLangParser::ArchitectureContext::getRuleIndex() const {
  return LogisticsLangParser::RuleArchitecture;
}


antlrcpp::Any LogisticsLangParser::ArchitectureContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitArchitecture(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::ArchitectureContext* LogisticsLangParser::architecture() {
  ArchitectureContext *_localctx = _tracker.createInstance<ArchitectureContext>(_ctx, getState());
  enterRule(_localctx, 52, LogisticsLangParser::RuleArchitecture);
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
    architectureKw();
    setState(213);
    match(LogisticsLangParser::COLON);
    setState(215); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(214);
      connection();
      setState(217); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LogisticsLangParser::ID);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RequirementContext ------------------------------------------------------------------

LogisticsLangParser::RequirementContext::RequirementContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* LogisticsLangParser::RequirementContext::NUMBER() {
  return getToken(LogisticsLangParser::NUMBER, 0);
}

tree::TerminalNode* LogisticsLangParser::RequirementContext::ID() {
  return getToken(LogisticsLangParser::ID, 0);
}

tree::TerminalNode* LogisticsLangParser::RequirementContext::ENDST() {
  return getToken(LogisticsLangParser::ENDST, 0);
}


size_t LogisticsLangParser::RequirementContext::getRuleIndex() const {
  return LogisticsLangParser::RuleRequirement;
}


antlrcpp::Any LogisticsLangParser::RequirementContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitRequirement(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::RequirementContext* LogisticsLangParser::requirement() {
  RequirementContext *_localctx = _tracker.createInstance<RequirementContext>(_ctx, getState());
  enterRule(_localctx, 54, LogisticsLangParser::RuleRequirement);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(219);
    match(LogisticsLangParser::NUMBER);
    setState(220);
    match(LogisticsLangParser::ID);
    setState(221);
    match(LogisticsLangParser::ENDST);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- RequirementsContext ------------------------------------------------------------------

LogisticsLangParser::RequirementsContext::RequirementsContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

LogisticsLangParser::RequirementsKwContext* LogisticsLangParser::RequirementsContext::requirementsKw() {
  return getRuleContext<LogisticsLangParser::RequirementsKwContext>(0);
}

tree::TerminalNode* LogisticsLangParser::RequirementsContext::COLON() {
  return getToken(LogisticsLangParser::COLON, 0);
}

std::vector<LogisticsLangParser::RequirementContext *> LogisticsLangParser::RequirementsContext::requirement() {
  return getRuleContexts<LogisticsLangParser::RequirementContext>();
}

LogisticsLangParser::RequirementContext* LogisticsLangParser::RequirementsContext::requirement(size_t i) {
  return getRuleContext<LogisticsLangParser::RequirementContext>(i);
}


size_t LogisticsLangParser::RequirementsContext::getRuleIndex() const {
  return LogisticsLangParser::RuleRequirements;
}


antlrcpp::Any LogisticsLangParser::RequirementsContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<LogisticsLangVisitor*>(visitor))
    return parserVisitor->visitRequirements(this);
  else
    return visitor->visitChildren(this);
}

LogisticsLangParser::RequirementsContext* LogisticsLangParser::requirements() {
  RequirementsContext *_localctx = _tracker.createInstance<RequirementsContext>(_ctx, getState());
  enterRule(_localctx, 56, LogisticsLangParser::RuleRequirements);
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
    setState(223);
    requirementsKw();
    setState(224);
    match(LogisticsLangParser::COLON);
    setState(226); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(225);
      requirement();
      setState(228); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == LogisticsLangParser::NUMBER);
   
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

LogisticsLangParser::WidgetsContext* LogisticsLangParser::SpecContext::widgets() {
  return getRuleContext<LogisticsLangParser::WidgetsContext>(0);
}

LogisticsLangParser::ComponentsContext* LogisticsLangParser::SpecContext::components() {
  return getRuleContext<LogisticsLangParser::ComponentsContext>(0);
}

LogisticsLangParser::CrossroadsContext* LogisticsLangParser::SpecContext::crossroads() {
  return getRuleContext<LogisticsLangParser::CrossroadsContext>(0);
}

LogisticsLangParser::ArchitectureContext* LogisticsLangParser::SpecContext::architecture() {
  return getRuleContext<LogisticsLangParser::ArchitectureContext>(0);
}

LogisticsLangParser::RequirementsContext* LogisticsLangParser::SpecContext::requirements() {
  return getRuleContext<LogisticsLangParser::RequirementsContext>(0);
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
  enterRule(_localctx, 58, LogisticsLangParser::RuleSpec);

#if __cplusplus > 201703L
  auto onExit = finally([=, this] {
#else
  auto onExit = finally([=] {
#endif
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(230);
    widgets();
    setState(231);
    components();
    setState(232);
    crossroads();
    setState(233);
    architecture();
    setState(234);
    requirements();
   
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
  "widgetsKw", "componentsKw", "crossroadsKw", "architectureKw", "isKw", 
  "requirementsKw", "producesKw", "requiresKw", "binKw", "machineKw", "sinkKw", 
  "nothingKw", "widgets", "item", "bin", "sink", "require", "produce", "machineSpec", 
  "machine", "component", "components", "crossroad", "crossroads", "road", 
  "connection", "architecture", "requirement", "requirements", "spec"
};

std::vector<std::string> LogisticsLangParser::_literalNames = {
  "", "'Widgets'", "'Components'", "'Crossroads'", "'Architecture'", "'is'", 
  "'Requirements'", "'produces'", "'requires'", "'Bin'", "'Machine'", "'Sink'", 
  "'Nothing'", "", "", "':'", "','", "'.'", "'='", "';'", "'('", "')'", 
  "", "", "", "'-('", "')->'"
};

std::vector<std::string> LogisticsLangParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "WS", "LINE_COMMENT", 
  "COLON", "COMMA", "DOT", "EQ", "ENDST", "LBRACKET", "RBRACKET", "ID", 
  "DECIMAL", "NUMBER", "ARROW_PRE", "ARROW_POST"
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
       0x3, 0x1c, 0xef, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 
       0x9, 0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 
       0x7, 0x4, 0x8, 0x9, 0x8, 0x4, 0x9, 0x9, 0x9, 0x4, 0xa, 0x9, 0xa, 
       0x4, 0xb, 0x9, 0xb, 0x4, 0xc, 0x9, 0xc, 0x4, 0xd, 0x9, 0xd, 0x4, 
       0xe, 0x9, 0xe, 0x4, 0xf, 0x9, 0xf, 0x4, 0x10, 0x9, 0x10, 0x4, 0x11, 
       0x9, 0x11, 0x4, 0x12, 0x9, 0x12, 0x4, 0x13, 0x9, 0x13, 0x4, 0x14, 
       0x9, 0x14, 0x4, 0x15, 0x9, 0x15, 0x4, 0x16, 0x9, 0x16, 0x4, 0x17, 
       0x9, 0x17, 0x4, 0x18, 0x9, 0x18, 0x4, 0x19, 0x9, 0x19, 0x4, 0x1a, 
       0x9, 0x1a, 0x4, 0x1b, 0x9, 0x1b, 0x4, 0x1c, 0x9, 0x1c, 0x4, 0x1d, 
       0x9, 0x1d, 0x4, 0x1e, 0x9, 0x1e, 0x4, 0x1f, 0x9, 0x1f, 0x3, 0x2, 
       0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x5, 0x3, 
       0x5, 0x3, 0x6, 0x3, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 0x8, 0x3, 0x8, 
       0x3, 0x9, 0x3, 0x9, 0x3, 0xa, 0x3, 0xa, 0x3, 0xb, 0x3, 0xb, 0x3, 
       0xc, 0x3, 0xc, 0x3, 0xd, 0x3, 0xd, 0x3, 0xe, 0x3, 0xe, 0x3, 0xe, 
       0x3, 0xe, 0x3, 0xe, 0x7, 0xe, 0x5c, 0xa, 0xe, 0xc, 0xe, 0xe, 0xe, 
       0x5f, 0xb, 0xe, 0x3, 0xe, 0x3, 0xe, 0x3, 0xf, 0x3, 0xf, 0x3, 0xf, 
       0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 
       0x3, 0x10, 0x7, 0x10, 0x6d, 0xa, 0x10, 0xc, 0x10, 0xe, 0x10, 0x70, 
       0xb, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x10, 0x3, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x3, 0x11, 0x7, 0x11, 
       0x7c, 0xa, 0x11, 0xc, 0x11, 0xe, 0x11, 0x7f, 0xb, 0x11, 0x3, 0x11, 
       0x3, 0x11, 0x3, 0x11, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 
       0x3, 0x12, 0x7, 0x12, 0x89, 0xa, 0x12, 0xc, 0x12, 0xe, 0x12, 0x8c, 
       0xb, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x3, 0x12, 0x5, 0x12, 
       0x92, 0xa, 0x12, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 
       0x13, 0x7, 0x13, 0x99, 0xa, 0x13, 0xc, 0x13, 0xe, 0x13, 0x9c, 0xb, 
       0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x3, 0x13, 0x5, 0x13, 0xa2, 
       0xa, 0x13, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 
       0x3, 0x14, 0x3, 0x14, 0x3, 0x14, 0x5, 0x14, 0xac, 0xa, 0x14, 0x3, 
       0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 0x15, 0x3, 
       0x15, 0x3, 0x15, 0x3, 0x16, 0x3, 0x16, 0x3, 0x16, 0x5, 0x16, 0xb9, 
       0xa, 0x16, 0x3, 0x17, 0x3, 0x17, 0x3, 0x17, 0x6, 0x17, 0xbe, 0xa, 
       0x17, 0xd, 0x17, 0xe, 0x17, 0xbf, 0x3, 0x18, 0x3, 0x18, 0x3, 0x18, 
       0x3, 0x18, 0x3, 0x18, 0x3, 0x19, 0x3, 0x19, 0x3, 0x19, 0x6, 0x19, 
       0xca, 0xa, 0x19, 0xd, 0x19, 0xe, 0x19, 0xcb, 0x3, 0x1a, 0x3, 0x1a, 
       0x3, 0x1a, 0x3, 0x1a, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 0x3, 0x1b, 
       0x3, 0x1b, 0x3, 0x1c, 0x3, 0x1c, 0x3, 0x1c, 0x6, 0x1c, 0xda, 0xa, 
       0x1c, 0xd, 0x1c, 0xe, 0x1c, 0xdb, 0x3, 0x1d, 0x3, 0x1d, 0x3, 0x1d, 
       0x3, 0x1d, 0x3, 0x1e, 0x3, 0x1e, 0x3, 0x1e, 0x6, 0x1e, 0xe5, 0xa, 
       0x1e, 0xd, 0x1e, 0xe, 0x1e, 0xe6, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 
       0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x3, 0x1f, 0x2, 0x2, 0x20, 0x2, 
       0x4, 0x6, 0x8, 0xa, 0xc, 0xe, 0x10, 0x12, 0x14, 0x16, 0x18, 0x1a, 
       0x1c, 0x1e, 0x20, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2c, 0x2e, 0x30, 
       0x32, 0x34, 0x36, 0x38, 0x3a, 0x3c, 0x2, 0x2, 0x2, 0xde, 0x2, 0x3e, 
       0x3, 0x2, 0x2, 0x2, 0x4, 0x40, 0x3, 0x2, 0x2, 0x2, 0x6, 0x42, 0x3, 
       0x2, 0x2, 0x2, 0x8, 0x44, 0x3, 0x2, 0x2, 0x2, 0xa, 0x46, 0x3, 0x2, 
       0x2, 0x2, 0xc, 0x48, 0x3, 0x2, 0x2, 0x2, 0xe, 0x4a, 0x3, 0x2, 0x2, 
       0x2, 0x10, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x12, 0x4e, 0x3, 0x2, 0x2, 0x2, 
       0x14, 0x50, 0x3, 0x2, 0x2, 0x2, 0x16, 0x52, 0x3, 0x2, 0x2, 0x2, 0x18, 
       0x54, 0x3, 0x2, 0x2, 0x2, 0x1a, 0x56, 0x3, 0x2, 0x2, 0x2, 0x1c, 0x62, 
       0x3, 0x2, 0x2, 0x2, 0x1e, 0x65, 0x3, 0x2, 0x2, 0x2, 0x20, 0x74, 0x3, 
       0x2, 0x2, 0x2, 0x22, 0x91, 0x3, 0x2, 0x2, 0x2, 0x24, 0xa1, 0x3, 0x2, 
       0x2, 0x2, 0x26, 0xab, 0x3, 0x2, 0x2, 0x2, 0x28, 0xad, 0x3, 0x2, 0x2, 
       0x2, 0x2a, 0xb8, 0x3, 0x2, 0x2, 0x2, 0x2c, 0xba, 0x3, 0x2, 0x2, 0x2, 
       0x2e, 0xc1, 0x3, 0x2, 0x2, 0x2, 0x30, 0xc6, 0x3, 0x2, 0x2, 0x2, 0x32, 
       0xcd, 0x3, 0x2, 0x2, 0x2, 0x34, 0xd1, 0x3, 0x2, 0x2, 0x2, 0x36, 0xd6, 
       0x3, 0x2, 0x2, 0x2, 0x38, 0xdd, 0x3, 0x2, 0x2, 0x2, 0x3a, 0xe1, 0x3, 
       0x2, 0x2, 0x2, 0x3c, 0xe8, 0x3, 0x2, 0x2, 0x2, 0x3e, 0x3f, 0x7, 0x3, 
       0x2, 0x2, 0x3f, 0x3, 0x3, 0x2, 0x2, 0x2, 0x40, 0x41, 0x7, 0x4, 0x2, 
       0x2, 0x41, 0x5, 0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x7, 0x5, 0x2, 0x2, 
       0x43, 0x7, 0x3, 0x2, 0x2, 0x2, 0x44, 0x45, 0x7, 0x6, 0x2, 0x2, 0x45, 
       0x9, 0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x7, 0x7, 0x2, 0x2, 0x47, 0xb, 
       0x3, 0x2, 0x2, 0x2, 0x48, 0x49, 0x7, 0x8, 0x2, 0x2, 0x49, 0xd, 0x3, 
       0x2, 0x2, 0x2, 0x4a, 0x4b, 0x7, 0x9, 0x2, 0x2, 0x4b, 0xf, 0x3, 0x2, 
       0x2, 0x2, 0x4c, 0x4d, 0x7, 0xa, 0x2, 0x2, 0x4d, 0x11, 0x3, 0x2, 0x2, 
       0x2, 0x4e, 0x4f, 0x7, 0xb, 0x2, 0x2, 0x4f, 0x13, 0x3, 0x2, 0x2, 0x2, 
       0x50, 0x51, 0x7, 0xc, 0x2, 0x2, 0x51, 0x15, 0x3, 0x2, 0x2, 0x2, 0x52, 
       0x53, 0x7, 0xd, 0x2, 0x2, 0x53, 0x17, 0x3, 0x2, 0x2, 0x2, 0x54, 0x55, 
       0x7, 0xe, 0x2, 0x2, 0x55, 0x19, 0x3, 0x2, 0x2, 0x2, 0x56, 0x57, 0x5, 
       0x2, 0x2, 0x2, 0x57, 0x58, 0x7, 0x11, 0x2, 0x2, 0x58, 0x5d, 0x7, 
       0x18, 0x2, 0x2, 0x59, 0x5a, 0x7, 0x12, 0x2, 0x2, 0x5a, 0x5c, 0x7, 
       0x18, 0x2, 0x2, 0x5b, 0x59, 0x3, 0x2, 0x2, 0x2, 0x5c, 0x5f, 0x3, 
       0x2, 0x2, 0x2, 0x5d, 0x5b, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x5e, 0x3, 0x2, 
       0x2, 0x2, 0x5e, 0x60, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x5d, 0x3, 0x2, 0x2, 
       0x2, 0x60, 0x61, 0x7, 0x15, 0x2, 0x2, 0x61, 0x1b, 0x3, 0x2, 0x2, 
       0x2, 0x62, 0x63, 0x7, 0x1a, 0x2, 0x2, 0x63, 0x64, 0x7, 0x18, 0x2, 
       0x2, 0x64, 0x1d, 0x3, 0x2, 0x2, 0x2, 0x65, 0x66, 0x7, 0x18, 0x2, 
       0x2, 0x66, 0x67, 0x5, 0xa, 0x6, 0x2, 0x67, 0x68, 0x5, 0x12, 0xa, 
       0x2, 0x68, 0x69, 0x7, 0x16, 0x2, 0x2, 0x69, 0x6e, 0x5, 0x1c, 0xf, 
       0x2, 0x6a, 0x6b, 0x7, 0x12, 0x2, 0x2, 0x6b, 0x6d, 0x5, 0x1c, 0xf, 
       0x2, 0x6c, 0x6a, 0x3, 0x2, 0x2, 0x2, 0x6d, 0x70, 0x3, 0x2, 0x2, 0x2, 
       0x6e, 0x6c, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x6f, 0x3, 0x2, 0x2, 0x2, 0x6f, 
       0x71, 0x3, 0x2, 0x2, 0x2, 0x70, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x71, 0x72, 
       0x7, 0x17, 0x2, 0x2, 0x72, 0x73, 0x7, 0x15, 0x2, 0x2, 0x73, 0x1f, 
       0x3, 0x2, 0x2, 0x2, 0x74, 0x75, 0x7, 0x18, 0x2, 0x2, 0x75, 0x76, 
       0x5, 0xa, 0x6, 0x2, 0x76, 0x77, 0x5, 0x16, 0xc, 0x2, 0x77, 0x78, 
       0x7, 0x16, 0x2, 0x2, 0x78, 0x7d, 0x5, 0x1c, 0xf, 0x2, 0x79, 0x7a, 
       0x7, 0x12, 0x2, 0x2, 0x7a, 0x7c, 0x5, 0x1c, 0xf, 0x2, 0x7b, 0x79, 
       0x3, 0x2, 0x2, 0x2, 0x7c, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7b, 0x3, 
       0x2, 0x2, 0x2, 0x7d, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x7e, 0x80, 0x3, 0x2, 
       0x2, 0x2, 0x7f, 0x7d, 0x3, 0x2, 0x2, 0x2, 0x80, 0x81, 0x7, 0x17, 
       0x2, 0x2, 0x81, 0x82, 0x7, 0x15, 0x2, 0x2, 0x82, 0x21, 0x3, 0x2, 
       0x2, 0x2, 0x83, 0x84, 0x5, 0x10, 0x9, 0x2, 0x84, 0x85, 0x7, 0x11, 
       0x2, 0x2, 0x85, 0x8a, 0x5, 0x1c, 0xf, 0x2, 0x86, 0x87, 0x7, 0x12, 
       0x2, 0x2, 0x87, 0x89, 0x5, 0x1c, 0xf, 0x2, 0x88, 0x86, 0x3, 0x2, 
       0x2, 0x2, 0x89, 0x8c, 0x3, 0x2, 0x2, 0x2, 0x8a, 0x88, 0x3, 0x2, 0x2, 
       0x2, 0x8a, 0x8b, 0x3, 0x2, 0x2, 0x2, 0x8b, 0x92, 0x3, 0x2, 0x2, 0x2, 
       0x8c, 0x8a, 0x3, 0x2, 0x2, 0x2, 0x8d, 0x8e, 0x5, 0x10, 0x9, 0x2, 
       0x8e, 0x8f, 0x7, 0x11, 0x2, 0x2, 0x8f, 0x90, 0x5, 0x18, 0xd, 0x2, 
       0x90, 0x92, 0x3, 0x2, 0x2, 0x2, 0x91, 0x83, 0x3, 0x2, 0x2, 0x2, 0x91, 
       0x8d, 0x3, 0x2, 0x2, 0x2, 0x92, 0x23, 0x3, 0x2, 0x2, 0x2, 0x93, 0x94, 
       0x5, 0xe, 0x8, 0x2, 0x94, 0x95, 0x7, 0x11, 0x2, 0x2, 0x95, 0x9a, 
       0x5, 0x1c, 0xf, 0x2, 0x96, 0x97, 0x7, 0x12, 0x2, 0x2, 0x97, 0x99, 
       0x5, 0x1c, 0xf, 0x2, 0x98, 0x96, 0x3, 0x2, 0x2, 0x2, 0x99, 0x9c, 
       0x3, 0x2, 0x2, 0x2, 0x9a, 0x98, 0x3, 0x2, 0x2, 0x2, 0x9a, 0x9b, 0x3, 
       0x2, 0x2, 0x2, 0x9b, 0xa2, 0x3, 0x2, 0x2, 0x2, 0x9c, 0x9a, 0x3, 0x2, 
       0x2, 0x2, 0x9d, 0x9e, 0x5, 0xe, 0x8, 0x2, 0x9e, 0x9f, 0x7, 0x11, 
       0x2, 0x2, 0x9f, 0xa0, 0x5, 0x18, 0xd, 0x2, 0xa0, 0xa2, 0x3, 0x2, 
       0x2, 0x2, 0xa1, 0x93, 0x3, 0x2, 0x2, 0x2, 0xa1, 0x9d, 0x3, 0x2, 0x2, 
       0x2, 0xa2, 0x25, 0x3, 0x2, 0x2, 0x2, 0xa3, 0xa4, 0x5, 0x24, 0x13, 
       0x2, 0xa4, 0xa5, 0x7, 0x15, 0x2, 0x2, 0xa5, 0xa6, 0x5, 0x22, 0x12, 
       0x2, 0xa6, 0xac, 0x3, 0x2, 0x2, 0x2, 0xa7, 0xa8, 0x5, 0x22, 0x12, 
       0x2, 0xa8, 0xa9, 0x7, 0x15, 0x2, 0x2, 0xa9, 0xaa, 0x5, 0x24, 0x13, 
       0x2, 0xaa, 0xac, 0x3, 0x2, 0x2, 0x2, 0xab, 0xa3, 0x3, 0x2, 0x2, 0x2, 
       0xab, 0xa7, 0x3, 0x2, 0x2, 0x2, 0xac, 0x27, 0x3, 0x2, 0x2, 0x2, 0xad, 
       0xae, 0x7, 0x18, 0x2, 0x2, 0xae, 0xaf, 0x5, 0xa, 0x6, 0x2, 0xaf, 
       0xb0, 0x5, 0x14, 0xb, 0x2, 0xb0, 0xb1, 0x7, 0x16, 0x2, 0x2, 0xb1, 
       0xb2, 0x5, 0x26, 0x14, 0x2, 0xb2, 0xb3, 0x7, 0x17, 0x2, 0x2, 0xb3, 
       0xb4, 0x7, 0x15, 0x2, 0x2, 0xb4, 0x29, 0x3, 0x2, 0x2, 0x2, 0xb5, 
       0xb9, 0x5, 0x28, 0x15, 0x2, 0xb6, 0xb9, 0x5, 0x20, 0x11, 0x2, 0xb7, 
       0xb9, 0x5, 0x1e, 0x10, 0x2, 0xb8, 0xb5, 0x3, 0x2, 0x2, 0x2, 0xb8, 
       0xb6, 0x3, 0x2, 0x2, 0x2, 0xb8, 0xb7, 0x3, 0x2, 0x2, 0x2, 0xb9, 0x2b, 
       0x3, 0x2, 0x2, 0x2, 0xba, 0xbb, 0x5, 0x4, 0x3, 0x2, 0xbb, 0xbd, 0x7, 
       0x11, 0x2, 0x2, 0xbc, 0xbe, 0x5, 0x2a, 0x16, 0x2, 0xbd, 0xbc, 0x3, 
       0x2, 0x2, 0x2, 0xbe, 0xbf, 0x3, 0x2, 0x2, 0x2, 0xbf, 0xbd, 0x3, 0x2, 
       0x2, 0x2, 0xbf, 0xc0, 0x3, 0x2, 0x2, 0x2, 0xc0, 0x2d, 0x3, 0x2, 0x2, 
       0x2, 0xc1, 0xc2, 0x7, 0x18, 0x2, 0x2, 0xc2, 0xc3, 0x7, 0x12, 0x2, 
       0x2, 0xc3, 0xc4, 0x7, 0x1a, 0x2, 0x2, 0xc4, 0xc5, 0x7, 0x15, 0x2, 
       0x2, 0xc5, 0x2f, 0x3, 0x2, 0x2, 0x2, 0xc6, 0xc7, 0x5, 0x6, 0x4, 0x2, 
       0xc7, 0xc9, 0x7, 0x11, 0x2, 0x2, 0xc8, 0xca, 0x5, 0x2e, 0x18, 0x2, 
       0xc9, 0xc8, 0x3, 0x2, 0x2, 0x2, 0xca, 0xcb, 0x3, 0x2, 0x2, 0x2, 0xcb, 
       0xc9, 0x3, 0x2, 0x2, 0x2, 0xcb, 0xcc, 0x3, 0x2, 0x2, 0x2, 0xcc, 0x31, 
       0x3, 0x2, 0x2, 0x2, 0xcd, 0xce, 0x7, 0x1b, 0x2, 0x2, 0xce, 0xcf, 
       0x7, 0x1a, 0x2, 0x2, 0xcf, 0xd0, 0x7, 0x1c, 0x2, 0x2, 0xd0, 0x33, 
       0x3, 0x2, 0x2, 0x2, 0xd1, 0xd2, 0x7, 0x18, 0x2, 0x2, 0xd2, 0xd3, 
       0x5, 0x32, 0x1a, 0x2, 0xd3, 0xd4, 0x7, 0x18, 0x2, 0x2, 0xd4, 0xd5, 
       0x7, 0x15, 0x2, 0x2, 0xd5, 0x35, 0x3, 0x2, 0x2, 0x2, 0xd6, 0xd7, 
       0x5, 0x8, 0x5, 0x2, 0xd7, 0xd9, 0x7, 0x11, 0x2, 0x2, 0xd8, 0xda, 
       0x5, 0x34, 0x1b, 0x2, 0xd9, 0xd8, 0x3, 0x2, 0x2, 0x2, 0xda, 0xdb, 
       0x3, 0x2, 0x2, 0x2, 0xdb, 0xd9, 0x3, 0x2, 0x2, 0x2, 0xdb, 0xdc, 0x3, 
       0x2, 0x2, 0x2, 0xdc, 0x37, 0x3, 0x2, 0x2, 0x2, 0xdd, 0xde, 0x7, 0x1a, 
       0x2, 0x2, 0xde, 0xdf, 0x7, 0x18, 0x2, 0x2, 0xdf, 0xe0, 0x7, 0x15, 
       0x2, 0x2, 0xe0, 0x39, 0x3, 0x2, 0x2, 0x2, 0xe1, 0xe2, 0x5, 0xc, 0x7, 
       0x2, 0xe2, 0xe4, 0x7, 0x11, 0x2, 0x2, 0xe3, 0xe5, 0x5, 0x38, 0x1d, 
       0x2, 0xe4, 0xe3, 0x3, 0x2, 0x2, 0x2, 0xe5, 0xe6, 0x3, 0x2, 0x2, 0x2, 
       0xe6, 0xe4, 0x3, 0x2, 0x2, 0x2, 0xe6, 0xe7, 0x3, 0x2, 0x2, 0x2, 0xe7, 
       0x3b, 0x3, 0x2, 0x2, 0x2, 0xe8, 0xe9, 0x5, 0x1a, 0xe, 0x2, 0xe9, 
       0xea, 0x5, 0x2c, 0x17, 0x2, 0xea, 0xeb, 0x5, 0x30, 0x19, 0x2, 0xeb, 
       0xec, 0x5, 0x36, 0x1c, 0x2, 0xec, 0xed, 0x5, 0x3a, 0x1e, 0x2, 0xed, 
       0x3d, 0x3, 0x2, 0x2, 0x2, 0xf, 0x5d, 0x6e, 0x7d, 0x8a, 0x91, 0x9a, 
       0xa1, 0xab, 0xb8, 0xbf, 0xcb, 0xdb, 0xe6, 
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
