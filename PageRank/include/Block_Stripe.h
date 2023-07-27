#ifndef BLOCK_STRIPE_H
#define BLOCK_STRIPE_H

#include <string>
#include <vector>
#include "Stripe.h"

using namespace std;

//稀疏矩阵分块
class Block_Stripe {
    private:
        vector<Stripe>* stripes;//分块内所有有效列的集合

    public:
        Block_Stripe();
        void addStripe(int fromNodeID, int toNodeID);
        vector<Stripe>* getStripes();
        int size();
        Stripe* getStripe(int index);
        int contain(int fromNodeID);
};


#endif // BLOCK_STRIPE_H
