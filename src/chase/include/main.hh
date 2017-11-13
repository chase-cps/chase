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
 * @file	chase/main.hh
 * @brief   Header file for the entry point of the chase main tool.
 */


#ifndef CHASE_MAIN_HH
#define CHASE_MAIN_HH



#include "DSLParser/SpecBuilder.hh"
#include "Specification/BaseFunctionsLibrary/BaseFunctionsLibrary.hh"
#include "GR1/GR1Builder.hh"
#include "Chase2TuLiP/SpinSyntaxWriter.hh"
#include "Chase2TuLiP/TuLiPPrinter.hh"
#include "manipulation/ResolveCounters.hh"
#include "Manipulation/Manipulation.hh"
#include "Chase2GR1C/GR1CPrinter.hh"

#include <unistd.h>

namespace chase
{

    /**
     * @brief Structure saving all the input console parameters.
     */
    typedef struct _params
    {
        /// @brief Input file path.
        std::string fileIn;
        /// @brief Output file path.
        std::string fileOut;
        /// @brief Verbose flag. Execution is verbose if flag is true.
        bool verbose;

        /// @brief Flag specifing the exact back-end to use.
        // Default value: tulip
        // The possible values are "tulip", "gr1c"
        std::string backend;

        /// @brief Standard constructor.
        _params() :
            fileIn("chase_spec.txt"),
            fileOut(""),
            verbose(false),
            backend("gr1c")
        {
        }
    } Params;


    /// @brief Method printing usage banner of chase.
    void printHelp();

    /// @brief Method parsing the input command line.
    Params * parseCmdLine( int argc, char * argv[] );


    /// @brief Managing of chase messages.
    /// @param p        Pointer to the structure storing the command line parameters.
    /// @param msg      Message to print.
    /// @param colored  Flag to enable colors. If true messages are colored.
    void chaseMessage( Params * p, std::string msg, bool colored=false ); 

}


#endif // CHASE_MAIN_HH
