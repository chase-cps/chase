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
 * @file	chase/include/Behaviors/Logics/LogicConstant.hh
 * @brief
 */



#ifndef LOGIC_CONSTANT_HH
#define LOGIC_CONSTANT_HH

#include "LogicAtom.hh"

namespace Behaviors{
    namespace Logics
    {
        class LogicConstant : public LogicAtom
        {
            public:
                LogicConstant();
                LogicConstant( bool value );
                LogicConstant( logic_constants value );
                ~LogicConstant();

                std::string print();
                logic_constants getValue();
                int accept_visitor( Manipulation::BaseVisitor & v );

                chase::BaseObject * copy();
               
            protected:
                logic_constants _value;

                LogicConstant & operator=(const LogicConstant & c);
                LogicConstant(const LogicConstant & c);


        };
    }
}

#endif // LOGIC_CONSTANT_HH
