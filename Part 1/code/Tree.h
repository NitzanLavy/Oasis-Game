//
// Created by Nitzan on 06/05/2018.
//

#ifndef CODE_TREE_H
#define CODE_TREE_H

#include <exception>
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
        if (T->lson != nullptr){
            T->h_left = 1+ max(T->lson->h_left, T->lson->h_right);
        } else {
            T->h_left = 0;
        }
        if (T->rson != nullptr){
            T->h_right = 1 + max(T->rson->h_left, T->rson->h_right);
        } else {
            T->h_right = 0;
        }
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
/*
    void balance() {
        Tree* T = this;
        while(T->father != nullptr){
            Tree* current_father = T->father;
            current_father->update_heights();
            int BF = current_father->h_left - current_father->h_right;
            if(BF >= 2 || BF <= -2){
                current_father->find_roll();
            }
            T = current_father;
        }
    }
*/
    void remove_leaf(){
        Tree* parent = this->father;
        bool is_left_son = false;
        if (parent->lson != nullptr){
            is_left_son = parent->lson->key == key;
        }
        if (is_left_son){
            //delete parent->lson;
            parent->lson = nullptr;
            parent->h_left  = 0;
        } else {
            //delete parent->rson;
            parent->rson = nullptr;
            parent->h_right = 0;
        }
        while (parent != nullptr){
            parent->size--;
            parent = parent->father;
        }
        this->point_to_null();
    }

    void remove_only_one_son(){
        Tree* parent = this->father;
        bool is_left_son = false;
        if (parent->lson != nullptr){
            is_left_son = parent->lson->key == key;
        }
        Tree* only_son = nullptr;
        if (this->lson == nullptr){
            only_son = this->rson;
        } else {
            only_son = this->lson;
        }
        if (is_left_son){
            //delete parent->lson;
            parent->lson = only_son;
            only_son->father = parent;
            parent->h_left--;
        } else {
            //delete parent->rson;
            parent->rson = only_son;
            only_son->father = parent;
            parent->h_right--;
        }
        while (parent != nullptr){
            parent->size--;
            parent = parent->father;
        }
        this->point_to_null();
    }

    Tree* switch_nodes(Tree& to_switch){
        Tree* temp_tree = this->father;
        if (temp_tree != nullptr){
            if (temp_tree->lson->key == this->key) {
                temp_tree->lson = &to_switch;
            }else{
                temp_tree->rson = &to_switch;
            }
        }
        Tree* to_switch_r_son = to_switch.rson;
        if (to_switch.father->key == this->key){
            to_switch.rson = this;
            this->father = &to_switch;
        } else {
            this->rson->father = &to_switch;
            if (to_switch.father->lson->key == to_switch.key){
                to_switch.father->lson = this;
            } else {
                to_switch.father->rson = this;
            }
            this->father = to_switch.father;
            to_switch.rson = this->rson;
        }

        to_switch.father = temp_tree;
        this->rson = to_switch_r_son;
        if (to_switch_r_son != nullptr){
            to_switch_r_son->father = this;
        }

        to_switch.lson = this->lson;
        if (to_switch.lson != nullptr){
            to_switch.lson->father = &to_switch;
        }
        this->lson = nullptr;

        to_switch.h_left = this->h_left;
        this->h_left = 0;

        int to_switch_hr = to_switch.h_right;
        to_switch.h_right = this->h_right;
        this->h_right = to_switch_hr;

        int to_switch_size = to_switch.size;
        to_switch.size = this->size;
        this->size = to_switch_size;

        return this->father;
    }

    Tree* remove_two_sons(Key& key){
        Tree* next_node = this->rson;
        while (next_node->lson != nullptr) {
            next_node = next_node->lson;
        }
        Tree* father_of_garbage = this->switch_nodes(*next_node);
        if (this->rson != nullptr){
            this->remove_only_one_son();
        } else {
            this->remove_leaf();
        }
        father_of_garbage->balance();
        Tree* root = next_node;
        while(root->father != nullptr){
            root = root->father;
        }
        return root;
    }

public:
    Tree(): key(nullptr), data(nullptr) , father(nullptr), lson(nullptr), rson(nullptr), h_left(0),
            h_right(0), size(0){}

    Tree(Key& key, Data& data): key(&key), data(&data), father(nullptr), lson(nullptr),
                                rson(nullptr), h_left(0), h_right(0), size(0){}

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
        while (T_copy->father != nullptr){
            T_copy->father->size++;
            T_copy = T_copy->father;
        }
        T->balance();
        Tree* root = this;
        while(root->father != nullptr){
            root = root->father;
        }
        return root;
    }

    Tree* remove(Key& key){
        Tree* T = &(this->find(key));
        if (this->lson == nullptr && this->rson == nullptr) {
            return nullptr;
        } else if (this->lson != nullptr && this->rson == nullptr && (*(this->key) == key)){
            Tree* root_lson = this->lson;
            this->lson->father = nullptr;
            this->lson = nullptr;
            return root_lson;
        } else if (this->rson != nullptr && this->lson == nullptr && (*(this->key) == key)){
            Tree* root_rson = this->rson;
            this->rson->father = nullptr;
            this->rson = nullptr;
            return root_rson;
        }
        if (*(T->key) != key){
            throw DoesNotExist();
        }
        Tree* parent = T->father;
        if (T->size == 1) {
            T->remove_leaf();
        } else if (T->lson == nullptr || T->rson == nullptr){ //T has only one son
            T->remove_only_one_son();
        } else {
            Tree* new_root = T->remove_two_sons(key);
            T->point_to_null();
            if (*(this->key) != key){
                delete T;
            }
            return new_root;
        }
        parent->balance();
        Tree* root = this;
        while(root->father != nullptr){
            root = root->father;
        }
        if (*(this->key) != key){
            delete T;
        }
        return root;
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
