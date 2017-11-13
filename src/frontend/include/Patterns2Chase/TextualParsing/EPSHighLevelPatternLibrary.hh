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
 * @file	frontend/include/Patterns2Chase/TextualParsing/EPSHighLevelPatternLibrary.hh
 * @brief
 */



#ifndef EPS_HIGH_LEVEL_PATTERN_LIBRARY_HH
#define EPS_HIGH_LEVEL_PATTERN_LIBRARY_HH

#include "Patterns2Chase/TextualParsing/PatternLibrary.hh"
#include "Patterns2Chase/EPSPatterns/HighLevelEPSPatterns.hh"

namespace Patterns2Chase
{
    namespace TextualParsing
    {
        class EPSHighLevelPatternLibrary : public PatternLibrary
        {
            public:
                EPSHighLevelPatternLibrary();
                ~EPSHighLevelPatternLibrary();

        };
    }
}

#endif // EPS_HIGH_LEVELPATTERN_LIBRARY_HH
