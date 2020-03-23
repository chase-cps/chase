#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "Chase.hh"

namespace py = pybind11;

PYBIND11_MODULE(chasecorebnd, m) {
    m.doc() = "chase core wrapper";
}