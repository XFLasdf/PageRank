#ifndef OUTPUTNODE_H
#define OUTPUTNODE_H


#include <iostream>
#include <cmath>

/**
*VectorBlock单个节点
*/
class OutputNode {
private:
    int nodeID;
    double score;

public:
    OutputNode(int nodeID, double score);
    OutputNode();
    double getScore() const ;
    int getNodeID();
    void setScore(double score);
    void setNodeID(int nodeID);
    bool operator==(const OutputNode& o) const;
    bool operator<(const OutputNode& other) const;
};
#endif // OUTPUTNODE_H
