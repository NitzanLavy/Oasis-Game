//
// Created by Nitzan on 06/05/2018.
//

#ifndef CODE_PLAYERTREE_H
#define CODE_PLAYERTREE_H

#include "Tree.h"
class Clan;

class Player {
    int player_id;
    int coins;
    int challenges;
    Clan* player_clan;

public:
    explicit Player(int id, int coins, int challenges);
    ~Player() = default;
    Player(const Player& player) = delete;
    int getPlayerId();
    int getCoins();
    int getChallenges();
    Clan* getClan();
    void setClan(Clan* new_clan);
    void addCoins(int coins);
    void incChallenges();
};

namespace PlayerTree {
    void insertPlayer(Tree<Player, int>** player_tree, Player& player, int& id_key);
    void removePlayer(Tree<Player, int>** player_tree, int playerId);
    void completeChallenge(Tree<Player, int>* player_tree, int playerId, int coins);
    void deleteTree(Tree<Player,int>* player_tree, bool delete_data);
}

#endif //CODE_PLAYERTREE_H
