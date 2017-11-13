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
 * @file	chase/include/Behaviors/Logics/BooleanFormula.hh
 * @brief
 */



#ifndef BOOLEAN_FORMULA_HH
#define BOOLEAN_FORMULA_HH

#include "Behaviors/Logics/WellFormedFormula.hh"

namespace Behaviors
{
    namespace Logics
    {
        class BooleanFormula : virtual 
                               public WellFormedFormula
        {
            public:

                BooleanFormula( logic_operators o );
                ~BooleanFormula();

                virtual formula_type getType() = 0;

                void setOp( logic_operators o );
                logic_operators getOp();

                int accept_visitor( Manipulation::BaseVisitor & v );

            protected:
                logic_operators _op;


        };
    }
}


#endif // BOOLEAN_FORMULA_HH
