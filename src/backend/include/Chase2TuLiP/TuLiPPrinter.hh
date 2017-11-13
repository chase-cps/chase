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
 * @file	backend/include/backend/Chase2TuLiP/TuLiPPrinter.hh
 * @brief
 */



#ifndef TULIP_PRINTER_HH
#define TULIP_PRINTER_HH

#include "GR1/GR1Builder.hh"
#include "Contracts/Contracts.hh"
#include "Behaviors/Behaviors.hh"
#include "Behaviors/Logics/Logics.hh"
#include "Chase2TuLiP/SpinSyntaxWriter.hh"

#include <fstream>
#include <sstream>
#include <list>



namespace chase { namespace backend { namespace Chase2TuLiP
    {
        class TuLiPPrinter
        {
            public:

                TuLiPPrinter( Contracts::AGContract * contract,
                        std::string path,
                        Behaviors::Logics::GR1Formula * assume,
                        Behaviors::Logics::GR1Formula * guarantee );
                ~TuLiPPrinter();

                void createTuLiPFile();

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

                std::string _printType( Behaviors::Variable * v );


                TuLiPPrinter( const TuLiPPrinter & );
                TuLiPPrinter & operator=( const TuLiPPrinter & );
        };
    }
}}

#endif // TULIP_PRINTER_HH
