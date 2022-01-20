
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/logistics/grammar\LogisticsLang.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "LogisticsLangVisitor.h"


/**
 * This class provides an empty implementation of LogisticsLangVisitor, which can be
 * extended to create a visitor which only needs to handle a subset of the available methods.
 */
class  LogisticsLangBaseVisitor : public LogisticsLangVisitor {
public:

  virtual antlrcpp::Any visitMapKw(LogisticsLangParser::MapKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitMap(LogisticsLangParser::MapContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProductsKw(LogisticsLangParser::ProductsKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitXpos(LogisticsLangParser::XposContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitYpos(LogisticsLangParser::YposContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitUnits(LogisticsLangParser::UnitsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTriple(LogisticsLangParser::TripleContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProduct(LogisticsLangParser::ProductContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitProducts(LogisticsLangParser::ProductsContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDestinationKw(LogisticsLangParser::DestinationKwContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitTime(LogisticsLangParser::TimeContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitLocation(LogisticsLangParser::LocationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitRequest(LogisticsLangParser::RequestContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitDestination(LogisticsLangParser::DestinationContext *ctx) override {
    return visitChildren(ctx);
  }

  virtual antlrcpp::Any visitSpec(LogisticsLangParser::SpecContext *ctx) override {
    return visitChildren(ctx);
  }


};

