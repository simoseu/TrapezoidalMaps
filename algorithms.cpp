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
 * @brief Locate in which trapezoid lies the left endpoint of a given segment. Usefull for the building of the trapezoidal map.
 * @param[in] querySegment segment to query
 * @param[in] dag The DAG search structure
 * @param[in] TrapezoidalMapData The trapezoidal map dataset data structure
 * @return The index of the trapezoid in which lies the left endpoint of the query segment
*/
size_t querySegment(const cg3::Segment2d &querySegment, const Dag &dag, const TrapezoidalMapDataset &trapezoidalMapData){
    Node node = dag.getRoot();
    // Search in the dag until a leaf is found
    while(node.getType() != Node::NodeType::LEAF){
        // If the node is type "X" it refer to a point
        if(node.getType() == Node::NodeType::X){
            // The point "q" is to the left or to the right of the point
            const cg3::Point2d &point = trapezoidalMapData.getPoint(node.getIdx());
            if(querySegment.p1().x() < point.x() ){// If the point lies to the left of the segment's left endpoint
                node = dag.getNode(node.getLeftIdx());
            }else{ //If the point lies to the right of the segment's right endpoint
                node = dag.getNode(node.getRightIdx());
            }
        }else if(node.getType() == Node::NodeType::Y){ // The node refer to a segment
            // The point "q" is above or below the segment
            cg3::Segment2d segment = trapezoidalMapData.getSegment(node.getIdx());
            orderSegment(segment);
            if(cg3::isPointAtLeft(segment, querySegment.p1())){ // return true if the left endpoint point is above, false otherwise
                node = dag.getNode(node.getLeftIdx());
            }else if(cg3::isPointAtRight(segment, querySegment.p1())){ // If the left endpoint is below
                node = dag.getNode(node.getRightIdx());
            }else{ // Check right endpoint
                if(cg3::isPointAtLeft(segment, querySegment.p2())){
                    node = dag.getNode(node.getLeftIdx());
                }else{
                    node = dag.getNode(node.getRightIdx());
                }
            }
        }
    }

    return node.getIdx();
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
            cg3::Segment2d segment = trapezoidalMapData.getSegment(node.getIdx());
            orderSegment(segment);
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
    size_t idxTrapezoid = querySegment(segment, dag, trapezoidalMapData);
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
    size_t newIdx = trapezoidalMap.numTrapezoids();  // new index - after the last trapezoid in vector
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
        newIdx = dag.numNodes(); // Taking the index next to the last node in the vector
        leafTrapLeft = newIdx++;
    }
    if(rightTrapezoidExists){        // The second x-node only if the right trapezoid exist
        xNodeRight = !leftTrapezoidExists ? newIdx : newIdx++;
        if(!leftTrapezoidExists) newIdx = dag.numNodes();
        leafTrapRight = newIdx++;
    }
    // Y-node
    size_t yNode = leftTrapezoidExists || rightTrapezoidExists ? newIdx++ : newIdx;
    // If left and righ trap not exist need to take the index next to the last node in the vector
    if( !(leftTrapezoidExists || rightTrapezoidExists) ) newIdx = dag.numNodes();
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

    // Index and copy of the current analyzed trapezoid
    size_t intersectedTrapIdx = intersectedTraps.front(); // Take the first element
    Trapezoid intersectedTrapCopy = trapezoidalMap.getTrapezoid(intersectedTrapIdx);
    // Checking if left and right trapezoid exist
    bool leftTrapezoidExists = segment.p1() != intersectedTrapCopy.getLeftPoint();   // If the leftPoint of the trapezoid is equal to the left endpoint of the segment the left trapezoid not exist
    bool rightTrapezoidExists = segment.p2() != trapezoidalMap.getTrapezoid(intersectedTraps.back()).getRightPoint();

    // ------------------------------- Leftmost Trapezoid intersected ---------------------------------------------
    // Creation, if exist, of the leftmost trapezoid and initialization of a top and bottom trapezoid that can extend in the next intersected trapezoids

    // - Setting Indexes of new trapezoids
    size_t newIdx = trapezoidalMap.numTrapezoids();

    size_t topTrapezoidIdx = intersectedTrapIdx;  // Taking the idx of the intersected trapezoid
    // left Trapezoid may not exist so the index is given after checking if they exist
    size_t leftTrapezoidIdx = leftTrapezoidExists ? newIdx++ : nullIdx;
    size_t bottomTrapezoidIdx = newIdx++;

    // - Setting indexes of the dag nodes
    newIdx = intersectedTrapCopy.getNodeIdx();     // Since we need to replace the node that represent the intersected trapezoid we get its index
    size_t xNodeLeft = nullIdx;
    size_t leafTrapLeft = nullIdx;

    if(leftTrapezoidExists){// x-node and left trap leaf only if left trapezoid exist
        xNodeLeft = newIdx;
        newIdx = dag.numNodes();
        leafTrapLeft = newIdx++;
    }
    size_t yNode = leftTrapezoidExists ? newIdx++ : newIdx;
    // If left trap not exist need to take the index next to the last node in the vector
    if( !leftTrapezoidExists ) newIdx = dag.numNodes();
    // Top and Bottom Trap leaf
    size_t topTrapLeaf = newIdx++;
    size_t bottomTrapLeaf = newIdx;

    // If the intersected trap has neighbors to its left, need to update their right neighbors
    if(intersectedTrapCopy.getLowerLeftNeighbor() != nullIdx){ // Lower Left neighbor
        size_t newNeighbor = leftTrapezoidExists ? leftTrapezoidIdx : bottomTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getLowerLeftNeighbor()).setLowerRightNeighbor(newNeighbor);
    }
    if(intersectedTrapCopy.getUpperLeftNeighbor() != nullIdx){  // Upper Left neighbor
        size_t newNeighbor = leftTrapezoidExists ? leftTrapezoidIdx : topTrapezoidIdx;
        trapezoidalMap.getTrapezoid(intersectedTrapCopy.getUpperLeftNeighbor()).setUpperRightNeigbor(newNeighbor);
    }

    // --------------UPDATING THE TRAPEZOIDAL MAP------------------
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

    // TOP TRAPEZOID (it will be inserted while checking the internal trapezoids)

    Trapezoid topTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), segment, segment.p1(), cg3::Point2d(0,0)); // Unknown right point
    // setting its neighbors
    if(leftTrapezoidExists) topTrapezoid.setUpperLeftNeighbor(leftTrapezoidIdx);
    else if (!leftPointEqualTopLeftEndpoint(intersectedTrapCopy)) topTrapezoid.setUpperLeftNeighbor(intersectedTrapCopy.getUpperLeftNeighbor());
    else topTrapezoid.setUpperLeftNeighbor(nullIdx);

    topTrapezoid.setUpperRightNeigbor(nullIdx);     // - top right neighbor UNKNOWN
    topTrapezoid.setLowerLeftNeighbor(nullIdx);     // - No lower Neighbor UNKNOWN
    topTrapezoid.setLowerRightNeighbor(nullIdx);

    topTrapezoid.setNodeIdx(topTrapLeaf);           // Dag leaf idx

    // BOTTOM TRAPEZOID
    Trapezoid bottomTrapezoid = Trapezoid(segment, intersectedTrapCopy.getBottomSegment(), segment.p1(), cg3::Point2d(0,0)); // Unknown right point

    // setting its neighbors
    bottomTrapezoid.setUpperLeftNeighbor(nullIdx);      // - No upper Neighbor
    bottomTrapezoid.setUpperRightNeigbor(nullIdx);
    bottomTrapezoid.setLowerRightNeighbor(nullIdx);    // - lower right neighbor

    if(leftTrapezoidExists) bottomTrapezoid.setLowerLeftNeighbor(leftTrapezoidIdx);
    else if(!leftPointEqualBottomLeftEndpoint(intersectedTrapCopy)) bottomTrapezoid.setLowerLeftNeighbor(intersectedTrapCopy.getLowerLeftNeighbor());
    else bottomTrapezoid.setLowerLeftNeighbor(nullIdx);

    bottomTrapezoid.setNodeIdx(bottomTrapLeaf);    // Dag leaf idx

    // -------------------- UPDATE THE DAG ---------------------------

    // Create the substree of the dag
    if(leftTrapezoidExists){
        bool found = false;
        size_t pointIdx = trapezoidalMapData.findPoint(segment.p1(), found);
        assert(found == true);
        // x-node
        Node newNode = Node(Node::NodeType::X, pointIdx, leafTrapLeft, yNode);
        dag.replaceNode(newNode, xNodeLeft);
        // trap leaf
        newNode = Node(Node::NodeType::LEAF, leftTrapezoidIdx, nullIdx, nullIdx);
        dag.addNode(newNode);
    }
    bool found = false;
    size_t segmentIdx = trapezoidalMapData.findSegment(segment, found);
    assert(found == true); // Se non lo trova esce
    // y-node
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

    /* Need to store a copy of the previous intersected trap and information about previous bottom and top trapezoid (since they have not been added)
     * If in the below section new top or bottom trapezoids are created, these variables will be updated so that the top and bottom traps can be added.
    */
    Trapezoid prevIntersectedTrapezoid = intersectedTrapCopy;   // Copy of the previos intersected trap
    size_t previousTopTrapLeaf = topTrapLeaf;                   // Leaf index of the dag for the previous top trap
    size_t previousBottomTrapLeaf = bottomTrapLeaf;             // Leaf index of the dag for the previous bottom trapezoid
    size_t previousTopTrapIdx = topTrapezoidIdx;                // Trapezoid index of the trapmap for the previous top trapezoid
    size_t previousBottomTrapIdx = bottomTrapezoidIdx;          // Trapezoid index of the trapmap for the previous bottom trapezoid

    /*
     * For each internal trapezoid we check if the current trapezoid is a lower or upper neighbor of the previous trapezoid
     * "If the right point of Ti is over the segment then Ti+1 is the lower right neighbor, otherwise it is the upper right neighbor"
     * - If the segment is above the right point of the intersected trapezoid, the top trapezoid will be the extension of the previously created.
     * - It will be added to the trapezoidal map only when its end is found, that is when the right point of the intersected trapezoid will be above of the segment. (Or when it intersect the last segment)
     * - While the previously created bottom trapezoid found its end, so it is added to the trapezoidal map and one new is created.
     *
     * - If the segmnet is below the right point of the intersected trapezoid it will be the same as above (changing top with bottom trapezoid).
    */

    for(size_t i = 1; i < intersectedTraps.size() - 1; i++){
        // Index and copy of the current intersected trapezoid
        intersectedTrapIdx = intersectedTraps[i];
        intersectedTrapCopy = trapezoidalMap.getTrapezoid(intersectedTrapIdx);

        // Right point above segment (if true the top trapezoid found its end, if false the bottom trapezoid found its end)
        bool topTrapEnds = cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint());

        // Index for the DAG nodes (one y-node and top and bottom trapezoid leaves)
        yNode = intersectedTrapCopy.getNodeIdx();
        // Trap Leaf idx change only when the a new trap is created
        topTrapLeaf = previousTopTrapLeaf;
        bottomTrapLeaf = previousBottomTrapLeaf;

        // If the top trapezoid ends need to update the upper right neighbor adjacency of the trapezoid previosly intersected
        if(topTrapEnds && prevIntersectedTrapezoid.getUpperRightNeighbor() != nullIdx){
            trapezoidalMap.getTrapezoid(prevIntersectedTrapezoid.getUpperRightNeighbor()).setUpperLeftNeighbor(previousTopTrapIdx);
        }
        // if the bottom trapezoid ends need to update the lower right neighbor adjacency of the trapezoid previosly intersected
        if(!topTrapEnds && prevIntersectedTrapezoid.getLowerRightNeighbor() != nullIdx){
            trapezoidalMap.getTrapezoid(prevIntersectedTrapezoid.getLowerRightNeighbor()).setLowerLeftNeighbor(previousBottomTrapIdx);
        }

        if(topTrapEnds){ // Previous top trapezoid will be updated and added to the trapezoidal map, a new top trapezoid will be created.
            topTrapezoidIdx = intersectedTrapIdx;
            topTrapLeaf = dag.numNodes(); // Idx of the new top trapezoid

            // Previously created top trapezoid update and added to the trapezoidal map
            topTrapezoid.setLowerRightNeighbor(intersectedTrapIdx);    // The new top trap became the lower right neighbor of the previous
            topTrapezoid.setUpperRightNeigbor(prevIntersectedTrapezoid.getUpperRightNeighbor());
            topTrapezoid.setRightPoint(intersectedTrapCopy.getLeftPoint());
            // If is not possible to replace (its index is greater than the dimension of the trapezoidalmap)
            if(!trapezoidalMap.replaceTrapezoid(topTrapezoid, previousTopTrapIdx)) trapezoidalMap.addTrapezoid(topTrapezoid);

            // New top trapezoid initialize
            topTrapezoid = Trapezoid(intersectedTrapCopy.getTopSegment(), segment, intersectedTrapCopy.getLeftPoint(), intersectedTrapCopy.getRightPoint());
            topTrapezoid.setUpperLeftNeighbor(intersectedTrapCopy.getUpperLeftNeighbor());
            topTrapezoid.setLowerLeftNeighbor(previousTopTrapIdx);
            // Right neighbors will be computed in the future iterations
            topTrapezoid.setUpperRightNeigbor(nullIdx);
            topTrapezoid.setLowerRightNeighbor(nullIdx);

            topTrapezoid.setNodeIdx(topTrapLeaf);

            previousTopTrapIdx = intersectedTrapIdx; // The new top trapezoid take the index of the intersected trap
            previousTopTrapLeaf = topTrapLeaf;    // and its dag leaf idx

        }else{  // Previous bottom trapezoid will be updated and added to the trapezidal map, a new botom trapezoid will be created.
            bottomTrapezoidIdx = intersectedTrapIdx;
            bottomTrapLeaf = dag.numNodes();// Idx of the new bottom trapezoid

            // Previously created bottom trapezoid update and added to the trapezoidal map
            bottomTrapezoid.setLowerRightNeighbor(prevIntersectedTrapezoid.getLowerRightNeighbor());
            bottomTrapezoid.setUpperRightNeigbor(intersectedTrapIdx);
            bottomTrapezoid.setRightPoint(intersectedTrapCopy.getLeftPoint());
            if(!trapezoidalMap.replaceTrapezoid(bottomTrapezoid, previousBottomTrapIdx)) trapezoidalMap.addTrapezoid(bottomTrapezoid);

            // New bottom trapezoid initialize
            bottomTrapezoid = Trapezoid(segment, intersectedTrapCopy.getBottomSegment(), intersectedTrapCopy.getLeftPoint(), intersectedTrapCopy.getRightPoint());
            bottomTrapezoid.setUpperLeftNeighbor(previousBottomTrapIdx);
            bottomTrapezoid.setLowerLeftNeighbor(intersectedTrapCopy.getLowerLeftNeighbor());
            // Right neighbors will be computed in the future iterations
            bottomTrapezoid.setUpperRightNeigbor(nullIdx);
            bottomTrapezoid.setLowerRightNeighbor(nullIdx);

            bottomTrapezoid.setNodeIdx(bottomTrapLeaf);

            previousBottomTrapIdx = intersectedTrapIdx; // Updating the previous bottom trapezoid index
            previousBottomTrapLeaf = bottomTrapLeaf;    // and its dag leaf idx
        }
            // DAG UPDATE
            // Y-node
            newNode = Node(Node::NodeType::Y, segmentIdx, topTrapLeaf, bottomTrapLeaf);
            dag.replaceNode(newNode, yNode);
            // Added Trap leaf
            newNode = Node(Node::NodeType::LEAF, intersectedTrapIdx, nullIdx, nullIdx);
            dag.addNode(newNode);
            // Updating the previous intersected trapezoid with the one just analyzed
            prevIntersectedTrapezoid = intersectedTrapCopy;
    }


    // ------------------------------- Rightmost Trapezoid intersected ---------------------------------------------

    intersectedTrapIdx = intersectedTraps.back(); // Taking the last trapezoid in the vector
    intersectedTrapCopy = trapezoidalMap.getTrapezoid(intersectedTrapIdx);

    if(cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint())){ // The right point is above the segment
        topTrapezoidIdx = intersectedTrapIdx;
    }else{
        bottomTrapezoidIdx = intersectedTrapIdx;
    }

    size_t rightTrapezoidIdx = rightTrapezoidExists ? trapezoidalMap.numTrapezoids() : nullIdx; // Aggiustare l'idx se esiste

    // Setting indexes of the dag nodes
    newIdx = intersectedTrapCopy.getNodeIdx();     // Since we need to replace node that represent the intersected trapezoid we get its index
    size_t xNodeRight = nullIdx;
    size_t leafTrapRight = nullIdx;

    if(rightTrapezoidExists){// the first x-node only if left trapezoid exist
        xNodeRight = newIdx;
        newIdx = dag.numNodes(); // Taking the index next to the last node in the vector
        leafTrapRight = newIdx++;
    }
    // Y-node
    yNode = rightTrapezoidExists ? newIdx++ : newIdx;
    // If left trap not exist need to take the index next to the last node in the vector
    if( !(rightTrapezoidExists ) ) newIdx = dag.numNodes();
    // Top and Bottom Trap
    topTrapLeaf = cg3::isPointAtLeft(segment, prevIntersectedTrapezoid.getRightPoint()) ?  newIdx : previousTopTrapLeaf;
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

        if(!trapezoidalMap.replaceTrapezoid(topTrapezoid, previousTopTrapIdx)) trapezoidalMap.addTrapezoid(topTrapezoid);

        // Also for the bottom trapezoid
        bottomTrapezoid.setRightPoint(segment.p2());
        bottomTrapezoid.setUpperRightNeigbor(nullIdx);

        if(rightTrapezoidExists) bottomTrapezoid.setLowerRightNeighbor(rightTrapezoidIdx);
        else if(!rightPointEqualBottomRightEndpoint(intersectedTrapCopy)) bottomTrapezoid.setLowerRightNeighbor(intersectedTrapCopy.getLowerRightNeighbor());
        else bottomTrapezoid.setLowerRightNeighbor(nullIdx);

        if(!trapezoidalMap.replaceTrapezoid(bottomTrapezoid, previousBottomTrapIdx)) trapezoidalMap.addTrapezoid(bottomTrapezoid);

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
        if(!trapezoidalMap.replaceTrapezoid(bottomTrapezoid, previousBottomTrapIdx))trapezoidalMap.addTrapezoid(bottomTrapezoid);

        topTrapezoid.setRightPoint(segment.p2());
        topTrapezoid.setLowerRightNeighbor(nullIdx);
        if(rightTrapezoidExists) topTrapezoid.setUpperRightNeigbor(rightTrapezoidIdx);
        else if (!rightPointEqualTopRightEndpoint(intersectedTrapCopy)) topTrapezoid.setUpperRightNeigbor(intersectedTrapCopy.getUpperRightNeighbor());
        else topTrapezoid.setUpperRightNeigbor(nullIdx);

        if(!trapezoidalMap.replaceTrapezoid(topTrapezoid, previousTopTrapIdx)) trapezoidalMap.addTrapezoid(topTrapezoid);

        bottomTrapezoid = Trapezoid(segment, intersectedTrapCopy.getBottomSegment(), intersectedTrapCopy.getLeftPoint(), segment.p2());
        bottomTrapezoid.setUpperLeftNeighbor(previousBottomTrapIdx);
        bottomTrapezoid.setUpperRightNeigbor(nullIdx);

        bottomTrapezoid.setLowerLeftNeighbor(intersectedTrapCopy.getLowerLeftNeighbor());

        if(rightTrapezoidExists) bottomTrapezoid.setLowerRightNeighbor(rightTrapezoidIdx);
        else if(!rightPointEqualBottomRightEndpoint(intersectedTrapCopy)) bottomTrapezoid.setLowerRightNeighbor(intersectedTrapCopy.getLowerRightNeighbor());
        else bottomTrapezoid.setLowerRightNeighbor(nullIdx);

        bottomTrapezoid.setNodeIdx(bottomTrapLeaf);

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

        dag.addNode(newNode);
    }else{
        newNode = Node(Node::NodeType::LEAF, previousBottomTrapIdx, nullIdx, nullIdx);

        dag.addNode(newNode);
    }

}
}
