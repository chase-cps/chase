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
 * @file	frontend/src/Patterns2Chase/patterns2chase.cc
 * @brief
 */



#include <iostream>

#include "Patterns2Chase/TextualParsing/TextualParsing.hh"

using namespace Patterns2Chase;
using namespace Patterns2Chase::TextualParsing;

int main(int argc, char * argv[])
{
    if( argc < 2 )
    {
        std::cerr << "[Error] Bad Usage:" << std::endl
            << "Usage: patterns2chase file_name" <<std::endl;
        exit(-1);
    }
    std::string filename(argv[1]);

    EPSPatternLibrary * library = new EPSPatternLibrary();

    Contracts::AGContract * c = 
        TextualParsing::Parsing::parseFile(
            filename, library);

    std::cout << c->print() << std::endl;

    return 1;
}
