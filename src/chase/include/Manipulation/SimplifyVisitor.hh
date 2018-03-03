/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2018 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2018 by University of Verona.\n
 *              Copyright (c) 2015-2018 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2018 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/Manipulation/SimplifyVisitor.hh
 * @brief
 */



#ifndef SIMPLIFY_VISITOR_HH
#define SIMPLIFY_VISITOR_HH

#include "Manipulation/GuideVisitor.hh"
#include "Behaviors/Logics/Logics.hh"

namespace Manipulation
{
    class SimplifyVisitor : public Manipulation::GuideVisitor
    {
        public:
            SimplifyVisitor();
            ~SimplifyVisitor();

            int visitBinaryFormula(
                    Behaviors::Logics::BinaryFormula & o );

            int visitUnaryFormula(
                    Behaviors::Logics::UnaryFormula & o );

            int visitBooleanFormula(
                    Behaviors::Logics::BooleanFormula & o );

            int visitTemporalFormula(
                    Behaviors::Logics::TemporalFormula & o );

            int visitUnaryLogicCombination(
                    Behaviors::Logics::UnaryLogicCombination & o );

            int visitBinaryLogicCombination(
                    Behaviors::Logics::BinaryLogicCombination & o );

            int visitBinaryTemporalFormula(
                    Behaviors::Logics::BinaryTemporalFormula & o );

            int visitUnaryTemporalFormula(
                    Behaviors::Logics::UnaryTemporalFormula & o );

            int visitLogicConstant(
                    Behaviors::Logics::LogicConstant & o );

        protected:

            SimplifyVisitor( const SimplifyVisitor & );
            SimplifyVisitor & operator=( const SimplifyVisitor & );

            Behaviors::Logics::WellFormedFormula * 
                _verifyDoubleNot( Behaviors::Logics::WellFormedFormula * wff );

    };
}

#endif // SimplifyVisitor
