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
 * @file	chase/include/Manipulation/GuideVisitor.hh
 * @brief
 */



// GuideVisitor.hh
//

/// @author <a href="mailto:michele.lora@univr.it">Michele Lora</a>
/// @version 1.0


#ifndef GUIDE_VISITOR_HH
#define GUIDE_VISITOR_HH

#include "Manipulation/BaseVisitor.hh"
#include <list>

namespace Manipulation
{
    /// @brief Base class to guide the visiting of the tree. The other visitors
    /// should inherit from this.
    /// @see BaseVisitor
    class GuideVisitor : public BaseVisitor
    {

        public:
            GuideVisitor( int rv );
            ~GuideVisitor();

            int visitLogicAtom( Behaviors::Logics::LogicAtom & o );
            int visitLogicConstant( Behaviors::Logics::LogicConstant & o );
            int visitLogicProposition( Behaviors::Logics::LogicProposition & o );

            int visitBinaryFormula(
                    Behaviors::Logics::BinaryFormula & o );
            int visitUnaryFormula(
                    Behaviors::Logics::UnaryFormula & o );

            int visitBooleanFormula(
                    Behaviors::Logics::BooleanFormula & o );
            int visitTemporalFormula(
                    Behaviors::Logics::TemporalFormula & o );


            int visitBinaryTemporalFormula( 
                    Behaviors::Logics::BinaryTemporalFormula & o );
            int visitUnaryTemporalFormula(
                    Behaviors::Logics::UnaryTemporalFormula & o );

            int visitBinaryLogicCombination(
                    Behaviors::Logics::BinaryLogicCombination & o );
            int visitUnaryLogicCombination(
                    Behaviors::Logics::UnaryLogicCombination & o );

            int visitGR1Formula(
                    Behaviors::Logics::GR1Formula & o );

            int visitVariable( Behaviors::Variable & o );

            int visitConstant( Behaviors::Constant & o );

            int visitFunction( Behaviors::Function & o );

            int visitLogicPredicate( Behaviors::Logics::LogicPredicate & o );

            int visitAGContract( Contracts::AGContract & o );





        protected:
            GuideVisitor( const GuideVisitor & );
            GuideVisitor & operator=( const GuideVisitor & );

            int _rv;

    };

}


#endif // GUIDE_VISITOR_HH
