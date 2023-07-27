#ifndef STRIPE_H
#define STRIPE_H

#include <string>
#include <vector>

//稀疏矩阵分块内的列
class Stripe {
    private:
        int src;//源网站节点
        int degree;//出度
        std::vector<int> destination;//目的网站节点集合

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
