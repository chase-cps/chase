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
 * @file	chase/include/Behaviors/Logics/UnaryFormula.hh
 * @brief
 */



#ifndef UNARY_FORMULA
#define UNARY_FORMULA

#include "Behaviors/Logics/WellFormedFormula.hh"
#include "Manipulation/Visitable.hh"
#include "Manipulation/BaseVisitor.hh"


namespace Behaviors
{
    namespace Logics
    {
        class UnaryFormula : virtual 
                             public WellFormedFormula
        {
            public:

                UnaryFormula( WellFormedFormula * o );

                void setOp1( WellFormedFormula * f );
                WellFormedFormula * getOp1();

                int accept_visitor( Manipulation::BaseVisitor & v );

            protected:

                WellFormedFormula * _op1;
        };
    }
}

#endif // UNARY_FORMULA
