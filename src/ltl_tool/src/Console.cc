/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/23/2020
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "Console.hh"

using namespace ltl_tool;
using namespace chase;

ltl_tool::Console::Console(System *system) :
    _system(system)
{
}

int Console::run() {
    return 1;
}
