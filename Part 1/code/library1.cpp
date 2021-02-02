//
// Created by Nitzan on 12/05/2018.
//

#include <new>
#include "library1.h"
#include "Oasis.h"
#include "CoinTree.h"
#include "PlayerTree.h"
#include "exception"

StatusType addPlayer(void *DS, int playerID, int initialCoins) {
    if (DS == nullptr || playerID <= 0 || initialCoins < 0) {
        return INVALID_INPUT;
    }
    Player* new_player = new Player(playerID,initialCoins,0);
    Pair* coin_key = new Pair(new_player->getPlayerId(), new_player->getCoins());
    int* id_key = new int(new_player->getPlayerId());
    try {
        ((Oasis*)DS)->insertPlayer(*new_player, *coin_key, *id_key);
    } catch (Tree<Player, int>::AlreadyExist& e){
        delete new_player;
        delete coin_key;
        delete id_key;
        return FAILURE;
    } catch (Tree<Player, Pair>::AlreadyExist& e){
        delete new_player;
        delete coin_key;
        delete id_key;
        return FAILURE;
    } catch (Tree<Clan, int>::AlreadyExist& e){
        delete new_player;
        delete coin_key;
        delete id_key;
        return FAILURE;
    } catch (std::bad_alloc& e) {
        delete new_player;
        delete coin_key;
        delete id_key;
        return ALLOCATION_ERROR;
    }  catch (Tree<Player, Pair>::EmptyTree& e) {
        delete new_player;
        delete coin_key;
        delete id_key;
        return FAILURE;
    } catch (std::exception& e) {
        delete new_player;
        delete coin_key;
        delete id_key;
        return FAILURE;
    }
    return SUCCESS;
}

StatusType addClan(void *DS, int clanID){
    if (DS == nullptr || clanID <= 0){
        return INVALID_INPUT;
    }
    Clan* new_clan = new Clan(clanID);
    int* key = new int(clanID);
    try{
        ((Oasis*)DS)->addClan(clanID, *new_clan, *key);
    } catch (std::bad_alloc& e){
        delete new_clan;
        delete key;
        return ALLOCATION_ERROR;
    } catch (Tree<Player, int>::AlreadyExist& e){
        delete new_clan;
        delete key;
        return FAILURE;
    } catch (Tree<Player, Pair>::AlreadyExist& e){
        delete new_clan;
        delete key;
        return FAILURE;
    } catch (Tree<Clan, int>::AlreadyExist& e){
        delete new_clan;
        delete key;
        return FAILURE;
    }  catch (Tree<Player, Pair>::EmptyTree& e) {
        delete new_clan;
        delete key;
        return FAILURE;
    } catch (std::exception& e) {
        delete new_clan;
        delete key;
        return FAILURE;
    }
    return SUCCESS;
}

StatusType joinClan(void *DS, int playerID, int clanID){
    if (DS == nullptr || playerID <= 0 || clanID <= 0){
        return INVALID_INPUT;
    }
    try {
        ((Oasis*)DS)->joinClan(playerID, clanID);
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (Tree<Player, int>::AlreadyExist& e){
        return FAILURE;
    } catch (Tree<Player, Pair>::AlreadyExist& e){
        return FAILURE;
    } catch (Tree<Clan, int>::AlreadyExist& e){
        return FAILURE;
    }  catch (ClanTree::AlreadyInClan& e){
        return FAILURE;
    }  catch (Tree<Player, Pair>::EmptyTree& e) {
        return FAILURE;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType completeChallange(void *DS, int playerID, int coins) {
    if (DS == nullptr || playerID <= 0 || coins <= 0) {
        return INVALID_INPUT;
    }
    try {
        ((Oasis*)DS)->completeChallenge(playerID, coins);
    } catch (Tree<Player, int>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Player, Pair>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Clan, int>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Player, Pair>::EmptyTree& e) {
        return FAILURE;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType getBestPlayer(void *DS, int clanID, int *playerID){
    if (clanID == 0 || DS == nullptr || playerID == nullptr){
        return INVALID_INPUT;
    }
    try{
        ((Oasis*)DS)->getBestPlayer(clanID, playerID);
    } catch (Tree<Player, int>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Player, Pair>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Clan, int>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Player, Pair>::EmptyTree& e) {
        return FAILURE;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType getScoreboard(void *DS, int clanID, int **players, int *numOfPlayers){
    if (DS == nullptr || clanID == 0 || players == nullptr || numOfPlayers == nullptr){
        return INVALID_INPUT;
    }
    try {
        ((Oasis*)DS)->getScoreboard(clanID, players, numOfPlayers);
    } catch (Tree<Player, int>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Player, Pair>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Clan, int>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Player, Pair>::EmptyTree& e) {
        return FAILURE;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType uniteClans(void *DS, int clanID1, int clanID2)
{
    if (DS == nullptr || clanID1 <= 0 || clanID2 <= 0 || clanID1 == clanID2){
        return INVALID_INPUT;
    }
    try {
        ((Oasis*)DS)->uniteClans(clanID1, clanID2);
    } catch (Tree<Player, int>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Player, Pair>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Clan, int>::DoesNotExist& e) {
        return FAILURE;
    }  catch (Tree<Clan, int>::EmptyTree& e) {
        return FAILURE;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

void* init() {
    try {
        Oasis* oasis = new Oasis();
        return ((void*)oasis);
    } catch (std::bad_alloc& e) {
        return nullptr;
    }
}

void quit(void** DS) {
    if (DS == nullptr || *DS == nullptr) {
        return;
    }
    delete ((Oasis*)(*DS));
    *DS = nullptr;
}