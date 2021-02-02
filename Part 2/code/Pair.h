//
// Created by Nitzan on 13/06/2018.
//

#ifndef WET2_PAIR_H
#define WET2_PAIR_H

class Pair {
    int id;
    int score;

public:
    explicit Pair (int id, int score);
    ~Pair() = default;
    Pair(const Pair& p) = delete;
    bool operator==(const Pair& p) const;
    bool operator!=(const Pair& p) const;
    bool operator>(const Pair& p) const;
    int operator+(int num);
};



#endif //WET2_PAIR_H
