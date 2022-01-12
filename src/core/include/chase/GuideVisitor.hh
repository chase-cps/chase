/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma  once

#include "BaseVisitor.hh"

#include "utilities/IOUtils.hh"

#include <set>
#include <list>
#include <vector>

namespace chase {


    /// @brief Basic visitor guiding the visit of the standard CHASE tree.
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
        int visitSystem( System & ) override;

        //  Values
        int visitRange( Range & ) override;
        int visitIntegerValue( IntegerValue & ) override;
        int visitRealValue( RealValue & ) override;
        int visitBooleanValue(BooleanValue & ) override;
        int visitExpression(Expression & ) override;
        int visitIdentifier(Identifier & ) override;

        int visitInterval(Interval &interval) override;
        int visitMatrix(Matrix &matrix) override;

        int visitProbabilityFunction(ProbabilityFunction &function) override;

        // Types
        int visitInteger( Integer & ) override;
        int visitReal( Real & ) override;
        int visitBoolean( Boolean & ) override;
        int visitCustomType( CustomType & ) override;
        int visitEnumeration( Enumeration & ) override;


        // Declarations
        int visitName( Name & ) override;
        int visitVariable( Variable & ) override;
        int visitConstant( Constant & ) override;
        int visitParameter( Parameter & ) override;

        int visitComponentDefinition(ComponentDefinition & ) override;

        int visitDistribution(Distribution & ) override;

        // Boolean Formulas
        int visitProposition( Proposition & ) override;
        int visitBooleanConstant( BooleanConstant & ) override;
        int visitBinaryBooleanOperation(BinaryBooleanFormula & ) override;
        int visitUnaryBooleanOperation(UnaryBooleanFormula & ) override;
        int visitLargeBooleanFormula(LargeBooleanFormula & ) override;

        int visitQuantifiedFormula(QuantifiedFormula & ) override;

        // Modal Formulas
        int visitModalFormula(ModalFormula & ) override;
        int visitUnaryTemporalOperation(UnaryTemporalFormula & ) override;
        int visitBinaryTemporalOperation(BinaryTemporalFormula & ) override;

        // Interfaces
        int visitContract(Contract & ) override;


        int visitComponent(Component & ) override;

        // Graphs
        int visitGraph(Graph & ) override;
        int visitEdge( Edge & ) override;
        int visitVertex(Vertex & ) override;

        int visitStringValue(StringValue &value) override;

        int visitString(String &s) override;

        /// @endcond

        // Generic methods for continue the visit in special cases.

        /// @brief Method guiding the visit when the object type in the AST is
        /// not known.
        /// @param o A pointer to the object to visit.
        /// @return T the standard return value of the visitor.
        virtual int continueVisit( ChaseObject *o );

        /// @brief Method to visit a list.
        /// @param l the list to visit.
        /// @return the standard return value of the visitor.
        template<typename T>
        int visitList(std::list<T *> &l)
        {
            int rv = _rv;
            for(auto it = l.begin(); it != l.end(); ++it)
            {
                auto * o = dynamic_cast< ChaseObject * >(*it);
                if( o == nullptr )
                    messageError("Visit List: wrong object in list");

                rv |= continueVisit(o);
            }
            return rv;
        }

        /// @brief Method to visit a vector.
        /// @param v the vector to visit.
        /// @return the standard return value of the visitor.
        template<typename T>
        int visitVector(std::vector<T *> &v) {
            int rv = _rv;
            for(auto it = v.begin(); it != v.end(); ++it)
            {
                auto * o = dynamic_cast< ChaseObject * >(*it);
                if( o == nullptr )
                    messageError("Visit Vector: wrong object in vector");

                rv |= continueVisit(o);
            }
            return rv;
        }

        /// @brief Method to visit a set.
        /// @param s the set to visit.
        /// @return the standard return value of the visitor.
        template<typename T>
        int visitSet(std::set<T *> &s) {
            int rv = _rv;
            for(auto it = s.begin(); it != s.end(); ++it)
            {
                auto * o = dynamic_cast< ChaseObject * >(*it);
                if( o == nullptr )
                    messageError("Visit Set: wrong object in set");

                rv |= continueVisit(o);
            }
            return rv;
        }

    protected:

        /// @brief Return value.
        int _rv;

    };
}