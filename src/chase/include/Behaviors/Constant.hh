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
 * @file	chase/include/Behaviors/Constant.hh
 * @brief
 */



#ifndef CONSTANT_HH
#define CONSTANT_HH

#include "Behaviors/Logics/Term.hh"
#include "Manipulation/BaseVisitor.hh"
#include "Behaviors.hh"

#include <string>
namespace Behaviors
{
    class Constant : public Logics::Term
    {
        public:
            Constant( std::string name, vartype type, double value );

            ~Constant();

            std::string getName();
            vartype getType();
            double getValue();

            int accept_visitor( Manipulation::BaseVisitor & v );

        protected:

            std::string _name;
            vartype _type;
            double _value;

            Constant( const Constant & );
            Constant & operator=( const Constant & );


    };
}

#endif // CONSTANT_HH

