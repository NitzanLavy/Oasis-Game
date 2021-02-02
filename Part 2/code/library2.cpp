//
// Created by Nitzan on 12/06/2018.
//

#include <new>
#include "library2.h"
#include "Oasis2.h"
#include "exception"

void* init(int n, int *clanIDs) {
    if (n<2 || clanIDs == nullptr) {
        return nullptr;
    }
    for (int i=0;i<n;i++) {
        if (*(clanIDs+i) < 0) {
            return nullptr;
        }
    }
    try {
        Oasis2* oasis2 = new Oasis2 (n, clanIDs);
        return oasis2;
    } catch (std::exception& e) {
        return nullptr;
    }
}

StatusType addClan(void *DS, int clanID) {
    if (DS == nullptr || clanID < 0) {
        return INVALID_INPUT;
    }
    try {
        ((Oasis2 *) DS)->addClan(clanID);
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType addPlayer(void *DS, int playerID, int score, int clan) {
    if (DS == nullptr || playerID < 0 || score < 0 || clan < 0) {
        return INVALID_INPUT;
    }
    try {
        ((Oasis2 *) DS)->addPlayer(playerID,score,clan);
        return SUCCESS;
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
}

StatusType clanFight(void *DS, int clan1, int clan2, int k1, int k2){
    if (DS == nullptr || k1 <= 0 || k2 <= 0 || clan1 < 0 || clan2 < 0){
        return INVALID_INPUT;
    }
    if (clan1 == clan2){
        return FAILURE;
    }
    try {
        ((Oasis2*)DS)->clanFight(clan1, clan2, k1, k2);
    } catch (std::bad_alloc& e) {
        return ALLOCATION_ERROR;
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

StatusType getMinClan(void *DS, int *clan) {
    if (DS == nullptr || clan == nullptr) {
        return INVALID_INPUT;
    }
    try {
        ((Oasis2*)DS)-> getMinClan(clan);
    } catch (std::exception& e) {
        return FAILURE;
    }
    return SUCCESS;
}

void quit(void** DS) {
    if (DS == nullptr) {
        return;
    }
    delete ((Oasis2*)(*DS));
    *DS = nullptr;
}

