/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once
#include "GuideVisitor.hh"
#include "representation/LogicFormula.hh"

#include <map>

namespace chase
{
    /// @brief Visitor trying to assign the causality to each variable which
    /// causality has not been assigned yet.
    class VarsCausalityVisitor : public GuideVisitor
    {
    public:

        std::map< Variable *, bool > used_in_assumptions;
        std::map< Variable *, bool > used_in_guarantees;

        /// @brief Constructor.
        VarsCausalityVisitor( Contract * contract = nullptr );
        /// @brief Destructor.
        virtual ~VarsCausalityVisitor();

        /// @cond
        Contract *getContract() const;
        void setContract(Contract *contract);

        int visitVariable(Variable &variable) override;
        int visitContract(Contract &contract) override;

        int visitIdentifier(Identifier &identifier) override;
        /// @endcond

    protected:

        /// @brief The contract being visited.
        Contract * _contract;

        /// @brief Flag stating whether the visit is in assumptions or not.
        bool _inAssumptions;
        /// @brief Flog stating whether the visit is in guarantees or not.
        bool _inGuarantees;

        /// @brief Procedure fixing the causality of the variables, according
        /// to their usage.
        void _fixVarsCausality();



    };
}
