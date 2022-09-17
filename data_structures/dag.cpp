#include "dag.h"

void Dag::addNode(Node node){
    nodes.push_back(node);
}

std::vector<Node> Dag::getNodes(){
    return nodes;
}
