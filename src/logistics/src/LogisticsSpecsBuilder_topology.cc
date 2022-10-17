/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/20/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "LogisticsSpecsBuilder.hh"
#include <iomanip>

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
                    _analyzeForum(nullptr, i, j);

    for(size_t i = 0; i < map_lines; ++i)
        for(size_t j = 0; j < map_columns; ++j)
            if (_components[i][j] == nullptr)
                if(asciimap[i][j] == 'p' || asciimap[i][j] == 'P' )
                    _analyzePickingStation(i, j);

    // Find all the crossroads.
    for(size_t i = 0; i < map_lines; ++i)
        for(size_t j = 0; j < map_columns; ++j)
            if (_components[i][j] == nullptr)
                if(asciimap[i][j] == '!')
                    _analyzeCrossroad(i, j);
}

void LogisticsSpecsBuilder::_analyzeRigthRoad(unsigned long i, unsigned long j) {
    char c = asciimap[i][j];
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(5) << ++_roads;
    std::string name("R");
    auto road = new Road(name + ss.str());
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
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(5) << ++_roads;
    std::string name("R");
    auto road = new Road(name + ss.str());
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
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(5) << ++_roads;
    std::string name("R");
    auto road = new Road(name + ss.str());
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
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(5) << ++_roads;
    std::string name("R");
    auto road = new Road(name + ss.str());
    road->out.x = j; road->out.y = i;
    while((c == 'u' || c == 'U') && i < map_lines ) {
        _components[i][j] = road;
        ++(road->len);
        road->in.x = j; road->in.y = i;
        ++i; c = asciimap[i][j];
    }
    warehouse->roads.push_back(road);
}

void LogisticsSpecsBuilder::_analyzeForum(
        Forum * forum, unsigned long i, unsigned long j)
{
    if((asciimap[i][j] == 'f' || asciimap[i][j] == 'F')
                && _components[i][j] == nullptr)
    {
        if(forum == nullptr) {
            std::stringstream ss;
            ss << std::setfill('0') << std::setw(5) << ++_forums;
            std::string name("F");
            forum = new Forum(name + ss.str());
            warehouse->forums.push_back(forum);
        }

        _components[i][j] = forum;
        ++forum->capacity;
        auto c = new coordinate();
        c->x = j; c->y = i;
        forum->coordinates.insert(c);
        if(i < map_lines - 1) _analyzeForum(forum, i + 1, j);
        if(i > 0)           _analyzeForum(forum, i - 1, j);
        if(j < map_columns - 1)   _analyzeForum(forum, i, j + 1);
        if(j > 0)           _analyzeForum(forum, i, j - 1);
    }
}

void LogisticsSpecsBuilder::_analyzePickingStation(
        unsigned long i, unsigned long j) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(5) << ++_stations;
    std::string name("P");
    auto station = new PickingStation(name + ss.str());
    warehouse->stations.push_back(station);
    // Find forum.
    Forum * forum = nullptr;
    if(i > 0 && (asciimap[i-1][j] == 'f' || asciimap[i-1][j] == 'F')) {
        forum = reinterpret_cast< Forum * >(_components[i-1][j]);
    }
    if(i < (map_lines - 1) &&
        (asciimap[i+1][j] == 'f' || asciimap[i+1][j] == 'F')) {
        if(forum == nullptr)
            forum = reinterpret_cast< Forum * >(_components[i+1][j]);
        else
            if( forum != _components[i+1][j])
                chase::messageError(
                        "Picking station in a wrong position: "
                        + std::to_string(i) + ", " + std::to_string(j)
                        + ")");
    }
    if(j > 0 &&
       (asciimap[i][j] == 'f' || asciimap[i][j] == 'F')) {
        if(forum == nullptr)
            forum = reinterpret_cast< Forum * >(_components[i][j-1]);
        else
        if( forum != _components[i][j-1])
            chase::messageError(
                    "Picking station in a wrong position: "
                    + std::to_string(i) + ", " + std::to_string(j)
                    + ")");
    }
    if(i < (map_columns - 1) &&
       (asciimap[i][j+1] == 'f' || asciimap[i][j+1] == 'F')) {
        if(forum == nullptr)
            forum = reinterpret_cast< Forum * >(_components[i][j+1]);
        else
        if( forum != _components[i][j+1])
            chase::messageError(
                    "Picking station in a wrong position: "
                    + std::to_string(i) + ", " + std::to_string(j)
                    + ")");
    }
    if( forum == nullptr)
        chase::messageError(
                "Picking station in a wrong position: "
                + std::to_string(i) + ", " + std::to_string(j)
                + ")");
    _components[i][j] = station;
}

void LogisticsSpecsBuilder::_analyzeCrossroad(unsigned long i, unsigned long j) {
    // auto crossroad = new Crossroad(chase::getRandomStr(5, "X"));
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(5) << ++_xroads;
    std::string name("X");
    auto crossroad = new Crossroad(name + ss.str());

    warehouse->crossroads.push_back(crossroad);
    crossroad->position.x = j; crossroad->position.y = i;
    _components[i][j] = crossroad;
}

void LogisticsSpecsBuilder::_analyzeBay(unsigned long i, unsigned long j) {
    auto bay = new Bay(chase::getRandomStr(5, "B"));
    _components[i][j] = bay;
    bay->position.x = j; bay->position.y = i;
    warehouse->bays.push_back(bay);
}