#include "TBread.h"
#include "TBdetector.h"
#include "TButility.h"
#include "TBmonit.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(pydrcTB) {
  boost::python::class_<TBread>("TBread")
    .def("ntuplizeWaveform", &TBread::ntuplizeWaveform)
    .def("ntuplizeFastmode", &TBread::ntuplizeFastmode)
    .def("setMappingPath", &TBread::setMappingPath)
    .def("setPedestalPath", &TBread::setPedestalPath);

  boost::python::class_<TBcid>("TBcid", boost::python::init<int, int>())
    .def("mid", &TBcid::mid)
    .def("channel", &TBcid::channel);

  boost::python::class_<TBdetector>("TBdetector")
    .def("isSiPM", &TBdetector::isSiPM)
    .def("isModule", &TBdetector::isModule)
    .def("isNull", &TBdetector::isNull)
    .def("module", &TBdetector::module)
    .def("tower", &TBdetector::tower)
    .def("isCeren", &TBdetector::isCeren)
    .def("plate", &TBdetector::plate)
    .def("column", &TBdetector::column);

  boost::python::class_<TButility>("TButility")
    .def("loading", &TButility::loading)
    .def("loadped", &TButility::loadped)
    .def("find", &TButility::find)
    .def("retrievePed", &TButility::retrievePed);

  boost::python::class_<TBmonit>("TBmonit")
    .def("setFastmodeFiles", &TBmonit::setFastmodeFiles)
    .def("setWaveformFiles", &TBmonit::setWaveformFiles)
    .def("setPedestalPath", &TBmonit::setPedestalPath)
    .def("setMappingPath", &TBmonit::setMappingPath)
    .def("SetADCmax", &TBmonit::SetADCmax)
    .def("SetOutputName", &TBmonit::SetOutputName)
    .def("MonitPlots", &TBmonit::MonitPlots);
}
