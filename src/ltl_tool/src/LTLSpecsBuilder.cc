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
    _system(nullptr)
{
}

System *LTLSpecsBuilder::getSystem() const {
    return _system;
}

System *LTLSpecsBuilder::parseSpecificationFile(std::string infile)
{
    ANTLRFileStream input( infile );
    LTLContractsLexer lexer( &input );
    CommonTokenStream tokens( &lexer );

    LTLContractsParser parser( &tokens );

    tree::ParseTree * tree = parser.systemSpec();

    auto walker = new tree::ParseTreeWalker();
    walker->walk(this, tree);

    return _system;
}

void
LTLSpecsBuilder::enterSystemSpec(LTLContractsParser::SystemSpecContext *ctx)
{
    LTLContractsParser::NameContext * n_ctx = ctx->name();
    std::cout << n_ctx->ID()->toString() << std::endl;
}

LTLSpecsBuilder::~LTLSpecsBuilder() = default;
