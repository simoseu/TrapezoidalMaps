#include "algorithms.h"
#include <cg3/geometry/utils2.h> // To use the isPoitAtLeft() utility

//Limits for the bounding box
//It defines where points can be added
//Do not change the following line
#define BOUNDINGBOX 1e+6


// Order the points of a segment by x values
void orderSegment(cg3::Segment2d &segment){
    if (segment.p1().x() > segment.p2().x()) {
        cg3::Point2d aux = segment.p1();
        segment.setP1(segment.p2());
        segment.setP2(aux);
    }
}

// Return true if the left point of a trapezoid is equal to the left endpoint of its top segment
bool leftPointEqualLeftEndpoint(Trapezoid trapezoid){
    cg3::Segment2d topSegment = trapezoid.getTopSegment();
    orderSegment(topSegment);
    return trapezoid.getLeftPoint() == topSegment.p1();
}

bool rightPointEqualRightEndpoint(Trapezoid trapezoid){
    cg3::Segment2d topSegment = trapezoid.getTopSegment();
    orderSegment(topSegment);
    return trapezoid.getRightPoint() == topSegment.p2();
}


/**
 * @brief Locate in which trapezoid lies the given point q
 * @param[in] q Query point
 * @param[in] dag The DAG search structure
 * @param[in] TrapezoidalMapData The trapezoidal map dataset data structure
 * @return The index of the trapezoid in which lies the query point
 */
size_t queryPoint(const cg3::Point2d &q, const Dag &dag, const TrapezoidalMapDataset &trapezoidalMapData){

    // Getting the root of the dag
    Node node = dag.getRoot();

    // Search in the dag until a leaf is found
    while(node.getType() != Node::NodeType::LEAF){
        // If the node is type "X" it refer to a point
        if(node.getType() == Node::NodeType::X){
            // The point "q" is to the left or to the right of the point
            const cg3::Point2d &point = trapezoidalMapData.getPoint(node.getIdx());
            if(q.x() < point.x() ){// If the point lies to the left of q
                node = dag.getNode(node.getLeftIdx());
            }else{ //If the point lies to the right of q
                node = dag.getNode(node.getRightIdx());
            }
        }else if(node.getType() == Node::NodeType::Y){ // The node refer to a segment
            // The point "q" is above or below the segment
            const cg3::Segment2d &segment = trapezoidalMapData.getSegment(node.getIdx());
            if(cg3::isPointAtLeft(segment, q)){ // return true if the point is above, false otherwise
                node = dag.getNode(node.getLeftIdx());
            }else{
                node = dag.getNode(node.getRightIdx());
            }

            // Maybe handle also the case when the point lies on the segment?
        }
    }
    // Index of the trapezoid containing the point q
    return node.getIdx();
}

/**
 * @brief Find the trapezoids intersected by a given segment
 * @param[in] segment The given segment
 * @param[in] dag The DAG search structure
 * @param[in] trapezoidalMap The trapezoidal Map data structure
 * @param[in] trapezoidalMapData The trapezoidal map dataset data structure
 * @return A vector containing the index of all the trapezoids intersected by the given segment
 */
std::vector<size_t> followSegment(const cg3::Segment2d &segment, const Dag &dag, const TrapezoidalMap &trapezoidalMap, const TrapezoidalMapDataset &trapezoidalMapData){

    // Vector that will contain all trapezoids intersected by the segment
    std::vector<size_t> intersectedTrapezoids;

    // Need to search the left endpoint of s in the DAG to find the trapezoid zero
    size_t idxTrapezoid = queryPoint(segment.p1(), dag, trapezoidalMapData);
    // Adding the trapezoid in the vector
    intersectedTrapezoids.push_back(idxTrapezoid);

    cg3::Point2d rightPoint = trapezoidalMap.getTrapezoid(idxTrapezoid).getRightPoint();
    // Check if p2 lies to the right of the right endpoint of the trapezoid
    while(segment.p2().x() > rightPoint.x()){
        // If the right point of the trapezoid lies above the segment put the lower right neighbor in the intersected trapezoids vector and go on with it
        if(cg3::isPointAtLeft(segment, rightPoint)){
            idxTrapezoid = trapezoidalMap.getTrapezoid(idxTrapezoid).getLowerRightNeighbor();
        }else{ // it is below set upper right neighbor
            idxTrapezoid = trapezoidalMap.getTrapezoid(idxTrapezoid).getUpperRightNeighbor();
        }
        // Add the trapezoid to the vector
        intersectedTrapezoids.push_back(idxTrapezoid);
        // Setting the rightPoint of the new trapezoid
        rightPoint = trapezoidalMap.getTrapezoid(idxTrapezoid).getRightPoint();
    }

    return intersectedTrapezoids;
}

