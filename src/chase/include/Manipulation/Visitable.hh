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
 * @file	chase/include/Manipulation/Visitable.hh
 * @brief
 */



// Visitable.hh
//

/// @author <a href="mailto:michele.lora@univr.it">Michele Lora</a>
/// @version 1.0


#ifndef VISITABLE_HH
#define VISITABLE_HH

#include "Manipulation/BaseVisitor.hh"

namespace Manipulation
{
    /// @brief Abstract call for any visitable object. Any visitable object must
    /// implement the accept_visitor function.
    /// @see GuideVisitor, BaseVisitor
    class Visitable
    {
        public:
            virtual int accept_visitor( Manipulation::BaseVisitor & v ) = 0;
    };
}

#endif // VISITABLE_HH
