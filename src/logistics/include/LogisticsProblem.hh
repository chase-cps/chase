/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/20/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "Chase.hh"

/// @brief Enumeration with the type of objects available in the warehouse.
enum equipment_type {generic, bin, sink, machine, road, crossroad, forum, bay};

typedef struct coordinate {
    unsigned x;
    unsigned y;
} coordinate;

/// @brief Base class for the objects in the warehouse.
class Equipment
{
public:
    ///@brief The name of the piece of equipment.
    std::string name;
    /// @brief The contract representing the piece of equipment.
    chase::Contract * contract;
    ///@brief Constructor.
    ///@param name The name of the piece of equipment.
    explicit Equipment(std::string name = std::string("equipment"));
    ///@param destructor.
    ~Equipment();
    /// @brief Getter of the name.
    /// @return The name of the piece of equipment.
    const std::string &getName() const;
    /// @brief Setter of the name.
    /// @param name The name of the piece of equipment.
    void setName(const std::string &name);

    /// @brief Type of equipment.
    equipment_type type;
};

/// @brief Class to represent a Bin.
class Bin : public Equipment
{
public:

    /// @brief Coordinate of the bin.
    coordinate position;
    /// @brief Constructor.
    ///@param name The name of the piece of equipment.
    explicit Bin(const std::string &name = std::string("equipment"));
    /// @brief Destructor.
    ~Bin() = default;
};

/// @brief Class to represent a Sink.
class Sink : public Equipment
{
public:
    /// @brief Coordinate of the sink.
    coordinate position;
    /// @brief Constructor.
    ///@param name The name of the piece of equipment.
    explicit Sink(const std::string &name = std::string("sink"));
    /// @brief Destructor.
    ~Sink() = default;
};

/// @brief Class to represent a Machine.
class Machine : public Equipment
{
public:
    /// @brief Coordinate of the machine.
    coordinate position;
    /// @brief Constructor.
    /// @param name The name of the piece of equipment.
    explicit Machine(const std::string &name = std::string("machine"));
    /// @brief Destructor.
    ~Machine() = default;
};

/// @brief Class to represent a Road.
class Road : public Equipment
{
public:
    /// @brief Length of the road. It is also the its capacity.
    unsigned len;

    coordinate in;
    coordinate out;
    /// @brief Constructor.
    /// @param name The name of the piece of equipment.
    explicit Road(const std::string &name = std::string("road"));
    /// @brief Destructor.
    ~Road() = default;
};

/// @brief Class to represent a Crossroad.
class Crossroad : public Equipment
{
public:
    /// @brief Set of entrances.
    std::vector< Road * > entrances;
    /// @brief Set of exits.
    std::vector< Road * > exits;
    /// @brief Coordinate of the crossroad.
    coordinate position;
    /// @brief Constructor.
    /// @param name The name of the piece of equipment.
    explicit Crossroad(const std::string &name = std::string("Crossroad"));
    /// @brief Destructor.
    ~Crossroad() = default;
};

/// @brief Class to represent a Forum.
class Forum : public Equipment
{
public:
    /// @brief Coordinate of the top-left corner.
    coordinate topleft;
    /// @brief Coordinate of the bottom-right corner.
    coordinate bottomright;
    /// @brief Total capacity of the Forum.
    unsigned capacity;
    /// @brief Constructor.
    /// @param name The name of the piece of equipment.
    explicit Forum(const std::string &name = std::string("Forum"));
    /// @brief Destructor.
    ~Forum() = default;
};

/// @brief Class to represent a Bay.
class Bay : public Equipment
{
public:
    /// @brief Coordinate of the bay.
    coordinate position;
    /// @brief Constructor.
    /// @param name The name of the piece of equipment.
    explicit Bay(const std::string &name = std::string("Bay"));
    /// @brief Destructor.
    ~Bay() = default;
};

/// @brief Class to represent the Warehouse.
class Warehouse
{
public:
    /// @brief Vector of roads.
    std::vector< Road * > roads;
    /// @brief Vector of forums.
    std::vector< Forum * > forums;
    /// @brief Vector of crossroads.
    std::vector< Crossroad * > crossroads;
    /// @brief Vector of bays.
    std::vector< Bay * > bays;
    /// @brief Constructor.
    Warehouse();
    /// @brief Destructor.
    ~Warehouse() = default;
};