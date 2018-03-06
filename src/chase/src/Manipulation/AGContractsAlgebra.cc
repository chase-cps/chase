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
 * @file	chase/src/Manipulation/AGContractsAlgebra.cc
 * @brief
 */



#include "Manipulation/AGContractsAlgebra.hh"
using namespace Manipulation;
using namespace Contracts;
using namespace Behaviors;
using namespace Behaviors::Logics;

Contracts::AGContract * AGContractsAlgebra::conjunction( 
        Contracts::AGContract * c1, Contracts::AGContract * c2 )
{
    Contracts::AGContract * result = new AGContract();

    WellFormedFormula * A1 = dynamic_cast< WellFormedFormula * >(c1->getAssumptions());
    WellFormedFormula * A2 = dynamic_cast< WellFormedFormula * >(c2->getAssumptions());

    WellFormedFormula * G1 = dynamic_cast< WellFormedFormula * >(c1->getGuarantees());
    WellFormedFormula * G2 = dynamic_cast< WellFormedFormula * >(c2->getGuarantees());

    if( A1 == nullptr || A2 == nullptr || G1 == nullptr || G2 == nullptr )
    {
        return nullptr;
    }

    A1 = static_cast< WellFormedFormula * >(A1->copy());
    A2 = static_cast< WellFormedFormula * >(A2->copy());
    G1 = static_cast< WellFormedFormula * >(G1->copy());
    G2 = static_cast< WellFormedFormula * >(G2->copy());

    result->setAssumptions(
            new BinaryLogicCombination( op_lor, A1, A2 ));
    result->setGuarantees(
            new BinaryLogicCombination( op_land, G1, G2));

    return result;
}



