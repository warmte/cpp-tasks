#pragma once

#include "counted.h"

struct my_vector {
    typedef counted value_type;

    size_t size() const noexcept {
        return s;
    }
    size_t capacity() const noexcept {
        return cap;
    }

    my_vector() noexcept : data((value_type*) operator new (0)), s(0), cap(0) {};

    my_vector(my_vector const& other) {
        cap = other.capacity();
        s = other.size();
        data = (value_type*) operator new (sizeof(value_type) * cap);
        for (size_t i = 0; i < s; ++i) {
            new(data + i)value_type(other[i]);
        }
    }
    my_vector& operator = (my_vector const& other) {
        my_vector tmp(other);
        swap(*this, tmp);
        return *this;
    }
    value_type& operator[](size_t position) {
        return data[position];
    }
    value_type const& operator[](size_t position) const {
        return data[position];
    }
    bool empty() const noexcept {
        return s > 0;
    };

    value_type const& back() const noexcept {
        return data[s - 1];
    };

    value_type back() noexcept {
        return data[0];
    };

    value_type const& front() const noexcept {
        return data[0];
    };

    value_type front() noexcept {
        return data[s - 1];
    };

    void update_data() {
        auto * ndata = (value_type*) operator new (sizeof(value_type) * cap);
        for (size_t i = 0; i < s; ++i) {
            new(ndata + i)value_type(data[i]);
        }
        swap(ndata, data);
        delete[] ndata;
    }

    void ensure_capacity() {
        if (cap < s + 1) {
            cap = (cap == 0 ? 1 : cap * 2);
            update_data();
        }
    }


    void push_back(value_type const& value) {
        insert(s, value);
    }

    void pop_back() noexcept {
        data[s--].~value_type();
    }
    void insert(size_t position, value_type const& value) {
        ensure_capacity();
        for (size_t i = s; i > position; --i) {
            data[i] = data[i - 1];
        }
        data[position] = value;
        s++;
    }
    void erase(size_t position) {
        for (size_t i = position; i < s - 1; ++i) {
            data[i] = data[i + 1];
        }
        pop_back();
    }
    void erase(size_t first, size_t last) {
        for (size_t to = first, from = last; from < s; ++to, ++from) {
            data[to] = data[from];
        }
        for (int i = last - first; i > 0; --i) {
            pop_back();
        }
    }

    void reserve(size_t n) {
        if (n * 2 <= cap) {
            return;
        }
        cap = n * 2;
        update_data();
    }

    void shrink_to_fit() {
        cap = s;
        update_data();
    }

    friend void swap(my_vector &a, my_vector &b) noexcept;

    ~my_vector() {
        delete[] data;
    }

private:
    value_type *data;
    size_t s;
    size_t cap;
};

void swap(my_vector &a, my_vector &b) noexcept {
    std::swap(a.data, b.data);
    std::swap(a.cap, b.cap);
    std::swap(a.s, b.s);
}

