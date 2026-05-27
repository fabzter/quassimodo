import Spike

# Read the C++-created object that was injected into this namespace.
cpp_label = cpp_w.label()
seen_value = cpp_w.value()

class PyWidget(Spike.Widget):
    def label(self):
        return "python-widget"

made = PyWidget()
