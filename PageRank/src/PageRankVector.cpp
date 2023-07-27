#include "PageRankVector.h"

PageRankVector::PageRankVector(int numBlocks, int colNumber, int nodeNumber) {
    this->numBlocks = numBlocks;
    this->colNumber = colNumber;
    this->nodeNumber = nodeNumber;
    blockSize = colNumber / numBlocks;
    VectorBlock::length = blockSize;
    this->nodeIDList = new set<int>;
    this->nodeIDBlocks = new vector<vector<int>>;

    for (int i = 0; i < numBlocks; i++) {
        this->pageRankVector.push_back(new VectorBlock(i * VectorBlock::length, (i + 1) * VectorBlock::length - 1));
        vector<int>* q = new vector<int>;
        this->nodeIDBlocks->push_back(*q);
    }
}

PageRankVector::PageRankVector( PageRankVector* vt) {
    this->numBlocks = vt->getNumBlocks();
    this->colNumber = vt->getColNumber();
    this->nodeNumber = vt->getNodeNumber();
    this->blockSize = vt->getBlockSize();
    for (int i = 0; i < numBlocks; i++) {
        this->pageRankVector.push_back(new VectorBlock(i * VectorBlock::length, (i + 1) * VectorBlock::length - 1));
    }
    this->nodeIDList = vt->getNodeIDList();
    this->nodeIDBlocks = vt->getNodeIDBlocks();
    memset();
}

PageRankVector::PageRankVector(int numBlocks, int colNumber, int nodeNumber, vector<string>* lines) {
    this->numBlocks = numBlocks;
    this->colNumber = colNumber;
    this->nodeNumber = nodeNumber;
    blockSize = colNumber / numBlocks;
    VectorBlock::length = blockSize;
    this->nodeIDList = new set<int>;
    this->nodeIDBlocks = new vector<vector<int>>;
    for (int i = 0; i < numBlocks; i++) {
        this->pageRankVector.push_back(new VectorBlock(i * VectorBlock::length, (i + 1) * VectorBlock::length - 1));
        vector<int> q;
        this->nodeIDBlocks->push_back(q);
    }
    memset(lines);
}

void PageRankVector::memset(vector<string>* lines) {
    double initialValue = static_cast<double>(1) / nodeNumber;
    for (string line : (*lines)) {
        int start = 0, last = 0;
        last = line.find(" ", start);
        int fromNodeID = stoi(line.substr(start, last - start));
        start = last + 1;
        int toNodeID = stoi(line.substr(start));
        nodeIDList->insert(fromNodeID);
        nodeIDList->insert(toNodeID);

        //cout << "getScore(fromNodeID)before: " << getScore(fromNodeID) << endl;
        setScore(fromNodeID, initialValue);//cout << "getScore(fromNodeID)after: " << getScore(fromNodeID) << endl;
        setScore(toNodeID, initialValue);
    }

    for(set<int>::iterator it = nodeIDList->begin(); it != nodeIDList->end(); ++it){
        (*nodeIDBlocks)[(*it)/VectorBlock::length].push_back(*it);
    }
    //cout << "nodeIDList.size(): " << nodeIDList.size() << endl;
}

void PageRankVector::memset() {
    multiply(0);
}

void PageRankVector::memset(int block_index) {
    multiply(0, block_index);
}

void PageRankVector::setScore(int index, double value) {
    pageRankVector[index / VectorBlock::length]->set(index % VectorBlock::length, value);
}

void PageRankVector::addScore(int index, double value) {
    double b = getScore(index);
    b += value;
    pageRankVector[index / VectorBlock::length]->set(index % VectorBlock::length, b);
}

double PageRankVector::getScore(int index) {
    return pageRankVector[index / VectorBlock::length]->getScoreByID(index % VectorBlock::length);
}

int PageRankVector::getBlockSize() {
    return pageRankVector.size();
}

int PageRankVector::getNumBlocks() {
    return numBlocks;
}

int PageRankVector::getColNumber() {
    return colNumber;
}

int PageRankVector::getNodeNumber() {
    return nodeNumber;
}

set<int>* PageRankVector::getNodeIDList() {
    return nodeIDList;
}

vector<vector<int>>* PageRankVector::getNodeIDBlocks() {
    return nodeIDBlocks;
}

vector<OutputNode>* PageRankVector::sort() {
    vector<OutputNode>* outputNodeList = new vector<OutputNode>;
    vector<OutputNode>* vtt = pageRankVector[0]->sort();
    outputNodeList->insert(outputNodeList->end(), vtt->begin(), vtt->end());
    //cout << "outputNodeList->size(): " << outputNodeList->size() << endl;
    for (auto it = next(pageRankVector.begin()); it != pageRankVector.end(); ++it) {
        vtt = (*it)->sort();
        outputNodeList->insert(outputNodeList->end(), vtt->begin(), vtt->end());
        std::sort(outputNodeList->begin(), outputNodeList->end());
        //cout << "outputNodeList->size(): " << outputNodeList->size() << endl;
        if (outputNodeList->size() > 100) {
            outputNodeList->resize(100);
        }
    }
    //for(int i = 0; i < outputNodeList->size(); i++){
     //   cout << (*outputNodeList)[i].getNodeID() << " " << (*outputNodeList)[i].getScore() << endl;
    //}
    return outputNodeList;
}

PageRankVector* PageRankVector::multiply(double number) {
    //for(int i = 0; i < nodeIDBlocks->size(); i++){
        //multiply(number, i);
    //}
    for (int nodeID : (*nodeIDList)) {
        setScore(nodeID, getScore(nodeID) * number);
    }
    return this;
}

PageRankVector* PageRankVector::multiply(double number, int block_index) {
    for (int nodeID : (*nodeIDBlocks)[block_index]) {
        setScore(nodeID, getScore(nodeID) * number);
    }
    return this;
}

PageRankVector* PageRankVector::add(double number) {
    //for(int i = 0; i < nodeIDBlocks->size(); i++){
        //add(number, i);
    //}
    for (int nodeID : (*nodeIDList)) {
        setScore(nodeID, getScore(nodeID) + number);
    }
    return this;
}

PageRankVector* PageRankVector::add(double number, int block_index) {
    for (int nodeID : (*nodeIDBlocks)[block_index]) {
        setScore(nodeID, getScore(nodeID) + number);
    }
    return this;
}

double PageRankVector::calculateDifference(PageRankVector* v) {
    double sum = 0;
    for (int nodeID : (*nodeIDList)) {
        double diff = getScore(nodeID) - v->getScore(nodeID);
        sum += diff * diff;
    }
    //double sum = 0;
    //for(int i = 0; i < nodeIDBlocks->size(); i++){
        //sum += calculateDifference(v, i);
    //}
    return sqrt(sum);
}

double PageRankVector::calculateDifference(PageRankVector* v, int block_index) {
    double sum = 0;
    for (int nodeID : (*nodeIDBlocks)[block_index]) {
        double diff = getScore(nodeID) - v->getScore(nodeID);
        sum += diff * diff;
    }
    return sum;
}

double PageRankVector::num() {
    double sum = 0;
    for (int nodeID : (*nodeIDList)) {
        sum += getScore(nodeID);
    }
    return sum;
}
