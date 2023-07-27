#ifndef BLOCK_STRIPE_H
#define BLOCK_STRIPE_H

#include <string>
#include <vector>
#include "Stripe.h"

using namespace std;

//ϡ�����ֿ�
class Block_Stripe {
    private:
        vector<Stripe>* stripes;//�ֿ���������Ч�еļ���

    public:
        Block_Stripe();
        void addStripe(int fromNodeID, int toNodeID);
        vector<Stripe>* getStripes();
        int size();
        Stripe* getStripe(int index);
        int contain(int fromNodeID);
};


#endif // BLOCK_STRIPE_H
