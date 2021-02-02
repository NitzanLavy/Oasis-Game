//
// Created by Nitzan on 06/05/2018.
//

#ifndef CODE_CLANTREE_H
#define CODE_CLANTREE_H

#include "Tree.h"
class Player;
class Pair;

class Clan {
    int clan_id;
    Tree<Player,int>* players;
    Tree<Player,Pair>* players_by_coins;
    Player* best_player;

public:
    explicit Clan(int id);
    ~Clan();
    Clan(const Clan& clan) = delete;
    bool operator>(const Clan& p) const;
    int getClanId() const;
    Player* getBestPlayer() const;
    void setBestPlayer(Player* player);
    int getSize() const;
    void getPlayersById(Player** player_arr) const;
    void getPlayersByCoin(Player** player_arr) const;
    void joinClan(Player& new_player, Pair& pair_key, int& id_key);
    void insertPlayerToClanPlayersTree(int& key, Player& player);
    void removePlayerFromClanCoins(Player& player);
    void insertPlayerToClanCoins(Pair& key, Player& player);
    void getScoreBoard(int **players, int *numOfPlayers) const;
    void completedChallenge(Player& player);
    void getClanTreesIntoArrays(Player*** By_Id, Player*** By_coins);
    void insertPlayersInorderFromTwoArraysById(Player** array1, int n1, Player** array2, int n2);
    void insertPlayersInorderFromTwoArraysByCoins(Player** array1, int n1, Player** array2, int n2);
};

namespace ClanTree{

    class AlreadyInClan : public std::exception {};

    void addClan(Tree<Clan, int>** clan_tree, Clan& clan, int& key);
    void uniteClans(Tree<Clan, int>** clan_tree, int id1, int id2);
    void deleteTree(Tree<Clan, int>* clan_tree);
}


#endif //CODE_CLANTREE_H
