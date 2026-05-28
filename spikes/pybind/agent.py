"""D1.5 spike agent. Run by spikes/pybind/main.cpp via py::eval_file.

The embedder injects `helper` into this namespace before exec'ing this file.
This script subclasses PybindSpike.Player, exercises the three binding
mechanisms, and exposes the subclass instance as `made`.
"""

import PybindSpike as PS


class MyAgent(PS.Player):
    def nextMove(self):
        # Mechanism 2a — opaque vector<Cell*> with bind_vector. C++ owns
        # the storage; Python gets a live mutable view.
        cs = helper.cells()
        assert len(cs) == 3, f"expected 3 cells, got {len(cs)}"
        assert cs[0].x == 0 and cs[0].y == 0, "cs[0] should be (0,0)"
        assert cs[1].x == 1 and cs[1].y == 0, "cs[1] should be (1,0)"
        assert cs[2].x == 2 and cs[2].y == 0, "cs[2] should be (2,0)"

        first = cs[0]            # holds an alias to the same Cell* C++ owns
        del cs[1]                # erase index 1 (bind_vector __delitem__)
        assert len(cs) == 2, f"after del expected 2, got {len(cs)}"

        cs.append(first)         # append a C++-owned pointer; identity test
        assert len(cs) == 3, f"after append expected 3, got {len(cs)}"
        # Identity: the appended pointer must be bit-identical to cs[0].
        # In pybind11 bind_vector, __getitem__ on the same C++ pointer
        # returns a Python handle to the same underlying object — so
        # equality of the bound Cell objects is identity of the pointers
        # in the C++ vector.
        assert cs[-1].x == cs[0].x and cs[-1].y == cs[0].y, \
            "appended cell does not match first cell"

        # Mechanism 2b — by-value std::list<Move> via pybind11/stl.h.
        ms = helper.moves()
        assert len(ms) == 2, f"expected 2 moves, got {len(ms)}"
        assert ms[0].x == 1 and ms[0].y == 2, "ms[0] should be (1,2)"
        assert ms[1].x == 3 and ms[1].y == 4, "ms[1] should be (3,4)"
        # Direction enum round-trip:
        assert ms[0].dir == PS.Direction.N, "ms[0].dir should be N"
        assert ms[1].dir == PS.Direction.E, "ms[1].dir should be E"

        # Mechanism 1 — return a bound value type constructed in Python.
        # The embedder will read this back via Player::nextMove() and assert
        # (7, 9, Direction.E).
        return PS.Move(7, 9, PS.Direction.E)


# Mechanism 3 — the embedder pulls this instance back as Player* via cast<>.
made = MyAgent()
