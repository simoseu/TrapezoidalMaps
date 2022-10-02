#include "dag.h"

/**
 * @brief empty constructor
 */
Dag::Dag(){}

/**
 * @brief Add a node in the dag
 * @param[in] node the node
 * Insert in the vector the new given node
 */
void Dag::addNode(Node &node){
    nodes.push_back(node);
}

/**
 * @brief Replace an old node with a new one
 * @param[in] node the node
 * @param[in] idx the index of the node to replace
 * Insert the given node replacing a old one
 */
void Dag::replaceNode(Node &node, size_t idx){
    nodes[idx] = node;
}

/**
 * @brief Get the nodes of the Dag
 * @return the vector containing the nodes
 * Return the vector of the nodes
 */
const std::vector<Node> &Dag::getNodes() const{
    return nodes;
}

/**
 * @brief Get a node of the Dag given its index
 * @param[in] idx the index of the node
 * @return a node of the dag given its index
 */
const Node &Dag::getNode(size_t idx) const{
    return nodes[idx];
}

/**
 * @brief Get the root node of the Dag
 * @return the root of the Dag
 */
const Node &Dag::getRoot() const{
    return nodes[0];
}

/**
 * @brief Get the number of nodes stored in the Dag
 * @return the number of nodes stored in the Dag
 */
size_t Dag::numNodes() const{
    return nodes.size();
}

/**
 * @brief Delete all nodes stored in the Dag
 */
void Dag::clear(){
    nodes.clear();
}
