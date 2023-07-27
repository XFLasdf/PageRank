#ifndef PAGERANKVECTOR_H
#define PAGERANKVECTOR_H

#include <string>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <iomanip>
#include "OutputNode.h"
#include "VectorBlock.h"

using namespace std;

//PageRank����
class PageRankVector {
private:
    int numBlocks;//�ֿ���
    int colNumber;//����������/����
    int blockSize;//�ֿ����������վID����
    int nodeNumber;//��վID����
    vector<VectorBlock*> pageRankVector;//�洢����PageRank�����ֿ�
    set<int>* nodeIDList;//������վID
    vector<vector<int>>* nodeIDBlocks;//������վID�ֿ�

public:
    PageRankVector(int numBlocks, int colNumber, int nodeNumber);
    PageRankVector(PageRankVector* vt);
    PageRankVector(int numBlocks, int colNumber, int nodeNumber, vector<string>* lines);

    void memset(vector<string>* lines);
    void memset();
    void memset(int block_index);
    void setScore(int index, double value);
    void addScore(int index, double value);
    double getScore(int index);
    int getBlockSize();
    int getNumBlocks();
    int getColNumber();
    int getNodeNumber();
    set<int>* getNodeIDList();
    vector<vector<int>>* getNodeIDBlocks();
    vector<OutputNode>* sort();
    PageRankVector* multiply(double number);
    PageRankVector* multiply(double number, int block_index);
    PageRankVector* add(double number);
    PageRankVector* add(double number, int block_index);
    double calculateDifference(PageRankVector* v);
    double calculateDifference(PageRankVector* v, int block_index);
    double num();
};

#endif // PAGERANKVECTOR_H
