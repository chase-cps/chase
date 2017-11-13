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
 * @file	chase/include/Specification/Requirement.hh
 * @brief
 */



#ifndef SPECIFICATION_REQUIREMENT_HH
#define SPECIFICATION_REQUIREMENT_HH

#include <list>
#include <vector>
#include <string>

namespace Specification
{
    class Requirement
    {
        protected:
            std::string _functionName;
            unsigned int _ariety;

            Requirement( const Requirement & );
            Requirement & operator=( const Requirement & );
        
        public:

            /// \todo better management of params. This should be
            // encapsulated. It requires a better ariety managing.
            std::vector< std::string > _params;
            Requirement();
            Requirement( 
                std::string functionName,
                unsigned int ariety );

            Requirement( 
                std::string functionName,
                std::vector< std::string > params );
                
            Requirement( 
                std::string functionName,
                unsigned int ariety,
                std::vector< std::string > params );


            ~Requirement();

            std::string getFunctionName();
            std::string getParameter(unsigned int position);
            unsigned int getAriety();

            void setFunctionName( std::string functionaName );
            void setAriety( unsigned int ariety );
            void setParameter(unsigned int position, std::string parameter );







    };
}

#endif // SPECIFICATION_REQUIREMENT_HH
