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
 * @file	chase/include/Behaviors/Function.hh
 * @brief
 */



#ifndef FUNCTION_HH
#define FUNCTION_HH

#include "Behaviors/Behaviors.hh"
#include "Manipulation/BaseVisitor.hh"


#include "Behaviors/Logics/Term.hh"
#include <string>

namespace Behaviors
{
    class Function : public Logics::Term
    {
        public:

            Behaviors::Logics::Term ** arguments;

            Function( 
                    std::string name,
                    vartype type,
                    unsigned int arity,
                    Behaviors::Logics::Term ** arguments );

            ~Function();

            Behaviors::Logics::Term * getArgument(unsigned int position);
            void setArgument(
                    unsigned int position,
                    Behaviors::Logics::Term * term );

            std::string getName();
            vartype getType();
            unsigned int getArity();
            
            int accept_visitor( Manipulation::BaseVisitor & v );

        protected:

            std::string _name;
            vartype _type;
            unsigned int _arity;

            Function( const Function & );
            Function & operator=( const Function & );

    };
}

#endif // FUNCTION_HH

