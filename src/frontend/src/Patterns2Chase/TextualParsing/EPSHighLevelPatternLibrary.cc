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
 * @file	frontend/src/Patterns2Chase/TextualParsing/EPSHighLevelPatternLibrary.cc
 * @brief
 */



#include "Patterns2Chase/TextualParsing/EPSHighLevelPatternLibrary.hh"

using namespace Patterns2Chase;
using namespace Patterns2Chase::TextualParsing;
using namespace Specification;

using namespace Specification::EPSPatterns;

EPSHighLevelPatternLibrary::~EPSHighLevelPatternLibrary()
{
}

EPSHighLevelPatternLibrary::EPSHighLevelPatternLibrary()
{
    patternSet.insert(
            new NoParallel(
                std::regex(".+ are never connected in parallel through .+")));

    patternSet.insert(
            new UnhealthySources(
                std::regex(".+ must be disconnected when unhealthy")));

    patternSet.insert(
            new UnhealthySourcesStrong(
                std::regex(".+ must be safely disconnected when unhealthy")));


    patternSet.insert(
            new KeepConnected(
                std::regex(".+ must never be disconnected from .+")));

    patternSet.insert(
            new KeepPowered(
                std::regex(".+ must never be unpowered from .+")));


    patternSet.insert(
            new KeepBalanced(
                std::regex(".+ should be powered by .+")));

    patternSet.insert(
            new AllowedFailures(
                std::regex(".+ failures are allowed for the .+")));

    patternSet.insert(
            new NoFailuresAllowed(
                std::regex(".+ are never failing")));

    patternSet.insert(
            new PermanentFailures(
                std::regex(".+ do not recover after failures"))); 
}

