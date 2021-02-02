//
// Created by Nitzan on 11/06/2018.
//

#ifndef WET2_PLAYER_H
#define WET2_PLAYER_H

#define nullptr 0

class Player {
    int score;
    int id;

public:
    explicit Player(int score, int id): score(score), id(id){}
    ~Player() = default;
    Player (const Player& p) = delete;

};

#endif //WET2_PLAYER_H
