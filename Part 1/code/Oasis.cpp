//
// Created by Nitzan on 12/05/2018.
//

#include "Oasis.h"
#include "Tree.h"

Oasis::Oasis() : players(nullptr), clans(nullptr), players_by_coins(nullptr), best_player(nullptr) {
    this->players  = new Tree<Player,int>();
    this->players_by_coins  = new Tree<Player,Pair>();
    this->clans  = new Tree<Clan,int>();
}

Oasis::~Oasis() {
    ClanTree::deleteTree(this->clans);
    this->clans = nullptr;
    PlayerTree::deleteTree(this->players, true);
    this->players = nullptr;
    CoinTree::deleteTree(this->players_by_coins, false);
    this->players_by_coins = nullptr;
}

void Oasis::insertPlayer(Player& player, Pair& coin_key, int& id_key) {
    if (this->best_player == nullptr || (this->best_player->getChallenges() == 0 &&
        this->best_player->getPlayerId() > player.getPlayerId())){
        this->best_player = &player;
    }
        PlayerTree::insertPlayer(&(this->players), player, id_key);
        CoinTree::insertPlayerByCoin(&(this->players_by_coins), player, coin_key);
}

void Oasis::addClan(int clanID, Clan& clan, int& key){
    ClanTree::addClan(&(this->clans), clan, key);
}

void Oasis::joinClan(int playerID, int clanID)
{
    Clan* clan = &(this->clans->find(clanID).getData());
    Player* player = &(this->players->find(playerID).getData());
    if (clan->getClanId() != clanID || player->getPlayerId() != playerID){
        throw Tree<Clan, int>::DoesNotExist();
    }
    Pair* pair_key = new Pair(player->getPlayerId(), player->getCoins());
    int* id_key = new int(player->getPlayerId());
    try {
        clan->joinClan(*player, *pair_key, *id_key);
    } catch (std::exception& e) {
        delete pair_key;
        delete id_key;
        throw e;
    }
}

void Oasis::completeChallenge(int playerId, int coins) {
    Player& advanced_player = this->players->find(playerId).getData();
    if (advanced_player.getPlayerId() != playerId){
        throw Tree<Player, int>::DoesNotExist();
    }
    int original_coins = advanced_player.getCoins();
    CoinTree::removePlayer(&(this->players_by_coins), playerId, original_coins);
    if (advanced_player.getClan() != nullptr){
        advanced_player.getClan()->removePlayerFromClanCoins(advanced_player);
    }
    PlayerTree::completeChallenge(this->players, playerId, coins);
    Pair* key = new Pair(advanced_player.getPlayerId(), advanced_player.getCoins());
    try {
        CoinTree::insertPlayerByCoin(&(this->players_by_coins), advanced_player, *key);
        if (advanced_player.getChallenges() > this->best_player->getChallenges()){
            this->best_player = &advanced_player;
        } else if (advanced_player.getChallenges() == this->best_player->getChallenges() &&
                   playerId < this->best_player->getPlayerId()){
            this->best_player = &advanced_player;
        }
        Clan* clan_of_player = advanced_player.getClan();
        if (clan_of_player != nullptr){
            Pair* key_for_clan = new Pair(advanced_player.getPlayerId(), advanced_player.getCoins());
            try {
                advanced_player.getClan()->insertPlayerToClanCoins(*key_for_clan, advanced_player);
                clan_of_player->completedChallenge(advanced_player);
            } catch (std::exception& e) {
                delete key_for_clan;
                throw e;
            }

        }
    } catch (std::exception& e) {
        delete key;
        throw e;
    }
}

void Oasis::getBestPlayer(int clanID, int *playerID){
    if (clanID < 0){
        if (this->best_player == nullptr){
            *playerID = -1;
        } else {
            *playerID = this->best_player->getPlayerId();
        }
    } else {
        Clan* clan = &(this->clans->find(clanID).getData());
        if (clan->getClanId() != clanID){
            throw Tree<Clan, int>::DoesNotExist();
        }
        if (clan->getBestPlayer() == nullptr){
            *playerID = -1;
        } else {
            *playerID = clan->getBestPlayer()->getPlayerId();
        }
    }
}

void Oasis::getScoreboard(int clanID, int **players, int *numOfPlayers){
    if (clanID < 0){
        if (this->players_by_coins->getSize() == 0){
            *players = nullptr;
            *numOfPlayers = 0;
        } else {
            CoinTree::getScoreBoard(this->players_by_coins, players, numOfPlayers);
        }
    } else {
        Clan *clan = &(this->clans->find(clanID).getData());
        if (clan->getClanId() != clanID) {
            throw Tree<Clan, int>::DoesNotExist();
        }
        if (clan->getBestPlayer() == nullptr) {
            *players = nullptr;
            *numOfPlayers = 0;
        } else {
            clan->getScoreBoard(players, numOfPlayers);
        }
    }
}

void Oasis::uniteClans(int clanID1, int clanID2)
{
    ClanTree::uniteClans(&(this->clans), clanID1, clanID2);
}