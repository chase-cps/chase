/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        7/2/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once

#include "Value.hh"

namespace chase {

    class StringValue : public Value {
    public:
        /// @brief Constructor.
        /// @param value Value of the integer.
        explicit StringValue( std::string value = "");
        /// @brief Destructor.
        ~StringValue() override;

        /// @brief Accessor to the value.
        /// @return the Value of the String.
        std::string getValue();

        /// @brief Setter for the value.
        /// @param v value to be set.
        void setValue( std::string v );

        /// @brief Copy constructor.
        /// @param o object to copy.
        StringValue( const StringValue & o );

        /// @brief Assignment operator.
        /// @param o the object to compare.
        /// @return true if they are equal.
        StringValue & operator=( const StringValue & o );

        /// @brief Accept Visitor.
        int accept_visitor(chase::BaseVisitor &v ) final;

        /// @brief Print function.
        std::string getString() override;

        /// @brief Clone method.
        /// @return Clone of the object.
        StringValue * clone() override; 
        

    protected:

        /// @brief The value being stored.
        std::string _value;

    };

}
