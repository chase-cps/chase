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

#include "LogisticsProblem.hh"

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

    /// @brief ASCII representation of the Map.
    std::vector< std::string > asciimap;
    /// @brief Number of lines in the Map.
    unsigned map_lines;
    /// @brief Number of columns in the Map.
    unsigned map_columns;
    /// @brief Model of the warehouse.
    Warehouse * warehouse;

    /// @brief Products availability.
    std::map< std::string, std::vector< Position * > > products;
    /// @brief Destinations requirements.
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

protected:
    /// @brief Matrix used to keep track of the components found
    /// and assigned.
    /// \todo Consider linearizing this matrix.
    std::vector< std::vector < Equipment * > > _components;
    /// @brief Main method to build the Warehouse model.
    Warehouse * buildWarehouseModel();

    /// @brief Method to find Right-oriented roads.
    /// @param i vertical starting point.
    /// @param j horizontal starting point.
    void _analyzeRigthRoad(unsigned  i, unsigned j);

    /// @brief Method to find Left-oriented roads.
    /// @param i vertical starting point.
    /// @param j horizontal starting point.
    void _analyzeLeftRoad(unsigned  i, unsigned j);

};
