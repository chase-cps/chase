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

#include "representation/SimpleType.hh"
#include "utilities.hh"

namespace chase {

    /// @brief Class describing the Real type.
    class Real : public SimpleType
    {
        public:

            /// @brief Constructor.
            Real();

            /// @brief Constructor.
            /// @param min Minimum value that can be represented.
            /// @param max Maximum value that can be represented.
            Real( double min, double max );

            /// @brief Destructor.
            ~Real() override;

            /// @brief Getter of the minimum representable number.
            /// @return The minimum representable number.
            double getMin() const;
            /// @brief Getter of the maximum representable number.
            /// @return The maximum representable number.
            double getMax() const;


        /// @brief Main function for visit.
            /// @param v The visitor being used.
            /// @return the return value of the visit.
            int accept_visitor( BaseVisitor &v ) override;


            /// @brief Function printing the type.
            /// @return the type as a string.
            std::string getString() override;

            /// @brief Clone method.
            /// @return Clone of the object.
            Real * clone() override ;

        protected:

            /// @brief Min value representable.
            double _min;
            /// @brief Max value representable.
            double _max;
    };


}
