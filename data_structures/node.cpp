#include "node.h"

Node::Node(NodeType type, size_t nodeIdx, size_t leftIdx, size_t rightIdx) :
            type(type), nodeIdx(nodeIdx), leftIdx(leftIdx), rightIdx(rightIdx)
{

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


char Node::printType() const{
    if(type == Node::X) return 'x';
    else if(type == Node::Y) return 'y';
    else return 'l';
}
