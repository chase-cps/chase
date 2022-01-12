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
            virtual int visitDesignProblem(DesignProblem & );
            //  Values
            virtual int visitRange( Range & );
            virtual int visitIntegerValue( IntegerValue & );
            virtual int visitRealValue( RealValue & );
            virtual int visitBooleanValue(BooleanValue & );
            virtual int visitStringValue( StringValue & );
            virtual int visitExpression(Expression & );
            virtual int visitIdentifier(Identifier &);

            virtual int visitInterval(Interval & );
            virtual int visitMatrix(Matrix & );
            virtual int visitProbabilityFunction(ProbabilityFunction & );

            // Types
            virtual int visitInteger( Integer & );
            virtual int visitReal( Real & );
            virtual int visitBoolean( Boolean & );
            virtual int visitCustomType( CustomType & );
            virtual int visitEnumeration( Enumeration & );
            virtual int visitString( String & );
            virtual int visitFunctionCall(FunctionCall & );

            // Declarations
            virtual int visitName( Name & );
            virtual int visitVariable( Variable & );
            virtual int visitConstant( Constant & );
            virtual int visitParameter( Parameter & );
            virtual int visitComponentDefinition(ComponentDefinition &);

            virtual int visitDistribution(Distribution &);
            virtual int visitFunction(Function &);

            // Boolean Formulas
            virtual int visitProposition( Proposition & );
            virtual int visitBooleanConstant( BooleanConstant & );
            virtual int visitBinaryBooleanOperation(BinaryBooleanFormula & );
            virtual int visitUnaryBooleanOperation(UnaryBooleanFormula & );
            virtual int visitLargeBooleanFormula(LargeBooleanFormula & );
            virtual int visitQuantifiedFormula(QuantifiedFormula &);

            // Modal Formulas
            virtual int visitModalFormula(ModalFormula & );
            virtual int visitUnaryTemporalOperation(UnaryTemporalFormula & );
            virtual int visitBinaryTemporalOperation(
                    BinaryTemporalFormula & );

            virtual int visitConstraint(Constraint & );

            // Interfaces
            virtual int visitContract(Contract & );
            virtual int visitLibrary(Library & );

            virtual int visitSystem( System & );

            virtual int visitComponent( Component & );

            // Graphs
            virtual int visitGraph(Graph &);
            virtual int visitEdge( Edge & );
            virtual int visitVertex(Vertex &);
            /// @endcond

    };

}



