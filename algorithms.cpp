#include "algorithms.h"

size_t queryPoint(cg3::Point2d &q, Dag &dag, TrapezoidalMap &trapezoidalMap){

    // Getting the root of the dag
    Node node = dag.getRoot();

    // Search in the dag until a leaf is found
    while(node.getType() != Node::NodeType::LEAF){
        // If the node is type "X" it refer to a point
        if(node.getType() == Node::NodeType::X){
            // The point "q" is to the left or to the right of the point

        }else if(node.getType() == Node::NodeType::Y){ // The node refer to a segment
            // The point "q" is above or below the segment

        }
    }
}
