#pragma once

struct Coord {
    inline Coord(int x, int y, int z) : x(x), y(y), z(z) {};

    inline bool operator==(const Coord& other) const {
        return x == other.x && y == other.y && z == other.z;
    };

    int x, y, z;
};

template<>
struct std::hash<Coord> {
    inline size_t operator()(const Coord& c) const {
        return (size_t)c.x + c.y + c.z;
    }
};