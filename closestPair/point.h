#pragma once
#include <functional>

struct Point {
    long x,y;

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    };
};

// return the square of the distance between points a and b
long long distSquared(const Point& a, const Point& b);

// compare by x as primary key (using y as secondary key)
bool compareByX(const Point& a, const Point& b);

// compare by y as primary key (using x as secondary key)
bool compareByY(const Point& a, const Point& b);

namespace std {
    template<>
    struct hash<Point> {
        std::size_t operator()(const Point& p) const noexcept {
            // Get hashes of individual members
            std::size_t h1 = std::hash<int>{}(p.x);
            std::size_t h2 = std::hash<int>{}(p.y);

            // Combine them using a common technique (e.g., XOR and bit shift)
            return h1 ^ (h2 << 1); 
        }
    };
}

