/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        8/25/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */


#pragma once
#include "GuideVisitor.hh"
#include <fstream>

namespace chase {

    /// @brief Visitor printing in the syntax for PySTL.
    class PySTLPrinter : public GuideVisitor {
    public:
        /// @brief Constructor.
        /// @param path The path of the output file.
        PySTLPrinter(std::string path = std::string("output.py"));

        /// @brief Destructor.
        ~PySTLPrinter();

        /// @brief Function printing on the file.
        /// @param contract The contract to be printed.
        void print(Contract * contract);

        /// @brief Procedure printing the variables declarations.
        void printDeclarations();
        /// @brief Procedure printing the LTL for the compatibility operation.
        void printAssumptions();

        /// @brief Procedure printing the LTL for the consistency operation.
        void printGuarantees();

    protected:
        /// @brief The contract to be printed.
        Contract * _contract;
        /// @brief The file to print.
        std::ofstream _fout;

        void _printLargeFormula( LargeBooleanFormula * lbf );

    };

}
