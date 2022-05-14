/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/18/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include <utility>

#include "../include/LogisticsSpecsBuilder.hh"
#include "utilities/Factory.hh"
#include "Z3Printer.hh"

using namespace chase;
using namespace antlr4;



LogisticsSpecsBuilder::LogisticsSpecsBuilder(chase::Params * params) :
    map_lines(0),
    map_columns(0),
    warehouse(nullptr),
    _roads(0),
    _xroads(0),
    _forums(0),
    _stations(0),
    _graph(nullptr),
    _params(params)
{
//    _crossroads2Nodes = new std::map< Crossroad *, chase::Vertex * >();
//    _nodes2Crossroads = new std::map< chase::Vertex *, Crossroad * >();
//    _bays2Nodes = new std::map< Bay *, chase::Vertex * >();
//    _nodes2Bays = new std::map< chase::Vertex *, Bay * >()
}

LogisticsSpecsBuilder::~LogisticsSpecsBuilder() = default;

void LogisticsSpecsBuilder::parseSpecificationFile(const std::string& infile) {
    ANTLRFileStream input( infile );
    LogisticsLangLexer lexer( &input );
    CommonTokenStream tokens( &lexer );

    LogisticsLangParser parser( &tokens );

    warehouse = new Warehouse();

    parser.setBuildParseTree(true);
    auto tree = parser.spec();

    messageInfo("Phase 1: parsing of the specification.");
    visitSpec(tree);

    messageInfo("Phase 2: build facility model.");
    buildWarehouseModel();

    // Populate the roads with products.
    _productsToRoad();

    messageInfo("Phase 3: formalize facility model.");
    buildGraph();

    _createContracts();

    // _composeWarehouse(); // Bypass it, go to printer.
    Z3Printer printer(warehouse);
}

antlrcpp::Any LogisticsSpecsBuilder::visitMap(
        LogisticsLangParser::MapContext *ctx) {

    map_lines = ctx->MAPLINE().size();
    map_columns = 0;
    for(auto i : ctx->MAPLINE()) {
        std::string line = i->toString();
        unsigned linesize = line.size();
        if(map_columns == 0) map_columns = linesize;
        if(linesize != map_columns)
            messageError("Ill-formed map.");
        asciimap.push_back(line);
    }
    return LogisticsLangBaseVisitor::visitMap(ctx);
}

antlrcpp::Any LogisticsSpecsBuilder::visitProduct(
        LogisticsLangParser::ProductContext *ctx) {
    std::string name(ctx->ID()->toString());

    auto product = new Product();
    product->name = name;

    for( auto it : ctx->triple() )
    {
        char * pEnd;
        unsigned xpos = std::strtoul(
                it->xpos()->NUMBER()->getText().c_str(), &pEnd, 10);
        unsigned ypos = std::strtoul(
                it->ypos()->NUMBER()->getText().c_str(), &pEnd, 10);
        unsigned units = std::strtoul(
                it->units()->NUMBER()->getText().c_str(), &pEnd, 10);
        auto pos = new Position(xpos, ypos, units);
        product->positions.push_back(pos);
    }

    warehouse->products.push_back(product);
    return LogisticsLangBaseVisitor::visitProduct(ctx);
}

antlrcpp::Any
LogisticsSpecsBuilder::visitDestination(
        LogisticsLangParser::DestinationContext *ctx) {
    std::string name(ctx->location()->ID()->getText());
    char * pEnd;
    unsigned time(
            std::strtoul(
                    ctx->time()->NUMBER()->getText().c_str(),
                    &pEnd, 10));
    auto destination = new Destination(name, time);

    for(auto it : ctx->request())
    {
        unsigned req = std::strtoul(
                it->NUMBER()->toString().c_str(), &pEnd, 10);
        std::pair< std::string, unsigned > p(it->ID()->getText(), req);
        destination->requests.insert(p);
    }

    return LogisticsLangBaseVisitor::visitDestination(ctx);
}

void LogisticsSpecsBuilder::_productsToRoad() const {
    for(auto product : warehouse->products) {
        for(auto position : product->positions) {
            auto j = position->xpos;
            auto i = position->ypos;
            auto quantity = position->quantity;
            if(_components[i][j+1] != nullptr &&
                _components[i][j+1]->type == road ) j = j + 1;
            else if(_components[i][j-1] != nullptr &&
                _components[i][j-1]->type == road ) j = j - 1;
            else messageError("Product not in shelved road: " +
                    std::to_string(i) + ", " + std::to_string(j));
            auto road = reinterpret_cast< Road * >(_components[i][j]);
            auto it = road->quantities.find(product);
            if(it != road->quantities.end()) {
                it->second = it->second + quantity;
            } else {
                road->quantities.insert(
                        std::pair< Product *, unsigned long >(product, quantity));
            }
        }
    }
}
