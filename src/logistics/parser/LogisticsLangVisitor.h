
// Generated from //wsl$/Ubuntu-20.04/home/lora/chase/repo/src/logistics/grammar\LogisticsLang.g4 by ANTLR 4.9.2

#pragma once


#include "antlr4-runtime.h"
#include "LogisticsLangParser.h"



/**
 * This class defines an abstract visitor for a parse tree
 * produced by LogisticsLangParser.
 */
class  LogisticsLangVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

  /**
   * Visit parse trees produced by LogisticsLangParser.
   */
    virtual antlrcpp::Any visitMapKw(LogisticsLangParser::MapKwContext *context) = 0;

    virtual antlrcpp::Any visitMap(LogisticsLangParser::MapContext *context) = 0;

    virtual antlrcpp::Any visitProductsKw(LogisticsLangParser::ProductsKwContext *context) = 0;

    virtual antlrcpp::Any visitXpos(LogisticsLangParser::XposContext *context) = 0;

    virtual antlrcpp::Any visitYpos(LogisticsLangParser::YposContext *context) = 0;

    virtual antlrcpp::Any visitUnits(LogisticsLangParser::UnitsContext *context) = 0;

    virtual antlrcpp::Any visitTriple(LogisticsLangParser::TripleContext *context) = 0;

    virtual antlrcpp::Any visitProduct(LogisticsLangParser::ProductContext *context) = 0;

    virtual antlrcpp::Any visitProducts(LogisticsLangParser::ProductsContext *context) = 0;

    virtual antlrcpp::Any visitDestinationKw(LogisticsLangParser::DestinationKwContext *context) = 0;

    virtual antlrcpp::Any visitTime(LogisticsLangParser::TimeContext *context) = 0;

    virtual antlrcpp::Any visitLocation(LogisticsLangParser::LocationContext *context) = 0;

    virtual antlrcpp::Any visitRequest(LogisticsLangParser::RequestContext *context) = 0;

    virtual antlrcpp::Any visitDestination(LogisticsLangParser::DestinationContext *context) = 0;

    virtual antlrcpp::Any visitSpec(LogisticsLangParser::SpecContext *context) = 0;


};

