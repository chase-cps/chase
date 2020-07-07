/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        6/20/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once
#include "representation/SimpleType.hh"
#include "representation/Name.hh"
#include "representation/Integer.hh"

namespace chase {

    /// @brief Class representing custom types that act as alias for types.
    /// Something like a typedef.
    /// @todo Fix CustomType into the python binding.
    class CustomType : public Type {
    public:
        /// @brief Constructor.
        /// @param name String of the name of the type to declare.
        /// @param type Pointer to the type.
        CustomType(
                std::string name = "",
                Type * type = nullptr);

        /// @brief Constructor.
        /// @param name Pointer to the name of the type to declare.
        /// @param type Pointer to the type.
        CustomType(
                Name * name = new Name(""),
                Type * type = nullptr);

        /// @brief Getter of the type.
        /// @return Pointer to the base type of the custom type.
        Type *getType() const;
        /// @brief Setter of the type.
        /// @param type Pointer to the base type of the custom type.
        void setType(Type *type);

        /// @brief Getter of the name.
        /// @return Pointer to the name object.
        Name * getName() const;
        /// @brief Setter of the name.
        /// @param name Pointer to the name object to set.
        void setName(Name * name);

        std::string getString() override;

        /// @brief Destructor.
        ~CustomType();

        int accept_visitor(BaseVisitor &v) override;

        Type *clone() override;

    protected:
        Name * _name;
        Type * _type;

    };

}
