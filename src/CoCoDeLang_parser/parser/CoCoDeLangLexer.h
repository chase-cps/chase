
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
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    ID = 33, INTEGER = 34, DECIMAL = 35, DesignKW = 36, WS = 37, LINE_COMMENT = 38, 
    MULTILINE_COMMENT = 39, AND = 40, OR = 41, NOT = 42, IMPLIES = 43, IFF = 44, 
    COLON = 45, ALWAYS = 46, EVENTUALLY = 47, NEXT = 48, UNTIL = 49, RELEASE = 50, 
    EQ = 51, NEQ = 52, LT = 53, LE = 54, GT = 55, GE = 56, PLUS = 57, MINUS = 58, 
    TIMES = 59, DIVIDE = 60, POWER = 61, MOD = 62, LROUND = 63, RROUND = 64, 
    LSQUARE = 65, RSQUARE = 66, LCURLY = 67, RCURLY = 68, COMMA = 69, DOT = 70, 
    ENDST = 71
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

