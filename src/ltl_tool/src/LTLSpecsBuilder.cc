/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#include "LTLSpecsBuilder.hh"

using namespace chase;
using namespace ltl_tool;
using namespace antlr4;

LTLSpecsBuilder::LTLSpecsBuilder() :
    _system(nullptr),
    _currContract(nullptr)
{
}

LTLSpecsBuilder::~LTLSpecsBuilder() = default;



System *LTLSpecsBuilder::getSystem() const {
    return _system;
}

System *LTLSpecsBuilder::parseSpecificationFile(std::string infile)
{
    ANTLRFileStream input( infile );
    LTLContractsLexer lexer( &input );
    CommonTokenStream tokens( &lexer );

    LTLContractsParser parser( &tokens );

    parser.setBuildParseTree(true);
    LTLContractsParser::SystemSpecContext * tree = parser.systemSpec();

    visitSystemSpec(tree);

    return _system;
}



antlrcpp::Any
LTLSpecsBuilder::visitSystemSpec(LTLContractsParser::SystemSpecContext *ctx) {
    LTLContractsParser::NameContext * n_ctx = ctx->name();
    if( n_ctx != nullptr )
    {
        std::string name = n_ctx->ID()->getText();
        messageInfo("Parsing the system: " + name);
        _system = new System(name);
    } else{
        messageInfo("Parsing the system: SYSTEM (default)");
        _system = new System();
    }

    return LTLContractsBaseVisitor::visitSystemSpec(ctx);
}

antlrcpp::Any
LTLSpecsBuilder::visitDeclaration(LTLContractsParser::DeclarationContext *ctx) {
    std::string name = ctx->ID()->getText();
    DataDeclaration * dec = nullptr;
    if (ctx->variableKW() != nullptr)
        dec = new Variable(new Integer(), new Name(name));
    else if (ctx->constantKW() != nullptr)
    {
        int num = std::stoi(ctx->NUMBER()->getText().c_str());
        dec = new Constant(
                new Integer(),
                new Name(name),
                new IntegerValue(num));
    }
    else if (ctx->propositionKw() != nullptr)
    {
        dec = new Variable(
                new Boolean(), new Name(name));

        Expression * exp = buildRelation(ctx->propositionValue()->relation());

        _map_props_values.insert(
                std::pair< Variable *, Expression *>(
                        reinterpret_cast<Variable *>(dec), exp));
    }
    else
        messageError("Unknown entry:\t" + ctx->getText());

    if( _currContract != nullptr )
        _currContract->addDeclaration(dec);
    else
        _system->addDeclaration(dec);

    return antlrcpp::Any();
}

Expression *
LTLSpecsBuilder::buildRelation(LTLContractsParser::RelationContext *ctx)
{

}







