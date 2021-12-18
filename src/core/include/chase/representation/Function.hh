/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/17/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "chase/representation/DataDeclaration.hh"
#include <vector>

namespace chase {

    /**
     * @brief Class to represent functions.
     * A function is characterized by a name, an arity and a return type
     * (which is the Type of the function).
     */
    /// @todo Complete the function class.
    class Function : public DataDeclaration {
    public:

        /// @brief Names of the parameters.
        std::vector< std::string > parameters;

        /// @brief Constructor.
        Function(Type * type, Name * name, unsigned int arity);

        /// @brief Destructor.
        ~Function() override;

        /// @brief Main function of the visit.
        /// @param v the visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;

        /// @brief Printing method of the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;

        /// @brief Clone function.
        /// @return The clone of the object.
        Function *clone() override;

        /// @brief Method returning the arity of the function.
        /// @return An integer that is the arity of the fuction.
        unsigned int getArity() const;
        /// @brief Method to set the arity of the function.
        /// @param arity The arity to be set.
        void setArity(unsigned int arity);

        Type * getDomainOfParameter(unsigned int position);
        void setDomainOfParameter(unsigned  int position, Type * type);

    protected:

        /// @brief The arity of the function.
        unsigned int _arity;

        /// @brief the domain of the function. It is to be set for every
        /// argument of the function. If it is not specified (i.e., nullptr),
        /// then, any Value is considered to be accepted.
        std::vector< Type * > _domain;


    };

}