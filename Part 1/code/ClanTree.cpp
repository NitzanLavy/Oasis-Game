//
// Created by Nitzan on 06/05/2018.
//

#include <cstdlib>
#include <assert.h>
#include "ClanTree.h"
#include "PlayerTree.h"
#include "CoinTree.h"

Clan::Clan(int id): clan_id(id), players(nullptr), players_by_coins(nullptr),
                             best_player(nullptr){
    this->players = new Tree<Player, int>();
    this->players_by_coins = new Tree<Player, Pair>();
}

Clan::~Clan(){
    PlayerTree::deleteTree(this->players, false);
    this->players = nullptr;
    CoinTree::deleteTree(this->players_by_coins, false);
    this->players_by_coins = nullptr;
}

bool Clan::operator>(const Clan& p) const {
    if (this->getSize() > p.getSize()) {
        return true;
    } else if (this->getSize() == p.getSize()) {
        if (this->getClanId() < p.getClanId()) {
            return true;
        }
    }
    return false;
}

int Clan::getClanId() const{
    return this->clan_id;
}

Player* Clan::getBestPlayer() const{
    return this->best_player;
}

void Clan::setBestPlayer(Player* player){
    this->best_player = player;
}

int Clan::getSize() const{
    return this->players->getSize();
}

void Clan::getPlayersById(Player** player_arr) const{
    this->players->inorder(player_arr);
}

void Clan::getPlayersByCoin(Player** player_arr) const{
    this->players_by_coins->inorder(player_arr);
}

void Clan::joinClan(Player &new_player, Pair& pair_key, int& id_key) {
    if (new_player.getClan() != nullptr){
        throw ClanTree::AlreadyInClan();
    }
    if (this->best_player == nullptr){
        this->best_player = &new_player;
    } else if (new_player.getChallenges() > this->best_player->getChallenges()){
        this->best_player = &new_player;
    } else if(new_player.getChallenges() == this->best_player->getChallenges()){
        if (new_player.getPlayerId() < this->best_player->getPlayerId()){
            this->best_player = &new_player;
        }
    }
    new_player.setClan(this);
    PlayerTree::insertPlayer(&(this->players), new_player, id_key);
    CoinTree::insertPlayerByCoin(&(this->players_by_coins), new_player, pair_key);
}

void Clan::getScoreBoard(int **players, int *numOfPlayers) const{
    if (this->players == nullptr){
        *players = nullptr;
        *numOfPlayers = 0;
        return;
    }
    CoinTree::getScoreBoard(this->players_by_coins, players, numOfPlayers);
}

void Clan::completedChallenge(Player& player){
    if (player.getChallenges() > this->best_player->getChallenges()) {
        this->best_player = &player;
    } else if (player.getChallenges() == this->best_player->getChallenges()){
        if (player.getPlayerId() < this->best_player->getPlayerId()){
            this->best_player = &player;
        }
    }
}


void ClanTree::addClan(Tree<Clan, int>** clan_tree, Clan& clan, int& key){
    (*clan_tree) = (*clan_tree)->insert(key, clan);
}

void Clan::insertPlayerToClanCoins(Pair& key, Player& player){
    CoinTree::insertPlayerByCoin(&(this->players_by_coins), player, key);
}

void Clan::insertPlayerToClanPlayersTree(int& key, Player& player){
    PlayerTree::insertPlayer(&(this->players), player, key);
}

void Clan::removePlayerFromClanCoins(Player& player){
    CoinTree::removePlayer(&(this->players_by_coins), player.getPlayerId(), player.getCoins());
}

void Clan::insertPlayersInorderFromTwoArraysById(Player** array1, int n1, Player** array2, int n2){
    int i=0, j=0;
    for(;i < n1 && j < n2;) {
        if ((*(array1 + i))->getPlayerId() < (*(array2 + j))->getPlayerId()) {
            if ((*(array1 + i))->getChallenges() != 0) {
                int* key = new int((*(array1 + i))->getPlayerId());
                try{
                    this->insertPlayerToClanPlayersTree(*key, **(array1 + i));
                } catch (std::exception& e){
                    delete key;
                }
                (*(array1 + i))->setClan(this);
            } else {
                (*(array1 + i))->setClan(nullptr);
            }
            i++;
        } else {
            if ((*(array2 + j))->getChallenges() != 0) {
                int* key = new int((*(array2 + j))->getPlayerId());
                try{
                    this->insertPlayerToClanPlayersTree(*key, **(array2 + j));
                } catch (std::exception& e){
                    delete key;
                }
                (*(array2 + j))->setClan(this);
            } else {
                (*(array2 + j))->setClan(nullptr);
            }
            j++;
        }
    }
    int k = 0;
    int k_limit = 0;
    Player** remaining_array = nullptr;
    if (i == n1){
        k = j;
        k_limit = n2;
        remaining_array = array2;
    } else {
        k = i;
        k_limit = n1;
        remaining_array = array1;
    }
    for (; k < k_limit; k++){
        if ((*(remaining_array + k))->getChallenges() != 0) {
            int* key = new int((*(remaining_array + k))->getPlayerId());
            try{
                this->insertPlayerToClanPlayersTree(*key, **(remaining_array + k));
            } catch (std::exception& e){
                delete key;
            }
            (*(remaining_array + k))->setClan(this);
        } else {
            (*(remaining_array + k))->setClan(nullptr);
        }
    }
}

