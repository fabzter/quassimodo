#pragma once
#include <string>

// Domain type stand-in (mirrors Reglas:: classes). Lives in ONE shared lib.
class Widget {
public:
    virtual ~Widget() = default;
    // C++ default; Python subclasses override this.
    virtual std::string label() const { return "cpp-widget"; }
    int value() const { return value_; }
    void setValue(int v) { value_ = v; }
private:
    int value_ = 0;
};
