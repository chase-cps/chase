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
 * @file	frontend/src/Patterns2Chase/TextualParsing/PatternLibrary.cc
 * @brief
 */



#include "Patterns2Chase/TextualParsing/PatternLibrary.hh"

using namespace Patterns2Chase;
using namespace Patterns2Chase::TextualParsing;
using namespace Specification;

TextBasedPattern * PatternLibrary::matchPattern( std::string text )
{
    std::set< Specification::TextBasedPattern * >::iterator it;
    for( it = patternSet.begin();
         it != patternSet.end() ;
         ++it )
    {
        TextBasedPattern * p = *it;
        bool matched = p->matchPattern(text);
        if( matched )
        {
            return p;
        }
    }
    return NULL;
}

