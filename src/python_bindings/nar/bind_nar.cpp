#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "algorithms/nar/mining_algorithms.h"
#include "algorithms/nar/nar.h"
#include "py_util/bind_primitive.h"

namespace {
namespace py = pybind11;
}  // namespace

namespace PYBIND11_NAMESPACE {
namespace detail {
    //FILL LATER
}  // namespace detail
}  // namespace PYBIND11_NAMESPACE

namespace python_bindings {
void BindNar(py::module_& main_module) {
    using namespace algos;
    using namespace algos::des;
    using model::NAR;

    auto nar_module = main_module.def_submodule("nar");
    py::class_<NAR>(nar_module, "NAR")
            .def("__str__", &NAR::ToString);
            .def("get_qualities", &NAR::GetQualities);

    py::class_<NARAlgorithm, Algorithm>(nar_module, "NarAlgorithm")
            .def("get_nars", &NARAlgorithm::GetNARVector);

    auto algos_module = nar_module.def_submodule("algorithms");
    auto default_algorithm = detail::RegisterAlgorithm<DES, NARAlgorithm>(algos_module, "DES");
    algos_module.attr("Default") = default_algorithm;
}
}  // namespace python_bindings
