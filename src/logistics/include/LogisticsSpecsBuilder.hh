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


#include <map>



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

    /// @brief Graph representing the architecture.
    chase::Graph * _graph;

    /// @brief Main method to build the Warehouse model.
    void buildWarehouseModel();

    /// @brief Main method to build the
    void buildGraph() const;

    /// @brief Method to find Right-oriented roads.
    /// @param i vertical starting point.
    /// @param j horizontal starting point.
    void _analyzeRigthRoad(unsigned long  i, unsigned long j);

    /// @brief Method to find Left-oriented roads.
    /// @param i vertical starting point.
    /// @param j horizontal starting point.
    void _analyzeLeftRoad(unsigned long  i, unsigned long j);

    /// @brief Method to find Down-oriented roads.
    /// @param i vertical starting point.
    /// @param j horizontal starting point.
    void _analyzeDownRoad(unsigned long i, unsigned long j);

    /// @brief Method to find Up-oriented roads.
    /// @param i vertical starting point.
    /// @param j horizontal starting point.
    void _analyzeUpRoad(unsigned long i, unsigned long j);

    /// @brief Method to find Forums. The method aims at maximizing forums
    /// size by exploring the space both vertically and horizontally.
    /// @param i vertical starting point.
    /// @param j horizontal starting point.
    void _analyzeForum(unsigned long i, unsigned long j);

    /// @brief Method analyzing a rectangle in the map to decide whether it
    /// a forum or not.
    /// @param io y coordinate of the top right corner.
    /// @param jo x coordinate of the top right corner.
    /// @param ie y coordinate of the bottom left corner.
    /// @param je x coordinate of the bottom left corner.
    /// @return True if the area is a forum. False otherwise.
    bool _isSingleForum(unsigned long io, unsigned long jo, unsigned long ie, unsigned long je);

    /// @brief Method to analyze Bay.
    /// @param i vertical position.
    /// @param j horizontal position.
    void _analyzeBay(unsigned long i, unsigned long j);

    /// @brief Method to analyze Crossroads.
    /// @param i vertical position.
    /// @param j horizontal position.
    void _analyzeCrossroad(unsigned long i, unsigned long j);

    /// @brief Method to identify the entry and exit roads of a crossroad.
    /// @param cross Pointer to the crossroad.
    void _connectCrossroad(Crossroad * cross) const;
    /// @brief Method to identify the entry and exit roads of a forum.
    /// @param cross Pointer to the forum.
    void _connectForum(Forum * forum) const;
    /// @brief Method to identify the entry and exit roads of a road.
    /// @param cross Pointer to the road.
    void _connectRoad(Road * road) const;

};
