/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona. Copyright (c) 2019
 *              by Singapore University of Technology and Design. All rights
 *              reserved. This project is released under the 3-Clause BSD
 *              License.
 *
*/

#pragma once

#include "representation/Declaration.hh"
#include "representation/Type.hh"

namespace chase {

    /// @brief Abstract class for all the object representing the declaration of
    /// data. E.g., Variables and constants declarations.
    class DataDeclaration : public Declaration
    {
        
    using sptr_name = std::shared_ptr<Name>;
    using sptr_type = std::shared_ptr<Type>;
    using sptr_datadecl = std::shared_ptr<DataDeclaration>;
    
        public:

            /// @brief Constructor.
            /// @param t Type of the declared object.
            /// @param n Name of the declared object.
            explicit DataDeclaration( sptr_type t = nullptr, 
                sptr_name n = nullptr);

            /// @brief Getter for the type.
            /// @return The type of the declaration.
            sptr_type getType();

            /// @brief Setter for the type.
            /// @param t The type of the declared object.
            void setType( sptr_type t );

        protected:

            /// @brief The type of the object represented by the declaration.
            sptr_type _type;

    };
}

