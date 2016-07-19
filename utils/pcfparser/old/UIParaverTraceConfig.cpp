
#include "UIParaverTraceConfig.h"
#include "ParaverTraceConfig.h"
#include "ParaverEventType.h"
#include "LibException.h"
#include "ParaverEventValue.h"
#include <sstream>

using namespace std;

namespace libparaver {

/**
 * \brief This is the constructor. The system parses and store internally the data defined in the Paraver Config File.
 * \param pcfFile refers to the path to the Paraver Config File (.pcf).
 */
UIParaverTraceConfig::UIParaverTraceConfig(const string pcfFile) {
  traceConfig = new ParaverTraceConfig(pcfFile);
}

/**
 * \brief This is the destructor.
 */
UIParaverTraceConfig::~UIParaverTraceConfig() {
  delete traceConfig;
}

/**
 * \brief This method returns the path to the Paraver Trace config file which this instance is working with.
 * \return The path to the Paraver Trace Config file (.pcf).
 */
string UIParaverTraceConfig::getTraceConfigFile() {
  return traceConfig->get_pcfFile();
}

/**
 * \brief This method returns the numeric key from a textual event type.
 * \param eventTypeValue This is the textual description of the event type.
 * \return The numeric key from the eventTypeValue.
 */
int UIParaverTraceConfig::getEventType(const string eventTypeValue) const {
  ParaverEventType * eventType = traceConfig->getEventType(eventTypeValue);
  return eventType->get_key();
}

/**
 * \brief This method returns the textual description for event type key.
 * \param eventTypeKey This is the numerical key of the event type.
 * \return The textual description from eventTypeValue.
 */
string UIParaverTraceConfig::getEventType(const int eventTypeKey) const {
  ParaverEventType * eventType = traceConfig->getEventType(eventTypeKey);
  return eventType->get_description();
}

/**
 * \brief This method returns the numeric key for an event value that belongs to an event type key.
 * \param eventTypeKey This is the numerical key of the event type.
 * \param eventValue This is the textual description of the event value.
 * \return The numeric key from an eventValue belonging to an eventTypeKey
 */
int UIParaverTraceConfig::getEventValue(const int eventTypeKey, const string eventValue) const {
  stringstream ss;
  ParaverEventType * eventType = traceConfig->getEventType(eventTypeKey);
  if(eventType == 0) {
    ss<<eventTypeKey<<" not found in "<<traceConfig->get_pcfFile()<<" file";
    throw LibException(__FILE__, __LINE__, ss.str());
  }
  return eventType->get_eventKeyOfValue(eventValue);
}

/**
 * \brief This method returns the textual description for an event value key that belongs to an event type key.
 * \param eventTypeKey This is the numerical ID of the event type.
 * \param eventValueKey This is the numeric ID of the event value.
 * \return The textual description from the eventValue belonging to an eventTypeKey.
 */
string UIParaverTraceConfig::getEventValue(const int eventTypeKey, const int eventValueKey) const {
  stringstream ss;
  ParaverEventType * eventType = traceConfig->getEventType(eventTypeKey);
  if(eventType == 0) {
    ss<<eventTypeKey<<" not found in "<<traceConfig->get_pcfFile()<<" file";
    throw LibException(__FILE__, __LINE__, ss.str());
  }
  return eventType->get_eventValueOfKey(eventValueKey);
}

/**
 * \brief This method returns an integer vector with a list of event type keys.
 * \return An integer vector with the list of event type keys.
 */
vector<unsigned int > UIParaverTraceConfig::getEventTypes() const {
  vector<unsigned> i_eventTypes;
  vector<ParaverEventType *> v_eventTypes = traceConfig->get_eventTypes();
  vector<ParaverEventType *>::const_iterator itEventTypes = v_eventTypes.begin();
  vector<ParaverEventType *>::const_iterator endEventTypes = v_eventTypes.end();
  while(itEventTypes != endEventTypes){
    i_eventTypes.push_back((*itEventTypes)->get_key());
    itEventTypes++;
  }
  return i_eventTypes;
}

/**
 * \brief This method returns an integer vector with a list of event value keys from an event type key.
 * \return An integer vector with the list of event value keys.
 */
vector<unsigned int > UIParaverTraceConfig::getEventValuesFromEventTypeKey(unsigned int eventTypeKey) const {
  vector<unsigned> i_eventValues;
  ParaverEventType * eventType = traceConfig->getEventType(eventTypeKey);
  if(eventType == 0){
      stringstream ss;
      ss<<"TypeEvent "<<eventTypeKey<<" not found!";
      throw LibException(__FILE__, __LINE__, ss.str());
  }else{
    vector<ParaverEventValue *> eventValues = eventType->get_eventValues();
    vector<ParaverEventValue *>::const_iterator itValues = eventValues.begin();
    vector<ParaverEventValue *>::const_iterator endValues = eventValues.end();
    while(itValues != endValues){
      i_eventValues.push_back((*itValues)->get_key());
      itValues++;
    }
  }
  return i_eventValues;
}

ostream & operator<<(ostream & os, const UIParaverTraceConfig & ptraceConfig) {
  return  os<<*ptraceConfig.traceConfig;
}


} // namespace libparaver
