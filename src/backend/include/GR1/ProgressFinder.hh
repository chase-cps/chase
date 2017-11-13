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
 * @file	backend/include/Chase2TuLiP/GR1/ProgressFinder.hh
 * @brief
 */



#ifndef PROGRESS_FINDER_HH
#define PROGRESS_FINDER_HH

#include "Manipulation/GuideVisitor.hh"

namespace chase { namespace backend
    {

        namespace GR1
        {
            class ProgressFinder : public Manipulation::GuideVisitor
            {
                public:

                    std::list< Behaviors::Logics::WellFormedFormula * > progress;

                    ProgressFinder();
                    ~ProgressFinder();

                    int visitUnaryTemporalFormula(
                            Behaviors::Logics::UnaryTemporalFormula & o );
                    int visitBinaryLogicCombination(
                            Behaviors::Logics::BinaryLogicCombination & o );
                    int visitUnaryLogicCombination(
                            Behaviors::Logics::UnaryLogicCombination & o );

                protected:

                    ProgressFinder( const ProgressFinder & );
                    ProgressFinder & operator=( const ProgressFinder & );

            };
        }
    }
}


#endif // PROGRESS_FINDER_HH
