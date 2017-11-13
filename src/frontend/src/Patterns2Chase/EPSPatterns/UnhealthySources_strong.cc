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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/UnhealthySources_strong.cc
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

// UNHEALTHY SOURCES
std::vector< std::string > UnhealthySourcesStrong::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;
    std::string pattern(" must be safely disconnected when unhealthy");
    std::size_t _begin = text.find(pattern);

    std::string components = text.substr(0,_begin);
    ret.push_back(components);
    return ret;
}

std::vector< WellFormedFormula * > UnhealthySourcesStrong::_createPathFormulae(  
        Contracts::AGContract *,
        std::vector< std::string > strings )
{
    std::vector< int > * nodes = 
        HighLevelEPSPattern::_getNodesByType( strings[0], _topology );

    std::vector< WellFormedFormula * > to_return;

    for( size_t i = 0; i < nodes->size(); ++i )
    {
        to_return.push_back( _disconnectNode(nodes->at(i), _topology));
        std::vector< WellFormedFormula * > wffs =
            _waitDisconnection(nodes->at(i), _topology);

        for( size_t j = 0; j < wffs.size(); ++j )
        {
            to_return.push_back(wffs[j]);
        }

    }
    return to_return;
}


void UnhealthySourcesStrong::_createConstraints(
        Contracts::AGContract * contract,
        std::vector< Behaviors::Logics::WellFormedFormula * > formulae )
{
    WellFormedFormula * guarantees = dynamic_cast< WellFormedFormula * >(
            contract->guarantees );

    if( guarantees == NULL )
    {
        std::cerr << "[ERROR]\n"
            << "The behaviors are not expressed using logics in this contract."
            << std::endl;
        exit(-1);
    }

    for( size_t i = 0; i < formulae.size(); ++i )
    {
        UnaryTemporalFormula * globally = new UnaryTemporalFormula(
                        op_globally,
                        formulae[i]);

        guarantees = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination(
                    op_land,
                    static_cast< WellFormedFormula *> (contract->guarantees), 
                    globally ));

        contract->guarantees = static_cast< Behavior * >( guarantees );

    }

}


/////////////////////////////////////////////////


