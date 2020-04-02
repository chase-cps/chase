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

    using sptr_type = std::shared_ptr<Type>;
    using sptr_name = std::shared_ptr<Name>;
    using sptr_param = std::shared_ptr<Parameter>;

    public:
        /// @brief Constructor.
        Parameter(sptr_type type = nullptr, 
            sptr_name name = nullptr);

        /// @brief Destructor.
        ~Parameter();

        int accept_visitor(chase::BaseVisitor &v) override;
        std::string getString() override;
        sptr_param clone();

    protected:

    };

}
