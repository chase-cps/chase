/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
 * @brief
 */


#pragma once

#include "representation.hh"


namespace chase
{
    /// @brief Copy of an ASTObject.
    /// @param obj The object to be copied.
    /// @return The new object.
    ChaseObject * copy(ChaseObject * obj );

    /// @brief Templatic function to copy an object.
    /// @param obj The object to be copied.
    /// @return The new copy of the object.
    template <typename T>
    T* copy( const T* obj )
    {
        return static_cast<T*>( copy( static_cast< const ChaseObject * >(obj) ));
    }


    /// @todo Implement the copy visitor.
}

