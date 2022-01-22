/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/20/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"

void LogisticsSpecsBuilder::buildWarehouseModel()
{
    warehouse = new Warehouse();
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

void LogisticsSpecsBuilder::_analyzeRigthRoad(unsigned int i, unsigned int j) {
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

void LogisticsSpecsBuilder::_analyzeLeftRoad(unsigned int i, unsigned int j) {
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

void LogisticsSpecsBuilder::_analyzeDownRoad(unsigned int i, unsigned int j)
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

void LogisticsSpecsBuilder::_analyzeUpRoad(unsigned int i, unsigned int j) {
    char c = asciimap[i][j];
    auto road = new Road();
    road->out.x = j; road->out.y = i;
    while((c == 'd' || c == 'D') && i < map_lines ) {
        _components[i][j] = road;
        ++(road->len);
        road->in.x = j; road->in.y = i;
        ++i; c = asciimap[i][j];
    }
    warehouse->roads.push_back(road);
}

void LogisticsSpecsBuilder::_analyzeForum(unsigned int i, unsigned int j) {
    char c = asciimap[i][j];
    auto forum = new Forum();
    coordinate h;
    coordinate v;
    forum->topleft.x = j; forum->topleft.y = i;

    // Search horizonatlly.
    for(unsigned jt = j; jt < map_columns; ++jt)
    {
        for(unsigned it = i; it < map_lines; ++it)
        {
            if(_isSingleForum(i, j, it, jt)) {
                h.x = jt;
                h.y = it;
            }
        }
    }
    // Search vertically.
    for(unsigned it = i; it < map_lines; ++it)
    {
        for(unsigned jt = j; jt < map_columns; ++jt)
        {
            if(_isSingleForum(i, j, it, jt)) {
                v.x = jt;
                v.y = it;
            }
        }
    }

    // Compute the best search.
    unsigned areah = (h.x - j) * (h.y - i);
    unsigned areav = (v.x - j) * (v.y - i);

    if(areah > areav){
        forum->bottomright.x = h.x; forum->bottomright.y = h.y;
    } else {
        forum->bottomright.x = v.x; forum->bottomright.y = v.y;
    }

    for(unsigned it = i; it <= forum->bottomright.y; ++it)
        for(unsigned jt = j; jt <= forum->bottomright.x; ++jt) {
            forum->capacity++;
            _components[it][jt] = forum;
        }
    warehouse->forums.push_back(forum);
}

bool LogisticsSpecsBuilder::_isSingleForum(unsigned int io,
                                           unsigned int jo,
                                           unsigned int ie,
                                           unsigned int je) {
    for(unsigned i = io; i <= ie; ++i)
        for(unsigned j = jo; j <= je; ++j)
            if((asciimap[i][j] != 'f' && asciimap[i][j] != 'F')
                || _components[i][j] != nullptr)
                return false;
    return true;
}

void LogisticsSpecsBuilder::_analyzeCrossroad(unsigned int i, unsigned int j) {
    auto crossroad = new Crossroad();
    bool flag = false;
    if(j < map_columns - 1)
        if(asciimap[i][j+1] == 'R') crossroad->exits.push_back(
            reinterpret_cast<Road*>(_components[i][j+1]));
        else if(asciimap[i][j+1] == 'l') crossroad->entrances.push_back(
                reinterpret_cast<Road*>(_components[i][j+1]));
        else{flag = true; std::cout << "1" << std::endl;}
    if(j > 0)
        if(asciimap[i][j-1] == 'L') crossroad->exits.push_back(
                reinterpret_cast<Road*>(_components[i][j-1]));
        else if(asciimap[i][j-1] == 'r') crossroad->entrances.push_back(
                    reinterpret_cast<Road*>(_components[i][j-1]));
        else{flag = true; std::cout << "2" << std::endl;}
    if(i < map_lines - 1)
        if(asciimap[i+1][j] == 'D') crossroad->exits.push_back(
                reinterpret_cast<Road*>(_components[i+1][j]));
        else if(asciimap[i+1][j] == 'u') crossroad->entrances.push_back(
                    reinterpret_cast<Road*>(_components[i+1][j]));
        else{flag = true; std::cout << "3" << std::endl;}
    if(i > 0)
        if(asciimap[i-1][j] == 'U') crossroad->exits.push_back(
                reinterpret_cast<Road*>(_components[i-1][j]));
        else if(asciimap[i-1][j] == 'd') crossroad->entrances.push_back(
                    reinterpret_cast<Road*>(_components[i-1][j]));
        else{flag = true; std::cout << "4" << std::endl;}

    if(crossroad->entrances.size() > 2 || flag)
        chase::messageError("Ill-formed crossroad in position "
            + std::to_string(i) + std::string(", ") + std::to_string(j));

    warehouse->crossroads.push_back(crossroad);
    crossroad->position.x = j; crossroad->position.y = i;
    _components[i][j] = crossroad;
}