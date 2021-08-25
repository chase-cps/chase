
// Generated from //wsl$/Ubuntu-20.04/home/lora/software/chase/repo/src/logics_frontend/grammar\LogicsContracts.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"




class  LogicsContractsLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, WS = 17, LINE_COMMENT = 18, AND = 19, OR = 20, 
    NOT = 21, IMPLIES = 22, IFF = 23, COLON = 24, ALWAYS = 25, EVENTUALLY = 26, 
    NEXT = 27, UNTIL = 28, EQ = 29, NEQ = 30, LT = 31, LE = 32, GT = 33, 
    GE = 34, PLUS = 35, MINUS = 36, TIMES = 37, DIVIDE = 38, LBRACKET = 39, 
    RBRACKET = 40, LSQUARE = 41, RSQUARE = 42, COMMA = 43, DOT = 44, ID = 45, 
    DECIMAL = 46, NUMBER = 47, ENDST = 48
  };

  explicit LogicsContractsLexer(antlr4::CharStream *input);
  ~LogicsContractsLexer();

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

