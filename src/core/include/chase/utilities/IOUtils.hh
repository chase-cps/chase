/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#pragma once

#include "representation/forwards.hh"
#include <iostream>
#include <string>
#include <ctime>
#include <string.h>

namespace chase {

    /// @brief Base function for all the messages.
    /// @param msg (C) string to be printed.
    /// @param object Object of the AST subject of the message.
    /// @param str Stream to use.
    void baseMessage(
            const char * msg,
            ChaseObject * object,
            const std::ostream & str 
            );

    /// @brief Function for all the error messages.
    /// @param msg (C) string to be printed.
    /// @param object Object of the AST causing the Error.
    void messageError(
            const char * msg,
            ChaseObject * object = nullptr );

    /// @brief Function for all the Information messages.
    /// @param msg (C) string to be printed.
    /// @param object Object of the AST to which the information refers to.
    void messageInfo(
            const char * msg,
            ChaseObject * object = nullptr );

    /// @brief Function for all the Warning messages.
    /// @param msg (C) string to be printed.
    /// @param object Object of the AST causing the warning.
    void messageWarning(
            const char * msg,
            ChaseObject * object = nullptr );

    /// @brief Function for all the error messages.
    /// @param msg (C) string to be printed.
    /// @param object Object of the AST causing the Error.
    void messageError(
            std::string msg,
            ChaseObject * object = nullptr );

    /// @brief Function for all the Information messages.
    /// @param msg (C) string to be printed.
    /// @param object Object of the AST to which the information refers to.
    void messageInfo(
            std::string msg,
            ChaseObject * object = nullptr );

    /// @brief Function for all the Warning messages.
    /// @param msg (C) string to be printed.
    /// @param object Object of the AST causing the warning.
    void messageWarning(
            std::string msg,
            ChaseObject * object = nullptr );

}

