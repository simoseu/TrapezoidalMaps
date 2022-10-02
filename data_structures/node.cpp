#include "node.h"

/**
 * @brief Constructor of a node
 * @param[in] type the node type
 * @param[in] nodeIdx index of the object represented by the node (x-node: point, y-node: segment, leaf: trapezoid)
 * @param[in] leftIdx index of the left child of the node
 * @param[in] rightIdx index of the right child of the node
 */
Node::Node(NodeType type, size_t nodeIdx, size_t leftIdx, size_t rightIdx) :
            type(type), nodeIdx(nodeIdx), leftIdx(leftIdx), rightIdx(rightIdx)
{

}

/**
 * @brief Get the type of the node
 * @return type the node type
 */
Node::NodeType Node::getType() const{
    return type;
}

/**
 * @brief Get the index stored in the node
 * @return the index of the object represented by the node (x-node: point, y-node: segment, leaf: trapezoid)
*/
size_t Node::getIdx() const{
    return nodeIdx;
}

/**
 * @brief Get the index of the left child of the node
 * @return the index of the left child of the node
*/
size_t Node::getLeftIdx() const{
    return leftIdx;
}

/**
 * @brief Get the index of the right child of the node
 * @return the index of the right child of the node
*/
size_t Node::getRightIdx() const{
    return rightIdx;
}
