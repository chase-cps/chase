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
 * @file	frontend/include/Patterns2Chase/TextualParsing/TextualParsing.hh
 * @brief
 */



#ifndef TEXTUAL_PARSING_HH
#define TEXTUAL_PARSING_HH

#include "Patterns2Chase/TextualParsing/EPSPatternLibrary.hh"
#include "Contracts/AGContract.hh"
#include "Behaviors/Variable.hh"
#include "Behaviors/Logics/Logics.hh"
#include "Patterns2Chase/TextualParsing/EPSHighLevelPatternLibrary.hh"
#include "Patterns2Chase/TextualParsing/EPSTimedPatternLibrary.hh"
#include "EPS/EPSTopology.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Utilities/Portability/Portability.hh"

namespace Patterns2Chase
{
    namespace TextualParsing
    {

        struct FileSections{
            std::list< std::string > _env;
            std::list< std::string > _sys;
            std::list< std::string > _req;

            FileSections();
            ~FileSections();
        };


        class Parsing
        {   
            public:
 
                static Contracts::AGContract * parseEPSFile(
                        std::string file_path,
                        PatternLibrary * library,
                        Contracts::AGContract * contract,
                        chase::epstool::EPS::EPSTopology * topology );


            protected:
                static FileSections _splitFile(std::string filePath); 

                static Contracts::AGContract * _analyzeDeclarations(
                        Contracts::AGContract * contract,
                        std::list< Behaviors::Variable * > * varlist,
                        std::list< std::string > * env );

                static Behaviors::Variable * _analyzeVariableDeclaration(
                        std::string declaration );

                static Behaviors::vartype _identifyType( std::string type );


        };
    }
}

#endif // TEXTUAL_PARSING_HH
