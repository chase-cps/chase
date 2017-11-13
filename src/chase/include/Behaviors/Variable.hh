/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2017 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2017 by University of Verona.\n
 *              Copyright (c) 2015-2017 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2017 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/Behaviors/Variable.hh
 * @brief
 */



// Variable.hh
//

/// @author <a href="mailto:michele.lora@univr.it">Michele Lora </a>
/// @version 1.0


#ifndef VARIABLE_HH
#define VARIABLE_HH

// Includes of the library
#include "Behaviors/Behaviors.hh"
#include "Manipulation/Visitable.hh"
#include "Manipulation/BaseVisitor.hh"

#include "Behaviors/Logics/Term.hh"

#include <string>

namespace Behaviors
{

    /// @brief Base class for the variables of the system (and of the specification). 
    class Variable : 
        public Logics::Term 
    {
        public:

            /// @brief Constructor.
            /// @param name Name of the variable.
            Variable( std::string name );

            /// @brief Constructor.
            /// @param name Name of the variable.
            /// @param type Type of the variable.
            Variable( std::string name, Behaviors::vartype type,
                  const double min = 0.0, const double max = 1.0   );

            /// @brief Destructor.
            ~Variable();

            /// @brief Function returning the variable name.
            /// @return string of the name of the variable.
            std::string GetName();

            /// @brief Function returning the variable type.
            /// @return type of the variable.
            Behaviors::vartype GetVarType();

            double getMaxValue();
            double getMinValue();

            /// @brief function used to visit the object.
            /// @param v the BaseVisitor object to apply to the Variable.
            /// @return the return value of the visitor
            /// @see BaseVisitor
            int accept_visitor( Manipulation::BaseVisitor & v );

            std::string print();


        protected:

            /// @brief name of the Variable
            std::string _name;
            /// @brief type of the Variable.
            Behaviors::vartype _vartype;

            double _minValue;
            double _maxValue;

            Variable( const Variable & );
            Variable & operator=( const Variable & );

    };
}

#endif // VARIABLE_HH
