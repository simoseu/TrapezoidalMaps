#ifndef DAG_H
#define DAG_H

#include "node.h"
#include <cstddef>
#include <vector>

/**
 * @brief This class defines the DAG data structure (will be used for the searching in the trapezoidal map)
 * A data structure used to search a point in the trapezoidal map.
 * It store in a vector all its nodes (internal and leaves), is possible to add new nodes to the vector, replace an existing one, get the number of stored nodes and get the root node of the dag.
 */
class Dag{

public:
    // Constructor
    Dag();
    // Add a node in vector
    void addNode(Node &node);
    // Replace an existing node with a new one (it needs the index of the node to replace)
    void replaceNode(Node &node, size_t idx);
    // Get the vector of nodes
    const std::vector<Node> &getNodes() const;
    // Get a node given its index
    const Node &getNode(size_t idx) const;
    // Get the number of stored nodes
    size_t numNodes() const;
    // Get the root node
    const Node &getRoot() const;
    // Remove all nodes stored in the vector
    void clear();

private:
    // Vector that stores all the nodes
    std::vector<Node> nodes;
};

#endif // DAG_H
