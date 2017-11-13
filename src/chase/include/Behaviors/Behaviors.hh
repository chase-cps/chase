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
 * @file	chase/include/Behaviors/Behaviors.hh
 * @brief
 */



// Behaviors.hh
//

/// @author <a href="mailto:michele.lora@univr.it">Michele Lora </a>
/// @version 1.0

#ifndef BEHAVIORS_HH
#define BEHAVIORS_HH

// Include the class composing the namespace Behavior.
#include "Behaviors/Behavior.hh"

namespace Behaviors
{    

    /// @brief Enumeration of allowed datatypes for the variables of a system.
    /// \todo 
    /// improve type system, moving to classes?
    enum vartype{
        t_bool,
        t_int,
        t_real,
        t_unvalid
    };

}

#endif // BEHAVIORS_HH
