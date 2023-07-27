#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "Stripe.h"
#include "Block_Stripe.h"
#include "PageRankVector.h"

using namespace std;

//稀疏分块矩阵
class SparseMatrix {
private:
    int numBlocks;//分块数
    int colNumber;//矩阵总列数/行数
    int blockSize;//分块最大容纳网站ID数量

    vector<Block_Stripe*> blockMatrix;//稀疏矩阵所有分块
    map<int, int>* degree;//网站ID到其出度的映射
    vector<int>* deadEnds;//终止点集合

    void createBlockMatrix(const vector<string>* lines, int* nodeNumber);
    void initBlockMatrix();
    void addLink(int fromNodeID, int toNodeID);

public:
    SparseMatrix(int numBlocks, int colNumber, const vector<string>* lines, int* nodeNumber);
    PageRankVector* multiply(PageRankVector* vOld, PageRankVector* vNew);
    void multiply(PageRankVector* vOld, PageRankVector* vNew, int block_index);
    int getNumBlocks();
    int getColNumber();
    Block_Stripe* getBlock(int index);
    vector<int>* getDeadEnds();
    void print();
};

#endif // SPARSEMATRIX_H
