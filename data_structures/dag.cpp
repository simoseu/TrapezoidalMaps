#include "dag.h"

void Dag::addNode(Node &node){
    nodes.push_back(node);
}

void Dag::replaceNode(Node &node, size_t idx){
    nodes[idx] = node;
}

const std::vector<Node> &Dag::getNodes() const{
    return nodes;
}

// Return a node of the dag given its index
const Node &Dag::getNode(size_t idx) const{
    return nodes[idx];
}

// Return the root of the DAG
const Node &Dag::getRoot() const{
    return nodes[0];
}
