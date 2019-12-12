/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/12/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once

#include "Chase.hh"

#include <map>
#include <set>
#include <utility>

namespace patternsOnNetworks {

    /// @brief Enumeration of the available components types.
    /// @todo Complete the documentation by explaining for each type what is
    /// the meaning.
    typedef enum {
        ct_generic,
        ct_source,
        ct_bus,
        ct_converter,
        ct_sink,
        ct_switch,
        ct_error
    } component_type;

    /// @brief Structure representing a domain specific type.
    typedef struct DomainSpecificType {
        /// @brief The name of the type.
        std::string type;
        /// @brief The abstract type implemented by the component type.
        component_type abstract_type;
        /// @brief The vector of flow parameters.
        std::string *flow_parameters;
        /// @brief Number of flow parameters. Depends on the abstract type.
        /// Only generic and converter may have two parameters. All the other
        /// types must have one, except for switch that has zero.
        unsigned int n_flow_parameters;

        /// @brief Constructor.
        /// @param name The name of the type.
        /// @param ct The abstract type implemented by the type.
        DomainSpecificType(std::string name, component_type ct) :
                type(std::move(name)),
                abstract_type(ct) {
            switch (abstract_type) {
                case ct_generic:
                    n_flow_parameters = 2;
                    break;
                case ct_source:
                    n_flow_parameters = 1;
                    break;
                case ct_bus:
                    n_flow_parameters = 1;
                    break;
                case ct_converter:
                    n_flow_parameters = 2;
                    break;
                case ct_sink:
                    n_flow_parameters = 1;
                    break;
                case ct_switch:
                    n_flow_parameters = 0;
                    break;
                default:
                    break;
            }
            if (abstract_type != ct_switch && abstract_type != ct_error)
                flow_parameters = new std::string[n_flow_parameters];
        }

        /// @brief Destructor.
        ~DomainSpecificType() { delete flow_parameters; }
    } DomainSpecificType;

    /// @brief Base structure to represent a component.
    typedef struct Component {
        /// @brief The name of the component.
        std::string name;
        /// @brief the abbreviation of the component.
        std::string abbreviation;
        /// @brief The domain specific type of the component.
        DomainSpecificType *type;

        Component(std::string name,
                  DomainSpecificType *type,
                  std::string abbreviation = std::string("")
        ) :
                name(std::move(name)),
                abbreviation(std::move(abbreviation)),
                type(type) {
        }

        ~Component() = default;

    } Component;

    /// @brief Struct representing a group of components.
    typedef struct ComponentsGroup {
        /// @brief The attribute to characterize the type.
        /// E.g., left in left generator.
        std::string attribute;

        /// @brief The domain specific type of the components in the group.
        DomainSpecificType *type;

        /// @brief Set of components.
        std::set<Component *> components;

        /// @brief Constructor.
        /// @param attribute The attribute to associate to the type.
        /// @param type The type to associate to the group.
        ComponentsGroup(
                const std::string &attribute,
                DomainSpecificType *type,
                unsigned int n = 0) :
                attribute(attribute),
                type(type) {
            for (unsigned int i = 0; i < n; ++i) {
                std::string name = attribute;
                if (!attribute.empty()) name += " ";
                name += type->type;
                name += " " + std::to_string(i + 1);
                auto component = new Component(name, type);
                components.insert(component);
            }
        }

        ~ComponentsGroup() = default;

    } ComponentsGroup;

    /// @brief Base class to represent a generic connection.
    class Connection {
    public:
        /// @brief Constructor.
        Connection(Component *source, Component *target);

        /// @brief Destructor.
        ~Connection() = default;

        /// @brief Function returning a pointer to the source node.
        /// @param Pointer to the source node.
        Component *getSource() const;

        /// @brief Function returning a pointer to the target node.
        /// @param Pointer to the target node.
        Component *getTarget() const;

        /// @brief Function returning the target node name.
        /// @param The target node name.
        std::string getTargetName();

        /// @brief Function returning the source node name.
        /// @param The source node name.
        std::string getSourceName();

    protected:
        /// @brief Pointer to the source node.
        Component *_source;
        /// @brief Pointer to the target node.
        Component *_target;

        /// @brief The method checks the consistency of the connection.
        /// Throws an error if not consistent.
        void _checkConsistency();

    };

    /// @brief Class to represent Switched Connections.
    class SwitchedConnection : public Connection {
    public:

        /// @brief Constructor.
        /// @param source The source of the connection.
        /// @param target The target of the connection.
        /// @param switchType The type of the connector.
        /// @param sw The switch component being used.
        SwitchedConnection(
                Component *source,
                Component *target,
                DomainSpecificType *switchType,
                Component *sw = nullptr );

        /// @brief Destructor.
        virtual ~SwitchedConnection();

        /// @brief Getter of the switch type.
        /// @return A pointer to the switch type.
        DomainSpecificType *getSwitchType() const;

        /// @brief Setter of the switch type.
        /// @param switchType The switch type to assign.
        void setSwitchType(DomainSpecificType *switchType);

        /// @brief Getter of the switch.
        /// @return The pointer to the component.
        Component *getSwitch() const;

        /// @brief Setter of the switch.
        /// @param sw The pointer to the component.
        void setSwitch(Component *sw);

    protected:
        /// @brief the type of the switch.
        DomainSpecificType *_switchType;
        /// @brief The explicit switch.
        Component *_switch;
    };

    typedef struct SpecFunction
    {
        /// @brief The name of the parsed function.
        std::string name;
        /// @brief The list of parameters parsed in the function.
        std::vector< std::string > parameters;

        /// @brief constructor.
        /// @param name The name of the function.
        SpecFunction(std::string name);

        /// @brief Destructor.
        virtual ~SpecFunction();

    } SpecFunction;

}

