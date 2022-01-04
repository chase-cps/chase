/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/3/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "chase/representation/Value.hh"


namespace chase {

    /// @brief Class representing a function call.
    class FunctionCall : public Value {
    public:
        /// @brief Constructor.
        FunctionCall();

        /// @brief Destructor.
        ~FunctionCall() override;

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        FunctionCall *clone() override;

        /// @brief Getter of the Function.
        /// @return Pointer to the declaration of the Function.
        Function *getFunction() const;

        /// @brief Setter of the Function. Setting a new function, clean the
        /// vector of parameters.
        /// @param function Pointer to the declaration of the Function.
        /// @param initialize If true, it initializes the vector of parameters.
        void setFunction(Function *function, bool initialize = true);

        /// @brief Function to access the parameter in a given position.
        /// @param i Position of the parameter.
        /// @param value Value of the parameter. If the value is NULL, then
        /// the operation is reading the parameter in position i. If the value
        /// is different than NULL, then the operation writes the value.
        /// @return The value of the parameter after the assignment. If
        /// the parameter is not within the function's arity, than it returns
        /// a null pointer.
        Value * parameter(size_t i, Value * value = nullptr);

    protected:

        /// @brief Pointer to the declaration of the function.
        Function * _function;
        /// @brief vector of parameters.
        std::vector< Value * > _parameters;


    };

}