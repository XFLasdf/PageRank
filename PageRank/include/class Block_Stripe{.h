#ifndef CLASS BLOCK_STRIPE{_H
#define CLASS BLOCK_STRIPE{_H

#include <string>
#include <vector>
#include "Stripe.h"

class Block_Stripe {
    private:
        std::vector<Stripe> stripes;

    public:
        Block_Stripe();
        void addStripe(int fromNodeID, int toNodeID);
        std::vector<Stripe> getStripes();
        int size();
        Stripe getStripe(int index);
        int contain(int fromNodeID);
};

#endif // CLASS BLOCK_STRIPE{_H
