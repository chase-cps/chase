/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/4/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "chase/representation/ChaseObject.hh"
#include <set>

namespace chase {


    class DesignProblem : public ChaseObject {
    public:

        std::set< Library * > libraries;
        std::set< Contract * > requirements;

        /// @brief Constructor.
        DesignProblem();

        /// @brief Destructor.
        ~DesignProblem();

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        DesignProblem *clone() override;

        /// @brief Getter of the system.
        /// @return Pointer to the system.
        System *getSystem() const;

        /// @brief Setter of the system.
        /// @param system Pointer to the system.
        void setSystem(System *system);

    protected:

        /// @brief The system being designed.
        System * _system;

    };

}