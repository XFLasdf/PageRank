#include "Block_Stripe.h"

Block_Stripe::Block_Stripe() {
    stripes = new vector<Stripe>;
}

void Block_Stripe::addStripe(int fromNodeID, int toNodeID) {
    int index = contain(fromNodeID);
    if (index == -1) {
        Stripe s(fromNodeID);
        s.addDestination(toNodeID);
        stripes->push_back(s);
    } else {
        (*stripes)[index].addDestination(toNodeID);
    }
}

vector<Stripe>* Block_Stripe::getStripes() {
    return stripes;
}

int Block_Stripe::size() {
    return stripes->size();
}

Stripe* Block_Stripe::getStripe(int index) {
    return &(*stripes)[index];
}

int Block_Stripe::contain(int fromNodeID) {
    if (stripes->empty()) {
        return -1;
    }
    for (int i = 0; i < stripes->size(); i++) {
        if (fromNodeID == (*stripes)[i].getSrc()) {
            return i;
        }
    }
    return -1;
}
