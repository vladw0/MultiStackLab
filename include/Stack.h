#pragma once

#include <iostream>

template <class T>
class TStack{
protected:
	T** data;
	int len;
    int top;
	bool isNew;
public:
	TStack();
	TStack(int len_);
	TStack(const TStack& obj);
    TStack(TStack&& obj);
    TStack(T** data_, int len_);
	~TStack();

	int GetLen() const;
    int GetTop() const;

	void Resize(int len_);
	void SetData(T** data_, int len);

	void Push(T* value);
    void Push(const T& value);

    T Pop();

    bool IsEmpty() const;
    bool IsFull() const;


    TStack& operator=(const TStack& obj);
    TStack& operator=(TStack&& obj);
    bool operator ==(const TStack& obj) const;
    bool operator!=(const TStack& obj) const;


    template <class O>
    friend std::ostream& operator<<(std::ostream& o, const TStack<O>& v);
    template <class I>
    friend std::istream& operator>>(std::istream& i, TStack<I>& v);

  
};

template<class T>
inline TStack <T>::TStack(){
    data = nullptr;
    len = 0;
    top = 0;
    isNew = true;
}

template<class T> 
inline TStack<T>::TStack(int len_){
    if (len_ < 0){
        throw - 1;
    }
    if (len_ == 0){
        data = nullptr;
        len = 0;
        top = 0;
        isNew = true;
    }
    else {
        len = len_;
        data = new T * [len];
        for (int i = 0; i < len; i++){
            data[i] = nullptr;
        }
        top = 0;
        isNew = true;
    }
} 

template<class T>
inline TStack<T>::TStack(const TStack& obj){
    data = nullptr;
    len = 0;
    top = 0;
    isNew = true;
    if (obj.len < 0){
        throw -1;
    }
    if (obj.len > 0){
        len = obj.len;
        data = new T*[len];
        for (int i = 0; i < len; i++){
            if (i < obj.top && obj.data[i] != 0){
                data[i] = new T(*(obj.data[i]));
            }
            else {
                data[i] = nullptr;
            }    
        }
        top = obj.top;
        isNew = true;
    }
}

template<class T>
inline TStack<T>::TStack(TStack&& obj){
    len = obj.len;
    data = obj.data;
    isNew = obj.isNew;
    top = obj.top;

    obj.len = 0;
    obj.data = nullptr;
    obj.isNew = false;
    obj.top = 0;

}

template<class T>
inline TStack<T>::TStack(T** data_, int len_){
    if (len < 0){
        throw -1;
    }
    data = data_;
    len = len_;
    isNew = false;
    top = 0;
    for (int i = 0; i < len; i++){
        if (data[i] == 0){
            top = i;
            break;
        }
        top = i + 1;
    }
}

template<class T>
inline TStack<T>::~TStack(){
    if (isNew && data != nullptr){
        for (int i = 0; i < len; i++){
            if (data[i] != nullptr){
                delete data[i];
                data[i] = 0;
            }
        }
        delete [] data;
        data = nullptr;
    }
} 

template<class T>
inline int TStack<T>::GetLen() const{
    return len;
}

template <class T>
inline int TStack<T>::GetTop() const{ 
    return top; 
}

template <class T>
inline void TStack<T>::Resize(int len_) {
    if (!isNew){ 
        throw -1; 
    }
    if (len_ < 0){ 
        throw -1; 
    }
    if (len_ == len){
        return;
    }
    T** newdata = nullptr;
    if (len_ > 0){
        newdata = new T*[len_];
        for (int i = 0; i < len_; i++){ 
            newdata[i] = 0; 
        }
        int limit = len_ < len ? len_ : len;
        for (int i = 0; i < limit; i++){ 
            newdata[i] = data[i]; 
        }
    }
    if (data != nullptr){ 
        delete [] data; 
    }
    data = newdata;
    len = len_;
    if (top > len){ 
        top = len; 
    }
}

template <class T>
inline void TStack<T>::SetData(T** data_, int len_) {
    if (isNew && data != nullptr) {
        for (int i = 0; i < len; i++) {
            if (data[i] != nullptr) { 
                delete data[i]; 
                data[i] = nullptr; 
            }
        }
        delete [] data;
    }
    data = data_; 
    len = len_; 
    isNew = false; 
    top = 0;
    for (int i = 0; i < len; i++) {
        if (data[i] == nullptr) { top = i; break; }
        top = i + 1;
    }
}

