#ifndef DAG_H
#define DAG_H

#include "node.h"
#include <cstddef>
#include <vector>

/**
 * @brief This class defined the DAG data structure (will be used for the searching)
 * Stores all the nodes in a vector, ...
 */
class Dag{

public:

    void addNode(Node &node);
    const std::vector<Node> &getNodes() const;
    const Node &getNode(size_t idx) const;

    const Node &getRoot() const;

private:
    std::vector<Node> nodes;

};

#endif // DAG_H
