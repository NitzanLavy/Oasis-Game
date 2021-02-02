//
// Created by Nitzan on 06/05/2018.
//

#ifndef CODE_TREE_H
#define CODE_TREE_H

#include <exception>
#include <assert.h>

#define nullptr 0

template <class Data, class Key>
class Tree {
    Key* key; // need to think
    Data* data;
    Tree* father;
    Tree* lson;
    Tree* rson;
    int h_left;
    int h_right;
    int size; // size includes left tree, right tree and current node. needs to add to system description.
    int sum; //sum of keys : includes left tree, right tree and current node.

    int max(int a, int b)
    {
        if (a > b){
            return a;
        } else {
            return b;
        }
    }

    void point_to_null(){
        this->father = nullptr;
        this->rson = nullptr;
        this->lson = nullptr;
    }

    void LL_Roll () {
        Tree* ptr_lson = this->lson;

        if (this->father == nullptr) {
            if (this->lson->rson != nullptr) {
                this->lson->rson->father = this;
            }
            this->lson = ptr_lson->rson;
            ptr_lson->rson = this;
            ptr_lson->father = nullptr;
            this->father = ptr_lson;
        }
        else {
            if (this->lson->rson != nullptr) {
                this->lson->rson->father = this;
            }
            this->lson = ptr_lson->rson;
            ptr_lson->rson = this;
            ptr_lson->father = this->father;
            if (this->father->rson == this) {
                this->father->rson = ptr_lson;
            }
            if (this->father->lson == this) {
                this->father->lson = ptr_lson;
            }
            this->father = ptr_lson;
        }

        this->update_heights();
        this->father->update_heights();

        int r_size = 0, l_size = 0;
        if (this->lson != nullptr) {
            l_size = this->lson->size;
        }
        if (this->rson != nullptr) {
            r_size = this->rson->size;
        }
        this->size = 1 + r_size + l_size;

        r_size = 0, l_size = 0;
        if (ptr_lson->lson != nullptr) {
            l_size = ptr_lson->lson->size;
        }
        if (ptr_lson->rson != nullptr) {
            r_size = ptr_lson->rson->size;
        }
        ptr_lson->size = 1 + r_size + l_size;
    }

    void RR_Roll () {
        Tree* ptr_rson = this->rson;

        if (this->father == nullptr) {
            if (this->rson->lson != nullptr) {
                this->rson->lson->father = this;
            }
            this->rson = ptr_rson->lson;
            ptr_rson->lson = this;
            ptr_rson->father = nullptr;
            this->father = ptr_rson;
        }
        else {
            if (this->rson->lson != nullptr) {
                this->rson->lson->father = this;
            }
            this->rson = ptr_rson->lson;
            ptr_rson->lson = this;
            ptr_rson->father = this->father;
            if (this->father->lson == this) {
                this->father->lson = ptr_rson;
            }
            if (this->father->rson == this) {
                this->father->rson = ptr_rson;
            }
            this->father = ptr_rson;
        }

        this->update_heights();
        this->father->update_heights();

        int r_size = 0, l_size = 0;
        if (this->lson != nullptr) {
            l_size = this->lson->size;
        }
        if (this->rson != nullptr) {
            r_size = this->rson->size;
        }
        this->size = 1 + r_size + l_size;

        r_size = 0, l_size = 0;
        if (ptr_rson->lson != nullptr) {
            l_size = ptr_rson->lson->size;
        }
        if (ptr_rson->rson != nullptr) {
            r_size = ptr_rson->rson->size;
        }
        ptr_rson->size = 1 + r_size + l_size;
    }

    void LR_Roll () {
        this->lson->RR_Roll();
        this->LL_Roll();
    }

    void RL_Roll () {
        this->rson->LL_Roll();
        this->RR_Roll();
    }

    void find_roll(){
        int BF = this->h_left - this->h_right;
        if (BF == 2){
            int lson_BF = this->lson->h_left - this->lson->h_right;
            if(lson_BF >= 0){
                this->LL_Roll();
            } else if (lson_BF == -1){
                this->LR_Roll();
            }
        } else if (BF == -2){
            int rson_BF = this->rson->h_left - this->rson->h_right;
            if(rson_BF <= 0){
                this->RR_Roll();
            } else if (rson_BF == 1){
                this->RL_Roll();
            }
        }
    }

    void update_heights(){
        Tree* T = this;
        int left_sum = 0, right_sum = 0;
        if (T->lson != nullptr){
            T->h_left = 1+ max(T->lson->h_left, T->lson->h_right);
            left_sum = T->lson->sum;
        } else {
            T->h_left = 0;
        }
        if (T->rson != nullptr){
            T->h_right = 1 + max(T->rson->h_left, T->rson->h_right);
            right_sum = T->rson->sum;
        } else {
            T->h_right = 0;
        }
        T->sum = (*T->key) + right_sum + left_sum;
    }

    void balance() {
        Tree* T = this;
        while(T != nullptr){
            T->update_heights();
            int BF = T->h_left - T->h_right;
            if(BF >= 2 || BF <= -2){
                T->find_roll();
            }
            T = T->father;
        }
    }

public:
    Tree(): key(nullptr), data(nullptr) , father(nullptr), lson(nullptr), rson(nullptr), h_left(0),
            h_right(0), size(0), sum(0){}

    Tree(Key& key, Data& data): key(&key), data(&data), father(nullptr), lson(nullptr),
                                rson(nullptr), h_left(0), h_right(0), size(0), sum(0){}

