#include "OutputNode.h"

OutputNode::OutputNode(int nodeID, double score) {
    this->nodeID = nodeID;
    this->score = score;
}

OutputNode::OutputNode() {

}

double OutputNode::getScore() const {
    return score;
}

int OutputNode::getNodeID() {
    return nodeID;
}

void OutputNode::setScore(double score) {
    this->score = score;
}

void OutputNode::setNodeID(int nodeID) {
    this->nodeID = nodeID;
}

bool OutputNode::operator==(const OutputNode& o) const {
    return nodeID == o.nodeID && std::abs(o.score - score) < 1e-9;
}

bool OutputNode::operator<(const OutputNode& other) const {
    return this->score > other.getScore();
}

