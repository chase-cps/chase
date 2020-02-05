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
        Console( System * system );

        /// @brief The main function of the console.
        /// @return 0 when the console finished. 1 Otherwise.
        int run();

    protected:
        System * _system;

        /// @brief Method parsing the command different than "show" and "exit".
        /// @param cmd The command to be parsed.
        /// @return 1 if the command has been executed properly. 0 otherwise.
        int _execCommand( std::string cmd );

        void _parseProjectionMap( names_projection_map & m );
    };
}