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
 * @file	chase/include/Manipulation/BaseVisitor.hh
 * @brief
 */



// BaseVisitor.hh
//

/// @author <a href="mailto:michele.lora@univr.it">Michele Lora</a>
/// @version 1.0


#ifndef BASE_VISITOR_HH
#define BASE_VISITOR_HH

#include "forwards.hh"

namespace Manipulation 
{
    /// @brief Base class for the Visitor Pattern.
    /// @see GuideVisitor, Visitable
    class BaseVisitor
    {
        public:

            BaseVisitor();
            virtual ~BaseVisitor();

            virtual int visitLogicAtom( 
                    Behaviors::Logics::LogicAtom & o );
            virtual int visitLogicConstant( 
                    Behaviors::Logics::LogicConstant & o );
            virtual int visitLogicProposition( 
                    Behaviors::Logics::LogicProposition & o );

            virtual int visitBinaryFormula(
                    Behaviors::Logics::BinaryFormula & o );
            virtual int visitUnaryFormula(
                    Behaviors::Logics::UnaryFormula & o );

            virtual int visitBooleanFormula(
                    Behaviors::Logics::BooleanFormula & o );
            virtual int visitTemporalFormula(
                    Behaviors::Logics::TemporalFormula & o );

            virtual int visitBinaryTemporalFormula( 
                    Behaviors::Logics::BinaryTemporalFormula & o );
            virtual int visitUnaryTemporalFormula(
                    Behaviors::Logics::UnaryTemporalFormula & o );

            virtual int visitBinaryLogicCombination(
                    Behaviors::Logics::BinaryLogicCombination & o );
            virtual int visitUnaryLogicCombination(
                    Behaviors::Logics::UnaryLogicCombination & o );

            virtual int visitGR1Formula(
                    Behaviors::Logics::GR1Formula & o );

            virtual int visitVariable( 
                    Behaviors::Variable & o );

            virtual int visitConstant(
                    Behaviors::Constant & o );

            virtual int visitFunction(
                    Behaviors::Function & o );

            virtual int visitLogicPredicate(
                    Behaviors::Logics::LogicPredicate & o );

            virtual int visitAGContract( 
                    Contracts::AGContract & o );


    };
}

#endif // BASE_VISITOR_HH
