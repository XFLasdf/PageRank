#include "PageRank.h"

//串行PageRank迭代
vector<OutputNode>* PageRank::calculate(SparseMatrix* matrix, PageRankVector* pageRankVector, PageRankVector* pageRankVector1, int numIterations, double convergenceThreshold, double teleportParameter) {
    PageRankVector* vOld = pageRankVector;
    PageRankVector* vNew = pageRankVector1;
    //cout << "初始vOld分量之和为: " << vOld->num() << endl;
    //cout << "初始vNew分量之和为: " << vNew->num() << endl;
    int numIter = 0;//迭代次数
    double diff = 1;//向量迭代前后差异
    double constant = (1 - teleportParameter) / vOld->getNodeNumber();

    //cout << "开始迭代:" << endl;
    while (diff > convergenceThreshold) {//迭代前后差异是否小于收敛阈值

        //迭代公式
        PageRankVector* v = matrix->multiply(vOld, vNew);//cout << "v->num(): " << v->num() << endl;
        vNew = v->multiply(teleportParameter)->add(constant);

        numIter++;
        //cout << "vOld分量之和为: " << setprecision(15) << vOld->num() << endl;
        //cout << "vNew分量之和为: " << setprecision(15) << vNew->num() << endl;
        diff = vNew->calculateDifference(vOld);
        //cout << "第" << numIter << "次迭代: 迭代前与迭代后的差异为: " << diff << endl;
        PageRankVector* temp = vOld;
        vOld = vNew;
        vNew = temp;
        vNew->memset();
    }
    //cout << endl;
    //cout << "本次PageRank迭代计算结束，相关参数如下：" << endl;
    //cout << "随机跳转概率teleportParameter为：" << teleportParameter << endl;
    //cout << "收敛阈值convergenceThreshold为：" << convergenceThreshold << endl;
    cout << "总共迭代了 " << numIter << " 次" << endl;
    return vOld->sort();
}

 //静态线程版本线程数据结构定义
typedef struct {
    int t_id; //线程 id
    int num_threads; //线程数
    PageRankVector* vOld;//旧向量
    PageRankVector* vNew;//新向量
    SparseMatrix* matrix;//稀疏分块矩阵
}threadParam_t;

pthread_mutex_t amutex;
pthread_barrier_t barrier_Iteration;
double diff = 1;//向量迭代前后差异
int numIter = 0;//实际迭代次数
double constant;
double converThre;// 收敛阈值
double teleParam;// 随机跳转概率
PageRankVector* pRVector;//保存最终结果向量

//线程函数定义
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
        //cout << "第" << numIter << "次迭代: 迭代前与迭代后的差异为: " << diff << endl;

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

        //所有线程一起进入下一轮
        pthread_barrier_wait(&barrier_Iteration);

        //if(t_id == 0){
            //cout << "第" << numIter << "次迭代: 迭代前与迭代后的差异为: " << diff << endl;
        //}
        //std::this_thread::sleep_for(std::chrono::seconds(10));

    }

    pthread_exit(NULL);
}

//并行PageRank迭代
vector<OutputNode>* PageRank::calculate_parallel(SparseMatrix* matrix, PageRankVector* pageRankVector, PageRankVector* pageRankVector1, int numIterations, double converThreshold, double tParameter) {

    int num_threads = matrix->getNumBlocks();//线程数

     //初始化barrier和amutex
    pthread_barrier_init(&barrier_Iteration, NULL, num_threads);
    pthread_mutex_init(&amutex, NULL);

    diff = 1;
    numIter = 0;
    constant = (1 - tParameter) / pageRankVector->getNodeNumber();
    converThre = converThreshold;
    teleParam = tParameter;

    //创建线程
    pthread_t handles[num_threads];// 创建对应的 Handle
    threadParam_t param[num_threads];// 创建对应的线程数据结构
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

    cout << "总共迭代了 " << numIter << " 次" << endl;

    pthread_barrier_destroy(&barrier_Iteration);
    pthread_mutex_destroy(&amutex);

    return pRVector->sort();
}
