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
 * @file	backend/include/Chase2TuLiP/backend/SpinSyntaxWriter.hh
 * @brief
 */



#ifndef SPIN_SYNTAX_WRITER_HH
#define SPIN_SYNTAX_WRITER_HH

#include "Manipulation/GuideVisitor.hh"
#include "Behaviors/Logics/Logics.hh"

namespace chase { namespace backend { namespace Chase2TuLiP
    {
        class SpinSyntaxWriter : public Manipulation::GuideVisitor
        {
            public:

                SpinSyntaxWriter();
                ~SpinSyntaxWriter();

                int visitLogicConstant( 
                        Behaviors::Logics::LogicConstant & o );
                int visitLogicProposition( 
                        Behaviors::Logics::LogicProposition & o );

                int visitBinaryTemporalFormula( 
                        Behaviors::Logics::BinaryTemporalFormula & o );
                int visitUnaryTemporalFormula(
                        Behaviors::Logics::UnaryTemporalFormula & o );

                int visitBinaryLogicCombination(
                        Behaviors::Logics::BinaryLogicCombination & o );
                int visitUnaryLogicCombination(
                        Behaviors::Logics::UnaryLogicCombination & o );

                std::string getSpinRepresentation( 
                        Behaviors::Logics::WellFormedFormula * formula );





            protected:

                std::string _spin;

                SpinSyntaxWriter( const SpinSyntaxWriter & );
                SpinSyntaxWriter & operator=( const SpinSyntaxWriter & );
        };
    }
}}

#endif // SPIN_SYNTAX_WRITER_HH
