
// Generated from //wsl$/Ubuntu-20.04/home/lora/software/chase/repo/src/ltl_tool/grammar\LTLContracts.g4 by ANTLR 4.8

#pragma once


#include "antlr4-runtime.h"




class  LTLContractsLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    WS = 15, LINE_COMMENT = 16, AND = 17, OR = 18, NOT = 19, IMPLIES = 20, 
    IFF = 21, COLON = 22, ALWAYS = 23, EVENTUALLY = 24, NEXT = 25, UNTIL = 26, 
    EQ = 27, NEQ = 28, LT = 29, LE = 30, GT = 31, GE = 32, PLUS = 33, MINUS = 34, 
    TIMES = 35, DIVIDE = 36, LBRACKET = 37, RBRACKET = 38, ID = 39, NUMBER = 40, 
    ENDST = 41
  };

  LTLContractsLexer(antlr4::CharStream *input);
  ~LTLContractsLexer();

  virtual std::string getGrammarFileName() const override;
  virtual const std::vector<std::string>& getRuleNames() const override;

  virtual const std::vector<std::string>& getChannelNames() const override;
  virtual const std::vector<std::string>& getModeNames() const override;
  virtual const std::vector<std::string>& getTokenNames() const override; // deprecated, use vocabulary instead
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;

  virtual const std::vector<uint16_t> getSerializedATN() const override;
  virtual const antlr4::atn::ATN& getATN() const override;

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;
  static std::vector<std::string> _channelNames;
  static std::vector<std::string> _modeNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

