/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        8/24/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once
#include "GuideVisitor.hh"

namespace chase {

    /// @brief Enumeration of logics that can be used to specify contracts.
    enum logics_type{
        no_logics,
        propositional,
        linear_temporal,
        metric_temporal,
        signal_temporal
    };

    /// @brief Visitor identifying the logics used to specify a contract.
    class LogicIdentificationVisitor : public GuideVisitor {
    public:
        /// @brief Constructor.
        LogicIdentificationVisitor();

        /// @brief Destructor.
        ~LogicIdentificationVisitor() override;

        /// @brief Function identifying the logic used in the contract.
        /// @return The type of logics of the contract.
        /// @param contract The contract to be analyzed.
        logics_type identifyContractType(Contract * contract);

        /// @cond
        int visitContract(Contract &contract) override;

        int visitRealValue(RealValue &value) override;
        int visitInterval(Interval &interval) override;
        int visitVariable(Variable &variable) override;
        int visitConstant(Constant &constant) override;
        int visitUnaryTemporalOperation(UnaryTemporalFormula &formula) override;
        int visitBinaryTemporalOperation(BinaryTemporalFormula &formula) override;
        int visitLargeBooleanFormula(LargeBooleanFormula &formula) override;
        /// @endcond

    protected:
        /// @brief Flag indicating whether the contract is using logics.
        bool _isLogics;
        /// @brief Flag indicating whether the contract has temporal operators.
        bool _isTemporal;
        /// @brief Flag indicating whether the contract has real variables.
        bool _hasReal;
        /// @brief Flag indicating whether the contract has intervals.
        bool _hasIntervals;

    };

}
