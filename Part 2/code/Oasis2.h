//
// Created by Nitzan on 11/06/2018.
//

#ifndef WET2_OASIS2_H
#define WET2_OASIS2_H

#include <new>
#include "Player.h"
#include "Clan.h"
#include "HashTable.h"
#include "Heap.h"
#include "library2.h"
#include "exception"
#include "Tree.h"

#define CHECK_EXIST 1

#define nullptr 0

class Oasis2 {
    Tree<Player,int>* players_tree;
    Heap<Clan>* heap;
    HashTable<Clan>* hashTable;

public:
    explicit  Oasis2(int n, int* clanIDs);
    ~Oasis2();
    Oasis2 (const Oasis2& oasis2) = delete;
    void addClan (int clan_id);
    void addPlayer (int playerID, int score, int clan);
    void clanFight(int clan1, int clan2, int k1, int k2);
    void getMinClan(int* clan);
    void delClanArr (Clan** clans_arr);
};



#endif //WET2_OASIS2_H
