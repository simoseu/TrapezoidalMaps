#include "algorithms.h"
#include <cg3/geometry/utils2.h> // To use the isPoitAtLeft() utility

//Limits for the bounding box
//It defines where points can be added
//Do not change the following line
#define BOUNDINGBOX 1e+6

namespace algorithms{
// Order the points of a segment by x values
void orderSegment(cg3::Segment2d &segment){
    if (segment.p1().x() > segment.p2().x()) {
        cg3::Point2d aux = segment.p1();
        segment.setP1(segment.p2());
        segment.setP2(aux);
    }
}

// Return true if the left point of a trapezoid is equal to the left endpoint of its top segment
bool leftPointEqualTopLeftEndpoint(Trapezoid trapezoid){
    cg3::Segment2d topSegment = trapezoid.getTopSegment();
    orderSegment(topSegment);
    return trapezoid.getLeftPoint() == topSegment.p1();
}

bool rightPointEqualTopRightEndpoint(Trapezoid trapezoid){
    cg3::Segment2d topSegment = trapezoid.getTopSegment();
    orderSegment(topSegment);
    return trapezoid.getRightPoint() == topSegment.p2();
}

bool leftPointEqualBottomLeftEndpoint(Trapezoid trapezoid){
    cg3::Segment2d bottomSegment = trapezoid.getBottomSegment();
    orderSegment(bottomSegment);
    return trapezoid.getLeftPoint() == bottomSegment.p1();
}

bool rightPointEqualBottomRightEndpoint(Trapezoid trapezoid){
    cg3::Segment2d bottomSegment = trapezoid.getBottomSegment();
    orderSegment(bottomSegment);
    return trapezoid.getRightPoint() == bottomSegment.p2();
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
    assert(segment.p1().x() < segment.p2().x());
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
void initializeStructures(Dag &dag, DrawableTrapezoidalMap &trapezoidalMap){
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

void buildTrapezoidalMap(const cg3::Segment2d &segment, Dag &dag, DrawableTrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapData){
    // Before adding a segment is necessary to: Determine a bounding box R that contains all segments of S, and initialize the trapezoidal map structure T and search structure D for it.

    // Ordering the segment for ensuring that the second point (p2) is the right endpoint of the segment
    cg3::Segment2d orderedSegment = segment;
    if (segment.p1().x() > segment.p2().x()) {
        orderedSegment.setP1(segment.p2());
        orderedSegment.setP2(segment.p1());
    }

    // Get the intersected trapezoids with the function followSegment
    std::vector<size_t> intersectedTrapezoids = followSegment(orderedSegment, dag, trapezoidalMap, trapezoidalMapData);
    assert(intersectedTrapezoids.size() >= 1);
    std::cout << std::endl;
    std::cout << "Numero intersected trap: " << intersectedTrapezoids.size() << std::endl;
    std::cout << "IDX intersected trap: " << intersectedTrapezoids[0] << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    // Split in two case to handle - the segment intersect only one trapezoid and the segment intersect more trapezoid
    // Only one trapezoid intersected
    if(intersectedTrapezoids.size() == 1){
        // In this case the trapezoid will be replaced with at most 4 trapezoid. Is possible that there is no left or right trapezoid
        size_t intersectedTrapIdx = intersectedTrapezoids[0];
        oneIntersectedTrapezoid(orderedSegment, intersectedTrapIdx, dag, trapezoidalMap, trapezoidalMapData);
    }else{ // If more trapezoids are intersected by the segment
        moreIntersectedTrapezoids(orderedSegment, intersectedTrapezoids, dag, trapezoidalMap, trapezoidalMapData);
    }
}


void oneIntersectedTrapezoid(const cg3::Segment2d &segment, size_t intersectedTrapIdx, Dag &dag, DrawableTrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapData){

    // Utility - use the max value of size_t as arbitrary index for a null index
    size_t nullIdx = std::numeric_limits<size_t>::max();

    // Copy of the intersected trapezoid
    Trapezoid intersectedTrapCopy = trapezoidalMap.getTrapezoid(intersectedTrapIdx);

    // Checking if left and right trapezoid exist
    bool leftTrapezoidExists = segment.p1() != intersectedTrapCopy.getLeftPoint();   // If the leftPoint of the trapezoid is equal to the left endpoint of the segment the left trapezoid not exist
    bool rightTrapezoidExists = segment.p2() != intersectedTrapCopy.getRightPoint(); // Same with rightPoint and right endpoint of the segment for the right trapezoid

    // Setting IDX of new trapezoids
    size_t newIdx = trapezoidalMap.getTrapezoids().size();  // new index - after the last trapezoid in vector
    size_t topTrapezoidIdx = intersectedTrapIdx;  // Taking the idx of the intersected trapezoid
    size_t bottomTrapezoidIdx = newIdx++;  // Taking the idx next to the last trapezoid in the vector
    // left and right Trapezoid may not exist so the index is given after checking if they exist
    size_t leftTrapezoidIdx = leftTrapezoidExists ? newIdx++ : nullIdx;
    size_t rightTrapezoidIdx = rightTrapezoidExists ? newIdx : nullIdx;

    // Setting indexes of the dag nodes
    newIdx = intersectedTrapCopy.getNodeIdx();     // Since we need to replace node that represent the intersected trapezoid we get its index
    size_t xNodeLeft = nullIdx;
    size_t leafTrapLeft = nullIdx;
    size_t xNodeRight = nullIdx;
    size_t leafTrapRight = nullIdx;

    if(leftTrapezoidExists){// the first x-node only if left trapezoid exist
        xNodeLeft = newIdx;
        newIdx = dag.getNodes().size(); // Taking the index next to the last node in the vector
        leafTrapLeft = newIdx++;
    }
    if(rightTrapezoidExists){        // The second x-node only if the right trapezoid exist
        xNodeRight = !leftTrapezoidExists ? newIdx : newIdx++;
        if(!leftTrapezoidExists) newIdx = dag.getNodes().size();
        leafTrapRight = newIdx++;
    }
    // Y-node
    size_t yNode = leftTrapezoidExists || rightTrapezoidExists ? newIdx++ : newIdx;
    // If left and righ trap not exist need to take the index next to the last node in the vector
    if( !(leftTrapezoidExists || rightTrapezoidExists) ) newIdx = dag.getNodes().size();
    // Top and Bottom Trap
    size_t topTrapLeaf = newIdx++;
    size_t bottomTrapLeaf = newIdx;

    // If the intersected trap has neighbors, need to update their neighbors
    if(intersectedTrapCopy.getLowerLeftNeighbor() != nullIdx){ // Lower Left neighbor
        size_t newNeighbor = leftTrapezoidExists ? leftTrapezoidIdx : bottomTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getLowerLeftNeighbor()).setLowerRightNeighbor(newNeighbor);
    }
    if(intersectedTrapCopy.getUpperLeftNeighbor() != nullIdx){  // Upper Left neighbor
        size_t newNeighbor = leftTrapezoidExists ? leftTrapezoidIdx : topTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getUpperLeftNeighbor()).setUpperRightNeigbor(newNeighbor);
    }
    if(intersectedTrapCopy.getUpperRightNeighbor() != nullIdx){ // Upper Right neighbor
        size_t newNeighbor = rightTrapezoidExists ? rightTrapezoidIdx : topTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getUpperRightNeighbor()).setUpperLeftNeighbor(newNeighbor);
    }
    if(intersectedTrapCopy.getLowerRightNeighbor() != nullIdx){ // Lower Right neighbor
        size_t newNeighbor = rightTrapezoidExists ? rightTrapezoidIdx : bottomTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getLowerRightNeighbor()).setLowerLeftNeighbor(newNeighbor);
    }

    // --------------UPDATING THE TRAPEZOIDAL MAP------------------
    // TOP TRAPEZOID

    Trapezoid topTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), segment, segment.p1(), segment.p2());
    if(leftTrapezoidExists) topTrapezoid.setUpperLeftNeighbor(leftTrapezoidIdx);
    else if (!leftPointEqualTopLeftEndpoint(intersectedTrapCopy)) topTrapezoid.setUpperLeftNeighbor(intersectedTrapCopy.getUpperLeftNeighbor());
    else topTrapezoid.setUpperLeftNeighbor(nullIdx);
    // - top right neighbor
    if(rightTrapezoidExists) topTrapezoid.setUpperRightNeigbor(rightTrapezoidIdx);
    else if (!rightPointEqualTopRightEndpoint(intersectedTrapCopy)) topTrapezoid.setUpperRightNeigbor(intersectedTrapCopy.getUpperRightNeighbor());
    else topTrapezoid.setUpperRightNeigbor(nullIdx);
    // - No lower Neighbor
    topTrapezoid.setLowerLeftNeighbor(nullIdx);
    topTrapezoid.setLowerRightNeighbor(nullIdx);
    // Dag leaf idx
    topTrapezoid.setNodeIdx(topTrapLeaf);
    // Add to the trapezoidal map in place of the intesected trapezoid
    trapezoidalMap.replaceTrapezoid(topTrapezoid, topTrapezoidIdx);

    // BOTTOM TRAPEZOID
    Trapezoid bottomTrapezoid = Trapezoid(segment, intersectedTrapCopy.getBottomSegment(), segment.p1(), segment.p2());
    // - No upper Neighbor
    bottomTrapezoid.setUpperLeftNeighbor(nullIdx);
    bottomTrapezoid.setUpperRightNeigbor(nullIdx);
    // - lower left neighbor
    if(leftTrapezoidExists) bottomTrapezoid.setLowerLeftNeighbor(leftTrapezoidIdx);
    else if(!leftPointEqualBottomLeftEndpoint(intersectedTrapCopy)) bottomTrapezoid.setLowerLeftNeighbor(intersectedTrapCopy.getLowerLeftNeighbor());
    else bottomTrapezoid.setLowerLeftNeighbor(nullIdx);
    // - lower right neighbor
    if(rightTrapezoidExists) bottomTrapezoid.setLowerRightNeighbor(rightTrapezoidIdx);
    else if(!rightPointEqualBottomRightEndpoint(intersectedTrapCopy)) bottomTrapezoid.setLowerRightNeighbor(intersectedTrapCopy.getLowerRightNeighbor());
    else bottomTrapezoid.setLowerRightNeighbor(nullIdx);
    // Dag leaf idx
    bottomTrapezoid.setNodeIdx(bottomTrapLeaf);
    trapezoidalMap.addTrapezoid(bottomTrapezoid);

    // LEFT TRAPEZOID
    if(leftTrapezoidExists){
        Trapezoid leftTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), intersectedTrapCopy.getBottomSegment(),
                                            intersectedTrapCopy.getLeftPoint(), segment.p1());
        leftTrapezoid.setUpperLeftNeighbor(intersectedTrapCopy.getUpperLeftNeighbor());
        leftTrapezoid.setUpperRightNeigbor(topTrapezoidIdx);
        leftTrapezoid.setLowerLeftNeighbor(intersectedTrapCopy.getLowerLeftNeighbor());
        leftTrapezoid.setLowerRightNeighbor(bottomTrapezoidIdx);
        // Dag leaf idx
        leftTrapezoid.setNodeIdx(leafTrapLeft);
        trapezoidalMap.addTrapezoid(leftTrapezoid);
    }

    // RIGHT TRAPEZOID
    if(rightTrapezoidExists){
        Trapezoid rightTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), intersectedTrapCopy.getBottomSegment(),
                                             segment.p2(), intersectedTrapCopy.getRightPoint());
        rightTrapezoid.setUpperLeftNeighbor(topTrapezoidIdx);
        rightTrapezoid.setUpperRightNeigbor(intersectedTrapCopy.getUpperRightNeighbor());
        rightTrapezoid.setLowerLeftNeighbor(bottomTrapezoidIdx);
        rightTrapezoid.setLowerRightNeighbor(intersectedTrapCopy.getLowerRightNeighbor());
        // Dag leaf idx
        rightTrapezoid.setNodeIdx(leafTrapRight);
        trapezoidalMap.addTrapezoid(rightTrapezoid);
    }

    // -------------------- UPDATE THE DAG ------------------------------
    /* Need to replace the leaf of the intersected trapezoid with a tree:
     *      - 1 x-node with the left endpoint of the inserted segment (if is to the left than the trapezoid is leftTrapezoid, if is to the right than test the other x-node)
     *      - 1 x-node with the right endpoint of the inserted segment (if is to the left than test the y-node, if is to the right is the rightTrapezoid)
     *      - 1 y-node with the intersected segment (if is above is the topTrap, if is below is the bottomTrap)
     *      - At most 4 leaf representing the new trapezoids
    */

    // Create the substree of the dag
    if(leftTrapezoidExists){
        // nodo x
        bool found = false;
        size_t pointIdx = trapezoidalMapData.findPoint(segment.p1(), found);
        assert(found == true);
        Node newNode = Node(Node::NodeType::X, pointIdx, leafTrapLeft, rightTrapezoidExists ? xNodeRight : yNode);
        dag.replaceNode(newNode, xNodeLeft);
        // left trap
        newNode = Node(Node::NodeType::LEAF, leftTrapezoidIdx, nullIdx, nullIdx);
        dag.addNode(newNode);
    }

    if(rightTrapezoidExists){
        // node x
        bool found = false;
        size_t pointIdx = trapezoidalMapData.findPoint(segment.p2(), found);
        assert(found == true);
        Node newNode = Node(Node::NodeType::X, pointIdx, yNode, leafTrapRight);
        if(leftTrapezoidExists) dag.addNode(newNode);
        else dag.replaceNode(newNode, xNodeRight);

        //right trap
        newNode = Node(Node::NodeType::LEAF, rightTrapezoidIdx, nullIdx, nullIdx);
        dag.addNode(newNode);
    }

    // y-node
    bool found = false;
    size_t segmentIdx = trapezoidalMapData.findSegment(segment, found); // SE NON FUNZIONA RIMETTERE SEMGMENT INVECE DI ORDERED SEGMENT
    assert(found == true); // Se non lo trova esce
    Node newNode = Node(Node::NodeType::Y, segmentIdx, topTrapLeaf, bottomTrapLeaf);
    if(leftTrapezoidExists || rightTrapezoidExists) dag.addNode(newNode);
    else dag.replaceNode(newNode, yNode);

    // Leaf top trap
    newNode = Node(Node::NodeType::LEAF, topTrapezoidIdx, nullIdx, nullIdx);
    dag.addNode(newNode);

    // Leaf bottom trap
    newNode = Node(Node::NodeType::LEAF, bottomTrapezoidIdx, nullIdx, nullIdx);
    dag.addNode(newNode);
}


