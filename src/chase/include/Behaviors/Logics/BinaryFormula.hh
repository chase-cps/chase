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
 * @file	chase/include/Behaviors/Logics/BinaryFormula.hh
 * @brief
 */



#ifndef BINARY_FORMULA_HH
#define BINARY_FORMULA_HH

#include "Behaviors/Logics/WellFormedFormula.hh"
#include "Manipulation/Visitable.hh"
#include "Manipulation/BaseVisitor.hh"


namespace Behaviors
{
    namespace Logics
    {
        class BinaryFormula : virtual 
                              public WellFormedFormula
        {
            public:

                BinaryFormula(
                        WellFormedFormula * op1,
                        WellFormedFormula * op2 );

                void setOp1( WellFormedFormula * f );
                void setOp2( WellFormedFormula * f );

                WellFormedFormula * getOp1();
                WellFormedFormula * getOp2();

                int accept_visitor( Manipulation::BaseVisitor & v );


            protected:

                WellFormedFormula * _op1;
                WellFormedFormula * _op2;
        };
    }
}

#endif // BINARY_FORMULA_HH
