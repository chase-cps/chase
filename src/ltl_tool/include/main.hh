/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "Chase.hh"

#include "LTLSpecsBuilder.hh"
#include "Console.hh"

#include <unistd.h>
#include <fstream>

namespace ltl_tool
{
    /// @brief Structure saving all the input console parameters.
    typedef struct _params
    {
        /// @brief Input file path.
        std::string fileIn;
        /// @brief Commands file path.
        std::string cmdFile;

        /// @brief Verbose flag. Execution is verbose when flag is set to true.
        bool verbose;

        _params() :
                fileIn("chase_spec.txt"),
                cmdFile(""),
                verbose(false)
        {
        }
    } Params;

    /// @brief Method parsing the input command line.
    /// @param argc The number of parameters to parse.
    /// @param argv The array of parameters.
    /// @return A structure storing all the parameters sorted out.
    Params * parseCmdLine( int argc, char * argv[] );

    /// @brief Method printing usage banner of the tool.
    void printHelp();
}
