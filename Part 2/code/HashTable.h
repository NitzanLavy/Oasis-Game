//
// Created by Amit Tsvieli on 6/9/2018.
//

#ifndef WET2_HASHTABLE_H
#define WET2_HASHTABLE_H

#include <exception>
#include "List.h"

#define nullptr 0


template <class T>
class HashTable{
    int number_of_elements;
    int hash_size;
    List<T>** array;

    void rehash(){
        List<T>** new_array = new List<T>*[3*hash_size];
        for (int i=0; i < hash_size; i++){
            for(typename List<T>::Iterator it = array[i]->begin(); it != array[i]->end(); it.operator++()){
                int index = (*it).getId() % (3*hash_size);
                if(new_array[index] == nullptr){
                    new_array[index] = new List<T>(*it);
                } else {
                    new_array[index] = &array[index]->insertFirst(*it);
                }
            }
            if (array[i] != nullptr){
                delete array[i];
                array[i] = nullptr;
            }
        }
        delete array;
        hash_size = 3*hash_size;
        array = new_array;
    }

public:
    HashTable(int n, T** data, int* original_keys): number_of_elements(0), hash_size(3*n),
                                                    array(nullptr){
        array = new List<T>*[3*n];
        for (int i=0; i < 3*n; i++){
            array[i] = nullptr;
        }
        for (int i=0; i < n; i++){
            T* current_data = *(data+i);
            int current_key = *(original_keys+i);
            this->insertElement(current_data, current_key, false);
        }
    }

    ~HashTable()
    {
        for(int i=0; i < hash_size; i++){
            if (array[i] != nullptr){
                array[i]->deleteDataFromList();
                delete array[i];
                array[i] = nullptr;
            }
        }
        delete[] array;
    }

    HashTable(const HashTable& list) = delete;

    class AlreadyInHash : public std::exception {};

    void insertElement(T* data, int original_key, bool check_existence){
        if (check_existence){
            T* found = this->findElement(original_key);
            if (found != nullptr){
                throw AlreadyInHash();
            }
        }
        int index = (original_key % hash_size);
        if(array[index] == nullptr){
            array[index] = new List<T>(*data);
        } else {
            array[index] = &array[index]->insertFirst(*data);
        }
        number_of_elements++;
        if (number_of_elements > 3*hash_size){
                this->rehash();
        }
    }

    T* findElement(int original_key){
        int index = original_key % hash_size;
        return array[index]->find(original_key);
    }
/*
    void removeElement(int original_key){
        int index = original_key % hash_size;
        array[index] = &array[index]->remove(original_key);
    }
    */
};


#endif //WET2_HASHTABLE_H
