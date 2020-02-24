/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.\n
 *              Copyright (c) 2019 by Singapore University of Technology and Design.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @brief       Base class for all the visitors.
 */



// Visitable.hh
//

/// @author <a href="mailto:michele.lora@univr.it">Michele Lora</a>
/// @version 1.0

#pragma once

//#include "Chase.hh"
#include "representation/forwards.hh"

namespace chase{

    /// @brief Base class for the visitor pattern.
    /// @see GuideVisitor, Visitable
    class BaseVisitor
    {
        public:
            /// @brief Constructor.
            BaseVisitor();
            /// @brief Destructor.
            virtual ~BaseVisitor();

            /// @cond

            /// Visit functions
            //
            //  Values
            virtual int visitRange( Range & );
            virtual int visitIntegerValue( IntegerValue & );
            virtual int visitRealValue( RealValue & );
            virtual int visitBooleanValue(BooleanValue & );
            virtual int visitExpression(Expression & );
            virtual int visitIdentifier(Identifier &);

            // Types
            virtual int visitInteger( Integer & );
            virtual int visitReal( Real & );
            virtual int visitBoolean( Boolean & );


            // Declarations
            virtual int visitName( Name & );
            virtual int visitVariable( Variable & );
            virtual int visitConstant( Constant & );
            virtual int visitComponentDefinition(ComponentDefinition &);

            // Boolean Formulas
            virtual int visitProposition( Proposition & );
            virtual int visitBooleanConstant( BooleanConstant & );
            virtual int visitBinaryBooleanOperation(BinaryBooleanFormula & );
            virtual int visitUnaryBooleanOperation(UnaryBooleanFormula & );
            virtual int visitLargeBooleanFormula(LargeBooleanFormula & );

            // Modal Formulas
            virtual int visitModalFormula(ModalFormula & );
            virtual int visitUnaryTemporalOperation(UnaryTemporalFormula & );
            virtual int visitBinaryTemporalOperation(
                    BinaryTemporalFormula & );

            // Interfaces
            virtual int visitContract(Contract & );

            virtual int visitSystem( System & );

            // Graphs
            virtual int visitGraph(Graph &);
            virtual int visitEdge( Edge & );
            virtual int visitVertex(Vertex &);
            /// @endcond

    };

}



