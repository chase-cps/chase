/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#pragma once

#include "representation/DataDeclaration.hh"

namespace chase {

    /// @brief Enumeration of the possible causalities of variables.
    typedef enum causality_t
    {
        generic,
        input,
        output,
        inout,
        internal
    }  causality_t;

    /// @brief Class representing a Variable declaration.
    class Variable : public DataDeclaration
    {
        public:
            /// @brief Constructor.
            /// @param type The name of the declared variable.
            /// @param name The name of the declared variable.
            /// @param causality The causality of the variable.
            explicit Variable(
                    Type * type = nullptr,
                    Name * name = nullptr,
                    causality_t causality = generic );
            
            /// @brief Destructor.
            ~Variable() override;

            /// @brief Function to print the variable.
            /// @return The textual representation of the variable.
            std::string getString() override;

            /// @brief Return the causality of the variable.
            /// @return The causality of the variable.
            causality_t getCausality() const;
            /// @brief Setter of the causality.
            /// @param causality The causality_t to be set.
            void setCausality(causality_t causality);



            /// @brief Main function for the visit.
            /// @param v The visitor visiting the variable.
            /// @return The return value of the visitor.
            int accept_visitor( BaseVisitor &v ) override;

            /// @brief Clone method.
            /// @return Clone of the object.
            Variable * clone() override;

        protected:

            /// @brief The causality of the variable.
            causality_t _causality;
    };
}
