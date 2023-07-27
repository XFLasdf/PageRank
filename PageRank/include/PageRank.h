#ifndef PAGERANK_H
#define PAGERANK_H

#include <iostream>
#include <vector>
#include <pthread.h>
#include <chrono>
#include <thread>
#include "SparseMatrix.h"
#include "PageRankVector.h"
#include "OutputNode.h"

using namespace std;

//���PageRank�ĺ��ĵ�������
class PageRank {
private:

public:
    static vector<OutputNode>* calculate(SparseMatrix* matrix, PageRankVector* pageRankVector, PageRankVector* pageRankVector1, int numIterations, double convergenceThreshold, double teleportParameter);
    static vector<OutputNode>* calculate_parallel(SparseMatrix* matrix, PageRankVector* pageRankVector, PageRankVector* pageRankVector1, int numIterations, double convergenceThreshold, double teleportParameter);
};

#endif // PAGERANK_H
