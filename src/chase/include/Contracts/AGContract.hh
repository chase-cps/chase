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
 * @file	chase/include/Contracts/AGContract.hh
 * @brief
 */



// AGCONTRACT.HH
//


/// @author <a href="mailto:michele.lora@univr.it">Michele Lora </a>
/// @version 1.0

#ifndef AG_CONTRACT_HH
#define AG_CONTRACT_HH

// Includes of the Library
#include "Behaviors/Logics/Logics.hh"
#include "Contracts/Contract.hh"
#include "Manipulation/Visitable.hh"
#include "Manipulation/BaseVisitor.hh"

namespace Contracts
{

    /// @brief Class representing Assume-Guarantees Contracts.
    class AGContract : 
        public Contract, 
        public Manipulation::Visitable 
    {
        public:
            
            /// @brief Pointer to the specification of the assumptions.
            Behaviors::Behavior * assumptions;
            /// @brief Pointer to the specification of the guarantees.
            Behaviors::Behavior * guarantees;

            /// @brief Constructor.
            AGContract();

            /// @brief Destructor.
            ~AGContract();

            /// @brief function returning the assumptions of the contract.
            /// @return pointer to the assumption specification.
            Behaviors::Behavior * getAssumptions();

            /// @brief function returning the guarantees of the contract.
            /// @return pointer to the guarantees specification.
            Behaviors::Behavior * getGuarantees();

            /// @brief function to set the assumptions in the contract.
            /// @param a Pointer to the assumptions.
            void setAssumptions( Behaviors::Behavior * a );
            /// @brief setter function for the guarantees.
            /// @param g Pointer to the guarantees.
            void setGuarantees( Behaviors::Behavior * g );

            /// @brief Printer function.
            /// @return String representing the AG Contract.
            std::string print();

            /// @brief Accepts a visitor to visit the current object.
            /// @param v the Visitor.
            /// @return Integer representing the result of the visit.
            int accept_visitor( Manipulation::BaseVisitor & v );
            
        protected: 

            AGContract( const AGContract & );
            AGContract & operator=(const AGContract & );

    };
}

#endif // AG_CONTRACT_HH
