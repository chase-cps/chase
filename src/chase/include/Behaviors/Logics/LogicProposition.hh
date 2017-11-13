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
 * @file	chase/include/Behaviors/Logics/LogicProposition.hh
 * @brief
 */



#ifndef LOGIC_PROPOSITION_HH
#define LOGIC_PROPOSITION_HH

#include "LogicAtom.hh"
#include "Behaviors/Variable.hh"

namespace Behaviors {
    namespace Logics
    {
        class LogicProposition : public LogicAtom, public Behaviors::Variable
        {
            public:
                LogicProposition();
                LogicProposition( std::string name );
                ~LogicProposition();

                std::string print();
                int accept_visitor( Manipulation::BaseVisitor & v );

            protected:

                LogicProposition & operator=( const LogicProposition & p );
                LogicProposition( const LogicProposition & p );

        };
    }
}


#endif // LOGIC_PROPOSITION_HH
