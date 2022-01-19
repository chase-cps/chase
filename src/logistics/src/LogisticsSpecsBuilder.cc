/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/18/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <utility>

#include "../include/LogisticsSpecsBuilder.hh"
#include "utilities/Factory.hh"

using namespace chase;
using namespace antlr4;


item::item(double rate, std::string n) :
    rate(rate), name(std::move(n)) {}

Equipment::Equipment(std::string name) : name(std::move(name)) {}

const std::string &Equipment::getName() const {return name;}

void Equipment::setName(const std::string &n) {name = n;}

Bin::Bin(const std::string &name) : Equipment(name) {}
Sink::Sink(const std::string &name) : Equipment(name) {}
Machine::Machine(const std::string &name) : Equipment(name) {}

LogisticsSpecsBuilder::LogisticsSpecsBuilder() = default;
LogisticsSpecsBuilder::~LogisticsSpecsBuilder() = default;

void LogisticsSpecsBuilder::parseSpecificationFile(const std::string& infile) {
    ANTLRFileStream input( infile );
    LogisticsLangLexer lexer( &input );
    CommonTokenStream tokens( &lexer );

    LogisticsLangParser parser( &tokens );

    parser.setBuildParseTree(true);
    LogisticsLangParser::SpecContext * tree = parser.spec();

    visitSpec(tree);
}

antlrcpp::Any LogisticsSpecsBuilder::visitWidgets(
        LogisticsLangParser::WidgetsContext *ctx) {
    for (size_t i = 0; i < ctx->ID().size(); ++i)
        widgets.emplace_back(ctx->ID()[i]->getText());
    return LogisticsLangBaseVisitor::visitWidgets(ctx);
}

antlrcpp::Any LogisticsSpecsBuilder::visitMachine(
        LogisticsLangParser::MachineContext *ctx) {
    auto machine = new Machine(ctx->ID()->getText());
    auto req = ctx->machineSpec()->require();
    for(auto it : req->item())
        machine->required.push_back(new item(
                std::atof(it->NUMBER()->getText().c_str()),
                it->ID()->getText()));
    auto prod = ctx->machineSpec()->produce();
    for(auto it : prod->item())
        machine->produced.push_back(new item(
                std::atof(it->NUMBER()->getText().c_str()),
                it->ID()->getText()));
    equipment.push_back(machine);
    return LogisticsLangBaseVisitor::visitMachine(ctx);
}

antlrcpp::Any LogisticsSpecsBuilder::visitSink(
        LogisticsLangParser::SinkContext *ctx) {
    auto sink = new Sink(ctx->ID()->getText());
    for(auto i : ctx->item())
        sink->items.push_back(new item(
                std::atof(i->NUMBER()->getText().c_str()),
                i->ID()->getText()));
    equipment.push_back(sink);
    return LogisticsLangBaseVisitor::visitSink(ctx);
}

antlrcpp::Any LogisticsSpecsBuilder::visitBin(
        LogisticsLangParser::BinContext *ctx) {
    auto bin = new Bin(ctx->ID()->getText());
    for(auto i : ctx->item())
        bin->items.push_back(new item(
                std::atof(i->NUMBER()->getText().c_str()),
                i->ID()->getText()));
    equipment.push_back(bin);
    return LogisticsLangBaseVisitor::visitBin(ctx);
}

antlrcpp::Any LogisticsSpecsBuilder::visitCrossroad(
        LogisticsLangParser::CrossroadContext *ctx) {
    crossroads.push_back(
            new item(
                    std::atof(ctx->NUMBER()->getText().c_str()),
                    ctx->ID()->getText()));
    return LogisticsLangBaseVisitor::visitCrossroad(ctx);
}

