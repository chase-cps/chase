/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2015-2016
 * @copyright   Copyright (c) 2015-2018 by University of California, Berkeley.\n
 *              Copyright (c) 2015-2018 by University of Verona.\n
 *              Copyright (c) 2015-2018 by International Business Machines Corporation.\n
 *              Copyright (c) 2016-2018 by University of Southern California.\n
 *              All rights reserved.\n
 *              This project is released under the 3-Clause BSD License.
 *
 * @file	chase/include/Manipulation/AGContractsAlgebra.hh
 * @brief
 */



#ifndef AG_CONTRACTS_ALGEBRA_HH
#define AG_CONTRACTS_ALGEBRA_HH

#include "Contracts/Contracts.hh"

namespace Manipulation
{
    /// @brief this class is implementing a set of manipulation to produce the
    /// contract representation useful to test a set of properties and to 
    /// manipulate contracts.
    /// In particular:
    /// - Consistency M != 0
    /// - Compatibility E != 0
    /// - Implementation is left to TuLiP
    /// - Refinement C' < C iff Ec' is a subset of Ec and Mc is a subset of Mc'
    /// - Conjunction GLB for refinement
    /// - Composition
    /// - Quotient
    /// - Saturation
    class AGContractsAlgebra
    {
        public:

         //   static

    };
}
#endif // AG_CONTRACTS_ALGEBRA_HH
