/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/22/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "chase/representation/LogicFormula.hh"
#include "chase/representation/Variable.hh"

namespace chase {

    /// @brief Quantifiers.
    enum logic_quantifier
    {
        forall,
        exists
    };

    /*
     * @brief Main class to represent a Quantified Formula.
     * A quantified formula is characterized by a Quantifier over a variable,
     * and a formula asserting facts on the variables.
     * "forall x in Integer [1, inf](log(x) >= 0)
     * */
    class QuantifiedFormula : public LogicFormula {
    public:
        /// @brief Constructor.
        /// @param quantifier Quantifier of the formula.
        /// @param variable Quantified variable of the formula.
        /// @param formula Quantified formula.
        QuantifiedFormula(
                logic_quantifier quantifier = forall,
                Variable * variable = new Variable(),
                LogicFormula * formula = nullptr
                );

        /// @brief Destructor.
        ~QuantifiedFormula();

        /// @brief Getter of the quantifier.
        /// @return The quantifier of the formula.
        logic_quantifier getQuantifier() const;
        /// @brief Setter of the quantifier.
        /// @param quantifier Quantifier to be set.
        void setQuantifier(logic_quantifier quantifier);
        /// @brief Getter of the variable.
        /// @return The declaration of the variable being quantified.
        Variable *getVariable() const;
        /// @brief Setter of the variable.
        /// @param variable The declaration of the variable being quantified.
        void setVariable(Variable *variable);
        /// @brief Getter of the formula being quantified.
        /// @return The formula being quantified.
        LogicFormula *getFormula() const;
        /// @brief Setter of the formula being quantified.
        /// @param formula The formula being quantified.
        void setFormula(LogicFormula *formula);

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        QuantifiedFormula *clone() override;

    protected:
        /// @brief Quantifier of the formula.
        logic_quantifier _quantifier;
        /// @brief Quantified variable.
        Variable * _variable;
        /// @brief Formula being quantified.
        LogicFormula * _formula;



    };

}