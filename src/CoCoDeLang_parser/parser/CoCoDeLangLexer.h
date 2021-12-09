
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/CoCoDeLang_parser/grammar\CoCoDeLang.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"




class  CoCoDeLangLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, ID = 28, INTEGER = 29, DECIMAL = 30, WS = 31, LINE_COMMENT = 32, 
    MULTILINE_COMMENT = 33, AND = 34, OR = 35, NOT = 36, IMPLIES = 37, IFF = 38, 
    COLON = 39, ALWAYS = 40, EVENTUALLY = 41, NEXT = 42, UNTIL = 43, RELEASE = 44, 
    EQ = 45, NEQ = 46, LT = 47, LE = 48, GT = 49, GE = 50, PLUS = 51, MINUS = 52, 
    TIMES = 53, DIVIDE = 54, LROUND = 55, RROUND = 56, LSQUARE = 57, RSQUARE = 58, 
    LCURLY = 59, RCURLY = 60, COMMA = 61, DOT = 62, ENDST = 63
  };

  explicit CoCoDeLangLexer(antlr4::CharStream *input);
  ~CoCoDeLangLexer();

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

