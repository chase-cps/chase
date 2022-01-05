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

#include "representation/ChaseObject.hh"
#include "representation/Name.hh"

namespace chase {
    /// @brief Base abstract class for all the declarations.
    class Declaration : public ChaseObject
    {
        public:
            /// @brief Constructor.
            Declaration();
            /// @brief Destructor.
            ~Declaration() override;

            /// @brief Constructor.
            /// @param n Name of the object being declared.
            explicit Declaration( Name * n );

            /// @brief Constructor.
            /// @param n String of the name.
            explicit Declaration( std::string n );

            /// @brief Getter function for the name of the declaration.
            /// @return The Name of the declaration. 
            Name * getName();

        virtual /// @brief Setter for the Name of the declaration.
            /// @param name The Name to be setted.
            void setName( Name * name );

            /// @brief Clone method.
            /// @return Clone of the object.
            Declaration * clone() override = 0;

        protected:

            /// @brief Name of the declaration.
            Name * _name;

    };
}

