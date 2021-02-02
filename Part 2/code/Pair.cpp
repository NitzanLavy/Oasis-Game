//
// Created by Nitzan on 13/06/2018.
//

#include "Pair.h"

Pair::Pair(int id, int score) : id(id), score(score){}

bool Pair::operator==(const Pair& p) const{
    return (this->id == p.id) && (this->score == p.score);
}

bool Pair::operator!=(const Pair& p) const{
    return !(this->operator==(p));
}

bool Pair::operator>(const Pair& p) const {
    return (this->score > p.score) || ((this->score == p.score) && (this->id < p.id));
}

int Pair::operator+(int num) {
    return (this->score)+num;
}
