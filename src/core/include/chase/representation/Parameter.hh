/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2/24/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once
#include "representation/DataDeclaration.hh"

namespace chase {

    class Parameter : public DataDeclaration {
    public:
        /// @brief Constructor.
        Parameter(Type * type = nullptr, Name * name = nullptr);

        /// @brief Destructor.
        ~Parameter();

        int accept_visitor(chase::BaseVisitor &v) override;
        std::string getString() override;
        Parameter *clone() override;

    protected:

    };

}
