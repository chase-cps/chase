/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "Chase.hh"

using namespace chase;

namespace ltl_tool
{
    /// @brief The Console class defines objects that provide to the end-user
    /// a textual interface to manipulate a System specifying LTL contracts.
    class Console
    {
    public:
        /// @brief Constructor of the console object.
        /// @param system System to be manipulated by the console.
        Console( System * system, std::string &outDir );

        /// @brief The main function of the console.
        /// @param cmd The command being read.
        /// @return 0 when the console finished. 1 Otherwise.
        int run(std::string cmd = std::string(""));

    protected:
        System * _system;
        std::string _outDir;

        /// @brief Method parsing the command different than "show" and "exit".
        /// @param cmd The command to be parsed.
        /// @return 1 if the command has been executed properly. 0 otherwise.
        int _execCommand( std::string cmd );

        /// @brief Method executing the conjunction command.
        /// @param tokens Tokens of the command.
        /// @return 1 if the command has been executed properly, 0 otherwise.
        int _execConjunction( std::vector< std::string >& tokens );

        /// @brief Method executing the composition command.
        /// @param tokens Tokens of the command.
        /// @return 1 if the command has been executed properly, 0 otherwise.
        int _execComposition( std::vector< std::string >& tokens );

        /// @brief Method executing the saturation command.
        /// @param cmd String of the command.
        /// @return 1 if the command has been executed properly, 0 otherwise.
        int _execSaturation(std::vector< std::string >& tokens );


        /// @param tokens Tokens of the command.
        /// @return 1 if the command has been executed properly, 0 otherwise.
        int _execSynthesis( std::vector< std::string >& tokens );

        /// @brief Method generating the files necessary to verify
        /// consistency and compatibility of a given contract.
        /// @param tokens Tokens of the command.
        /// @return 1 if the command has been executed properly, 0 otherwise.
        int _execVerification( std::vector< std::string >& tokens );

        /// @brief Method generating the files necessary to check a refinement
        /// relation between two contracts.
        /// @param tokens Tokens of the command.
        /// @return 1 if the command has been executed properly, 0 otherwise.
        int _checkRefinement( std::vector< std::string >& tokens );

        /// @brief Method computing the Quotient between two contracts.
        /// relation between two contracts.
        /// @param tokens Tokens of the command.
        /// @return 1 if the command has been executed properly, 0 otherwise.
        int _computeQuotient( std::vector< std::string >& tokens );

        /// @brief Method showing the system or a contract.
        /// @param tokens Tokens of the command.
        /// @return 1 if the command has been executed properly, 0 otherwise.
        int _execShow( std::vector< std::string >& tokens );

        /// @brief Function parsing the projection map.
        /// @param m Projection map to be parsed.
        /// @param mode Mode of the creation.
        void _createProjectionMap(
                names_projection_map & m, std::string mode,
                Contract * c1 = nullptr, Contract * c2 = nullptr);

        /// @brief Function parsing the projection map given manually.
        /// @param m Projection map to be parsed.
        void _createProjectionMapManual(names_projection_map & m );

        /// @brief Function creating the projection map by name matching.
        /// @param m Projection map to be parsed.
        void _createProjectionMapName(
                names_projection_map & m, Contract * c1, Contract * c2);

        /// @brief Function parsing a file containing the projection map.
        /// @param m Projection map to be parsed.
        /// @param file File to be parsed to create the map.
        void _createProjectionMapFile(
                names_projection_map & m, std::string file );

        /// @brief Procedure printing the help of the console.
        void _printHelp(std::string cmd = std::string(""));
    };
}