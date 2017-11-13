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
 * @file	backend/include/backend/Chase2GR1C/GR1CPrinter.hh
 * @brief
 */


#ifndef GR1C_PRINTER
#define GR1C_PRINTER

#include "GR1/GR1Builder.hh"
#include "Contracts/Contracts.hh"
#include "Behaviors/Behaviors.hh"
#include "Behaviors/Logics/Logics.hh"
#include "Chase2GR1C/GR1CSyntaxWriter.hh"


#include <fstream>
#include <sstream>
#include <list>


namespace chase { namespace backend { namespace Chase2GR1C {

    /// @brief Class implementing the operation necessary to print
    /// a gr1c representation from the internal CHASE representation.
    class GR1CPrinter
    {
        public:

            /// @brief Main constructor.
            /// @param contract The contract to represent in gr1c.
            /// @param path Path to the file to produce.
            /// @param assume GR1 representation of the contract assumptions.
            /// @param guarantee GR1 representation of the contract guarantees.
            GR1CPrinter( Contracts::AGContract * contract,
                    std::string path,
                    Behaviors::Logics::GR1Formula * assume,
                    Behaviors::Logics::GR1Formula * guarantee );
            ~GR1CPrinter();

            void createGR1CFile();

        protected:

                Contracts::AGContract * _contract;
                Behaviors::Logics::GR1Formula * _assume;
                Behaviors::Logics::GR1Formula * _guarantee;

                std::ofstream _file;

                void _printHead();
                void _printDeclarations();
                void _printInit();
                void _printSafe();
                void _printProgress();
                void _finalize();


                /// @brief Function producing the string used to declare a 
                /// variable when using gr1c.
                /// @param A variable of the contract.
                /// @return The string to use in gr1c to declare the variable.
                ///     The returned string is already defining the type. 
                std::string _getVarDeclaration( Behaviors::Variable * var );

    };
}}}
#endif // GR1C_PRINTER
