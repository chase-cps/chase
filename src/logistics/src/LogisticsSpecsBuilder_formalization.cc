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
    for(size_t i = 0; i < map_lines; ++i) {
        for(size_t j = 0; j < map_columns; ++j) {
            if(_components[i][j] != nullptr) {
                switch(asciimap[i][j]){
                    case 'r':
                    case 'R':
                        _analyzeRigthRoad(i, j);
                        break;
                    case 'l':
                    case 'L':
                    case 'u':
                    case 'U':
                    case 'd':
                    case 'D':
                    default:
                        break;
                }
            }
        }
    }

    // Find all the forums.

    // Find all the crossroads.

    return nullptr;
}

void LogisticsSpecsBuilder::_analyzeRigthRoad(unsigned int i, unsigned int j) {
    char c = asciimap[i][j];
    auto road = new Road();
    road->in.x = j; road->in.y = i;
    while((c == 'r' || c == 'R') && j < map_columns ) {
        _components[i][j] = road;
        ++(road->len); ++j;
        road->out.x = j; road->out.y = i;
    }
    warehouse->roads.push_back(road);
}

void LogisticsSpecsBuilder::_analyzeLeftRoad(unsigned int i, unsigned int j) {
    char c = asciimap[i][j];
    auto road = new Road();
    road->out.x = j; road->out.y = i;
    while((c == 'r' || c == 'R') && j < map_columns ) {
        _components[i][j] = road;
        ++(road->len); ++j;
        road->in.x = j; road->in.y = i;
    }
    warehouse->roads.push_back(road);
}