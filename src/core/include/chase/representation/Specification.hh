/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        10/31/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "ChaseObject.hh"

namespace chase {

    /// @brief The enumeration describing the different possible
    /// semantic domains.
    enum semantic_domain
    {
        logic,
        constraints,
        graph
    };


    /// @brief The abstract class for all the specification formalisms.
    class Specification : public ChaseObject {
    public:
        /// @brief Constructor.
        Specification();

        /// @brief Destructor.
        ~Specification() override;

        /// @brief Clone method.
        /// @return Clone of the object.
        Specification * clone() override = 0;

    protected:

    };

}
