#ifndef NODE_H
#define NODE_H


/**
 * @brief This class defines the data structure for the nodes of the DAG.
 * An internal node can be of two different types (x-nodes and y-nodes).
 * For this reason there is an enumeration that gives 3 different types of node (x-node, y-node and leaf)
 */
class Node{
    /* Enum of different node types
     * X and Y are the two types of internal nodes (x-node and y-node)
     * X refer to a point
     * Y refer to a segment
     * and LEAF represent a Trapezoid
     */
    enum NodeType {X, Y, LEAF};

public:

    Node();

    NodeType getType() const;

private:
    NodeType type;
    // Storing the node childs
    // leftChild;
    // rightChild;

};

#endif // NODE_H