    ~Tree(){
        delete lson;
        delete rson;
        delete key;
    }

    Tree(const Tree& tree) = delete;

    int getSize(){
        return this->size;
    }

    Data& getData(){
        return *(this->data);
    }

    Key& getRootKey(){
        return *(this->key);
    }

    class AlreadyExist : public std::exception {};
    class DoesNotExist : public std::exception {};
    class EmptyTree : public std::exception {};

    Tree& find(Key& key) {
        if (this->data == nullptr) {
            throw EmptyTree();
        }
        if (*(this->key) == key) {
            return *this;
        }
        else {
            if (*(this->key) > key) {
                if (this->lson == nullptr) {
                    return *this;
                } else {
                    return this->lson->find(key);
                }
            }
            else {
                if (this->rson == nullptr) {
                    return *this;
                } else {
                    return this->rson->find(key);
                }
            }
        }
    }

    Data** inorder (Data** data) {
        if (this->data == nullptr) {
            throw EmptyTree();
        }
        if (this->lson != nullptr) {
            data = this->lson->inorder(data);
        }
        *data = this->data;
        data++;
        if (this->rson != nullptr) {
            data = this->rson->inorder(data);
        }
        return data;
    }

    Tree* insert(Key& key, Data& data) {
        if (this->data == nullptr){
            this->key = &key;
            this->data = &data;
            this->size = 1;
            return this;
        }
        Tree* T = &(this->find(key));
        if (*(T->key) == key) {
            throw AlreadyExist();
        }
        if (*(T->key) > key){
            T->lson = new Tree(key, data);
            T->lson->father = T;
            T = T->lson;
        } else {
            T->rson = new Tree(key, data);
            T->rson->father = T;
            T = T->rson;
        }
        Tree* T_copy = T;
        T->size++;
        T->sum = key + T->sum;
        while (T_copy->father != nullptr){
            T_copy->father->size++;
            T_copy->father->sum = key + T_copy->father->sum;
            T_copy = T_copy->father;
        }
        T->balance();
        Tree* root = this;
        while(root->father != nullptr){
            root = root->father;
        }
        return root;
    }

    void getRankOfKey(Key& key, int* rank){
        if ((this->find(key)).key != key){
            throw DoesNotExist();
        }
        if (*(this->key) == key) {
            (*rank) += (this->lson->size + 1);
            return;
        }
        else {
            if (*(this->key) > key) {
                this->lson->getRankOfKey(key, rank);
            } else {
                (*rank) += (this->lson->size + 1);
                this->rson->getRankOfKey(key ,rank);
            }
        }
    }

    Key& getKeyOfRank(int rank, int* current_rank){
        int check_rank = *current_rank +1;
        if (this->rson != nullptr){
            check_rank += this->rson->size;
        }
        if (check_rank == rank){
            return *this->key;
        } else if (check_rank > rank){
            assert(this->rson != nullptr);
            return this->rson->getKeyOfRank(rank, current_rank);
        } else {
            assert(this->lson != nullptr);
            if (this->rson != nullptr){
                *current_rank += this->rson->size + 1;
            } else {
                *current_rank += 1;
            }
            return this->lson->getKeyOfRank(rank, current_rank);
        }
    }
/*
    void getSumOfBigger(Key& key, int* new_sum){
        if (*(this->find(key)).key != key){
            throw DoesNotExist();
        }
        if (*(this->key) == key) {
            (*new_sum) += ((*this->lson->key)+ this->lson->sum);
            return;
        }
        else {
            if (*(this->key) > key) {
                (*new_sum) += ((*this->lson->key) + this->lson->sum);
                this->lson->getSumOfBigger(key, new_sum);
            } else {
                this->rson->getSumOfBigger(key ,new_sum);
            }
        }
    }
    */

    void getSumOfBigger(Key& key, int* new_sum){
        if (*(this->find(key)).key != key){
            throw DoesNotExist();
        }
        if (*(this->key) == key) {
            if (this->rson != nullptr){
                (*new_sum) += ((*this->key) + this->rson->sum);
            } else {
                (*new_sum) += *this->key+0;
            }
            return;
        }
        else {
            if (*(this->key) > key) {
                if (this->rson != nullptr){
                    (*new_sum) += ((*this->key) + this->rson->sum);
                } else {
                    (*new_sum) += *this->key+0;
                }
                this->lson->getSumOfBigger(key, new_sum);
            } else {
                this->rson->getSumOfBigger(key ,new_sum);
            }
        }
    }

    void deleteTree (bool delete_data) {
        if (this->data == nullptr) {
            return;
            //throw EmptyTree(); not relevant to this usage
        }
        if (this->lson != nullptr) {
            this->lson->deleteTree(delete_data);
            //this->lson = nullptr;
        }
        /*if (this->lson != nullptr && this->lson->rson != nullptr) {
            this->lson->deleteTree();
        }*/
        if (this->rson != nullptr) {
            this->rson->deleteTree(delete_data);
            //this->rson = nullptr;
        }
        /*if (this->rson != nullptr && this->rson->rson != nullptr) {
            this->rson->deleteTree();
        }*/
        /*delete this->lson->data;
        delete this->rson->data;
        delete this->lson->key;
        delete this->rson->key;*/
        delete this->lson;
        delete this->rson;
        if (delete_data == true) {
            delete this->data;
        }
        this->data = nullptr;
        this->lson = nullptr;
        this->rson = nullptr;
    }
};

#endif //CODE_TREE_H
