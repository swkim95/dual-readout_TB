#include "TBread.h"
#include "TBdetector.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(pydrcTB) {
  boost::python::class_<TBread>("TBread")
    .def("ntuplizeWaveform", &TBread::ntuplizeWaveform)
    .def("ntuplizeFastmode", &TBread::ntuplizeFastmode)
    .def("setMappingPath", &TBread::setMappingPath)
    .def("setPedestalPath", &TBread::setPedestalPath);

  boost::python::class_<TBcid>("TBcid")
    .def("mid", &TBcid::mid)
    .def("channel", &TBcid::channel);
}
