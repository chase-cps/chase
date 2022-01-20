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
    auto tree = parser.spec();

    visitSpec(tree);
}

antlrcpp::Any LogisticsSpecsBuilder::visitMap(
        LogisticsLangParser::MapContext *ctx) {

    map_lines = ctx->MAPLINE().size();
    map_columns = 0;
    for(auto i : ctx->MAPLINE()) {
        std::string line = i->toString();
        unsigned int linesize = line.size();
        if(map_columns == 0) map_columns = linesize;
        if(linesize != map_columns)
            messageError("Ill-formed map.");
        asciimap.push_back(line);
    }
    return LogisticsLangBaseVisitor::visitMap(ctx);
}

