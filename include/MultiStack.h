#pragma once

#include "Stack.h"
#include <iostream>

template <class T>
class TMultiStack {
protected:
    T** memory;
    int total;
    int k;
    int* start;
    int* cap;
    TStack<T>* stacks;
public:
    TMultiStack();
    TMultiStack(int total_, int k_);
    TMultiStack(const TMultiStack& other);
    TMultiStack(TMultiStack&& other);
    ~TMultiStack();

    void Push(const T& value, int stackIndex);
    T Pop(int stackIndex);
    bool IsEmpty(int stackIndex) const;
    bool IsFull(int stackIndex) const;

    class AllIterator {
        TMultiStack<T>* ms;
        int pos;
    public:
        AllIterator(TMultiStack<T>* ms_, int pos_) : ms(ms_), pos(pos_) {}
        bool operator!=(const AllIterator& o) const { return pos != o.pos || ms != o.ms; }
        AllIterator& operator++() {
            pos++;
            while (pos < ms->total && ms->memory[pos] == nullptr) pos++;
            return *this;
        }
        T& operator*() const { return *(ms->memory[pos]); }
    };
    AllIterator beginAll() {
        int p = 0; while (p < total && memory[p] == nullptr) p++; return AllIterator(this, p);
    }
    AllIterator endAll() { return AllIterator(this, total); }

    TStack<T>& operator[](int idx);

protected:
    void allocateMeta(int total_, int k_);
    void freeMeta();
    void updateStacksData();
    void checkIndex(int idx) const;
    bool tryShiftRight(int idx);
    bool tryShiftLeft(int idx);
    void FullRepack();
};



template <class T>
inline TMultiStack<T>::TMultiStack() {
    memory = nullptr; total = 0; k = 0; start = nullptr; cap = nullptr; stacks = nullptr;
}

template <class T>
inline void TMultiStack<T>::allocateMeta(int total_, int k_) {
    if (k_ <= 0) { throw -1; }
    if (total_ < 0) { throw -1; }
    total = total_;
    k = k_;
    memory = new T*[total];
    for (int i = 0; i < total; ++i) memory[i] = nullptr;
    start = new int[k];
    cap = new int[k];
    stacks = new TStack<T>[k];
    int base = total / k;
    int rem = total % k;
    int pos = 0;
    for (int i = 0; i < k; ++i) {
        cap[i] = base + (i < rem ? 1 : 0);
        start[i] = pos;
        pos += cap[i];
    }
    updateStacksData();
}

template <class T>
inline void TMultiStack<T>::freeMeta() {
    if (memory != nullptr) {
        for (int i = 0; i < total; ++i) {
            if (memory[i] != nullptr) {
                delete memory[i];
                memory[i] = nullptr;
            }
        }
        delete [] memory;
        memory = nullptr;
    }
    if (start != nullptr) { delete [] start; start = nullptr; }
    if (cap != nullptr) { delete [] cap; cap = nullptr; }
    if (stacks != nullptr) { delete [] stacks; stacks = nullptr; }
    total = 0; k = 0;
}

template <class T>
inline TMultiStack<T>::TMultiStack(int total_, int k_) {
    allocateMeta(total_, k_);
}

template <class T>
inline TMultiStack<T>::TMultiStack(const TMultiStack& other) {
    total = other.total; k = other.k;
    memory = new T*[total];
    for (int i = 0; i < total; ++i) {
        if (other.memory[i] == nullptr) memory[i] = nullptr;
        else memory[i] = new T(*(other.memory[i]));
    }
    start = new int[k]; cap = new int[k];
    stacks = new TStack<T>[k];
    for (int i = 0; i < k; ++i) { start[i] = other.start[i]; cap[i] = other.cap[i]; }
    updateStacksData();
}

template <class T>
inline TMultiStack<T>::TMultiStack(TMultiStack&& other) {
    memory = other.memory; total = other.total; k = other.k;
    start = other.start; cap = other.cap; stacks = other.stacks;
    other.memory = nullptr; other.total = 0; other.k = 0; other.start = nullptr; other.cap = nullptr; other.stacks = nullptr;
}

template <class T>
inline TMultiStack<T>::~TMultiStack() {
    freeMeta();
}

template <class T>
inline void TMultiStack<T>::updateStacksData() {
    for (int i = 0; i < k; ++i) {
        stacks[i].SetData(memory + start[i], cap[i]);
    }
}

template <class T>
inline void TMultiStack<T>::checkIndex(int idx) const {
    if (idx < 0 || idx >= k) throw -1;
}

template <class T>
inline TStack<T>& TMultiStack<T>::operator[](int idx) {
    checkIndex(idx);
    return stacks[idx];
}

