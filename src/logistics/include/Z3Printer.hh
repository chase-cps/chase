/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        5/13/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "Chase.hh"
#include "LogisticsProblem.hh"

namespace chase {

class Z3Printer : public chase::GuideVisitor {
    public:
        /// @brief Constructor.
        Z3Printer(
                Warehouse * warehouse,
                std::string path = std::string("output.py"));

        /// @brief Destructor.
        ~Z3Printer();

    protected:

        void _printVariables();
        void _printContracts();

        /// @brief The file to print.
        std::ofstream _fout;
        /// @brief Model of the warehouse.
        Warehouse * _warehouse;
    };

}
