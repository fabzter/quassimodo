#pragma once

#include <list>
#include <vector>

namespace pybindspike {

enum class Direction { N, E, S, W };

struct Move {
    int x = 0;
    int y = 0;
    Direction dir = Direction::N;

    Move() = default;
    Move(int x_, int y_, Direction d) : x(x_), y(y_), dir(d) {}
};

struct Cell {
    int x = 0;
    int y = 0;
    bool blocked = false;
};

class Player {
public:
    virtual ~Player();
    virtual Move nextMove() = 0;
};

class Helper {
public:
    Helper();

    // Returns the live mutable view by reference. Bound with
    // py::return_value_policy::reference_internal so its lifetime is tied
    // to the Helper.
    std::vector<Cell*>& cells();

    // Returns a copy. Crosses to Python via pybind11/stl.h (by-value list).
    std::list<Move> moves();

    // Public so the embedder can inspect the post-round-trip state without
    // re-crossing into Python. Not part of the binding surface.
    std::vector<Cell>   storage;
    std::vector<Cell*>  cells_view;
    std::list<Move>     moves_;
};

} // namespace pybindspike
