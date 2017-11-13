/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2017 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2017 by University of Verona.\n
 *              Copyright (c) 2015-2017 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2017 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	frontend/src/Patterns2Chase/EPSPatterns/PermanentFailures.cc
 * @brief
 */



#include "Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.hh"

#include "Patterns2Chase/TextualParsing/AorGVisitor.hh"

using namespace Specification;
using namespace EPSPatterns;
using namespace Behaviors;
using namespace Behaviors::Logics;
using namespace Patterns2Chase::TextualParsing;
using namespace Contracts;
using namespace chase::epstool::EPS;

using namespace Utilities;
using namespace Graphs;

std::vector< std::string > PermanentFailures::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;

    std::string pattern(" do not recover after failures");

    std::size_t _begin = text.find(pattern);

    std::string components = text.substr(0, _begin);

    ret.push_back(components);

    return ret;

}

std::vector< WellFormedFormula * > PermanentFailures::_createPathFormulae( 
        Contracts::AGContract *,
        std::vector< std::string > strings )
{

    std::vector< WellFormedFormula * >ret;

    std::string components = strings[0];

    std::vector< int > * nodes = _getNodesByType(components, _topology );

    for(size_t i = 0; i < nodes->size(); ++i )
    {
        int node = nodes->at(i);
        WellFormedFormula * wff = static_cast< WellFormedFormula * >(
                _topology->components[node]->stateVariable );

        UnaryLogicCombination * broken = 
            new UnaryLogicCombination(op_lnot, wff);

        UnaryTemporalFormula * next = new UnaryTemporalFormula(op_next, broken);

        wff = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( op_implies, broken, next ));

        ret.push_back(wff);

    }

    return ret;

}




void PermanentFailures::_createConstraints(
        Contracts::AGContract * contract,
        std::vector< Behaviors::Logics::WellFormedFormula * > formulae )
{
    WellFormedFormula * assumptions = static_cast< WellFormedFormula * >(
            contract->assumptions );

    if( assumptions == NULL )
    {
        std::cerr << "[ERROR]\n"
            << "The behaviors are not expressed using logics in this contract."
            << std::endl;
        exit(-1);
    }


    contract->assumptions = static_cast< Behavior * >(
            new BinaryLogicCombination(
                op_land,
                assumptions,
                new UnaryTemporalFormula( op_globally, formulae[0])));

    for( size_t i = 1; i < formulae.size(); ++i )
    {
        assumptions = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    static_cast< WellFormedFormula * >(contract->assumptions),
                    new UnaryTemporalFormula( op_globally, formulae[i])));

        contract->assumptions = static_cast< Behavior * >( assumptions );
    }

}


