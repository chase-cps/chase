
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
    virtual antlrcpp::Any visitWidgetsKw(LogisticsLangParser::WidgetsKwContext *context) = 0;

    virtual antlrcpp::Any visitComponentsKw(LogisticsLangParser::ComponentsKwContext *context) = 0;

    virtual antlrcpp::Any visitCrossroadsKw(LogisticsLangParser::CrossroadsKwContext *context) = 0;

    virtual antlrcpp::Any visitArchitectureKw(LogisticsLangParser::ArchitectureKwContext *context) = 0;

    virtual antlrcpp::Any visitIsKw(LogisticsLangParser::IsKwContext *context) = 0;

    virtual antlrcpp::Any visitRequirementsKw(LogisticsLangParser::RequirementsKwContext *context) = 0;

    virtual antlrcpp::Any visitProducesKw(LogisticsLangParser::ProducesKwContext *context) = 0;

    virtual antlrcpp::Any visitRequiresKw(LogisticsLangParser::RequiresKwContext *context) = 0;

    virtual antlrcpp::Any visitBinKw(LogisticsLangParser::BinKwContext *context) = 0;

    virtual antlrcpp::Any visitMachineKw(LogisticsLangParser::MachineKwContext *context) = 0;

    virtual antlrcpp::Any visitSinkKw(LogisticsLangParser::SinkKwContext *context) = 0;

    virtual antlrcpp::Any visitNothingKw(LogisticsLangParser::NothingKwContext *context) = 0;

    virtual antlrcpp::Any visitWidgets(LogisticsLangParser::WidgetsContext *context) = 0;

    virtual antlrcpp::Any visitItem(LogisticsLangParser::ItemContext *context) = 0;

    virtual antlrcpp::Any visitBin(LogisticsLangParser::BinContext *context) = 0;

    virtual antlrcpp::Any visitSink(LogisticsLangParser::SinkContext *context) = 0;

    virtual antlrcpp::Any visitRequire(LogisticsLangParser::RequireContext *context) = 0;

    virtual antlrcpp::Any visitProduce(LogisticsLangParser::ProduceContext *context) = 0;

    virtual antlrcpp::Any visitMachineSpec(LogisticsLangParser::MachineSpecContext *context) = 0;

    virtual antlrcpp::Any visitMachine(LogisticsLangParser::MachineContext *context) = 0;

    virtual antlrcpp::Any visitComponent(LogisticsLangParser::ComponentContext *context) = 0;

    virtual antlrcpp::Any visitComponents(LogisticsLangParser::ComponentsContext *context) = 0;

    virtual antlrcpp::Any visitCrossroad(LogisticsLangParser::CrossroadContext *context) = 0;

    virtual antlrcpp::Any visitCrossroads(LogisticsLangParser::CrossroadsContext *context) = 0;

    virtual antlrcpp::Any visitRoad(LogisticsLangParser::RoadContext *context) = 0;

    virtual antlrcpp::Any visitConnection(LogisticsLangParser::ConnectionContext *context) = 0;

    virtual antlrcpp::Any visitArchitecture(LogisticsLangParser::ArchitectureContext *context) = 0;

    virtual antlrcpp::Any visitRequirement(LogisticsLangParser::RequirementContext *context) = 0;

    virtual antlrcpp::Any visitRequirements(LogisticsLangParser::RequirementsContext *context) = 0;

    virtual antlrcpp::Any visitSpec(LogisticsLangParser::SpecContext *context) = 0;


};

