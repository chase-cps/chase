
// Generated from //wsl$/Ubuntu-20.04/home/lora/software/chase/repo/src/chase/grammar\LTLContracts.g4 by ANTLR 4.9.1

#pragma once


#include "antlr4-runtime.h"




class  LTLContractsLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, WS = 16, LINE_COMMENT = 17, AND = 18, OR = 19, NOT = 20, 
    IMPLIES = 21, IFF = 22, COLON = 23, ALWAYS = 24, EVENTUALLY = 25, NEXT = 26, 
    UNTIL = 27, EQ = 28, NEQ = 29, LT = 30, LE = 31, GT = 32, GE = 33, PLUS = 34, 
    MINUS = 35, TIMES = 36, DIVIDE = 37, LBRACKET = 38, RBRACKET = 39, LSQUARE = 40, 
    RSQUARE = 41, COMMA = 42, DOT = 43, ID = 44, DECIMAL = 45, NUMBER = 46, 
    ENDST = 47
  };

  explicit LTLContractsLexer(antlr4::CharStream *input);
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

