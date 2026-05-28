#include "domain.hpp"

namespace pybindspike {

Player::~Player() = default;

Helper::Helper() {
    storage.reserve(3);
    storage.push_back(Cell{0, 0, false});
    storage.push_back(Cell{1, 0, false});
    storage.push_back(Cell{2, 0, true});

    cells_view.reserve(3);
    for (auto& c : storage) {
        cells_view.push_back(&c);
    }

    moves_.push_back(Move{1, 2, Direction::N});
    moves_.push_back(Move{3, 4, Direction::E});
}

std::vector<Cell*>& Helper::cells() { return cells_view; }
std::list<Move>     Helper::moves() { return moves_; }

} // namespace pybindspike
