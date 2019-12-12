/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma  once

#include "BaseVisitor.hh"
#include <set>
#include <list>
#include <vector>

namespace chase {

    class GuideVisitor : public BaseVisitor {

    public:
        /// @brief Constructor.
        /// @param rv The default return value of the visitor.
        explicit GuideVisitor( int rv = 0 );

        /// @brief Destructor.
        ~GuideVisitor() override;

        /// @cond

        /// Visit functions
        //
        //  Values
        virtual int visitRange( Range &o );
        virtual int visitIntegerValue( IntegerValue &o );
        virtual int visitRealValue( RealValue &o );
        virtual int visitBooleanValue(BooleanValue &o );
        virtual int visitExpression(Expression &o );

        virtual int visitIdentifier(Identifier &o);

        /// Types
        virtual int visitInteger( Integer &o );
        virtual int visitReal( Real &o );
        virtual int visitBoolean( Boolean &o );


        /// Declarations
        virtual int visitName( Name &o );
        virtual int visitVariable( Variable &o );
        virtual int visitConstant( Constant &o );

        /// Boolean Formulas
        virtual int visitProposition( Proposition &o );
        virtual int visitBooleanConstant( BooleanConstant &o );
        virtual int visitBinaryBooleanOperation(BinaryBooleanFormula &o );
        virtual int visitUnaryBooleanOperation(UnaryBooleanFormula &o );
        virtual int visitLargeBooleanFormula(LargeBooleanFormula &o );

        virtual int visitModalOperation(ModalFormula &o );
        virtual int visitUnaryTemporalOperation(UnaryTemporalFormula &o );
        virtual int visitBinaryTemporalOperation(
                BinaryTemporalFormula &o );

        virtual int visitContract(Contract &o );

        virtual int visitEdge( Edge &o );
        virtual int visitVertex(Vertex &o);
        virtual int visitGraph(Graph &o);

        /// @endcond

    protected:

        /// @brief Return value.
        int _rv;

    };
}