#ifndef VECTORBLOCK_H
#define VECTORBLOCK_H

#include <vector>
#include <algorithm>
#include "OutputNode.h"

using namespace std;

//PageRankœÚ¡ø∑÷øÈ
class VectorBlock {
private:
    int begin;
    int end;
    double * data;
    vector<OutputNode>* vtO;

public:
    static int length;
    VectorBlock(int begin, int end);
    void memset(double initialValue);
    vector<OutputNode>* getVector();
    double getScoreByID(int index);
    void set(int index, double value);
    vector<OutputNode>* sort();
    vector<OutputNode>* subList(int fromIndex, int toIndex);
    static int getLength();
    vector<OutputNode>* createVON();

};

#endif // VECTORBLOCK_H
