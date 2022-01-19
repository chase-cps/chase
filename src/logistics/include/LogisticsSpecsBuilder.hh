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

/// @brief Main class of the logistics specification builder.
class LogisticsSpecsBuilder : public LogisticsLangBaseVisitor {
public:

    /// @brief List of widgets.
    std::vector< std::string > widgets;
    /// @brief List of Equipment.
    std::vector< Equipment * > equipment;
    /// @brief List of Crossroads.
    std::vector< item * > crossroads;

    /// @brief Constructor.
    LogisticsSpecsBuilder();
    /// @brief Destructor.
    ~LogisticsSpecsBuilder() override;

    /// @brief Main parsing function.
    /// @param infile The specification file to be parsed.
    /// @return A pointer to the System object constructed by the parser.
    void parseSpecificationFile( const std::string& infile );

    /// @cond
    antlrcpp::Any visitWidgets(
            LogisticsLangParser::WidgetsContext *ctx) override;
    antlrcpp::Any visitMachine(
            LogisticsLangParser::MachineContext *ctx) override;
    antlrcpp::Any visitSink(
            LogisticsLangParser::SinkContext *ctx) override;
    antlrcpp::Any visitBin(LogisticsLangParser::BinContext *ctx) override;
    antlrcpp::Any visitCrossroad(
            LogisticsLangParser::CrossroadContext *ctx) override;
    /// @endcond
};
