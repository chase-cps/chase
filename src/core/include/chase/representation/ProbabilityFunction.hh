/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/11/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "chase/representation/Value.hh"

namespace chase {

    class ProbabilityFunction : public Value {
    public:
        /// @brief Constructor.
        ProbabilityFunction(Specification * specification = nullptr);

        /// @brief Destructor.
        ~ProbabilityFunction() override;

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        ProbabilityFunction *clone() override;

        /// @brief Getter of the Specification.
        /// @return Pointer to the specification.
        Specification *getSpecification() const;

        /// @brief Setter of the specification.
        /// @param specification Pointer to th specification.
        void setSpecification(Specification *specification);

    protected:
        /// @brief The specification being evaluated.
        Specification * _specification{};

    };

}