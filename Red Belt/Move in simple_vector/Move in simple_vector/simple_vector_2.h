//
//  simple_vector_2.h
//  Move in simple_vector
//
//  Created by Andrew Kireev on 08.06.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#ifndef simple_vector_2_h
#define simple_vector_2_h


#include <algorithm>
#include <iostream>

using namespace std;

template <typename T>
class SimpleVector {
public:
    SimpleVector() {
        size_ = capacity_ = 0;
        data = nullptr;
    }
    SimpleVector (const SimpleVector<T>& vec) = delete;
    SimpleVector (SimpleVector<T>&& vec) : data(new T [vec.capacity_]), size_(vec.size_),
    capacity_(vec.capacity_){
        std::move(vec.begin(), vec.end(), begin());
    }
    explicit SimpleVector(size_t size) {
        size_ = capacity_ = size;
        data = new T[size];
    }
    ~SimpleVector() {
        delete [] data;
    }
    
    T& operator[](size_t index) {
        return data [index];
    }
    
    
    void operator=(SimpleVector rhs) {
        if (rhs.Size() <= capacity_) {
            std::move(rhs.begin(), rhs.end(), begin());
            size_ = rhs.Size();
            
        } else {
            SimpleVector<T> vec(rhs);
            std::swap(vec.data, data);
            std::swap(vec.capacity_, capacity_);
            std::swap(vec.size_, size_);
        }
    }
    T* begin() { return data;}
    T* end() { return data + size_;}
    
    const T* begin() const { return data;}
    const T* end() const { return data + size_;}
    
    size_t Size() const {
        return size_;
    }
    size_t Capacity() const {
        return capacity_;
    }
    void PushBack(T value) {
        if (this->Size() == 0) {
            data = new T[++capacity_];
        } else if (Size() == Capacity()){
            T* old_data = data;
            capacity_ *= 2;
            data = new T[capacity_];
            
            for (size_t i = 0; i < Size(); ++i) {
                data[i] = std::move(old_data[i]);
            }
            
            delete[] old_data;
        }
        data[size_++] = std::move(value);
        
    }
    
    
private:
    size_t size_;
    size_t capacity_;
    T* data;
};

#endif /* simple_vector_2_h */




