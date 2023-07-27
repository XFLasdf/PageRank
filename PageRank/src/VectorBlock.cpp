#include "VectorBlock.h"

VectorBlock::VectorBlock(int begin, int end) {
    this->begin = begin;
    this->end = end;
    data = new double[length];
    vtO = new vector<OutputNode>;
    memset(0.0);
}

void VectorBlock::memset(double initialValue) {
    for (int i = 0; i < length; i++) {
        data[i] = initialValue;
    }
}

vector<OutputNode>* VectorBlock::getVector() {
    return vtO;
}

double VectorBlock::getScoreByID(int index) {
    return data[index];
    //return vector[index].getScore();
}

void VectorBlock::set(int index, double value) {
    data[index] = value;
    //vector[index].setScore(value);
}

vector<OutputNode>* VectorBlock::sort() {
    if(vtO->size() == 0){
        createVON();
    }
    std::sort(vtO->begin(), vtO->end());
    return vtO;
}

vector<OutputNode>* VectorBlock::subList(int fromIndex, int toIndex) {
    return new vector<OutputNode>(vtO->begin() + fromIndex, vtO->begin() + toIndex);
}

int VectorBlock::getLength() {
    return length;
}

int VectorBlock::length = 0;

vector<OutputNode>* VectorBlock::createVON(){
    //vt.resize(length);
    for (int i = 0; i < length; i++) {
        OutputNode node(begin + i, data[i]);
        vtO->push_back(node);
    }
    return vtO;
}
