//
// Created by Nitzan on 06/05/2018.
//

#include <cstdlib>
#include <new>
#include "CoinTree.h"
#include "PlayerTree.h"

Pair::Pair(int id, int coin) : id(id), coin(coin){}

bool Pair::operator==(const Pair& p) const{
    return (this->id == p.id) && (this->coin == p.coin);
}

bool Pair::operator!=(const Pair& p) const{
    return !(this->operator==(p));
}

bool Pair::operator>(const Pair& p) const {
    return (this->coin > p.coin) || ((this->coin == p.coin) && (this->id < p.id));
}

void CoinTree::getScoreBoard(Tree<Player, Pair>* coin_tree, int **players, int *numOfPlayers){
    int n = coin_tree->getSize();
    *numOfPlayers = n;
    Player** player_arr = new Player*[n];
    try {
        coin_tree->inorder(player_arr);
        *players = (int*)malloc(n*sizeof(int));
        if(*players == nullptr){
            throw std::bad_alloc();
        }
        for (int i=0; i < n; i++){
            *((*players)+(n-1-i)) = (*(player_arr+i))->getPlayerId();
        }
    } catch (std::exception& e) {
        delete[] player_arr;
        throw e;
    }
    delete[] player_arr;
}

void CoinTree::insertPlayerByCoin(Tree<Player, Pair>** coin_tree, Player& player, Pair& key) {
    (*coin_tree) = (*coin_tree)->insert(key, player);
}

void CoinTree::removePlayer(Tree<Player, Pair>** coin_tree, int playerId, int coins) {
    Pair* p = new Pair(playerId, coins);
    Tree<Player, Pair>* current_root = (*coin_tree);
    (*coin_tree) = (*coin_tree)->remove(*p);
    if (current_root->getData().getPlayerId() == playerId &&
        current_root->getData().getCoins() == coins){
        delete current_root;
    }
    if ((*coin_tree) == nullptr){
        (*coin_tree) = new Tree<Player, Pair>();
    }
    delete p;
}

void CoinTree::deleteTree(Tree<Player, Pair>* coin_tree, bool delete_data) {
    coin_tree->deleteTree(delete_data);
    delete coin_tree;
}