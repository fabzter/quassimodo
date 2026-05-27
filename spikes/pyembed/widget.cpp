#include "widget.hpp"
// Out-of-line anchor so libwidget owns Widget's vtable/typeinfo (single RTTI source).
std::string widget_tu_anchor() { return "widget translation unit"; }
