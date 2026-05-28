#include <pybind11/pybind11.h>
#include <pybind11/stl.h>        // by-value crossing for std::list<Move>
#include <pybind11/stl_bind.h>   // py::bind_vector

#include "domain.hpp"

namespace py = pybind11;
using namespace pybindspike;

// Must be visible to every TU that references std::vector<Cell*> across the
// binding boundary. For this spike, only bindings.cpp does so; domain.cpp
// only touches it as a plain field.
PYBIND11_MAKE_OPAQUE(std::vector<Cell*>);

class PyPlayer : public Player {
public:
    using Player::Player;
    Move nextMove() override {
        PYBIND11_OVERRIDE_PURE(Move, Player, nextMove);
    }
};

PYBIND11_MODULE(PybindSpike, m) {
    py::enum_<Direction>(m, "Direction")
        .value("N", Direction::N)
        .value("E", Direction::E)
        .value("S", Direction::S)
        .value("W", Direction::W);

    py::class_<Move>(m, "Move")
        .def(py::init<>())
        .def(py::init<int, int, Direction>())
        .def_readwrite("x",   &Move::x)
        .def_readwrite("y",   &Move::y)
        .def_readwrite("dir", &Move::dir);

    py::class_<Cell>(m, "Cell")
        .def_readwrite("x",       &Cell::x)
        .def_readwrite("y",       &Cell::y)
        .def_readwrite("blocked", &Cell::blocked);

    // Opaque mutable view of std::vector<Cell*>. py::bind_vector exposes
    // the usual sequence protocol: __len__, __getitem__, __setitem__,
    // __delitem__, append, extend, etc. — verified at runtime by Task 5.
    py::bind_vector<std::vector<Cell*>>(m, "CellPtrVector");

    py::class_<Player, PyPlayer>(m, "Player")
        .def(py::init<>())
        .def("nextMove", &Player::nextMove);

    py::class_<Helper>(m, "Helper")
        .def(py::init<>())
        .def("cells", &Helper::cells, py::return_value_policy::reference_internal)
        .def("moves", &Helper::moves);  // returns std::list<Move> by value via stl.h
}
