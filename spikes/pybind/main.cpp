#include <pybind11/embed.h>
#include <pybind11/eval.h>

#include <cstdlib>
#include <iostream>
#include <string>

#include "domain.hpp"

namespace py = pybind11;

int main(int argc, char** argv) {
    // Allow override of the spike root for out-of-tree runs; defaults to
    // "spikes/pybind" relative to the current working directory (run from
    // the repo root, same as `consola` does for `lib/Reglas.so`).
    const std::string spike_dir = (argc > 1) ? argv[1] : "spikes/pybind";

    py::scoped_interpreter guard{};
    try {
        // 1) Make the spike directory importable.
        py::module_::import("sys").attr("path").attr("insert")(0, spike_dir);

        // 2) Register types BEFORE injecting C++ objects into the namespace.
        //    Lesson from spikes/pyembed (cerebrum, 2026-05-26): you MUST
        //    import the extension first or py::cast(&helper, ...) raises
        //    "no converter for C++ type ...".
        py::module_ ext = py::module_::import("PybindSpike");

        // 3) Construct a live Helper and inject it into __main__'s dict.
        pybindspike::Helper helper;
        py::module_ main_mod = py::module_::import("__main__");
        py::object  ns       = main_mod.attr("__dict__");
        ns["helper"] = py::cast(&helper, py::return_value_policy::reference);

        // 4) Exec the agent file in the same namespace, so it sees `helper`.
        py::eval_file(spike_dir + "/agent.py", ns);

        // 5) Pull the Python subclass back as a C++ pointer; dispatch
        //    virtual call into Python (mechanism 1).
        auto* p = ns["made"].cast<pybindspike::Player*>();
        pybindspike::Move m = p->nextMove();

        // 6) Gate assertions (spec §6).
        const bool move_ok =
            m.x == 7 && m.y == 9 && m.dir == pybindspike::Direction::E;

        // Opaque vector identity: Python erased index 1 and appended cs[0]
        // back. Size should still be 3 and the appended pointer must be
        // bit-identical to the first element pointer (identity, not copy).
        const bool opaque_ok =
            helper.cells_view.size() == 3
            && helper.cells_view.back() == helper.cells_view.front();

        // The stl.h list<Move> assertion lives inside agent.py; failure
        // there surfaces here as py::error_already_set.

        if (move_ok && opaque_ok) {
            std::cout << "PYBIND ROUNDTRIP OK" << std::endl;
            return 0;
        }
        std::cerr << "PYBIND ROUNDTRIP FAIL"
                  << " move_ok=" << move_ok
                  << " opaque_ok=" << opaque_ok
                  << " m=(" << m.x << "," << m.y
                  << ",dir=" << static_cast<int>(m.dir) << ")"
                  << " size=" << helper.cells_view.size()
                  << std::endl;
        return 1;
    } catch (const py::error_already_set& e) {
        std::cerr << "PYBIND ROUNDTRIP FAIL (python): " << e.what()
                  << std::endl;
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "PYBIND ROUNDTRIP FAIL (c++): " << e.what() << std::endl;
        return 3;
    }
}
