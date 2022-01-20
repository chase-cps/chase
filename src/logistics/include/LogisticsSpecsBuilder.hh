/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        1/18/2022
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "Chase.hh"
#include "parser/LogisticsLangBaseVisitor.h"
#include "parser/LogisticsLangVisitor.h"
#include "parser/LogisticsLangParser.h"
#include "parser/LogisticsLangLexer.h"

typedef struct item
{
    /// @brief The rate of the item.
    double rate;
    /// @brief The name of the item.
    std::string name;
    /// @brief Constructor.
    /// @param rate the rate of the item.
    /// @param n the name of the item.
    item(double rate, std::string n);
    /// @brief Destructor.
    ~item() = default;
} item;

class Equipment
{
public:
    ///@brief The name of the piece of equipment.
    std::string name;
    ///@brief Constructor.
    ///@param name The name of the piece of equipment.
    explicit Equipment(std::string name);
    ///@param destructor.
    ~Equipment() = default;
    /// @brief Getter of the name.
    /// @return The name of the piece of equipment.
    const std::string &getName() const;
    /// @brief Setter of the name.
    /// @param name The name of the piece of equipment.
    void setName(const std::string &name);
};

class Bin : public Equipment
{
public:
    /// @brief List of items.
    std::vector< item * > items;
    /// @brief Constructor.
    ///@param name The name of the piece of equipment.
    explicit Bin(const std::string &name);
    /// @brief Destructor.
    ~Bin() = default;
};

class Sink : public Equipment
{
public:
    /// @brief List of items.
    std::vector< item * > items;
    /// @brief Constructor.
    ///@param name The name of the piece of equipment.
    explicit Sink(const std::string &name);
    /// @brief Destructor.
    ~Sink() = default;
};

class Machine : public Equipment
{
public:
    /// @brief List of produced widgets.
    std::vector< item * > produced;
    /// @brief List of required widgets.
    std::vector< item * > required;
    /// @brief Constructor.
    /// @param name The name of the piece of equipment.
    explicit Machine(const std::string &name);
    /// @brief Destructor.
    ~Machine() = default;
};

typedef struct Destination
{
    /// @brief The name of the destination.
    std::string name;
    /// @brief The delivery time.
    unsigned time;
    /// @brief List of request for the destination.
    std::map< std::string, unsigned > requests;

    /// @brief Constructor.
    /// @param name The name of the destination.
    /// @param time Delivery time.
    Destination(std::string name, unsigned time);
    /// @brief Destructor.
    virtual ~Destination();
} Destination;

typedef struct Position
{
    /// @brief Longitudinal position of the position.
    unsigned xpos;
    /// @brief Latitudinal position of the position.
    unsigned ypos;
    /// @brief Quantity available at the position.
    unsigned quantity;

    /// @brief Constructor.
    /// @param xpos Longitudinal position of the position.
    /// @param ypos Latitudinal position of the position.
    /// @param quantity Quantity available at the position.
    Position(unsigned xpos, unsigned ypos, unsigned quantity);
    /// @brief Destructor.
    virtual ~Position();
} Position;

/// @brief Main class of the logistics specification builder.
class LogisticsSpecsBuilder : public LogisticsLangBaseVisitor {
public:

    /// ASCII representation of the Map.
    std::vector< std::string > asciimap;
    /// Number of lines in the Map.
    unsigned map_lines;
    /// Number of columns in the Map.
    unsigned map_columns;

    std::map< std::string, std::vector< Position * > > products;
    std::vector< Destination * > destinations;

    /// @brief Constructor.
    LogisticsSpecsBuilder();
    /// @brief Destructor.
    ~LogisticsSpecsBuilder() override;

    /// @brief Main parsing function.
    /// @param infile The specification file to be parsed.
    /// @return A pointer to the System object constructed by the parser.
    void parseSpecificationFile( const std::string& infile );

    /// @cond
    antlrcpp::Any visitMap(LogisticsLangParser::MapContext *ctx) override;
    antlrcpp::Any visitProduct(LogisticsLangParser::ProductContext *ctx) override;
    antlrcpp::Any visitDestination(LogisticsLangParser::DestinationContext *ctx) override;
    /// @endcond
};
