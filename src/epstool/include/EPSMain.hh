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
 * @file	chase/src/Architecture/EPS/EPSMain.cc
 * @brief   header of the main file for the tool implementing the EPS demo.
 */



#include "EPS/EPSTopology.hh"
#include "Utilities/Graphs/Graphs.hh"

#include "Patterns2Chase/TextualParsing/TextualParsing.hh"

#include "GR1/GR1Builder.hh"
#include "Chase2TuLiP/SpinSyntaxWriter.hh"
#include "Chase2TuLiP/TuLiPPrinter.hh"
#include "manipulation/ResolveCounters.hh"

#include "Manipulation/Manipulation.hh"

#include <unistd.h>

namespace chase { namespace epstool {
    typedef struct _params
    {
        std::string fileInBehavior;
        std::string fileInArchitecture;
        std::string fileOut;
        bool physical;
        bool verbose;

        _params() :
            fileInBehavior("behavior.txt"),
            fileInArchitecture("architecture.txt"),
            fileOut("EPSTool_out.py"),
            physical(false)
        {
        }

    } Params;


    Params * parseCmdLine( int argc, char * argv[] );

    void printHelp();


}}
