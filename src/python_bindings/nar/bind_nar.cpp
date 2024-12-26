#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "algorithms/nar/mining_algorithms.h"
#include "algorithms/nar/nar.h"
#include "py_util/bind_primitive.h"
#include "algorithms/nar/value_range.h"

namespace {
namespace py = pybind11;
}  // namespace

namespace python_bindings {
void BindNar(py::module_& main_module) {
    using namespace algos;
    using namespace algos::des;
    using namespace model;

    auto nar_module = main_module.def_submodule("nar");

    py::class_<ValueRange>(nar_module, "ValueRange")
        .def_property_readonly("type", &ValueRange::GetTypeId);

    py::class_<StringValueRange, ValueRange>(nar_module, "StringValueRange")
        .def_readwrite("domain", &StringValueRange::domain);

    py::class_<NumericValueRange<Double>, ValueRange>(nar_module, "FloatValueRange")
        .def_readwrite("lower_bound", &NumericValueRange<Double>::lower_bound)
        .def_readwrite("upper_bound", &NumericValueRange<Double>::upper_bound);

    py::class_<NumericValueRange<Int>, ValueRange>(nar_module, "IntValueRange")
        .def_readwrite("lower_bound", &NumericValueRange<Int>::lower_bound)
        .def_readwrite("upper_bound", &NumericValueRange<Int>::upper_bound);

    py::class_<NARQualities>(nar_module, "NarQualities")
        .def("__str__", &NARQualities::ToString)
        .def_readonly("support", &NARQualities::support)
        .def_readonly("confidence", &NARQualities::confidence)
        .def_readonly("fitness", &NARQualities::fitness);

    py::class_<NAR>(nar_module, "NAR")
        .def("__str__", &NAR::ToString)
        .def_property_readonly("qualities", &NAR::GetQualities)
        .def_property_readonly("ante", &NAR::GetAnte)
        .def_property_readonly("cons", &NAR::GetCons);

    py::class_<NARAlgorithm, Algorithm>(nar_module, "NarAlgorithm")
        .def("get_nars", &NARAlgorithm::GetNARVector);

    auto algos_module = nar_module.def_submodule("algorithms");
    auto default_algorithm = detail::RegisterAlgorithm<DES, NARAlgorithm>(algos_module, "DES");
    algos_module.attr("Default") = default_algorithm;
}
}  // namespace python_bindings
