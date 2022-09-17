#ifndef DAG_H
#define DAG_H

#include "node.h"

#include <vector>

/**
 * @brief This class defined the DAG data structure (will be used for the searching)
 * Stores all the nodes in a vector, ...
 */
class Dag{

public:

    void addNode(Node node);
    std::vector<Node> getNodes();

private:
    std::vector<Node> nodes;

};

#endif // DAG_H
