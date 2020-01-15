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

        /// Visit functions
        //
        /// @cond

        /// Visit functions
        //
        //  Values
        int visitRange( Range & ) override;
        int visitIntegerValue( IntegerValue & ) override;
        int visitRealValue( RealValue & ) override;
        int visitBooleanValue(BooleanValue & ) override;
        int visitExpression(Expression & ) override;
        int visitIdentifier(Identifier & ) override;

        // Types
        int visitInteger( Integer & ) override;
        int visitReal( Real & ) override;
        int visitBoolean( Boolean & ) override;


        // Declarations
        int visitName( Name & ) override;
        int visitVariable( Variable & ) override;
        int visitConstant( Constant & ) override;

        // Boolean Formulas
        int visitProposition( Proposition & ) override;
        int visitBooleanConstant( BooleanConstant & ) override;
        int visitBinaryBooleanOperation(BinaryBooleanFormula & ) override;
        int visitUnaryBooleanOperation(UnaryBooleanFormula & ) override;
        int visitLargeBooleanFormula(LargeBooleanFormula & ) override;

        // Modal Formulas
        int visitModalFormula(ModalFormula & ) override;
        int visitUnaryTemporalOperation(UnaryTemporalFormula & ) override;
        int visitBinaryTemporalOperation(BinaryTemporalFormula & ) override;

        // Interfaces
        int visitContract(Contract & ) override;

        // Graphs
        int visitGraph(Graph & ) override;
        int visitEdge( Edge & ) override;
        int visitVertex(Vertex & ) override;
        /// @endcond

        // Generic methods for continue the visit in special cases.

        virtual int continueVisit( ChaseObject *o );
        virtual int visitList( std::list< ChaseObject * > &l );
        virtual int visitVector( std::vector< ChaseObject * > &v );
        virtual int visitSet( std::set< ChaseObject * >  &s );

    protected:

        /// @brief Return value.
        int _rv;

    };
}