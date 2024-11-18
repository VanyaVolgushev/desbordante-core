#include "bind_nar.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "algorithms/nar/nar.h"
#include "algorithms/nar/mining_algorithms.h"
#include "py_util/bind_primitive.h"

namespace {
namespace py = pybind11;
}  // namespace

namespace python_bindings {
void BindNar(py::module_& main_module) {
    using namespace algos;
    using model::NAR;

    auto nar_module = main_module.def_submodule("nar");
    py::class_<NAR>(nar_module, "NAR")
            .def("__str__", &NAR::ToString)
            .def_readonly("ante", &NAR::ante)
            .def_readonly("cons", &NAR::right)
            .def_readonly("confidence", &NAR::confidence);
            .def_readonly("support", &NAR::confidence);
            .def_readonly("fitness", &NAR::confidence);

    py::class_<ArIDs>(ar_module, "ArIDs")
            .def_readonly("left", &ArIDs::left)
            .def_readonly("right", &ArIDs::right)
            .def_readonly("confidence", &ArIDs::confidence);

    py::class_<ARAlgorithm, Algorithm>(ar_module, "ArAlgorithm")
            .def("get_ars", &ARAlgorithm::GetArStringsList, py::return_value_policy::move)
            .def("get_itemnames", &ARAlgorithm::GetItemNamesVector)
            .def("get_ar_ids", &ARAlgorithm::GetArIDsList);

    auto algos_module = ar_module.def_submodule("algorithms");
    auto default_algorithm =
            detail::RegisterAlgorithm<Apriori, ARAlgorithm>(algos_module, "Apriori");
    algos_module.attr("Default") = default_algorithm;

    // Perhaps in the future there will be a need for:
    // default_algorithm.def("get_frequent_list", &Apriori::GetFrequentList);
}
}  // namespace python_bindings
