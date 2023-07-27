#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_set>
#include <windows.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <nmmintrin.h>
#include <string>
#include "SparseMatrix.h"
#include "Stripe.h"
#include "Block_Stripe.h"
#include "PageRankVector.h"
#include "OutputNode.h"
#include "PageRank.h"

using namespace std;

vector<string>* readLines(const string&);
void writeTop100RanksToFile(vector<OutputNode>*, string, SparseMatrix*);
void pageRank(vector<string>*, int, int);//串行算法
void pageRank_parallel(vector<string>*, int, int);//多线程+SIMD算法

int colNumber = 9000; // 矩阵总列数/行数
int nodeNumber = 6263;//网站ID总数
double convergenceThreshold = 0.00000000000001; // 收敛阈值
double teleportParameter = 0.85; // 随机跳转概率
int numIterations = 200; // 最大迭代次数
string inputFilePath = "Data.txt"; // 输入文件路径
string outputFilePath = "result"; // 输出文件

long long head, tail , freq ; // timers

int main()
{

    QueryPerformanceFrequency((LARGE_INTEGER *)&freq );

    // 读取输入文件
    vector<string>* lines = readLines(inputFilePath);

    for(int links = 10000; links <= 90000; links+=10000){

        int length = links > lines->size() ? lines->size() : links;
        cout << "链接数links: " << length << endl;cout << endl;

        vector<string>* sub_lines = new vector<string>(lines->begin(), lines->begin()+length);

        for(int numBlocks = 2; numBlocks <= 32; numBlocks += 2){

            cout << "分块数/多线程的线程数: " << numBlocks << endl;cout << endl;

            pageRank(sub_lines, numBlocks, length);
            pageRank_parallel(sub_lines, numBlocks, length);

        }

        //std::this_thread::sleep_for(std::chrono::seconds(100));
    }

    return 0;
}

//串行算法
void pageRank(vector<string>* lines, int numBlocks, int links){

    // 构建分块稀疏转移矩阵
    SparseMatrix* matrix = new SparseMatrix(numBlocks, colNumber, lines, &nodeNumber);

    // matrix.print();

    // 初始化PageRank向量
    PageRankVector* pageRankVector = new PageRankVector(numBlocks, colNumber, nodeNumber, lines);

    // 初始化PageRank迭代向量，每个分量为0
    PageRankVector* pageRankVector1 = new PageRankVector(pageRankVector);

    QueryPerformanceCounter((LARGE_INTEGER *)&head);
    // 完成PageRank算法
    vector<OutputNode>* ranks = PageRank::calculate(matrix, pageRankVector, pageRankVector1, numIterations, convergenceThreshold, teleportParameter);
    QueryPerformanceCounter((LARGE_INTEGER *)&tail );
    cout << "串行算法时间：" << (tail - head) * 1000.0 / freq << "ms" << endl;
    cout << endl;

    // 输出前100个PageRank score最大的节点到文件
    writeTop100RanksToFile(ranks, outputFilePath+"_"+to_string(numBlocks)+"_"+to_string(links)+".txt", matrix);

}

//多线程并行算法
void pageRank_parallel(vector<string>* lines, int numBlocks, int links){

    // 构建分块稀疏转移矩阵
    SparseMatrix* matrix = new SparseMatrix(numBlocks, colNumber, lines, &nodeNumber);
    // matrix.print();

    // 初始化PageRank向量
    PageRankVector* pageRankVector = new PageRankVector(numBlocks, colNumber, nodeNumber, lines);

    // 初始化PageRank迭代向量，每个分量为0
    PageRankVector* pageRankVector1 = new PageRankVector(pageRankVector);

    QueryPerformanceCounter((LARGE_INTEGER *)&head);
    // 完成PageRank算法
    vector<OutputNode>* ranks = PageRank::calculate_parallel(matrix, pageRankVector, pageRankVector1, numIterations, convergenceThreshold, teleportParameter);
    QueryPerformanceCounter((LARGE_INTEGER *)&tail );
    cout << "多线程并行化算法时间：" << (tail - head) * 1000.0 / freq << "ms" << endl;
    cout << endl;

    // 输出前100个PageRank score最大的节点到文件
    writeTop100RanksToFile(ranks, outputFilePath+"_parallel_"+to_string(numBlocks)+"_"+to_string(links)+".txt", matrix);

}

vector<string>* readLines(const string& filePath) {
    vector<string>* lines = new vector<string>;
    ifstream reader;
    if (filePath.empty()) {
        reader.open("Data.txt");
    } else {
        reader.open(filePath);
    }

    if(!reader){
        cerr << "文件打开失败！" << endl;
    }
    string line;
    while (getline(reader, line)) {
        lines->push_back(line);
    }
    reader.close();
    return lines;
}

void writeTop100RanksToFile(vector<OutputNode>* ranks, string filePath, SparseMatrix* matrix) {
    vector<int> deadEndsIn100Top;
    vector<int> noDeadEndsIn100Top;
    ofstream writer(filePath);
    for (int i = 0; i < ranks->size(); i++) {
        int nodeId = (*ranks)[i].getNodeID();
        writer << nodeId << " " << setprecision(15) << (*ranks)[i].getScore() << endl;
        vector<int>* it = matrix->getDeadEnds();
        if (find(it->begin(), it->end(), nodeId) != it->end()) {
            deadEndsIn100Top.push_back(nodeId);
        } else {
            noDeadEndsIn100Top.push_back(nodeId);
        }
    }
    writer.close();

    //cout << endl;
    //cout << "在PageRank score最大的前100个NodeID中，终止点共有 " << deadEndsIn100Top.size() << " 个，NodeID分别为：" << endl;
    //for (int nodeID : deadEndsIn100Top) {
        //cout << " " << nodeID;
    //}
    //cout << endl;
    //cout << "非终止点共有 " << noDeadEndsIn100Top.size() << " 个，NodeID分别为：" << endl;
    //for (int nodeID : noDeadEndsIn100Top) {
        //cout << " " << nodeID;
    //}
    //cout << endl;
}
