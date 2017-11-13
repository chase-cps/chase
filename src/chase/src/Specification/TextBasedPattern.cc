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
 * @file	chase/src/Specification/TextBasedPattern.cc
 * @brief
 */



#include "Specification/TextBasedPattern.hh"

using namespace Specification;

TextBasedPattern::TextBasedPattern() :
    _regex("*")
{
}

TextBasedPattern::TextBasedPattern( std::regex regexp ) :
    _regex(regexp)
{
}

TextBasedPattern::~TextBasedPattern()
{
}

bool TextBasedPattern::matchPattern(std::string text)
{
    return regex_match(text, _regex);
}

void TextBasedPattern::setPatternRegExp(std::regex regexp)
{
    _regex = regexp;
}

std::regex TextBasedPattern::getPatternRegExp()
{
    return _regex;
}

std::vector< std::string > TextBasedPattern::extractFormulae( std::string text )
{
    return this->_extractFormulae( text );
}

/// \todo It should be changed, in order to receive string rather than
/// formulas. Then, it should be able to create formulas. This is necessary
/// to support other kinds of patterns that does not use logic forumalae
/// (e.g., higher level EPS patterns).
void TextBasedPattern::createConstraints( 
        Contracts::AGContract * contract,
        std::vector< Behaviors::Logics::WellFormedFormula * > formulae )
{
    this->_createConstraints( contract, formulae );
}


