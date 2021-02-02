//
// Created by Amit Tsvieli on 6/9/2018.
//

#ifndef WET2_LIST_H
#define WET2_LIST_H

#include <exception>

#define nullptr 0

template <class T>
class List {
    T* data;
    List* next;


public:

    class Iterator{
        List* node_ptr;

    public:
        class NodeIsNull : public std::exception {};

        Iterator(): node_ptr(nullptr){};

        explicit Iterator(List* node){
            node_ptr = node;
        }

        ~Iterator() = default;

        Iterator(const Iterator& iterator) = default;

        Iterator& operator=(const Iterator& rhs) = default;

        T& operator*() const{
            if (this->node_ptr == nullptr){
                throw NodeIsNull();
            }
            return *(node_ptr->data);
        }

        Iterator& operator++(){
            if (this->node_ptr == nullptr){
                throw NodeIsNull();
            }
            this->node_ptr = node_ptr->next;
            return *this;
        }

        bool operator==(const Iterator& rhs) const{
            return (rhs.node_ptr == this->node_ptr);
        }

        bool operator!=(const Iterator& rhs) const{
            return !(*this == rhs);
        }
    };


    //List(T& t_data): data(nullptr), next(nullptr){}
    explicit List(T& t_data): data(&t_data), next(nullptr){}
    ~List(){
        data = nullptr;
        delete next;
        next = nullptr;
    }
    List(const List& list) = delete;

    Iterator begin(){
        Iterator beginning(this);
        return beginning;
    }

    Iterator end(){
        Iterator end_of_list(nullptr);
        return end_of_list;
    }

    List& insertFirst(T& t_data){
        /*
        if(this->data == nullptr){
            this->data = &t_data;
            return *this;
        }
        */
        List* new_list = new List(t_data);
        new_list->next = this;
        return *new_list;
    }

    void deleteDataFromList(){
        List* current = this;
        while (current != nullptr){
            delete current->data;
            current->data = nullptr;
            current = current->next;
        }
    }

    T* find(int key){
        List* current = this;
        while(current != nullptr){
            if(current->data->getId() == key){
                return current->data;
            } else {
                current = current->next;
            }
        }
        return nullptr;
    }
/*
    List& remove(int key){
        List* current = this;
        if (current->data->getId() == key){
            List* new_head = this->next;
            dele
        }
        while(current != nullptr){
            if(current->data->getId() == key){
                return current->data;
            } else {
                current = current->next;
            }
        }
        return;
    }
*/
};



#endif //WET2_LIST_H
