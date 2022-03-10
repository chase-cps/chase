/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/21/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include <vector>
#include <list>

#include <string>

namespace chase
{

/// \todo Document it!!!
template< typename T >
void getSubsetBySize(
        std::vector< T > elements,
        unsigned int size,
        unsigned int left,
        unsigned int index,
        std::vector< T >& combination,
        std::list< std::vector<T> > &results);


    template<>
    void getSubsetBySize(
            std::vector< std::string > elements,
            unsigned int size,
            unsigned int left,
            unsigned int index,
            std::vector< std::string >& combination,
            std::list< std::vector<std::string> > &results);

    std::string getRandomStr(int len,
                             std::string prefix = std::string(""),
                             std::string suffix = std::string(""));


}
