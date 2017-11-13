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
 * @file	frontend/src/Patterns2Chase/TextualParsing/EPSTimedPatternLibrary.cc
 * @brief
 */



#include "Patterns2Chase/TextualParsing/EPSTimedPatternLibrary.hh"


using namespace Patterns2Chase;
using namespace Patterns2Chase::TextualParsing;
using namespace Specification;

using namespace Specification::EPSPatterns;

EPSTimedPatternLibrary::~EPSTimedPatternLibrary()
{
}

EPSTimedPatternLibrary::EPSTimedPatternLibrary() :
    EPSHighLevelPatternLibrary()
{
    patternSet.insert(
            new TimedKeepPowered(
                std::regex(
                    ".+ must never be unpowered for more than .+ from .+")));

    patternSet.insert(
            new TimedUnhealthySources(
                std::regex(".+ must be disconnected when unhealthy within .+")));

    patternSet.insert(
            new TimedContactors(
                std::regex("Contactors requires .+ to .+")));
}

