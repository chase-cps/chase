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
 * @file	chase/include/Behaviors/Logics/Term.hh
 * @brief
 */



#ifndef TERM_HH
#define TERM_HH

#include "Manipulation/Visitable.hh"

namespace Behaviors
{
    namespace Logics
    {
        class Term : public Manipulation::Visitable
        {
            public:
                virtual ~Term(){}
                virtual int accept_visitor( Manipulation::BaseVisitor & v ) = 0;
        };
    }
}

#endif // TERM_HH