void Clan::insertPlayersInorderFromTwoArraysByCoins(Player** array1, int n1, Player** array2, int n2){
    int i=0, j=0;
    for(;i < n1 && j < n2;) {
        if (((*(array1 + i))->getCoins() < (*(array2 + j))->getCoins()) ||
            (((*(array1 + i))->getCoins() == (*(array2 + j))->getCoins()) &&
             ((*(array1 + i))->getPlayerId() > (*(array2 + j))->getPlayerId()))){
            if ((*(array1 + i))->getChallenges() != 0) {
                Pair* key = new Pair((*(array1 + i))->getPlayerId(), (*(array1 + i))->getCoins());
                try{
                    this->insertPlayerToClanCoins(*key, **(array1 + i));
                } catch (std::exception& e){
                    delete key;
                }
                (*(array1 + i))->setClan(this);
            } else {
                (*(array1 + i))->setClan(nullptr);
            }
            i++;
        } else {
            if ((*(array2 + j))->getChallenges() != 0) {
                Pair* key = new Pair((*(array2 + j))->getPlayerId(), (*(array2 + j))->getCoins());
                try{
                    this->insertPlayerToClanCoins(*key, **(array2 + j));
                } catch (std::exception& e){
                    delete key;
                }
                (*(array2 + j))->setClan(this);
            } else {
                (*(array2 + j))->setClan(nullptr);
            }
            j++;
        }
    }
    int k = 0;
    int k_limit = 0;
    Player** remaining_array = nullptr;
    if (i == n1){
        k = j;
        k_limit = n2;
        remaining_array = array2;
    } else {
        k = i;
        k_limit = n1;
        remaining_array = array1;
    }
    for (; k < k_limit; k++){
        if ((*(remaining_array + k))->getChallenges() != 0) {
            Pair* key = new Pair((*(remaining_array + k))->getPlayerId(), (*(remaining_array + k))->getCoins());
            try{
                this->insertPlayerToClanCoins(*key, **(remaining_array + k));
            } catch (std::exception& e){
                delete key;
            }
            (*(remaining_array + k))->setClan(this);
        } else {
            (*(remaining_array + k))->setClan(nullptr);
        }
    }
}

void Clan::getClanTreesIntoArrays(Player*** By_Id, Player*** By_coins){
    int n = this->getSize();
    (*By_Id) = new Player*[n];
    (*By_coins) = new Player*[n];
    this->getPlayersById((*By_Id));
    this->getPlayersByCoin((*By_coins));
}

