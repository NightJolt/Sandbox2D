#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <utility>
#include <cstring>
#include <memory>

#include <iostream> // debug

#define INIT_SCREEN_SIZE sf::Vector2i(1920, 1440)
#define INIT_VIEW_SIZE sf::Vector2f(1920, 1440)
#define INIT_VIEW_ORIGIN Vector2f(0, 0)

#define CHUNK_SIZE 16
#define GRID_SIZE 8.f

typedef char int8;                     //                       -127 to 127
typedef short int16;                   //                    -32,768 to 32,767
typedef int int32;                     //             -2,147,483,648 to 2,147,483,647
typedef long long int64;               // -9,223,372,036,854,775,807 to 9,223,372,036,854,775,807
typedef unsigned char uint8;           //                          0 to 255
typedef unsigned short uint16;         //                          0 to 65,535
typedef unsigned int uint32;           //                          0 to 4,294,967,295
typedef unsigned long long uint64;     //                          0 to 18,446,744,073,709,551,615

struct UniqueKey {
    UniqueKey(void* p, std::string k) : ptr(p), key(std::move(k)) {}

    void* ptr;
    std::string key;

    bool operator <(const UniqueKey& u_k) const {
        return key < u_k.key;
    }
};

template <typename T>
std::string to_string_key(const sf::Vector2<T>& a) {
    return std::to_string(a.x) + ' ' + std::to_string(a.y);
}

template <typename T>
std::string to_string(const sf::Vector2<T>& a) {
    return "Vector2(" + std::to_string(a.x) + ", " + std::to_string(a.y) + ")";
}

template <typename T>
std::ostream& operator <<(std::ostream& out, const sf::Vector2<T>& a) {
    return out << to_string(a);
}

inline std::string to_string(const sf::Color& c) {
    return "Color(" + std::to_string(c.r) + ", " + std::to_string(c.g) + ", " + std::to_string(c.b) + ", " + std::to_string(c.a) + ")";
}

inline std::ostream& operator <<(std::ostream& out, const sf::Color& c) {
    return out << to_string(c);
}

template <typename T>
sf::Vector2<T> operator *(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return sf::Vector2<T>(a.x * b.x, a.y * b.y);
}

template <typename T>
void operator *=(sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    a = a * b;
}

template <typename T>
sf::Vector2<T> operator /(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return sf::Vector2<T>(a.x / b.x, a.y / b.y);
}

template <typename T>
void operator /=(sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    a = a / b;
}

template <typename T>
sf::Vector2<T> operator +(const sf::Vector2<T>& a, const T& b) {
    return sf::Vector2<T>(a.x + b, a.y + b);
}

template <typename T>
void operator +=(sf::Vector2<T>& a, const T& b) {
    a = a + b;
}

template <typename T>
sf::Vector2<T> operator -(const sf::Vector2<T>& a, const T& b) {
    return sf::Vector2<T>(a.x - b, a.y - b);
}

template <typename T>
void operator -=(sf::Vector2<T>& a, const T& b) {
    a = a - b;
}

template <typename T>
bool operator <(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return a.x < b.x && a.y < b.y;
}

template <typename T>
bool operator <=(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return a.x <= b.x && a.y <= b.y;
}

template <typename T>
bool operator >(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return b < a;
}

template <typename T>
bool operator >=(const sf::Vector2<T>& a, const sf::Vector2<T>& b) {
    return b <= a;
}

template <class T>
T** init_2d_ptr_arr(const unsigned int &x, const unsigned int &y) {
    T** arr = new T*[x];

    for (T** t = arr; t != arr + x; t++) {
        *t = new T[y];

        for (int i = 0; i != y; i++) {
            *(*t + i) = T();
        }
    }

    return arr;
}

template <class T>
void del_2d_ptr_arr(T** arr, const unsigned int& x) {
    for (T** t = arr; t != arr + x; t++) {
        delete[] *t;
    }

    delete[] arr;
}

template <class T>
void cpy_2d_ptr_arr(T** f, T** t, const unsigned int& x, const unsigned int &y) {
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < y; j++) {
            t[i][j] = f[i][j];
        }
    }
}

template <class T>
void move_2d_ptr_arr(T** f, T** t, const unsigned int& x, const unsigned int &y) {
    cpy_2d_ptr_arr(f, t, x, y);
    del_2d_ptr_arr(f, x);
}