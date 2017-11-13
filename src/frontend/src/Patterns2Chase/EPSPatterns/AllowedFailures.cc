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
 * @file	frontend/src/Patterns2Chase/EPSPatterns/AllowedFailures.cc
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

std::vector< std::string > AllowedFailures::_extractFormulae( std::string text )
{
    std::vector< std::string > ret;

    std::string pattern(" failures are allowed for the ");
    int pattern_length = pattern.length();

    std::size_t _begin = text.find(pattern);
    std::size_t _end = _begin + pattern_length;

    std::string number = text.substr(0, _begin);
    std::string components = text.substr(_end);

    ret.push_back(number);
    ret.push_back(components);

    return ret;

}

std::vector< WellFormedFormula * > AllowedFailures::_createPathFormulae( 
        Contracts::AGContract * contract,
        std::vector< std::string > strings )
{

    std::vector< WellFormedFormula * > ret;

    int allowedFailures = atoi(strings[0].c_str());
    std::string components = strings[1];

    std::vector< int > * nodes = _getNodesByType( components, _topology );

    if( allowedFailures == 0 )
    {
        for( size_t i = 0; i < nodes->size(); ++i )
        { 
            Variable * v = _topology->components[nodes->at(i)]->stateVariable;
            contract->environment_variables.remove(v);
            _topology->components[nodes->at(i)]->stateVariable = NULL;
        }
        return ret;
    }

    std::list< std::vector<int> > results;
    std::vector< int > first;
    int safe_components = nodes->size() - allowedFailures;

    HighLevelEPSPattern::getSubsetBySize( 
            *nodes, nodes->size(), safe_components, 0, first, results );

    std::list< std::vector<int> >::iterator lit;
    for( lit = results.begin(); lit != results.end(); ++lit )
    {
        std::vector<int> currv = *lit;
        WellFormedFormula * wff = static_cast< WellFormedFormula * >(
                _topology->components[currv[0]]->stateVariable);
        for( size_t i = 1; i < currv.size(); ++i )
        {
            if( _topology->components[currv[i]]->stateVariable != NULL )
                wff = static_cast< WellFormedFormula * >(
                        new BinaryLogicCombination(
                            op_land,
                            wff,
                            _topology->components[currv[i]]->stateVariable));
        }
        ret.push_back(wff);
    }

    return ret;
}




void AllowedFailures::_createConstraints(
        Contracts::AGContract * contract,
        std::vector< Behaviors::Logics::WellFormedFormula * > formulae )
{

    if( formulae.size() > 0 )
    {
        WellFormedFormula * assumptions = dynamic_cast< WellFormedFormula * >(
                contract->assumptions );

        if( assumptions == NULL )
        {
            std::cerr << "[ERROR]\n"
                << "The behaviors are not expressed using logics in this contract."
                << std::endl;
            exit(-1);
        }


        WellFormedFormula * wff = static_cast< WellFormedFormula * >(
                formulae[0]);

        for(size_t i = 1; i < formulae.size(); ++i )
        {


            wff = static_cast< WellFormedFormula * >(
                    new BinaryLogicCombination( op_lor, wff, formulae[i] ));
        }

        wff = static_cast< WellFormedFormula * >(
                new UnaryTemporalFormula( op_globally, wff ));

        assumptions = static_cast< WellFormedFormula * >(
                new BinaryLogicCombination( 
                    op_land, 
                    static_cast< WellFormedFormula *>(contract->assumptions),
                    wff ));

        contract->assumptions = static_cast< Behavior * >( assumptions );
    }

}

    std::vector< std::string > 
NoFailuresAllowed::_extractFormulae( std::string text )
{
    // Implement this as a particular case of the previous Pattern.
    std::vector< std::string > ret;

    std::string pattern(" are never failing");

    std::size_t _begin = text.find(pattern);

    std::string components = text.substr(0, _begin);
    std::string number("0");

    ret.push_back(number);
    ret.push_back(components);

    return ret;


}




