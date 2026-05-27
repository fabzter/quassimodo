#include <boost/python.hpp>
#include "widget.hpp"
using namespace boost::python;

// Wrapper enabling Python subclasses to override label().
struct WidgetWrap : Widget, wrapper<Widget> {
    std::string label() const override {
        if (override f = this->get_override("label")) return f();
        return Widget::label();
    }
    std::string default_label() const { return this->Widget::label(); }
};

BOOST_PYTHON_MODULE(Spike) {
    class_<WidgetWrap, boost::noncopyable>("Widget")
        .def("label", &Widget::label, &WidgetWrap::default_label)
        .def("value", &Widget::value)
        .def("setValue", &Widget::setValue);
}
