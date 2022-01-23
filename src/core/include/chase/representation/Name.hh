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

#include <string>
#include "representation/ChaseObject.hh"
#include "BaseVisitor.hh"
#include "utilities/UtilityFunctions.hh"

namespace chase {

    /// @brief Class representing the name of a object in a system.
    class Name : public ChaseObject
    {
        public:

            /// @brief Constructor.
            /// @param s The string of the name.
            explicit Name( std::string s = getRandomStr(5));
            /// @brief Destructor.
            ~Name() override;

            /// @brief Copy constructor.
            /// @param o Instance to copy.
            Name( const Name & o );

            /// @brief Overload the assignment operator.
            /// @param o the Name to assign.
            /// @return itself.
            Name & operator=( const Name & o );

            /// @brief Getter of the string for the name.
            /// @return the string.
            std::string getString() override;

            /// @brief Setter for the string for the name.
            /// @param name The string to set.
            void changeName( std::string name );

            /// @brief Main function for visiting a Name object.
            /// @param v The visitor visiting the object.
            /// @return The return value of the visitor.
            int accept_visitor( BaseVisitor &v ) override;

            /// @brief Clone method.
            /// @return Clone of the name.
            Name * clone() override;

        protected:

            /// @brief Name stored as a string.
            std::string _name;

    };
}


