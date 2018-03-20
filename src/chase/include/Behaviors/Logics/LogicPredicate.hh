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
 * @file	chase/include/Behaviors/Logics/LogicPredicate.hh
 * @brief
 */



#ifndef LOGIC_PREDICATE_HH
#define LOGIC_PREDICATE_HH

#include <string>
#include "Behaviors/Logics/LogicAtom.hh"
#include "Behaviors/Logics/Term.hh"

namespace Behaviors
{
    namespace Logics
    {
        class LogicPredicate : public LogicAtom
        {
            public:

                /// \TODO Manage the terms in a better way. A vector maybe???
                Term ** arguments;

                LogicPredicate( 
                        std::string name,
                        unsigned int arity,
                        Behaviors::Logics::Term ** args );

                ~LogicPredicate();

                formula_type getType();
                int accept_visitor( Manipulation::BaseVisitor & v );

                unsigned int getArity();
                std::string getName();

                chase::BaseObject * copy();


            protected:

                unsigned int _arity;
                std::string _name;

                LogicPredicate( const LogicPredicate & );
                LogicPredicate & operator=( const LogicPredicate & );

        };
    }
}

#endif //LOGIC_PREDICATE_HH
