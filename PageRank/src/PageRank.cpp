#include "PageRank.h"

//����PageRank����
vector<OutputNode>* PageRank::calculate(SparseMatrix* matrix, PageRankVector* pageRankVector, PageRankVector* pageRankVector1, int numIterations, double convergenceThreshold, double teleportParameter) {
    PageRankVector* vOld = pageRankVector;
    PageRankVector* vNew = pageRankVector1;
    //cout << "��ʼvOld����֮��Ϊ: " << vOld->num() << endl;
    //cout << "��ʼvNew����֮��Ϊ: " << vNew->num() << endl;
    int numIter = 0;//��������
    double diff = 1;//��������ǰ�����
    double constant = (1 - teleportParameter) / vOld->getNodeNumber();

    //cout << "��ʼ����:" << endl;
    while (diff > convergenceThreshold) {//����ǰ������Ƿ�С��������ֵ

        //������ʽ
        PageRankVector* v = matrix->multiply(vOld, vNew);//cout << "v->num(): " << v->num() << endl;
        vNew = v->multiply(teleportParameter)->add(constant);

        numIter++;
        //cout << "vOld����֮��Ϊ: " << setprecision(15) << vOld->num() << endl;
        //cout << "vNew����֮��Ϊ: " << setprecision(15) << vNew->num() << endl;
        diff = vNew->calculateDifference(vOld);
        //cout << "��" << numIter << "�ε���: ����ǰ�������Ĳ���Ϊ: " << diff << endl;
        PageRankVector* temp = vOld;
        vOld = vNew;
        vNew = temp;
        vNew->memset();
    }
    //cout << endl;
    //cout << "����PageRank���������������ز������£�" << endl;
    //cout << "�����ת����teleportParameterΪ��" << teleportParameter << endl;
    //cout << "������ֵconvergenceThresholdΪ��" << convergenceThreshold << endl;
    cout << "�ܹ������� " << numIter << " ��" << endl;
    return vOld->sort();
}

 //��̬�̰߳汾�߳����ݽṹ����
typedef struct {
    int t_id; //�߳� id
    int num_threads; //�߳���
    PageRankVector* vOld;//������
    PageRankVector* vNew;//������
    SparseMatrix* matrix;//ϡ��ֿ����
}threadParam_t;

pthread_mutex_t amutex;
pthread_barrier_t barrier_Iteration;
double diff = 1;//��������ǰ�����
int numIter = 0;//ʵ�ʵ�������
double constant;
double converThre;// ������ֵ
double teleParam;// �����ת����
PageRankVector* pRVector;//�������ս������

//�̺߳�������
void *threadFunc_v(void *param) {

    threadParam_t *p = (threadParam_t*)param;
    int t_id = p -> t_id;
    int num_threads = p -> num_threads;
    PageRankVector* vOld = p->vOld;
    PageRankVector* vNew = p->vNew;
    SparseMatrix* matrix = p->matrix;

    while (diff > converThre) {

        pthread_barrier_wait(&barrier_Iteration);
        if(t_id == 0){
            diff = 0;
        }

        pthread_barrier_wait(&barrier_Iteration);
        matrix->multiply(vOld, vNew, t_id);
        vNew->multiply(teleParam, t_id)->add(constant, t_id);

        double myDiff = vNew->calculateDifference(vOld, t_id);

        pthread_mutex_lock(&amutex);
        diff += myDiff;
        pthread_mutex_unlock(&amutex);
        //cout << "��" << numIter << "�ε���: ����ǰ�������Ĳ���Ϊ: " << diff << endl;

        PageRankVector* temp = vOld;
        vOld = vNew;
        vNew = temp;

        pthread_barrier_wait(&barrier_Iteration);
        if(t_id == 0){
            diff = sqrt(diff);
            pRVector = vOld;
            numIter++;
        }

        vNew->memset(t_id);

        //�����߳�һ�������һ��
        pthread_barrier_wait(&barrier_Iteration);

        //if(t_id == 0){
            //cout << "��" << numIter << "�ε���: ����ǰ�������Ĳ���Ϊ: " << diff << endl;
        //}
        //std::this_thread::sleep_for(std::chrono::seconds(10));

    }

    pthread_exit(NULL);
}

//����PageRank����
vector<OutputNode>* PageRank::calculate_parallel(SparseMatrix* matrix, PageRankVector* pageRankVector, PageRankVector* pageRankVector1, int numIterations, double converThreshold, double tParameter) {

    int num_threads = matrix->getNumBlocks();//�߳���

     //��ʼ��barrier��amutex
    pthread_barrier_init(&barrier_Iteration, NULL, num_threads);
    pthread_mutex_init(&amutex, NULL);

    diff = 1;
    numIter = 0;
    constant = (1 - tParameter) / pageRankVector->getNodeNumber();
    converThre = converThreshold;
    teleParam = tParameter;

    //�����߳�
    pthread_t handles[num_threads];// ������Ӧ�� Handle
    threadParam_t param[num_threads];// ������Ӧ���߳����ݽṹ
    for(int t_id = 0; t_id < num_threads; t_id++){
        param[t_id].t_id = t_id;
        param[t_id].num_threads = num_threads;
        param[t_id].vOld = pageRankVector;
        param[t_id].vNew = pageRankVector1;
        param[t_id].matrix = matrix;
        pthread_create(&handles[t_id], NULL, threadFunc_v, (void*)&param[t_id]);
    }

    for(int t_id = 0; t_id < num_threads; t_id++){
        pthread_join(handles[t_id], NULL);
    }

    cout << "�ܹ������� " << numIter << " ��" << endl;

    pthread_barrier_destroy(&barrier_Iteration);
    pthread_mutex_destroy(&amutex);

    return pRVector->sort();
}