void ClanTree::uniteClans(Tree<Clan, int>** clan_tree, int id1, int id2){
    Clan* clan1 = &((*clan_tree)->find(id1).getData());
    Clan* clan2 = &((*clan_tree)->find(id2).getData());
    if (clan1->getClanId() != id1 || clan2->getClanId() != id2){
        throw Tree<Clan, int>::DoesNotExist();
    }
    Clan* to = nullptr;
    Clan* from = nullptr;
    if (*clan1 > *clan2){
        to = clan1;
        from = clan2;
    } else {
        to = clan2;
        from = clan1;
    }
    if (to->getSize() == 0 && from->getSize() == 0){
        int* from_id_key = new int(from->getClanId());
        Tree<Clan, int>* current_root = (*clan_tree);
        int current_root_id = current_root->getData().getClanId();
        (*clan_tree) = (*clan_tree)->remove(*from_id_key);
        if (current_root_id == *from_id_key){
            delete current_root;
        }
        delete from;
        delete from_id_key;
        return;
    }
    int n_from = from->getSize();
    Player** from_player_player_arr = nullptr;
    Player** from_player_coin_arr = nullptr;
    if (n_from != 0) {
        try{
            from->getClanTreesIntoArrays(&from_player_player_arr, &from_player_coin_arr);
        }catch (std::exception &e) {
            delete[] from_player_player_arr;
            delete[] from_player_coin_arr;
            throw e;
        }
    }
    int n_to = to->getSize();
    Player** to_player_player_arr = nullptr;
    Player** to_player_coin_arr = nullptr;
    if (n_to != 0){
        try{
            to->getClanTreesIntoArrays(&to_player_player_arr, &to_player_coin_arr);
        }catch (std::exception& e) {
            if (n_from != 0){
                delete[] from_player_player_arr;
                delete[] from_player_coin_arr;
            }
            delete[] to_player_player_arr;
            delete[] to_player_coin_arr;
            throw e;
        }
    }
    int* to_id_key = new int(to->getClanId());
    int* from_id_key = new int(from->getClanId());
    Tree<Clan, int>* current_root = (*clan_tree);
    int current_root_id = current_root->getData().getClanId();
    (*clan_tree) = (*clan_tree)->remove(*to_id_key);
    if (current_root_id == *to_id_key){
        delete current_root;
    }
    current_root = (*clan_tree);
    current_root_id = current_root->getData().getClanId();
    (*clan_tree) = (*clan_tree)->remove(*from_id_key);
    if (current_root_id == *from_id_key){
        delete current_root;
    }
    if ((*clan_tree) == nullptr){
        (*clan_tree) = new Tree<Clan,int>();
    }
    delete from;
    delete from_id_key;
    Clan* new_empty_clan = new Clan(to->getClanId());
    delete to;
    ClanTree::addClan(clan_tree, *new_empty_clan, *to_id_key);
    if (n_from == 0){
        for(int i = 0; i < n_to; i++) {
            if ((*(to_player_player_arr+i))->getChallenges() != 0) {
                int* id_key = new int((*(to_player_player_arr+i))->getPlayerId());
                try{
                    new_empty_clan->insertPlayerToClanPlayersTree(*id_key, **(to_player_player_arr+i));
                } catch (std::exception& e){
                    delete id_key;
                    throw e;
                }
                (*(to_player_player_arr+i))->setClan(new_empty_clan);
            } else {
                (*(to_player_player_arr+i))->setClan(nullptr);
            }
            if ((*(to_player_coin_arr+i))->getChallenges() != 0) {
                Pair* pair_key = new Pair((*(to_player_coin_arr+i))->getPlayerId(), (*(to_player_coin_arr+i))->getCoins());
                try{
                    new_empty_clan->insertPlayerToClanCoins(*pair_key, **(to_player_coin_arr+i));
                } catch (std::exception& e){
                    delete pair_key;
                    throw e;
                }
                (*(to_player_coin_arr + i))->setClan(new_empty_clan);
            } else {
                (*(to_player_coin_arr + i))->setClan(nullptr);
            }
        }
    } else {
        try{
            new_empty_clan->insertPlayersInorderFromTwoArraysById(to_player_player_arr, n_to,
                                                              from_player_player_arr, n_from);
            new_empty_clan->insertPlayersInorderFromTwoArraysByCoins(to_player_coin_arr, n_to,
                                                              from_player_coin_arr, n_from);
        }catch (std::exception& e) {
            delete[] from_player_player_arr;
            delete[] from_player_coin_arr;
            delete[] to_player_player_arr;
            delete[] to_player_coin_arr;
            throw e;
        }
    }
    if (n_from != 0){
        delete[] from_player_player_arr;
        delete[] from_player_coin_arr;
    }
    delete[] to_player_player_arr;
    delete[] to_player_coin_arr;
    if (new_empty_clan->getSize() != 0){
        Player** final_array = new Player*[new_empty_clan->getSize()];
        try{
            new_empty_clan->getPlayersById(final_array);
            Player* best = *final_array;
            for (int l=0; l < new_empty_clan->getSize(); l++){
                if ((*(final_array+l))->getChallenges() > best->getChallenges()){
                    best = *(final_array+l);
                } else if((*(final_array+l))->getChallenges() == best->getChallenges()) {
                    if ((*(final_array+l))->getPlayerId() < best->getPlayerId()) {
                        best = *(final_array+l);
                    }
                }
            }
            new_empty_clan->setBestPlayer(best);
        }catch (std::exception& e) {
            delete[] final_array;
            throw e;
        }
        delete[] final_array;
    }
}



void ClanTree::deleteTree(Tree<Clan, int>* clan_tree) {
    clan_tree->deleteTree(true);
    delete clan_tree;
}



