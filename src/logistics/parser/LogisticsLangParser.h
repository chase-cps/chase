
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/logistics/grammar\LogisticsLang.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  LogisticsLangParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, WS = 5, LINE_COMMENT = 6, COLON = 7, 
    ENDST = 8, COMMA = 9, DOT = 10, LBRACKET = 11, RBRACKET = 12, MAPCHAR = 13, 
    MAPLINE = 14, DECIMAL = 15, NUMBER = 16, ID = 17
  };

  enum {
    RuleMapKw = 0, RuleMap = 1, RuleProductsKw = 2, RuleCoordx = 3, RuleCoordy = 4, 
    RuleUnits = 5, RuleTriple = 6, RuleProduct = 7, RuleProducts = 8, RuleVariableKw = 9, 
    RuleDestinationKw = 10, RuleTime = 11, RuleLocation = 12, RuleRequest = 13, 
    RuleDestination = 14, RuleSpec = 15
  };

  explicit LogisticsLangParser(antlr4::TokenStream *input);
  ~LogisticsLangParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class MapKwContext;
  class MapContext;
  class ProductsKwContext;
  class CoordxContext;
  class CoordyContext;
  class UnitsContext;
  class TripleContext;
  class ProductContext;
  class ProductsContext;
  class VariableKwContext;
  class DestinationKwContext;
  class TimeContext;
  class LocationContext;
  class RequestContext;
  class DestinationContext;
  class SpecContext; 

  class  MapKwContext : public antlr4::ParserRuleContext {
  public:
    MapKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MapKwContext* mapKw();

  class  MapContext : public antlr4::ParserRuleContext {
  public:
    MapContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MapKwContext *mapKw();
    antlr4::tree::TerminalNode *LBRACKET();
    antlr4::tree::TerminalNode *RBRACKET();
    std::vector<antlr4::tree::TerminalNode *> MAPLINE();
    antlr4::tree::TerminalNode* MAPLINE(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  MapContext* map();

  class  ProductsKwContext : public antlr4::ParserRuleContext {
  public:
    ProductsKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProductsKwContext* productsKw();

  class  CoordxContext : public antlr4::ParserRuleContext {
  public:
    CoordxContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CoordxContext* coordx();

  class  CoordyContext : public antlr4::ParserRuleContext {
  public:
    CoordyContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CoordyContext* coordy();

  class  UnitsContext : public antlr4::ParserRuleContext {
  public:
    UnitsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  UnitsContext* units();

  class  TripleContext : public antlr4::ParserRuleContext {
  public:
    TripleContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *LBRACKET();
    CoordxContext *coordx();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);
    CoordyContext *coordy();
    UnitsContext *units();
    antlr4::tree::TerminalNode *RBRACKET();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TripleContext* triple();

  class  ProductContext : public antlr4::ParserRuleContext {
  public:
    ProductContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *COLON();
    std::vector<TripleContext *> triple();
    TripleContext* triple(size_t i);
    antlr4::tree::TerminalNode *ENDST();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProductContext* product();

  class  ProductsContext : public antlr4::ParserRuleContext {
  public:
    ProductsContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    ProductsKwContext *productsKw();
    antlr4::tree::TerminalNode *LBRACKET();
    antlr4::tree::TerminalNode *RBRACKET();
    std::vector<ProductContext *> product();
    ProductContext* product(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  ProductsContext* products();

  class  VariableKwContext : public antlr4::ParserRuleContext {
  public:
    VariableKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  VariableKwContext* variableKw();

  class  DestinationKwContext : public antlr4::ParserRuleContext {
  public:
    DestinationKwContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DestinationKwContext* destinationKw();

  class  TimeContext : public antlr4::ParserRuleContext {
  public:
    TimeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *NUMBER();
    VariableKwContext *variableKw();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  TimeContext* time();

  class  LocationContext : public antlr4::ParserRuleContext {
  public:
    LocationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  LocationContext* location();

  class  RequestContext : public antlr4::ParserRuleContext {
  public:
    RequestContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *ID();
    antlr4::tree::TerminalNode *COLON();
    antlr4::tree::TerminalNode *NUMBER();


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  RequestContext* request();

  class  DestinationContext : public antlr4::ParserRuleContext {
  public:
    DestinationContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    DestinationKwContext *destinationKw();
    LocationContext *location();
    TimeContext *time();
    antlr4::tree::TerminalNode *LBRACKET();
    std::vector<RequestContext *> request();
    RequestContext* request(size_t i);
    antlr4::tree::TerminalNode *RBRACKET();
    std::vector<antlr4::tree::TerminalNode *> COMMA();
    antlr4::tree::TerminalNode* COMMA(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  DestinationContext* destination();

  class  SpecContext : public antlr4::ParserRuleContext {
  public:
    SpecContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    MapContext *map();
    ProductsContext *products();
    std::vector<DestinationContext *> destination();
    DestinationContext* destination(size_t i);


    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  SpecContext* spec();


private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

