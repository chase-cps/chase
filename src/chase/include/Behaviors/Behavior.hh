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
 * @file	chase/include/Behaviors/Behavior.hh
 * @brief
 */



// Behavior.hh
//


/// @author <a href="mailto:michele.lora@univr.it">Michele Lora </a>
/// @version 1.0

#ifndef BEHAVIOR_HH
#define BEHAVIOR_HH

#include <string>

// Library includes
#include "Manipulation/Visitable.hh"
#include "Manipulation/BaseVisitor.hh"
#include "AST/BaseObject.hh"

namespace Behaviors
{
    /// @brief Abstract class for behavior classes (e.g., WellFormedFomula)
    ///
    class Behavior : public Manipulation::Visitable, public chase::BaseObject
    {
        public:
            /// @brief Print the behavior specification represented by the instance in the default fashion.
            /// @return the string to print, formatted as default, for the behavior.
            virtual std::string print() = 0;

            /// @brief base method for the visit using a BaseVisitor.
            /// @see BaseVisitor
            virtual int accept_visitor( Manipulation::BaseVisitor & v ) = 0;
    };
}

#endif // BEHAVIOR_HH
