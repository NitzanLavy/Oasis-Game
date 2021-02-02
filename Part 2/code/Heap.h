//
// Created by Nitzan on 09/06/2018.
//

#ifndef WET2_HEAP_H
#define WET2_HEAP_H

#include "DynamicArray.h"
#define nullptr 0
#define DELETE_INDEX {-1}

template <class T>
class Heap {
    DynamicArray<T*>* arr;      //array starts in index 1
    int num_of_elements;

    void swap (int index1, int index2) {
        (*arr)[index1]->setIndex(index2);
        (*arr)[index2]->setIndex(index1);
        T* temp = (*arr)[index1];
        (*arr)[index1] = (*arr)[index2];
        (*arr)[index2] = temp;
    }

    void siftDown (int index) {
        if (num_of_elements == 1) {
            return;
        }
        if (index*4 <= num_of_elements) {
            siftDown(index*2);
        }
        if ((index*4)+2 <= num_of_elements) {
            siftDown((index*2)+1);
        }
        int min = index*2;
        if (((index*2)+1) <= num_of_elements &&
            (*arr)[index*2]->getId() > (*arr)[(index*2) +1]->getId()) {
            min = (index*2)+1;
        }
        if ((*arr)[index]->getId() > (*arr)[min]->getId()) {
            this->swap (index, min);
        }
    }

    void siftUp(int index) {
        if (index == 1) {
            return;
        }
        if ((*arr)[index]->getId() < (*arr)[index/2]->getId()) {
            this->swap(index, index/2);
        }
        return siftUp(index/2);
    }

public:
    explicit Heap(int n, T** keys_arr): arr(nullptr), num_of_elements(0) {
        arr = new DynamicArray<T*>(n*2);
        for (int i=0;i<n;i++) {
            this->insert(*(keys_arr+i),true);
        }
        this->siftDown(1);
    }

    ~Heap() {
        delete arr;
    }

    Heap (const Heap& heap) = delete;

    void insert (T* data, bool is_start) {
        (*arr)[num_of_elements+1] = data;
        num_of_elements++;
        if (!is_start) {
            siftUp(num_of_elements);
        }
    }

    void decKey (int index, int new_key) {
        if ((*arr)[index]->getId() <= new_key) {
            return;
        }
        int org_key = (*arr)[index]->getId();
        (*arr)[index]->setId(new_key);
        siftUp(index);
        (*arr)[1]->setId(org_key);
    }

    T* findMin() {
        return (*arr)[1];
    }

    void deleteMin() {
        swap(1,num_of_elements);
        (*arr)[num_of_elements] = nullptr;
        num_of_elements--;
        this->siftDown(1);
    }

    void remove(int index) {
        this->decKey(index,DELETE_INDEX);
        this->deleteMin();
    }

    int getNumOfElements () {
        return num_of_elements;
    }

    void setNumOfElements (int num) {
        num_of_elements = num;
    }
};

#endif //WET2_HEAP_H