/**
 * @brief Initialize the data structures Trapezoidal Map and DAG
 * @param[in] dag The DAG search structure
 * @param[in] trapezoidalMap The trapezoidal Map data structure
 *
 */
void initializeStructures(Dag &dag, TrapezoidalMap &trapezoidalMap){
    // The trapezoidal map for the empty set consist of a single trapezoid, which is the bounding rectangle.
    cg3::Segment2d topSegment = cg3::Segment2d(cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX), cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX));
    cg3::Segment2d bottomSegment = cg3::Segment2d(cg3::Point2d(-BOUNDINGBOX, -BOUNDINGBOX), cg3::Point2d(BOUNDINGBOX, -BOUNDINGBOX));
    cg3::Point2d leftPoint = cg3::Point2d(-BOUNDINGBOX, BOUNDINGBOX);
    cg3::Point2d rightPoint = cg3::Point2d(BOUNDINGBOX, BOUNDINGBOX);

    // Since the bounding trapezoid has no neighbour, use the max value of size_t as arbitrary index for a null index
    size_t nullIdx = std::numeric_limits<size_t>::max();
    // Setting the node index to 0 since it will be the first node added at the dag
    Trapezoid boundingBoxTrapezoid = Trapezoid(topSegment, bottomSegment, leftPoint, rightPoint, nullIdx, nullIdx, nullIdx, nullIdx, 0);
    trapezoidalMap.addTrapezoid(boundingBoxTrapezoid);

    // The DAG consist of a single leaf node representing the bounding rectangle
    // Setting the node index to 0 since the trapezoid inserted is the first of the vector
    Node boundingBoxNode = Node(Node::LEAF, 0, nullIdx, nullIdx);
    dag.addNode(boundingBoxNode);
}