template <class T>
inline bool TMultiStack<T>::tryShiftRight(int idx) {
    checkIndex(idx);
    int r = -1;
    for (int i = idx + 1; i < k; ++i) {
        bool foundFree = false;
        for (int p = 0; p < cap[i]; ++p) {
            if (memory[start[i] + p] == nullptr) { foundFree = true; break; }
        }
        if (foundFree) { r = i; break; }
    }
    if (r == -1) return false;
    for (int t = r; t >= idx + 1; --t) {
        for (int p = cap[t] - 1; p >= 0; --p) {
            int src = start[t] + p;
            int dst = start[t] + p + 1;
            memory[dst] = memory[src];
        }
        memory[start[t]] = nullptr;
        start[t] = start[t] + 1;
    }
    for (int t = idx; t <= r; ++t) stacks[t].SetData(memory + start[t], cap[t]);
    return true;
}

template <class T>
inline bool TMultiStack<T>::tryShiftLeft(int idx) {
    checkIndex(idx);
    int l = -1;
    for (int i = idx - 1; i >= 0; --i) {
        bool foundFree = false;
        for (int p = 0; p < cap[i]; ++p) {
            if (memory[start[i] + p] == nullptr) { foundFree = true; break; }
        }
        if (foundFree) { l = i; break; }
    }
    if (l == -1) return false;
    for (int t = l; t <= idx - 1; ++t) {
        for (int p = 0; p < cap[t]; ++p) {
            int src = start[t] + p;
            int dst = start[t] + p - 1;
            memory[dst] = memory[src];
        }
        memory[start[t] + cap[t] - 1] = nullptr;
        start[t] = start[t] - 1;
    }
    for (int t = l; t <= idx; ++t) stacks[t].SetData(memory + start[t], cap[t]);
    return true;
}

template <class T>
inline void TMultiStack<T>::FullRepack() {
    if (k <= 0) return;
    int* used = new int[k];
    int sumUsed = 0;
    for (int i = 0; i < k; ++i) {
        int count = 0;
        for (int p = 0; p < cap[i]; ++p) if (memory[start[i] + p] != nullptr) count++;
        used[i] = count;
        sumUsed += count;
    }
    if (sumUsed > total) { delete [] used; throw -1; }
    int* newCap = new int[k];
    for (int i = 0; i < k; ++i) newCap[i] = used[i];
    int freeSlots = total - sumUsed;
    int idx = 0;
    while (freeSlots > 0) {
        newCap[idx]++; freeSlots--; idx++; if (idx >= k) idx = 0;
    }
    T** newMem = new T*[total];
    for (int i = 0; i < total; ++i) newMem[i] = nullptr;
    int pos = 0;
    for (int s = 0; s < k; ++s) {
        int copied = 0;
        for (int p = 0; p < cap[s] && copied < used[s]; ++p) {
            if (memory[start[s] + p] != nullptr) {
                newMem[pos++] = memory[start[s] + p];
                copied++;
            }
        }
    }
    delete [] memory;
    memory = newMem;
    pos = 0;
    for (int i = 0; i < k; ++i) {
        start[i] = pos;
        cap[i] = newCap[i];
        pos += cap[i];
    }
    updateStacksData();
    delete [] used; delete [] newCap;
}

template <class T>
inline void TMultiStack<T>::Push(const T& value, int stackIndex) {
    checkIndex(stackIndex);
    try {
        stacks[stackIndex].Push(value);
        return;
    } catch (...) {
    }
    if (tryShiftRight(stackIndex)) {
        try { stacks[stackIndex].Push(value); return; } catch (...) {}
    }
    if (tryShiftLeft(stackIndex)) {
        try { stacks[stackIndex].Push(value); return; } catch (...) {}
    }
    FullRepack();
    try { stacks[stackIndex].Push(value); return; } catch (...) { throw -1; }
}

template <class T>
inline T TMultiStack<T>::Pop(int stackIndex) {
    checkIndex(stackIndex);
    return stacks[stackIndex].Pop();
}

template <class T>
inline bool TMultiStack<T>::IsEmpty(int stackIndex) const {
    if (stackIndex < 0 || stackIndex >= k) throw -1;
    for (int p = 0; p < cap[stackIndex]; ++p) if (memory[start[stackIndex] + p] != nullptr) return false;
    return true;
}

template <class T>
inline bool TMultiStack<T>::IsFull(int stackIndex) const {
    if (stackIndex < 0 || stackIndex >= k) throw -1;
    int count = 0;
    for (int p = 0; p < cap[stackIndex]; ++p) if (memory[start[stackIndex] + p] != nullptr) count++;
    return count >= cap[stackIndex];
}
