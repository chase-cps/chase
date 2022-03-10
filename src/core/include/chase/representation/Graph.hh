/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        11/11/2019
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once


#include "ChaseObject.hh"
#include "Chase.hh"

#include <map>
#include <set>
#include <vector>

namespace chase {



    /// @brief Base class to represent the edge in a graph.
    class Edge : public ChaseObject
    {
    public:
        /// @brief Constructor.
        /// @param source the source node of the edge.
        /// @param target the target node of the edge.
        explicit Edge( unsigned int source = 0, unsigned int target = 0 );

        /// @brief Destructor.
        ~Edge() override;

        /// @brief Getter of the source node.
        /// @return The index of the source node.
        unsigned int getSource() const;
        /// @brief Setter of the source node.
        /// @param source The index of the source node.
        void setSource(unsigned int source);
        /// @brief Getter of the target node.
        /// @return The index of the target node.
        unsigned int getTarget() const;
        /// @brief Setter of the target node.
        /// @param target The index of the target node.
        void setTarget(unsigned int target);

        /// @brief Base method for the visit.
        /// @param v The visitor to be used.
        /// @return The return value of the visitor being used.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief Function to print the edge.
        /// @return Return the textual representation of the edge.
        std::string getString() override;

        /// @brief Clone method.
        /// @return Clone of the object.
        Edge * clone() override;

    protected:
        /// @brief The index of the source node.
        unsigned int _source;
        /// @brief The index of the target node.
        unsigned int _target;

    };

    /// @brief Base class to represent nodes. This is necessary to allow
    /// extensions to graphs. For instance, as in the original DATE 2018 paper
    /// introducing the first version of Chase, nodes may be enriched by
    /// assigning to them types.
    class Vertex : public ChaseObject
    {
    public:
        /// @brief Constructor.
        Vertex(Name *name = new Name());

        /// @brief Destructor.
        ~Vertex() override;

        /// @brief Function returning the name of the vertex.
        /// @return The name of the vertex.
        Name *getName() const;

        /// @brief Function to set the name of the vertex.
        /// @param name the standard string of the vertex name to be assigned.
        void setName(std::string name);

        /// @brief Base class for the visitor pattarn.
        /// @param v The visitor to be used.
        /// @return The return value of the visitor.
        int accept_visitor(chase::BaseVisitor &v) override;

        /// @brief ATTENTION: this function is not implemented.
        /// \todo Implement the getGraphViz function.
        virtual std::string getGraphViz();

        /// @brief Function printing the vertex.
        /// @return The string representing the node.
        std::string getString() override;

        /// @brief Clone method.
        /// @return Clone of the object.
        Vertex * clone() override;

    protected:
        /// @brief Pointer to the name object of the vertex.
        Name * _name;
    };

    /// @brief Base class for the weighted edges.
    class WeightedEdge : public Edge
    {
    public:
        /// @brief Constructor.
        /// @param source the source node of the edge.
        /// @param target the target node of the edge.
        /// @param weight The weight to apply to the edge.
        WeightedEdge(unsigned int source, unsigned int target, Value *weight);

        /// @brief Destructor.
        ~WeightedEdge() override;

        /// @brief Function retrieving the weight of the edge.
        /// @return The weight Value of the edge.
        Value *getWeight() const;
        /// @brief Setter of the weight of the edge.
        /// @param weight The Value to be set as weight.
        void setWeight(Value *weight);

        /// @brief Function to print the edge.
        /// @return Return the textual representation of the edge.
        std::string getString() override;

        /// @brief Clone method.
        /// @return Clone of the object.
        WeightedEdge * clone() override;

    protected:
        /// @brief The weight of the edge, expressed as a Value object.
        Value * _weight;
    };

    /// @brief Adjacency Map defined as a map of map of edges. Nullptr edges
    /// represents missing edges between two nodes.
    typedef std::map<
    std::pair< unsigned int, unsigned int>, Edge * > AdjMatrix;
    /// @brief Base element of the matrix.
    typedef std::pair<
    std::pair< unsigned int, unsigned int>, Edge * > AdjMatrixElement;



    /// @brief Base class to represent graphs.
    /// @todo GraphViz support for the graphical representation of the Graph.
    class Graph : public Specification {
    public:

        /// @brief Constructor.
        Graph( unsigned int size, bool directed = false,
                Name * name = new Name("GenericGraph") );

        /// @brief Destructor.
        ~Graph() override;

        /// @brief Base function for the visit.
        /// @param v The visitor to be used.
        /// @return The return value of the used visitor.
        int accept_visitor(chase::BaseVisitor &v) override;
        /// @brief Function providing the text representing the graph.
        /// @return The string representation of the graph.
        std::string getString() override;

        /// @brief Function to add a Vertex.
        /// @param vertex The vertex to be added.
        /// @param index The index of the node to be associated with the
        /// Vertex object.
        void associateVertex(unsigned int index, Vertex *vertex);

        /// @brief Function to add a edge to the graph.
        /// @param edge The edge to be added.
        void addEdge( Edge * edge );

        /// @brief Function retrieving the type of graph.
        /// @return True if the graph is directed. False otherwise.
        bool isDirected() const;

        /// @brief Analyze whether an edge exists between two given nodes.
        /// @param source The source of the searched edge.
        /// @param target The target of the searched edge.
        /// @return A pointer to the edge if the edge exists.
        /// A nullptr otherwise.
        Edge * getEdge( unsigned int source, unsigned int target );

        /// @brief Return the Vertex object associated to a index of vertex in
        /// the graph. Notice: it is not mandatory to associate a vertex in the
        /// graph with a Vertex object.
        /// @param vertex_id The ID of the vertex which associated object is to
        /// return.
        /// @return A pointer to the associated vertex object (if any). A nullptr
        /// is returned if the node is not in the graph, or if the node is not
        /// associated to a Vertex object.
        Vertex * getVertex( unsigned int vertex_id );

        /// @brief Return the size of the graph, i.e., the number of nodes.
        /// @return The number of nodes.
        unsigned int getSize() const;

        /// @brief Function searching a Vertex by name in a graph. It returns
        /// the index of the vertex.
        /// @param name The name to search.
        /// @return the index of the vertex if found, -1 if not found.
        int getVertexIndex(std::string name );

        /// @brief Getter of the name.
        /// @return The name of the graph.
        Name * getName() const;

        /// @brief Setter of the name.
        /// @param name Pointer to the Name object containing the name.
        void setName(Name *name);

        /// @brief ATTENTION: this function is not implemented.
        /// \todo Implement the getGraphViz function.
        std::string getGraphViz();

        /// @brief Function to retrieve all the nodes adjacent to a given node.
        /// A node is considered adjacent if it as an entering edge starting
        /// from the given node.
        /// @param id The ID of the node to where to start the search.
        /// @return a vector containing the indexes of the adjacent nodes to id.
        std::set< unsigned int > getAdjacentNodes( unsigned int id );

        /// @brief Clone method.
        /// @return Clone of the object.
        Graph * clone() override;

    protected:

        /// @brief Adjacency matrix of the graph.
        AdjMatrix _matrix;
        /// @brief Set of edges of the graph.
        std::set< Edge * > _edges;
        /// @brief Set of nodes in the graph.
        std::vector< Vertex * > _vertexes;

        /// @brief The number of vertexes in the graph.
        unsigned int _size;
        /// @brief The variable is true if the graph is directed.
        bool _directed;

        /// @brief the name of the graph.
        Name * _name;

    };

}
