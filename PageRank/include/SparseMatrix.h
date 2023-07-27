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

//ϡ��ֿ����
class SparseMatrix {
private:
    int numBlocks;//�ֿ���
    int colNumber;//����������/����
    int blockSize;//�ֿ����������վID����

    vector<Block_Stripe*> blockMatrix;//ϡ��������зֿ�
    map<int, int>* degree;//��վID������ȵ�ӳ��
    vector<int>* deadEnds;//��ֹ�㼯��

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
