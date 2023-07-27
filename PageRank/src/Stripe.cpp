#include "Stripe.h"

#include "Stripe.h"
#include <iostream>

Stripe::Stripe(int src) {
    this->src = src;
    this->degree = 0;
}

Stripe::Stripe(int src, int degree) {
    this->src = src;
    this->degree = degree;
}

int Stripe::size() {
    return destination.size();
}

void Stripe::addDestination(int des) {
    if (!contain(des)) {
        destination.push_back(des);
    }
}

bool Stripe::contain(int des) {
    for (int i = 0; i < destination.size(); i++) {
        if (destination[i] == des) {
            return true;
        }
    }
    return false;
}

int Stripe::getSrc() {
    return src;
}

int Stripe::getDegree() {
    return degree;
}

int Stripe::getDes(int index) {
    if (index >= 0 && index < destination.size()) {
        return destination[index];
    } else {
        std::cout << "Stripe访问下标越界" << std::endl;
        return -1;
    }
}

void Stripe::setDegree(int degree) {
    this->degree = degree;
}

std::vector<int> Stripe::getDestination() {
    return destination;
}

bool Stripe::operator==(const Stripe& other) {
    return src == other.src;
}

int Stripe::hashCode() {
    return std::hash<int>{}(src);
}
