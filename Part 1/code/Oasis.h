//
// Created by Nitzan on 06/05/2018.
//

#ifndef CODE_OASIS_H
#define CODE_OASIS_H

#include "PlayerTree.h"
#include "ClanTree.h"
#include "CoinTree.h"

class Oasis {
    Tree<Player,int>* players;
    Tree<Clan,int>* clans;
    Tree<Player,Pair>* players_by_coins;
    Player* best_player;

public:
    explicit Oasis();
    ~Oasis();
    Oasis(const Oasis& oasis) = delete;
    void addClan(int clanID, Clan& clan, int& key);
    void joinClan(int playerID, int clanID);
    void insertPlayer(Player& player, Pair& coin_key, int& id_key);
    void completeChallenge(int playerId, int coins);
    void getBestPlayer(int clanID, int *playerID);
    void getScoreboard(int clanID, int **players, int *numOfPlayers);
    void uniteClans(int clanID1, int clanID2);
};

#endif //CODE_OASIS_H
