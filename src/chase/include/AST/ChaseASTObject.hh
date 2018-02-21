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
 * @file	chase/include/ChaseASTObject.hh
 * @brief
 */



/// @author <a href="mailto:michele.lora@univr.it">Michele Lora </a>
/// @version 1.0

#pragma once


namespace chase
{


    class ChaseASTObject
    {
        protected:
            /// @brief Pointer to the parent node in the Abstract Syntax Tree.
            ChaseASTObject * _parent;

        public:

            ChaseASTObject();
            ~ChaseASTObject();

            /// @brief Copy constructor.
            ChaseASTObject( const ChaseASTObject & );
            ChaseASTObject & operator=( const ChaseASTObject & );

            /// @brief Getter function to the parent pointer.
            /// @return a pointer to the parent node.
            ChaseASTObject * getParent();
            /// @brief Setter function for the parent pointer.
            /// @param p Pointer to the node to set as a pointer.
            void setParent( ChaseASTObject * p );


    };

    typedef ChaseASTObject Object;

}


