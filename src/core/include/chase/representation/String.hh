/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        7/2/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once
#include "representation/SimpleType.hh"

namespace chase {

    /// @brief Class representing the String type.
    class String : public SimpleType {
    public:
        /// @brief Constructor.
        String();
        /// @brief Destructor.
        ~String() override;

        /// @brief Main function for visit.
        /// @param v The visitor being used.
        /// @return the return value of the visit.
        int accept_visitor( BaseVisitor &v ) override;

        /// @brief Function printing the type.
        /// @return the type as a string.
        std::string getString() override;


        /// @brief Clone method.
        /// @return A clone of the object.
        String * clone() override;

    protected:

    };

}