void buildTrapezoidalMap(const cg3::Segment2d &segment, Dag &dag, TrapezoidalMap &trapezoidalMap, const TrapezoidalMapDataset &TrapezoidalMapData){
    // Before adding a segment is necessary to: Determine a bounding box R that contains all segments of S, and initialize the trapezoidal map structure T and search structure D for it.

    // Ordering the segment for ensuring that the second point (p2) is the right endpoint of the segment
    cg3::Segment2d orderedSegment = segment;
    if (segment.p1().x() > segment.p2().x()) {
        orderedSegment.setP1(segment.p2());
        orderedSegment.setP2(segment.p1());
    }

    // Get the intersected trapezoids with the function followSegment
    std::vector<size_t> intersectedTrapezoids = followSegment(orderedSegment, dag, trapezoidalMap, TrapezoidalMapData);

    // Split in two case to handle - the segment intersect only one trapezoid and the segment intersect more trapezoid
    // Only one trapezoid intersected
    if(intersectedTrapezoids.size() == 1){
        // Nuovi IDX
        size_t newIdx = trapezoidalMap.getTrapezoids().size();

        // In this case the trapezoid will be replaced with at most 4 trapezoid. Is possible that there is no left or right trapezoid
        size_t intersectedT = intersectedTrapezoids[0];

        // Top trapezoid, need to add the adjacency later and the node Index

        size_t topTrapezoidIDX = intersectedT;  // Taking the idx of the intersected trapezoid

        Trapezoid topTrapezoid = Trapezoid(trapezoidalMap.getTrapezoid(intersectedT).getTopSegment(), orderedSegment, segment.p1(), segment.p2());

        // Bottom trapezoid

        size_t bottomTrapezoidIDX = newIdx;  // Taking the idx next to the last trapezoid in the vector

        Trapezoid bottomTrapezoid = Trapezoid(orderedSegment, trapezoidalMap.getTrapezoid(intersectedT).getBottomSegment(), segment.p1(), segment.p2());

        // Left trapezoid only if segment.p1 is not equal to the left point of the trapezoid

        bool leftTrapezoidExist = segment.p1() != trapezoidalMap.getTrapezoid(intersectedT).getLeftPoint();

        size_t leftTrapezoidIDX = std::numeric_limits<size_t>::max();

        if(leftTrapezoidExist){

            leftTrapezoidIDX = newIdx++; // Taking the next id of the bottomTrapezoid

            Trapezoid leftTrapezoid = Trapezoid(trapezoidalMap.getTrapezoid(intersectedT).getBottomSegment(), trapezoidalMap.getTrapezoid(intersectedT).getBottomSegment(),
                                                trapezoidalMap.getTrapezoid(intersectedT).getLeftPoint(), segment.p2());
        }

        // Right trapezoid only if segment.p2 is not equal to the right point of the trapezoid
        bool rightTrapezoidExist = segment.p2() != trapezoidalMap.getTrapezoid(intersectedT).getRightPoint();

        size_t rightTrapezoidIDX = std::numeric_limits<size_t>::max();

        if(rightTrapezoidExist){

            rightTrapezoidIDX = newIdx++; // Taking the next id of the bottomTrapezoid or leftTrapezoid if exist

            Trapezoid rightTrapezoid = Trapezoid(trapezoidalMap.getTrapezoid(intersectedT).getTopSegment(), trapezoidalMap.getTrapezoid(intersectedT).getBottomSegment(),
                                                 segment.p2(), trapezoidalMap.getTrapezoid(intersectedT).getRightPoint());
        }

        // Adding neighbor to every trapezoid
        // TOP Trap

        // if leftTrapezoid exist it will be the upperleft neighbor, else the left point of the intersected trapezoid is equal to its top segment left endpoint then it has no neighbor,
        // else the neighbor is the upperLeft neighbor of the intersected trapezoid
        if(leftTrapezoidExist) topTrapezoid.setUpperLeftNeighbor(leftTrapezoidIDX); //ID del trapezoid non il trapezoid
        else if (!leftPointEqualLeftEndpoint(trapezoidalMap.getTrapezoid(intersectedT))) topTrapezoid.setUpperLeftNeighbor(trapezoidalMap.getTrapezoid(intersectedT).getUpperLeftNeighbor());
        else topTrapezoid.setUpperLeftNeighbor(std::numeric_limits<size_t>::max());


        if(rightTrapezoidExist) topTrapezoid.setUpperRightNeigbor(rightTrapezoidIDX);
        else if (!rightPointEqualRightEndpoint(trapezoidalMap.getTrapezoid(intersectedT))) topTrapezoid.setUpperRightNeigbor(trapezoidalMap.getTrapezoid(intersectedT).getUpperRightNeighbor());
        else topTrapezoid.setUpperRightNeigbor(std::numeric_limits<size_t>::max());

        // No lower Neighbor
        topTrapezoid.setLowerLeftNeighbor(std::numeric_limits<size_t>::max());
        topTrapezoid.setLowerRightNeighbor(std::numeric_limits<size_t>::max());

        // Bottom Trap

        // No upper Neighbor
        bottomTrapezoid.setUpperLeftNeighbor(std::numeric_limits<size_t>::max());
        bottomTrapezoid.setUpperRightNeigbor(std::numeric_limits<size_t>::max());

        // Need to do as above for the bottom segment
    }

}
