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
 * @file	chase/src/Behaviors/Logics/Globals.cc
 * @brief
 */



#include "Behaviors/Logics/Globals.hh"

#include <iostream>

namespace Behaviors {
    namespace Logics
    {
        std::string printOperator( 
                Behaviors::Logics::logic_operators op )
        {
            switch(op)
            {
                case(Behaviors::Logics::op_land):
                    return std::string("&");
                    break;
                case(Behaviors::Logics::op_lor):
                    return std::string("|");
                    break;
                case(Behaviors::Logics::op_lxor):
                    return std::string("^");
                    break;
                case(Behaviors::Logics::op_lnot):
                    return std::string("~");
                    break;
                case(Behaviors::Logics::op_implies):
                    return std::string("->");
                    break;
                case(Behaviors::Logics::op_iff):
                    return std::string("<->");
                    break;
                default:
                    return std::string("INVALID_OP");
            }
        }



    }
}


