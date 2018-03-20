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
 * @file	chase/include/Behaviors/Logics/WellFormedFormula.hh
 * @brief
 */



#ifndef WELL_FORMED_FORMULA_HH
#define WELL_FORMED_FORMULA_HH

#include "Behaviors/Logics/Globals.hh"
#include "Behaviors/Behavior.hh"
#include "Manipulation/BaseVisitor.hh"
#include "Manipulation/Visitable.hh"

namespace Behaviors{
namespace Logics
{
    class WellFormedFormula : public Behaviors::Behavior
    {
        public:

            virtual void setType( formula_type t );
            virtual formula_type getType() = 0;
            virtual std::string print() = 0;

            virtual chase::BaseObject * copy() = 0;

        protected:
            formula_type _type;
    };
}}


#endif // WELL_FORMED_FORMULA_HH
