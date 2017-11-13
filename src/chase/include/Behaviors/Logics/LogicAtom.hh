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
 * @file	chase/include/Behaviors/Logics/LogicAtom.hh
 * @brief
 */



#ifndef LOGIC_ATOM_HH
#define LOGIC_ATOM_HH

#include "WellFormedFormula.hh"

namespace Behaviors {
    namespace Logics{

        class LogicAtom : public WellFormedFormula
        {
            public:
                formula_type getType();
                int accept_visitor( Manipulation::BaseVisitor & v );

            protected:
       };
    }
}


#endif // LOGIC_ATOM_HH
