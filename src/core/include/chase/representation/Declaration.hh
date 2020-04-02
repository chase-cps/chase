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
            
    using sptr_name = std::shared_ptr<Name>;
    using sptr_decl = std::shared_ptr<Declaration>;
    
        public:
            /// @brief Constructor.
            Declaration();
            /// @brief Destructor.
            ~Declaration() override;

            /// @brief Constructor.
            /// @param n Name of the object being declared.
            explicit Declaration( sptr_name n );

            /// @brief Constructor.
            /// @param n String of the name.
            explicit Declaration( std::string n );

            /// @brief Getter function for the name of the declaration.
            /// @return The Name of the declaration. 
            sptr_name getName();

            /// @brief Setter for the Name of the declaration.
            /// @param name The Name to be setted.
            void setName( sptr_name name );

        protected:

            /// @brief Name of the declaration.
            sptr_name _name;

    };
}