void moreIntersectedTrapezoids(const cg3::Segment2d &segment, std::vector<size_t> intersectedTraps, Dag &dag, DrawableTrapezoidalMap &trapezoidalMap, TrapezoidalMapDataset &trapezoidalMapData){
    // Utility - use the max value of size_t as arbitrary index for a null index
    size_t nullIdx = std::numeric_limits<size_t>::max();
    // ------------------------------- Leftmost Trapezoid intersected ---------------------------------------------
    // Index and copy of the current analyzed trapezoid
    size_t intersectedTrapIdx = intersectedTraps[0];
    Trapezoid intersectedTrapCopy = trapezoidalMap.getTrapezoid(intersectedTrapIdx);

    // Checking if left and right trapezoid exist
    bool leftTrapezoidExists = segment.p1() != intersectedTrapCopy.getLeftPoint();   // If the leftPoint of the trapezoid is equal to the left endpoint of the segment the left trapezoid not exist
    bool rightTrapezoidExists = segment.p2() != trapezoidalMap.getTrapezoid(intersectedTraps.back()).getRightPoint();

    // Setting IDX of new trapezoids
    size_t newIdx = trapezoidalMap.getTrapezoids().size();  // new index - after the last trapezoid in vector
    size_t topTrapezoidIdx = intersectedTrapIdx;  // Taking the idx of the intersected trapezoid
    size_t bottomTrapezoidIdx = newIdx++;  // Taking the idx next to the last trapezoid in the vector
    // left and right Trapezoid may not exist so the index is given after checking if they exist
    size_t leftTrapezoidIdx = leftTrapezoidExists ? newIdx++ : nullIdx;

    // Setting indexes of the dag nodes
    newIdx = intersectedTrapCopy.getNodeIdx();     // Since we need to replace node that represent the intersected trapezoid we get its index
    size_t xNodeLeft = nullIdx;
    size_t leafTrapLeft = nullIdx;

    if(leftTrapezoidExists){// the first x-node only if left trapezoid exist
        xNodeLeft = newIdx;
        newIdx = dag.getNodes().size(); // Taking the index next to the last node in the vector
        leafTrapLeft = newIdx++;
    }
    // Y-node
    size_t yNode = leftTrapezoidExists ? newIdx++ : newIdx;
    // If left trap not exist need to take the index next to the last node in the vector
    if( !(leftTrapezoidExists ) ) newIdx = dag.getNodes().size();
    // Top and Bottom Trap
    size_t topTrapLeaf = newIdx++;
    size_t bottomTrapLeaf = newIdx;

    // If the intersected trap has neighbors, need to update their neighbors
    if(intersectedTrapCopy.getLowerLeftNeighbor() != nullIdx){ // Lower Left neighbor
        size_t newNeighbor = leftTrapezoidExists ? leftTrapezoidIdx : bottomTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getLowerLeftNeighbor()).setLowerRightNeighbor(newNeighbor);
    }
    if(intersectedTrapCopy.getUpperLeftNeighbor() != nullIdx){  // Upper Left neighbor
        size_t newNeighbor = leftTrapezoidExists ? leftTrapezoidIdx : topTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getUpperLeftNeighbor()).setUpperRightNeigbor(newNeighbor);
    }

    // --------------UPDATING THE TRAPEZOIDAL MAP------------------
    // TOP TRAPEZOID

    Trapezoid topTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), segment, segment.p1(), cg3::Point2d()); // Unknown right point

    if(leftTrapezoidExists) topTrapezoid.setUpperLeftNeighbor(leftTrapezoidIdx);
    else if (!leftPointEqualTopLeftEndpoint(intersectedTrapCopy)) topTrapezoid.setUpperLeftNeighbor(intersectedTrapCopy.getUpperLeftNeighbor());
    else topTrapezoid.setUpperLeftNeighbor(nullIdx);

    // - top right neighbor UNKNOWN
    topTrapezoid.setUpperRightNeigbor(nullIdx);
    // - No lower Neighbor UNKNOWN
    topTrapezoid.setLowerLeftNeighbor(nullIdx);
    topTrapezoid.setLowerRightNeighbor(nullIdx);
    // Dag leaf idx
    topTrapezoid.setNodeIdx(topTrapLeaf);
    // Add to the trapezoidal map in place of the intesected trapezoid
    //trapezoidalMap.replaceTrapezoid(topTrapezoid, topTrapezoidIdx);

    // BOTTOM TRAPEZOID
    Trapezoid bottomTrapezoid = Trapezoid(segment, intersectedTrapCopy.getBottomSegment(), segment.p1(), cg3::Point2d()); // Unknown right point
    // - No upper Neighbor
    bottomTrapezoid.setUpperLeftNeighbor(nullIdx);
    bottomTrapezoid.setUpperRightNeigbor(nullIdx);
    // - lower left neighbor
    if(leftTrapezoidExists) bottomTrapezoid.setLowerLeftNeighbor(leftTrapezoidIdx);
    else if(!leftPointEqualBottomLeftEndpoint(intersectedTrapCopy)) bottomTrapezoid.setLowerLeftNeighbor(intersectedTrapCopy.getLowerLeftNeighbor());
    else bottomTrapezoid.setLowerLeftNeighbor(nullIdx);

    // - lower right neighbor UNDNOWN
    bottomTrapezoid.setLowerRightNeighbor(nullIdx);
    // Dag leaf idx
    bottomTrapezoid.setNodeIdx(bottomTrapLeaf);
    //trapezoidalMap.addTrapezoid(bottomTrapezoid);

    // LEFT TRAPEZOID
    if(leftTrapezoidExists){
        Trapezoid leftTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), intersectedTrapCopy.getBottomSegment(),
                                            intersectedTrapCopy.getLeftPoint(), segment.p1());
        leftTrapezoid.setUpperLeftNeighbor(intersectedTrapCopy.getUpperLeftNeighbor());
        leftTrapezoid.setUpperRightNeigbor(topTrapezoidIdx);
        leftTrapezoid.setLowerLeftNeighbor(intersectedTrapCopy.getLowerLeftNeighbor());
        leftTrapezoid.setLowerRightNeighbor(bottomTrapezoidIdx);
        // Dag leaf idx
        leftTrapezoid.setNodeIdx(leafTrapLeft);
        trapezoidalMap.addTrapezoid(leftTrapezoid);
    }

    // -------------------- UPDATE THE DAG ---------------------------

    // Create the substree of the dag
    if(leftTrapezoidExists){
        // nodo x
        bool found = false;
        size_t pointIdx = trapezoidalMapData.findPoint(segment.p1(), found);
        assert(found == true);
        Node newNode = Node(Node::NodeType::X, pointIdx, leafTrapLeft, yNode);
        dag.replaceNode(newNode, xNodeLeft);
        // left trap
        newNode = Node(Node::NodeType::LEAF, leftTrapezoidIdx, nullIdx, nullIdx);
        dag.addNode(newNode);
    }

    // y-node
    bool found = false;
    size_t segmentIdx = trapezoidalMapData.findSegment(segment, found);
    assert(found == true); // Se non lo trova esce
    Node newNode = Node(Node::NodeType::Y, segmentIdx, topTrapLeaf, bottomTrapLeaf);
    if(leftTrapezoidExists) dag.addNode(newNode);
    else dag.replaceNode(newNode, yNode);

    // Leaf top trap
    newNode = Node(Node::NodeType::LEAF, topTrapezoidIdx, nullIdx, nullIdx);
    dag.addNode(newNode);

    // Leaf bottom trap
    newNode = Node(Node::NodeType::LEAF, bottomTrapezoidIdx, nullIdx, nullIdx);
    dag.addNode(newNode);


    // ------------------------------- Internal Trapezoid intersected ---------------------------------------------

    // Need to store a copy of the previous intersected trap and information about previous bottom and top trapezoid (since they have not been added)
    Trapezoid prevIntersectedTrapezoid = intersectedTrapCopy;
    size_t previousTopTrapLeaf = topTrapLeaf;
    size_t previousBottomTrapLeaf = bottomTrapLeaf;
    size_t previousTopTrapIdx = topTrapezoidIdx;
    size_t previousBottomTrapIdx = bottomTrapezoidIdx;

    // For each internal trapezoid we create 2 trapezoids (top and bottom)
    for(size_t i = 1; i < intersectedTraps.size()-1; i++){
        // Index and copy of the current analyzed trapezoid
        intersectedTrapIdx = intersectedTraps[i];
        intersectedTrapCopy = trapezoidalMap.getTrapezoid(intersectedTrapIdx);

        // The trapezoid could be an upper right or a lower right neighbor of the previously analyzed trapezoid
        /*
         * If the right point of Ti is over the segment then Ti+1 is the lower right neighbor, otherwise it is the upper right neighbor
        */
        // If the right point is above
        if(cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint())){
            // Index of the new top trapezoid
            topTrapezoidIdx = intersectedTrapIdx;
            // Index for the DAG nodes (one y-node and top and bottom trapezoid leaves)
            yNode = intersectedTrapCopy.getNodeIdx();
            topTrapLeaf = dag.getNodes().size();
            bottomTrapLeaf = previousBottomTrapLeaf; // It is not changed

            // UPDATE THE NEIGHBOR of previous trap?
            // ------------------------ Trapezoidal Map Update -----------------------------
            // Now the top trapezoid previously created can be added to the trapezoidal map (topTrapezoid is already the previous
            topTrapezoid.setLowerRightNeighbor(topTrapezoidIdx);    // The new top trap became the lower right neighbor of the previous
            topTrapezoid.setUpperRightNeigbor(prevIntersectedTrapezoid.getUpperRightNeighbor());
            topTrapezoid.setRightPoint(intersectedTrapCopy.getLeftPoint());

            assert(previousTopTrapIdx >= trapezoidalMap.getTrapezoids().size()); // Fare un controllo per decidere se fare il replace o l'add
            trapezoidalMap.replaceTrapezoid(topTrapezoid, previousTopTrapIdx);

            // New top trapezoid
            topTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), segment, intersectedTrapCopy.getLeftPoint(), intersectedTrapCopy.getRightPoint(),
                                     intersectedTrapCopy.getUpperLeftNeighbor(), previousTopTrapIdx, intersectedTrapCopy.getUpperRightNeighbor(), intersectedTrapCopy.getLowerRightNeighbor(),
                                     topTrapLeaf);
            // ------------------------ DAG Update -----------------------------
            // Y-node
            newNode = Node(Node::NodeType::Y, segmentIdx, topTrapLeaf, bottomTrapLeaf);
            dag.replaceNode(newNode, yNode);
            // Top trap leaf
            newNode = Node(Node::NodeType::LEAF, topTrapezoidIdx, nullIdx, nullIdx);

            previousTopTrapIdx = topTrapezoidIdx; // Updating the top trapezoid idx
            previousTopTrapLeaf = topTrapLeaf;    // and its dag leaf idx

        }else{ // The right point is below the segment (Ti+1 is the upper right neighbor)
            // Index of the new bottom trapezoid
            bottomTrapezoidIdx = intersectedTrapIdx;
            // Index for the DAG nodes (one y-node and top and bottom trapezoid leaves)
            yNode = intersectedTrapCopy.getNodeIdx();
            topTrapLeaf = previousTopTrapLeaf; // It is not changed
            bottomTrapLeaf = dag.getNodes().size();

            // ------------------------ Trapezoidal Map Update -----------------------------
            // The bottom trapezoid previously created can be added to the trapezoidal map (updating its data)
            bottomTrapezoid.setLowerRightNeighbor(prevIntersectedTrapezoid.getLowerRightNeighbor());
            bottomTrapezoid.setUpperRightNeigbor(bottomTrapezoidIdx);
            bottomTrapezoid.setRightPoint(intersectedTrapCopy.getLeftPoint());

            assert(previousBottomTrapIdx < trapezoidalMap.getTrapezoids().size());
            trapezoidalMap.replaceTrapezoid(bottomTrapezoid, previousBottomTrapIdx);

            // New bottom trap
            bottomTrapezoid = Trapezoid(segment, intersectedTrapCopy.getBottomSegment(), intersectedTrapCopy.getLeftPoint(), intersectedTrapCopy.getRightPoint(),
                                     previousBottomTrapIdx, intersectedTrapCopy.getLowerLeftNeighbor(), intersectedTrapCopy.getUpperRightNeighbor(), intersectedTrapCopy.getLowerRightNeighbor(),
                                     bottomTrapLeaf);

            // ------------------------ DAG Update -----------------------------
            // Y-node
            newNode = Node(Node::NodeType::Y, segmentIdx, topTrapLeaf, bottomTrapLeaf);
            dag.replaceNode(newNode, yNode);
            // Top trap leaf
            newNode = Node(Node::NodeType::LEAF, bottomTrapezoidIdx, nullIdx, nullIdx);

            previousBottomTrapIdx = bottomTrapezoidIdx; // Updating the bottom trapezoid idx
            previousBottomTrapLeaf = bottomTrapLeaf;    // and its dag leaf idx
        }

        prevIntersectedTrapezoid = intersectedTrapCopy;
    }

    // ------------------------------- Rightmost Trapezoid intersected ---------------------------------------------

    intersectedTrapIdx = intersectedTraps.back(); // Taking the last trapezoid in the vector
    intersectedTrapCopy = trapezoidalMap.getTrapezoid(intersectedTrapIdx);

    if(cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint())){
        topTrapezoidIdx = intersectedTrapIdx;
    }else{
        bottomTrapezoidIdx = intersectedTrapIdx;
    }
    size_t rightTrapezoidIdx = nullIdx; // Aggiustare l'idx se esiste


    // Setting indexes of the dag nodes
    newIdx = intersectedTrapCopy.getNodeIdx();     // Since we need to replace node that represent the intersected trapezoid we get its index
    size_t xNodeRight = nullIdx;
    size_t leafTrapRight = nullIdx;

    if(rightTrapezoidExists){// the first x-node only if left trapezoid exist
        xNodeRight = newIdx;
        newIdx = dag.getNodes().size(); // Taking the index next to the last node in the vector
        leafTrapRight = newIdx++;
    }
    // Y-node
    yNode = rightTrapezoidExists ? newIdx++ : newIdx;
    // If left trap not exist need to take the index next to the last node in the vector
    if( !(rightTrapezoidExists ) ) newIdx = dag.getNodes().size();
    // Top and Bottom Trap
    topTrapLeaf = cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint()) ?  newIdx++ : previousTopTrapLeaf;
    bottomTrapLeaf = cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint()) ? previousBottomTrapLeaf : newIdx;

    // Update the neighbors of the neighbors trapezoid
    if(prevIntersectedTrapezoid.getUpperRightNeighbor() != nullIdx){
        trapezoidalMap.getTrapezoid(prevIntersectedTrapezoid.getUpperRightNeighbor()).setUpperLeftNeighbor(previousTopTrapIdx);
    }
    if(prevIntersectedTrapezoid.getLowerRightNeighbor() != nullIdx){
        trapezoidalMap.getTrapezoid(prevIntersectedTrapezoid.getLowerRightNeighbor()).setLowerLeftNeighbor(previousBottomTrapIdx);
    }
    if(intersectedTrapCopy.getUpperRightNeighbor() != nullIdx){
        size_t neighbor = rightTrapezoidExists ? rightTrapezoidIdx : topTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getUpperRightNeighbor()).setUpperLeftNeighbor(neighbor);
    }
    if(intersectedTrapCopy.getLowerRightNeighbor() != nullIdx){
        size_t neighbor = rightTrapezoidExists ? rightTrapezoidIdx : bottomTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getLowerRightNeighbor()).setLowerLeftNeighbor(neighbor);
    }

    // ------------------------ Trapezoidal Map Update -----------------------------
    if(cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint())){
        // Now is possible to add the top and bottom trapezoids previously initializated
        topTrapezoid.setRightPoint(intersectedTrapCopy.getLeftPoint());
        topTrapezoid.setUpperRightNeigbor(prevIntersectedTrapezoid.getUpperRightNeighbor());
        topTrapezoid.setLowerRightNeighbor(topTrapezoidIdx);

        assert(previousTopTrapIdx < trapezoidalMap.getTrapezoids().size());
        trapezoidalMap.replaceTrapezoid(topTrapezoid, previousTopTrapIdx);

        // Also for the bottom trapezoid
        bottomTrapezoid.setRightPoint(segment.p2());
        bottomTrapezoid.setUpperRightNeigbor(nullIdx);

        if(rightTrapezoidExists) bottomTrapezoid.setLowerRightNeighbor(rightTrapezoidIdx);
        else if(!rightPointEqualBottomRightEndpoint(intersectedTrapCopy)) bottomTrapezoid.setLowerRightNeighbor(intersectedTrapCopy.getLowerRightNeighbor());
        else bottomTrapezoid.setLowerRightNeighbor(nullIdx);

        assert(previousBottomTrapIdx < trapezoidalMap.getTrapezoids().size());
        trapezoidalMap.replaceTrapezoid(bottomTrapezoid, previousBottomTrapIdx);

        // New top trap
        topTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), segment, intersectedTrapCopy.getLeftPoint(), segment.p2());
        topTrapezoid.setUpperLeftNeighbor(intersectedTrapCopy.getUpperLeftNeighbor());
        // - top right neighbor
        if(rightTrapezoidExists) topTrapezoid.setUpperRightNeigbor(rightTrapezoidIdx);
        else if (!rightPointEqualTopRightEndpoint(intersectedTrapCopy)) topTrapezoid.setUpperRightNeigbor(intersectedTrapCopy.getUpperRightNeighbor());
        else topTrapezoid.setUpperRightNeigbor(nullIdx);

        topTrapezoid.setLowerLeftNeighbor(previousTopTrapIdx);
        topTrapezoid.setLowerRightNeighbor(nullIdx);
        topTrapezoid.setNodeIdx(topTrapLeaf);

        trapezoidalMap.replaceTrapezoid(topTrapezoid, topTrapezoidIdx);

        previousTopTrapIdx = topTrapezoidIdx;

    }else{

        bottomTrapezoid.setRightPoint(intersectedTrapCopy.getLeftPoint());
        bottomTrapezoid.setUpperRightNeigbor(bottomTrapezoidIdx);
        bottomTrapezoid.setLowerRightNeighbor(prevIntersectedTrapezoid.getLowerRightNeighbor());
        assert(previousBottomTrapIdx < trapezoidalMap.getTrapezoids().size());
        trapezoidalMap.replaceTrapezoid(bottomTrapezoid, previousBottomTrapIdx);

        topTrapezoid.setRightPoint(segment.p2());
        topTrapezoid.setLowerRightNeighbor(nullIdx);
        if(rightTrapezoidExists) topTrapezoid.setUpperRightNeigbor(rightTrapezoidIdx);
        else if (!rightPointEqualTopRightEndpoint(intersectedTrapCopy)) topTrapezoid.setUpperRightNeigbor(intersectedTrapCopy.getUpperRightNeighbor());
        else topTrapezoid.setUpperRightNeigbor(nullIdx);

        assert(previousTopTrapIdx < trapezoidalMap.getTrapezoids().size());
        trapezoidalMap.replaceTrapezoid(topTrapezoid, previousTopTrapIdx);

        bottomTrapezoid = Trapezoid(segment, intersectedTrapCopy.getBottomSegment(), intersectedTrapCopy.getLeftPoint(), segment.p2());
        bottomTrapezoid.setUpperLeftNeighbor(previousBottomTrapIdx);
        bottomTrapezoid.setUpperRightNeigbor(nullIdx);
        bottomTrapezoid.setLowerLeftNeighbor(intersectedTrapCopy.getLowerLeftNeighbor());
        if(rightTrapezoidExists) bottomTrapezoid.setLowerRightNeighbor(rightTrapezoidIdx);
        else if(!rightPointEqualBottomRightEndpoint(intersectedTrapCopy)) bottomTrapezoid.setLowerRightNeighbor(intersectedTrapCopy.getLowerRightNeighbor());
        else bottomTrapezoid.setLowerRightNeighbor(nullIdx);

        trapezoidalMap.replaceTrapezoid(bottomTrapezoid, bottomTrapezoidIdx);

        previousBottomTrapIdx = bottomTrapezoidIdx;
    }

    // Possibile right Trapezoid
    if(rightTrapezoidExists){
        Trapezoid rightTrapezoid = intersectedTrapCopy;
        rightTrapezoid.setLeftPoint(segment.p2());
        rightTrapezoid.setUpperLeftNeighbor(previousTopTrapIdx);
        rightTrapezoid.setLowerLeftNeighbor(previousBottomTrapIdx);
        rightTrapezoid.setNodeIdx(leafTrapRight);
        trapezoidalMap.addTrapezoid(rightTrapezoid);
    }

    // DAG UPDATE
    if(rightTrapezoidExists){

        size_t pointIdx = trapezoidalMapData.findPoint(segment.p2(), found);
        assert(found == true);
        newNode = Node(Node::NodeType::X, pointIdx, yNode, leafTrapRight);
        dag.replaceNode(newNode, xNodeRight);

        newNode = Node(Node::NodeType::LEAF, rightTrapezoidIdx, nullIdx, nullIdx);
        dag.addNode(newNode);
    }

    newNode = Node(Node::NodeType::Y, segmentIdx, topTrapLeaf, bottomTrapLeaf);
    rightTrapezoidExists ? dag.addNode(newNode) : dag.replaceNode(newNode, yNode);

    if(cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint())){
        newNode = Node(Node::NodeType::LEAF, previousTopTrapIdx, nullIdx, nullIdx);
    }else{
        newNode = Node(Node::NodeType::LEAF, previousBottomTrapIdx, nullIdx, nullIdx);
    }
    dag.addNode(newNode);

}

}
