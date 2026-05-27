#include <boost/python.hpp>
#include <iostream>
#include <string>
#include "widget.hpp"
using namespace boost::python;

int main(int argc, char** argv) {
    const std::string moddir = (argc > 1) ? argv[1] : ".";
    Py_Initialize();
    try {
        object sys = import("sys");
        sys.attr("path").attr("insert")(0, moddir);

        object main_mod = import("__main__");
        object ns = main_mod.attr("__dict__");

        // Import the SEPARATE Spike.so module so its type converters are registered
        // before we attempt to push a C++ Widget into Python.
        import("Spike");

        // 1) C++ -> Python: push a live C++ Widget into Python (uses converters
        //    registered by the SEPARATE Spike.so module).
        Widget cpp_w;
        cpp_w.setValue(42);
        ns["cpp_w"] = object(boost::python::ptr(&cpp_w));

        // 2) Run agent.py: imports Spike, subclasses Spike.Widget, reads cpp_w.
        exec_file("spikes/pyembed/agent.py", ns, ns);

        // 3) Python -> C++: pull the subclass instance back as a C++ Widget&
        //    (requires consistent RTTI between Spike.so and this binary via libwidget).
        Widget& py_w = extract<Widget&>(ns["made"]);

        std::string from_cpp_obj = extract<std::string>(ns["cpp_label"]);
        std::string from_py_obj  = py_w.label();
        int round_tripped_value  = extract<int>(ns["seen_value"]);

        std::cout << "cpp_w.label via python = " << from_cpp_obj << "\n";
        std::cout << "py subclass.label via C++ = " << from_py_obj << "\n";
        std::cout << "value seen in python = " << round_tripped_value << "\n";

        bool ok = (from_cpp_obj == "cpp-widget")
                && (from_py_obj == "python-widget")
                && (round_tripped_value == 42);
        std::cout << (ok ? "ROUNDTRIP OK\n" : "ROUNDTRIP FAIL\n");
        return ok ? 0 : 1;
    } catch (error_already_set&) {
        PyErr_Print();
        return 2;
    }
}
