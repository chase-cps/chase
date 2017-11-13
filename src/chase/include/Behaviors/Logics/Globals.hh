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
 * @file	chase/include/Behaviors/Logics/Globals.hh
 * @brief
 */



#ifndef LOGICS_GLOBALS_HH
#define LOGICS_GLOBALS_HH

#include <string>

namespace Behaviors {
    namespace Logics
    {

        enum formula_type
        {
            ft_propositional,
            ft_ltl,
            ft_stl,
            ft_predicative,
            ft_qltl,
            ft_qstl,
            ft_not_well_formed
        };

        enum arithmetic_operators
        {
            op_minus,
            op_sum,
            op_mult,
            op_div,
            op_pow,
            op_equal,
            op_leq,
            op_lt,
            op_gt,
            op_geq
        };

        enum logic_operators
        {
            op_land,
            op_lor,
            op_lxor,
            op_lnot,
            op_implies,
            op_iff                
        };

        enum logic_constants
        {
            lc_false,
            lc_true
        };

        enum temporal_operators
        {
            op_next,
            op_future,
            op_globally,
            op_until,
            op_release
        };

        enum quantifiers
        {
            q_forall,
            q_exists
        };

        std::string printOperator( 
                Behaviors::Logics::logic_operators op );

    }
}

#endif
