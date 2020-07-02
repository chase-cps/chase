/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        6/20/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "representation/CustomType.hh"
#include "chase/representation/Constant.hh"

namespace chase {

    /// @brief Class representing a enumeration.
    /// @todo Add Enumeration into the python bindings.
    class Enumeration : public chase::CustomType {
    public:
        /// @brief Constructor.
        /// @brief name String of the name
        Enumeration( std::string name = "" );
        /// @brief Constructor.
        /// @brief name Pointer to the name object.
        Enumeration( Name * name );

        /// @brief Destructor.
        ~Enumeration() override;

        /// @brief Function to add an Item to the enumeration.
        /// @param item The string identifying the item.
        void addItem(std::string item);
        /// @brief Function to get the string identifieng the item given the position.
        /// @param position The position to access.
        /// @return The string corresponding to the position. NULL if
        /// the position does not exists.
        Constant * getItemInPosition( size_t position );
        /// @brief Function to get the position given the enum value.
        /// @param name Name of the enumeration value.
        /// @return Position (int) of the enumaration value if found. Otherwise, -1.
        int getPositionByName( std::string name );

        /// @brief Function printing the enumeration.
        /// @return String representing the enumeration.
        std::string getString() override;
        /// @brief Accept visitor function.
        /// @param v Visitor to be accepted.
        /// @return The return value of the visitor.
        int accept_visitor(BaseVisitor &v) override;
        /// @brief Function cloning the enumeration.
        /// @return The clone of the original enumeration.
        Enumeration *clone() override;

    protected:

        /// @brief Vector of values.
        std::vector< chase::Constant * > _values;

    };

}