template <class T>
inline void TStack<T>::Push(T* value){
    if (top >= len) { 
        throw -1; 
    }
    data[top++] = value;
}

template <class T>
inline void TStack<T>::Push(const T& value) {
    if (top >= len) { 
        throw -1; 
    }
    data[top++] = new T(value);
}

template <class T>
inline T TStack<T>::Pop() {
    if (top == 0) { 
        throw -1; 
    }
    top--;
    if (data[top] == nullptr) { 
        throw -1; 
    }
    T val = *(data[top]);
    delete data[top]; 
    data[top] = nullptr;
    return val;
}

template <class T>
inline bool TStack<T>::IsEmpty() const { 
    return top == 0; 
}

template <class T>
inline bool TStack<T>::IsFull() const {
    return top >= len; 
}

template <class T>
inline TStack<T>& TStack<T>::operator=(const TStack& obj) {
    if (this == &obj) {
        return *this;
    }
    if (isNew && data != nullptr) {
        for (int i = 0; i < len; i++) {
            if (data[i] != nullptr) { 
                delete data[i]; 
                data[i] = nullptr; 
            }
        }
        delete [] data; 
        data = nullptr;
    }
    if (obj.len == 0) { 
        data = nullptr; 
        len = 0; 
        top = 0; 
        isNew = true;
        return *this; 
    }
    len = obj.len;
    data = new T*[len];
    for (int i = 0; i < len; i++) {
        if (i < obj.top && obj.data[i] != nullptr){ 
            data[i] = new T(*(obj.data[i]));
        }
        else {
            data[i] = nullptr;
        }
    }
    top = obj.top; 
    isNew = true;
    return *this;
}

template <class T>
inline TStack<T>& TStack<T>::operator=(TStack&& obj) {
    if (this == &obj) { 
        return *this;
    }
    if (isNew && data != nullptr) {
        for (int i = 0; i < len; i++) { 
            if (data[i] != nullptr) { 
                delete data[i]; data[i] = nullptr; 
            } 
        }
        delete [] data; 
        data = nullptr;
    }
    data = obj.data; 
    len = obj.len; 
    top = obj.top; 
    isNew = obj.isNew;
    obj.data = nullptr; 
    obj.len = 0; 
    obj.top = 0; 
    obj.isNew = false;
    return *this;
}

template <class T>
inline bool TStack<T>::operator==(const TStack& obj) const {
    if (len != obj.len || top != obj.top) {
        return false;
    }
    for (int i = 0; i < len; i++) {
        bool a = (data != nullptr && data[i] != nullptr);
        bool b = (obj.data != nullptr && obj.data[i]!= nullptr);
        if (a != b) {
            return false;
        }
        if (a && b && !(*data[i] == *obj.data[i])) {
            return false;
        }
    }
    return true;
}

template <class T>
inline bool TStack<T>::operator!=(const TStack& obj) const { 
    return !(*this == obj); 
}

template <class O>
inline std::ostream& operator<<(std::ostream& o, const TStack<O>& v) {
    o << "Top=" << v.top << " Len=" << v.len << " [ ";
    for (int i = v.top-1; i >= 0; i--) { 
        if (v.data!= nullptr && v.data[i]!= nullptr) o << *(v.data[i]) << " "; 
    }
    o << "]";
    return o;
}

template <class I>
inline std::istream& operator>>(std::istream& i, TStack<I>& v) {
    int n; 
    if (!(i >> n)) { 
        return i;
    } 
        if (n < 0) { 
            i.setstate(std::ios::failbit); 
            return i;
        }
    if (v.isNew && v.data != nullptr) { 
        for(int k = 0; k < v.len; k++) { 
            if(v.data[k]!= nullptr) { 
                delete v.data[k]; 
                v.data[k] = nullptr;
            }
        } 
        delete [] v.data; 
        v.data = nullptr;
    }
    v.len = n; 
    v.top = 0; 
    v.isNew = true;
    if(n > 0) { 
        v.data = new I*[n]; 
        for(int k = 0; k < n; k++){ 
            v.data[k] = nullptr; 
        } 
        for(int k = 0; k<n; k++) { 
            I tmp; 
            i>>tmp; 
            v.data[k] = new I(tmp); 
            v.top = k+1; 
        } 
    }
    return i;
}
