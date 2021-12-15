/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/10/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "CoCoDeSpecsBuilder.hh"

using namespace chase;
using namespace antlr4;

CoCoDeSpecsBuilder::CoCoDeSpecsBuilder() {
}

System * CoCoDeSpecsBuilder::parseSpecificationFile(std::string infile)
{
    ANTLRFileStream input(infile);
    CoCoDeLangLexer lexer(&input);
    CommonTokenStream tokens(&lexer);

    CoCoDeLangParser parser(&tokens);

    parser.setBuildParseTree(true);
    CoCoDeLangParser::DeclarationsContext * tree = parser.declarations();

    CoCoDeLangBaseVisitor::visitDeclarations(tree);

}


CoCoDeSpecsBuilder::~CoCoDeSpecsBuilder() = default;