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
#include "utilities.hh"

namespace chase {

    /// @brief Abstract class for all types.
    class Type : public ChaseObject
    {
        public:

            /// @brief It is a enum to support different types of type.
            enum TypeVariant
            {
                NATIVE_TYPE,     /// <@brief Default: no variant is used.
            };

            /// @brief Constructor.
            Type();
            /// @brief Destructor.
            virtual ~Type();

            /// @brief Returns the type variant attribute.
            /// @return The type variant.
            TypeVariant getTypeVariant() const;

            /// @brief Set the type variant attribute.
            /// @param tv The Type variant to be set.
            void setTypeVariant( TypeVariant tv );

            /// @brief Return the string representation of the type variant.
            /// @return a String showing the type variant.
            std::string getTypeVariantString();

            /// @brief Function for the visitor pattern.
            /// @return the default return value if the visit works properly.
            /// A negative value otherwise.
            /// @param v a Visitor class visiting the type.
            int accept_visitor(chase::BaseVisitor & v ) override = 0;

            /// @brief Clone method.
            /// @return clone of the object.
            Type * clone() override = 0;

        protected:

            /// @brief Type variant of the Type.
            TypeVariant _typeVariant;

        

    };
}
