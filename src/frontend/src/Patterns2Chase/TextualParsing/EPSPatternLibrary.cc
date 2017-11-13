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
 * @file	frontend/src/Patterns2Chase/TextualParsing/EPSPatternLibrary.cc
 * @brief
 */



#include "Patterns2Chase/TextualParsing/EPSPatternLibrary.hh"

using namespace Patterns2Chase;
using namespace Patterns2Chase::TextualParsing;
using namespace Specification;

using namespace Specification::EPSPatterns;

EPSPatternLibrary::~EPSPatternLibrary()
{
}

EPSPatternLibrary::EPSPatternLibrary()
{
    // X start with {true|false}
    patternSet.insert( 
            new Pattern_1(
                std::regex(".+ starts with (true|false)")));

    // Always X
    patternSet.insert(
            new  Pattern_2(
                std::regex("always .+")));

    // if system sensed X then do Y
    patternSet.insert(
            new  Pattern_3(
                std::regex("if system sensed .+ then do .+")));

    // if system is sensing X then do Y
    patternSet.insert(
            new  Pattern_4(
                std::regex("if system is sensing .+ then do .+")));

    // if system activated X and sensed Y do Z
    patternSet.insert(
            new  Pattern_5(
                std::regex("if system activated .+ and sensed .+ do .+")));

    // do X if and only if system is sensing Y
    patternSet.insert(
            new  Pattern_6(
                std::regex("do .+ if and only if system is sensing .+")));

}

