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
void pageRank(vector<string>*, int, int);//�����㷨
void pageRank_parallel(vector<string>*, int, int);//���߳�+SIMD�㷨

int colNumber = 9000; // ����������/����
int nodeNumber = 6263;//��վID����
double convergenceThreshold = 0.00000000000001; // ������ֵ
double teleportParameter = 0.85; // �����ת����
int numIterations = 200; // ����������
string inputFilePath = "Data.txt"; // �����ļ�·��
string outputFilePath = "result"; // ����ļ�

long long head, tail , freq ; // timers

int main()
{

    QueryPerformanceFrequency((LARGE_INTEGER *)&freq );

    // ��ȡ�����ļ�
    vector<string>* lines = readLines(inputFilePath);

    for(int links = 10000; links <= 90000; links+=10000){

        int length = links > lines->size() ? lines->size() : links;
        cout << "������links: " << length << endl;cout << endl;

        vector<string>* sub_lines = new vector<string>(lines->begin(), lines->begin()+length);

        for(int numBlocks = 2; numBlocks <= 32; numBlocks += 2){

            cout << "�ֿ���/���̵߳��߳���: " << numBlocks << endl;cout << endl;

            pageRank(sub_lines, numBlocks, length);
            pageRank_parallel(sub_lines, numBlocks, length);

        }

        //std::this_thread::sleep_for(std::chrono::seconds(100));
    }

    return 0;
}

//�����㷨
void pageRank(vector<string>* lines, int numBlocks, int links){

    // �����ֿ�ϡ��ת�ƾ���
    SparseMatrix* matrix = new SparseMatrix(numBlocks, colNumber, lines, &nodeNumber);

    // matrix.print();

    // ��ʼ��PageRank����
    PageRankVector* pageRankVector = new PageRankVector(numBlocks, colNumber, nodeNumber, lines);

    // ��ʼ��PageRank����������ÿ������Ϊ0
    PageRankVector* pageRankVector1 = new PageRankVector(pageRankVector);

    QueryPerformanceCounter((LARGE_INTEGER *)&head);
    // ���PageRank�㷨
    vector<OutputNode>* ranks = PageRank::calculate(matrix, pageRankVector, pageRankVector1, numIterations, convergenceThreshold, teleportParameter);
    QueryPerformanceCounter((LARGE_INTEGER *)&tail );
    cout << "�����㷨ʱ�䣺" << (tail - head) * 1000.0 / freq << "ms" << endl;
    cout << endl;

    // ���ǰ100��PageRank score���Ľڵ㵽�ļ�
    writeTop100RanksToFile(ranks, outputFilePath+"_"+to_string(numBlocks)+"_"+to_string(links)+".txt", matrix);

}

//���̲߳����㷨
void pageRank_parallel(vector<string>* lines, int numBlocks, int links){

    // �����ֿ�ϡ��ת�ƾ���
    SparseMatrix* matrix = new SparseMatrix(numBlocks, colNumber, lines, &nodeNumber);
    // matrix.print();

    // ��ʼ��PageRank����
    PageRankVector* pageRankVector = new PageRankVector(numBlocks, colNumber, nodeNumber, lines);

    // ��ʼ��PageRank����������ÿ������Ϊ0
    PageRankVector* pageRankVector1 = new PageRankVector(pageRankVector);

    QueryPerformanceCounter((LARGE_INTEGER *)&head);
    // ���PageRank�㷨
    vector<OutputNode>* ranks = PageRank::calculate_parallel(matrix, pageRankVector, pageRankVector1, numIterations, convergenceThreshold, teleportParameter);
    QueryPerformanceCounter((LARGE_INTEGER *)&tail );
    cout << "���̲߳��л��㷨ʱ�䣺" << (tail - head) * 1000.0 / freq << "ms" << endl;
    cout << endl;

    // ���ǰ100��PageRank score���Ľڵ㵽�ļ�
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
        cerr << "�ļ���ʧ�ܣ�" << endl;
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
    //cout << "��PageRank score����ǰ100��NodeID�У���ֹ�㹲�� " << deadEndsIn100Top.size() << " ����NodeID�ֱ�Ϊ��" << endl;
    //for (int nodeID : deadEndsIn100Top) {
        //cout << " " << nodeID;
    //}
    //cout << endl;
    //cout << "����ֹ�㹲�� " << noDeadEndsIn100Top.size() << " ����NodeID�ֱ�Ϊ��" << endl;
    //for (int nodeID : noDeadEndsIn100Top) {
        //cout << " " << nodeID;
    //}
    //cout << endl;
}
