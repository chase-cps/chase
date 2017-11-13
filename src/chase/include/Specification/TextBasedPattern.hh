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
 * @file	chase/include/Specification/TextBasedPattern.hh
 * @brief
 */



#ifndef TEXT_BASED_PATTERN_HH
#define TEXT_BASED_PATTERN_HH

#include <string>
#include <regex>
#include <list>
#include <vector>

#include "Behaviors/Behaviors.hh"
#include "Behaviors/Logics/Logics.hh"
#include "Contracts/Contracts.hh"



namespace Specification
{
    class TextBasedPattern
    {
        public:
            TextBasedPattern();
            TextBasedPattern(std::regex regexp);

            ~TextBasedPattern();


            bool matchPattern(std::string text);
            void setPatternRegExp(std::regex regexp);
            std::regex getPatternRegExp();

            std::vector< std::string > extractFormulae(
                    std::string text );

            void createConstraints( 
                    Contracts::AGContract * contract,
                    std::vector< 
                    Behaviors::Logics::WellFormedFormula * > formulae );


        protected:

            std::regex _regex;

            TextBasedPattern( const TextBasedPattern & );
            TextBasedPattern & operator=( const TextBasedPattern & );

            virtual std::vector< std::string > _extractFormulae(
                    std::string text ) = 0;

            virtual void _createConstraints(
                    Contracts::AGContract * contract,
                    std::vector<
                    Behaviors::Logics::WellFormedFormula * > formulae ) = 0;

    };
}

#endif //PATTERN_HH
