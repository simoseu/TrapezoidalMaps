#include "node.h"

Node::Node(){

}

Node::NodeType Node::getType() const{
    return type;
}

size_t Node::getIdx() const{
    return nodeIdx;
}

size_t Node::getLeftIdx() const{
    return leftIdx;
}

size_t Node::getRightIdx() const{
    return rightIdx;
}
