#include "TBread.h"
#include "TBevt.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(pydrcTB) {
  boost::python::class_<TBread>("TBread")
    .def("ntuplizeWaveform", &TBread::ntuplizeWaveform)
    .def("ntuplizeFastmode", &TBread::ntuplizeFastmode)
    .def("setMappingPath", &TBread::setMappingPath)
    .def("setPedestalPath", &TBread::setPedestalPath);
}
