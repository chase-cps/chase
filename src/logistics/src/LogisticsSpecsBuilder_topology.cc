/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/20/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"

void LogisticsSpecsBuilder::buildWarehouseModel()
{
    for(size_t it = 0; it < map_lines; ++it)
    {
        std::vector< Equipment * > vec;
        for(size_t jt = 0; jt < map_columns; ++jt)
             vec.push_back(nullptr);
        _components.push_back(vec);
    }

    // Find all the roads.
    for(size_t i = 0; i < map_lines; ++i) {
        for(size_t j = 0; j < map_columns; ++j) {
            if(_components[i][j] == nullptr)
                switch(asciimap[i][j]){
                    case 'r':
                    case 'R':
                        _analyzeRigthRoad(i, j);
                        break;
                    case 'l':
                    case 'L':
                        _analyzeLeftRoad(i, j);
                        break;
                    case 'd':
                    case 'D':
                        _analyzeDownRoad(i, j);
                        break;
                    case 'u':
                    case 'U':
                        _analyzeUpRoad(i, j);
                        break;
                    case 'b':
                    case 'B':
                        _analyzeBay(i, j);
                    default:
                        break;
                }
        }
    }

    // Find all the forums.
    for(size_t i = 0; i < map_lines; ++i)
        for(size_t j = 0; j < map_columns; ++j)
            if (_components[i][j] == nullptr)
                if(asciimap[i][j] == 'f' || asciimap[i][j] == 'F' )
                    _analyzeForum(i, j);

    // Find all the crossroads.
    for(size_t i = 0; i < map_lines; ++i)
        for(size_t j = 0; j < map_columns; ++j)
            if (_components[i][j] == nullptr)
                if(asciimap[i][j] == '!')
                    _analyzeCrossroad(i, j);
}

void LogisticsSpecsBuilder::_analyzeRigthRoad(unsigned long i, unsigned long j) {
    char c = asciimap[i][j];
    auto road = new Road();
    road->in.x = j; road->in.y = i;
    while((c == 'r' || c == 'R') && j < map_columns ) {
        _components[i][j] = road;
        ++(road->len);
        road->out.x = j; road->out.y = i;
        ++j; c = asciimap[i][j];
    }
    warehouse->roads.push_back(road);
}

void LogisticsSpecsBuilder::_analyzeLeftRoad(unsigned long i, unsigned long j) {
    char c = asciimap[i][j];
    auto road = new Road();
    road->out.x = j; road->out.y = i;
    while((c == 'l' || c == 'L') && j < map_columns ) {
        _components[i][j] = road;
        ++(road->len);
        road->in.x = j; road->in.y = i;
        ++j; c = asciimap[i][j];
    }
    warehouse->roads.push_back(road);
}

void LogisticsSpecsBuilder::_analyzeDownRoad(unsigned long i, unsigned long j)
{
    char c = asciimap[i][j];
    auto road = new Road();
    road->in.x = j; road->in.y = i;
    while((c == 'd' || c == 'D') && i < map_lines ) {
        _components[i][j] = road;
        ++(road->len);
        road->out.x = j; road->out.y = i;
        ++i; c = asciimap[i][j];
    }
    warehouse->roads.push_back(road);
}

void LogisticsSpecsBuilder::_analyzeUpRoad(unsigned long i, unsigned long j) {
    char c = asciimap[i][j];
    auto road = new Road();
    road->out.x = j; road->out.y = i;
    while((c == 'u' || c == 'U') && i < map_lines ) {
        _components[i][j] = road;
        ++(road->len);
        road->in.x = j; road->in.y = i;
        ++i; c = asciimap[i][j];
    }
    warehouse->roads.push_back(road);
}

void LogisticsSpecsBuilder::_analyzeForum(unsigned long i, unsigned long j) {
    char c = asciimap[i][j];
    auto forum = new Forum();
    coordinate h;
    coordinate v;
    forum->topleft.x = j; forum->topleft.y = i;

    // Search horizonatlly.
    for(unsigned long jt = j; jt < map_columns; ++jt)
    {
        for(unsigned long it = i; it < map_lines; ++it)
        {
            if(_isSingleForum(i, j, it, jt)) {
                h.x = jt;
                h.y = it;
            }
        }
    }
    // Search vertically.
    for(unsigned long it = i; it < map_lines; ++it)
    {
        for(unsigned long jt = j; jt < map_columns; ++jt)
        {
            if(_isSingleForum(i, j, it, jt)) {
                v.x = jt;
                v.y = it;
            }
        }
    }

    // Compute the best search.
    unsigned long areah = (h.x - j) * (h.y - i);
    unsigned long areav = (v.x - j) * (v.y - i);

    if(areah > areav){
        forum->bottomright.x = h.x; forum->bottomright.y = h.y;
    } else {
        forum->bottomright.x = v.x; forum->bottomright.y = v.y;
    }

    for(unsigned long it = i; it <= forum->bottomright.y; ++it)
        for(unsigned long jt = j; jt <= forum->bottomright.x; ++jt) {
            forum->capacity++;
            _components[it][jt] = forum;
        }
    warehouse->forums.push_back(forum);
}

bool LogisticsSpecsBuilder::_isSingleForum(unsigned long io,
                                           unsigned long jo,
                                           unsigned long ie,
                                           unsigned long je) {
    for(unsigned long i = io; i <= ie; ++i)
        for(unsigned long j = jo; j <= je; ++j)
            if((asciimap[i][j] != 'f' && asciimap[i][j] != 'F')
                || _components[i][j] != nullptr)
                return false;
    return true;
}

void LogisticsSpecsBuilder::_analyzeCrossroad(unsigned long i, unsigned long j) {
    auto crossroad = new Crossroad();
    warehouse->crossroads.push_back(crossroad);
    crossroad->position.x = j; crossroad->position.y = i;
    _components[i][j] = crossroad;
}

void LogisticsSpecsBuilder::_analyzeBay(unsigned long i, unsigned long j) {
    auto bay = new Bay();
    _components[i][j] = bay;
    bay->position.x = j; bay->position.y = i;
    warehouse->bays.push_back(bay);
}