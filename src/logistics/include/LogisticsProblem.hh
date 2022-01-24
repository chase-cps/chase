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
    unsigned long x;
    unsigned long y;
} coordinate;

typedef struct Position
{
    /// @brief Longitudinal position of the position.
    unsigned long xpos;
    /// @brief Latitudinal position of the position.
    unsigned long ypos;
    /// @brief Quantity available at the position.
    unsigned long quantity;

    /// @brief Constructor.
    /// @param xpos Longitudinal position of the position.
    /// @param ypos Latitudinal position of the position.
    /// @param quantity Quantity available at the position.
    Position(unsigned long xpos, unsigned long ypos, unsigned long quantity);
    /// @brief Destructor.
    virtual ~Position();
} Position;

typedef struct Product
{
    /// @brief Name of the product.
    std::string name;
    /// @brief Positions where it can be found.
    std::vector< Position *> positions;
    /// @brief Constructor.
    Product();
    /// @brief Destructor.
    ~Product();

} Product;

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

    /// @brief Product stored in the bin.
    Product * product;
    /// @brief Coordinate of the position.
    coordinate position;
    /// @brief Quantity of product available.
    unsigned long quantity;
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
    unsigned long len;

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
    unsigned long capacity;
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

typedef struct Destination
{
    /// @brief The name of the destination.
    std::string name;
    /// @brief The delivery time.
    unsigned long time;
    /// @brief List of request for the destination.
    std::map< std::string, unsigned > requests;

    /// @brief Constructor.
    /// @param name The name of the destination.
    /// @param time Delivery time.
    Destination(std::string name, unsigned long time);
    /// @brief Destructor.
    virtual ~Destination();
} Destination;



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
    std::vector< Bin * > shelves;

    /// @brief Products availability.
    std::vector< Product * > products;
    /// @brief Destinations requirements.
    std::vector< Destination * > destinations;

    /// @brief Map storing the correspondence between crossroads and vertexes.
    std::map< Crossroad *, chase::Vertex * > crossroads2Nodes;
    /// @brief Map storing the correspondence between vertexes and crossroads.
    std::map< chase::Vertex *, Crossroad * > nodes2Crossroads;

    /// @brief Map storing the correspondence between bays and vertexes.
    std::map< Bay *, chase::Vertex * > bays2Nodes;
    /// @brief Map storing the correspondence between vertexes and bays.
    std::map< chase::Vertex *, Bay * > nodes2Bays;

    /// @brief Map storing the correspondence between forums and vertexes.
    std::map< Forum *, chase::Vertex * > forums2Nodes;
    /// @brief Map storing the correspondence between vertexes and forums.
    std::map< chase::Vertex *, Forum * > nodes2Forums;

    /// @brief Map storing the correspondence between road and edges.
    std::map< Road *, chase::Edge * > roads2Edges;
    /// @brief Map storing the correspondence between edges and road.
    std::map< chase::Edge *, Road * > edges2Roads;

    Warehouse();
    /// @brief Destructor.
    ~Warehouse() = default;
};