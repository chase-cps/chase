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
 * @file	backend/include/Chase2GR1C/backend/GR1CSyntaxWriter.hh
 * @brief
 */



#ifndef GR1C_SYNTAX_WRITER_HH
#define GR1C_SYNTAX_WRITER_HH

#include "Manipulation/GuideVisitor.hh"
#include "Behaviors/Logics/Logics.hh"

namespace chase { namespace backend { namespace Chase2GR1C
    {
        class GR1CSyntaxWriter : public Manipulation::GuideVisitor
        {
            public:

                GR1CSyntaxWriter();
                ~GR1CSyntaxWriter();

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

                /// @brief Method creating the representation of an LTL formula
                /// into the syntax of gr1c.
                /// @param formula The formula to convert.
                /// @return A string containing the gr1c representation of a given forumla.
                std::string getGR1CRepresentation( 
                        Behaviors::Logics::WellFormedFormula * formula );





            protected:

                std::string _gr1c;

                /// @brief The gr1c syntax does not uses the X operator. It
                /// rather refers to the next instant making the variables primed.
                unsigned int _next;

                GR1CSyntaxWriter( const GR1CSyntaxWriter & );
                GR1CSyntaxWriter & operator=( const GR1CSyntaxWriter & );
        };
    }
}}

#endif // GR1C_SYNTAX_WRITER_HH
