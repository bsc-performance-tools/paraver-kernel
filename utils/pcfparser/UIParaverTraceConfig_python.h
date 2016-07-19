#ifndef _UIPARAVER_TRACE_CONFIG_PYTHON_H
#define _UIPARAVER_TRACE_CONFIG_PYTHON_H

#include "UIParaverTraceConfig.h"
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

// Overladed methods
bool (libparaver::UIParaverTraceConfig::*parse1)(const std::string &, bool) = &libparaver::UIParaverTraceConfig::parse;
int (libparaver::UIParaverTraceConfig::*getEventType_int)(const std::string) const = &libparaver::UIParaverTraceConfig::getEventType;
std::string (libparaver::UIParaverTraceConfig::*getEventType_str)(const int) const = &libparaver::UIParaverTraceConfig::getEventType;

int (libparaver::UIParaverTraceConfig::*getEventValue_int)(const int eventTypeKey, const std::string) const = &libparaver::UIParaverTraceConfig::getEventValue;
std::string (libparaver::UIParaverTraceConfig::*getEventValue_str)(const int eventTypeKey, const int eventValueKey) const = &libparaver::UIParaverTraceConfig::getEventValue;

BOOST_PYTHON_MODULE(pcfparser)
{
    using namespace boost::python;

    class_<std::vector<unsigned int > >("std::vector<unsigned int >")
        .def(vector_indexing_suite<std::vector<unsigned int > >());

    class_<libparaver::UIParaverTraceConfig::value_not_found>("value_not_found");
    class_<libparaver::UIParaverTraceConfig::pcf_format_error>("pcf_format_error");
        

    class_<libparaver::UIParaverTraceConfig>("UIParaverTraceConfig")
        .def("getDebug", &libparaver::UIParaverTraceConfig::getDebug)
        .def("setDebug", &libparaver::UIParaverTraceConfig::getDebug)
        .def("getEventType_int", getEventType_int)
        .def("getEventType_str", getEventType_str)
        .def("getEventTypes", &libparaver::UIParaverTraceConfig::getEventTypes)
        .def("getEventValues", &libparaver::UIParaverTraceConfig::getEventValues)
        .def("getEventValue_int", getEventValue_int)
        .def("getEventValue_str", getEventValue_str)
        .def("parse", parse1);
}

#endif
