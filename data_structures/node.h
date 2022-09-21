#ifndef NODE_H
#define NODE_H

#include <cstddef>

/**
 * @brief This class defines the data structure for the nodes of the DAG.
 * An internal node can be of two different types (x-nodes and y-nodes).
 * For this reason there is an enumeration that gives 3 different types of node (x-node, y-node and leaf)
 */
class Node{


public:
    /* Enum of different node types
     * X and Y are the two types of internal nodes (x-node and y-node)
     * X refer to a point
     * Y refer to a segment
     * and LEAF represent a Trapezoid
     */
    enum NodeType {X, Y, LEAF};

    Node(NodeType type, size_t nodeIdx, size_t leftIdx, size_t rightIdx);

    NodeType getType() const;
    size_t getIdx() const;
    size_t getLeftIdx() const;
    size_t getRightIdx() const;

private:
    NodeType type;
    size_t nodeIdx;
    // Storing the "child" nodes
    size_t leftIdx, rightIdx;


};

#endif // NODE_H
