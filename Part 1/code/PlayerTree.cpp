//
// Created by Nitzan on 06/05/2018.
//

#include "PlayerTree.h"
#include "ClanTree.h"

Player::Player(int id, int coins, int challenges) : player_id(id),
                        coins(coins), challenges(challenges), player_clan(nullptr) {}

int Player::getPlayerId() {
    return this->player_id;
}

int Player::getCoins() {
    return this->coins;
}

int Player::getChallenges() {
    return this->challenges;
}

Clan* Player::getClan() {
    return this->player_clan;
}

void Player::setClan(Clan* new_clan) {
    this->player_clan = new_clan;
}

void Player::addCoins(int coins) {
    this->coins += coins;
}

void Player::incChallenges() {
    this->challenges++;
}

void PlayerTree::insertPlayer(Tree<Player, int>** player_tree, Player& player, int& id_key) {
    (*player_tree) = (*player_tree)->insert(id_key, player);
}

void PlayerTree::removePlayer(Tree<Player, int>** player_tree, int playerId){
    int* p = new int(playerId);
    Tree<Player, int>* current_root = (*player_tree);
    (*player_tree) = (*player_tree)->remove(*p);
    if (current_root->getData().getPlayerId() == playerId){
        delete current_root;
    }
    if ((*player_tree) == nullptr) {
        (*player_tree) = new Tree<Player, int>();
    }
    delete p;
}

void PlayerTree::completeChallenge(Tree<Player, int>* player_tree, int playerId, int coins) {
    Tree<Player, int>& node = player_tree->find(playerId);
    Player& player = node.getData();
    if (player.getPlayerId() != playerId) {
        throw Tree<Player,int>::DoesNotExist();
    }
    player.addCoins(coins);
    player.incChallenges();
}

void PlayerTree::deleteTree(Tree<Player,int>* player_tree, bool delete_data) {
    player_tree->deleteTree(delete_data);
    delete player_tree;
}