#ifndef NODE_H
#define NODE_H

/* Implementing the node class of the DAG search structure*/
class Node{
    /* Enum of different node types
     * X and Y are the two types of internal nodes (x-node and y-node)
     * X refer to a point
     * Y refer to a segment
     * and LEAF represent a Trapezoid
     */
    enum NodeType {X, Y, LEAF};

private:
    NodeType type;


public:

    Node();

    NodeType getType() const;
};

#endif // NODE_H
