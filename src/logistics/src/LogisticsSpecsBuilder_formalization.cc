/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/20/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"

Warehouse *LogisticsSpecsBuilder::buildWarehouseModel()
{
    _components.reserve(map_lines);
    for(size_t it = 0; it < map_lines; ++it)
    {
        _components[it].reserve(map_columns);
        for(size_t jt = 0; jt < map_columns; ++jt)
            _components[it][jt] = nullptr;
    }

    // Find all the roads.

    // Find all the forums.

    // Find all the crossroads.

    return nullptr;
}