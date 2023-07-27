#ifndef STRIPE_H
#define STRIPE_H

#include <string>
#include <vector>

//ϡ�����ֿ��ڵ���
class Stripe {
    private:
        int src;//Դ��վ�ڵ�
        int degree;//����
        std::vector<int> destination;//Ŀ����վ�ڵ㼯��

    public:
        Stripe(int src);
        Stripe(int src, int degree);
        int size();
        void addDestination(int des);
        bool contain(int des);
        int getSrc();
        int getDegree();
        int getDes(int index);
        void setDegree(int degree);
        std::vector<int> getDestination();
        bool operator==(const Stripe& other);
        int hashCode();
};

#endif // STRIPE_H
