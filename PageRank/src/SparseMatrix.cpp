#include "SparseMatrix.h"

SparseMatrix::SparseMatrix(int numBlocks, int colNumber, const vector<string>* lines, int* nodeNumber) {
    this->numBlocks = numBlocks;
    this->colNumber = colNumber;
    blockSize = colNumber / numBlocks;
    degree = new map<int, int>;
    deadEnds = new vector<int>;
    createBlockMatrix(lines, nodeNumber);
}

void SparseMatrix::createBlockMatrix(const vector<string>* lines, int* nodeNumber) {
    initBlockMatrix();
    int linkNum = 0;
    for (string line : (*lines)) {
        linkNum++;
        int start = 0, last = 0;
        last = line.find(" ", start);
        int fromNodeID = stoi(line.substr(start, last - start));
        start = last + 1;
        int toNodeID = stoi(line.substr(start));
        addLink(fromNodeID, toNodeID);
    }
    //cout << "�ܹ���" << linkNum << "����վ����" << endl;

    int deadEndsNum = 0;
    int num = 0;
    //cout << "����NodeIDΪ��ǰ����dead end��ֹ��NodeID: " << endl;
    for (const auto& node : (*degree)) {
        if (node.second == 0) {
            deadEnds->push_back(node.first);
            //cout << " " << node.first;
            deadEndsNum++;
        }
        num++;
    }

    *nodeNumber = num;

    cout << "�ܹ���" << num << "����վID�����й���" << deadEndsNum << "��dead ends��ֹ��" << endl;

    for (int i = 0; i < blockMatrix.size(); i++) {
        Block_Stripe* b = getBlock(i);
        int blockSize = b->size();
        for (int j = 0; j < blockSize; j++) {
            Stripe* s = b->getStripe(j);
            s->setDegree((*degree)[s->getSrc()]);
        }
    }
}

void SparseMatrix::initBlockMatrix() {
    for (int i = 0; i < numBlocks; i++) {
        blockMatrix.push_back(new Block_Stripe());
    }
}

void SparseMatrix::addLink(int fromNodeID, int toNodeID) {
    int index = toNodeID / blockSize;
    blockMatrix[index]->addStripe(fromNodeID, toNodeID);
    if (degree->find(fromNodeID) == degree->end()) {
        (*degree)[fromNodeID] = 1;
    }else{
        (*degree)[fromNodeID]++;
    }
    if (degree->find(toNodeID) == degree->end()) {
        (*degree)[toNodeID] = 0;
    }
}

PageRankVector* SparseMatrix::multiply(PageRankVector* vOld, PageRankVector* vNew) {
    int vectorOldSize = vOld->getBlockSize();
    int vectorNewSize = vNew->getBlockSize();
    if (vectorOldSize != numBlocks || vectorNewSize != numBlocks) {
        cout << "����ֿ��������ֿ�������һ�£�" << endl;
        return nullptr;
    }
    for (int i = 0; i < blockMatrix.size(); i++) {
        multiply(vOld, vNew, i);
    }
    return vNew;
}

//�ֿ����ϡ����������˷�
void SparseMatrix::multiply(PageRankVector* vOld, PageRankVector* vNew, int block_index) {

    Block_Stripe* b = getBlock(block_index);
    int blockSize = b->size();
    for (int j = 0; j < blockSize; j++) {
        Stripe* s = b->getStripe(j);
        double product = vOld->getScore(s->getSrc()) / s->getDegree();
        for (int k = 0; k < s->size(); k++) {
            int des = s->getDes(k);
            vNew->addScore(des, product);
        }
    }
}

int SparseMatrix::getNumBlocks() {
    return numBlocks;
}

int SparseMatrix::getColNumber() {
    return colNumber;
}

Block_Stripe* SparseMatrix::getBlock(int index) {
    return blockMatrix[index];
}

vector<int>* SparseMatrix::getDeadEnds() {
    return deadEnds;
}

void SparseMatrix::print() {
    for (int i = 0; i < blockMatrix.size(); i++) {
        Block_Stripe* b = getBlock(i);
        int blockSize = b->size();
        for (int j = 0; j < blockSize; j++) {
            Stripe* s = b->getStripe(j);
            cout << "��" << s->getSrc() << "�г���Ϊ: " << s->getDegree() << " �ֱ���:";
            for (int k = 0; k < s->size(); k++) {
                cout << " " << s->getDes(k);
            }
            cout << endl;
        }
    }
}
