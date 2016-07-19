#ifndef LIBPARAVER_UIPARAVERTRACECONFIG_H
#define LIBPARAVER_UIPARAVERTRACECONFIG_H


#include <string>
#include <vector>
#include <iostream>

namespace libparaver { class ParaverTraceConfig; }
namespace libparaver { class ParaverEventType; }
namespace libparaver { class LibException; }
namespace libparaver { class ParaverEventValue; }

namespace libparaver {

/**
 * \brief Class interface to read the values defined in a Paraver Trace config file.
 */
class UIParaverTraceConfig {
  public:
    /**
     * \brief This is the constructor. The system parses and store internally the data defined in the Paraver Config File.
     * \param pcfFile refers to the path to the Paraver Config File (.pcf).
     */
    UIParaverTraceConfig(const std::string pcfFile);

    /**
     * \brief This is the destructor.
     */
    virtual ~UIParaverTraceConfig();

    /**
     * \brief This method returns the path to the Paraver Trace config file which this instance is working with.
     * \return The path to the Paraver Trace Config file (.pcf).
     */
    std::string getTraceConfigFile();

    /**
     * \brief This method returns the numeric key from a textual event type.
     * \param eventTypeValue This is the textual description of the event type.
     * \return The numeric key from the eventTypeValue.
     */
    int getEventType(const std::string eventTypeValue) const;

    /**
     * \brief This method returns the textual description for event type key.
     * \param eventTypeKey This is the numerical key of the event type.
     * \return The textual description from eventTypeValue.
     */
    std::string getEventType(const int eventTypeKey) const;

    /**
     * \brief This method returns the numeric key for an event value that belongs to an event type key.
     * \param eventTypeKey This is the numerical key of the event type.
     * \param eventValue This is the textual description of the event value.
     * \return The numeric key from an eventValue belonging to an eventTypeKey
     */
    int getEventValue(const int eventTypeKey, const std::string eventValue) const;

    /**
     * \brief This method returns the textual description for an event value key that belongs to an event type key.
     * \param eventTypeKey This is the numerical ID of the event type.
     * \param eventValueKey This is the numeric ID of the event value.
     * \return The textual description from the eventValue belonging to an eventTypeKey.
     */
    std::string getEventValue(const int eventTypeKey, const int eventValueKey) const;

    /**
     * \brief This method returns an integer vector with a list of event type keys.
     * \return An integer vector with the list of event type keys.
     */
    std::vector<unsigned int > getEventTypes() const;

    /**
     * \brief This method returns an integer vector with a list of event value keys from an event type key.
     * \return An integer vector with the list of event value keys.
     */
    std::vector<unsigned int > getEventValuesFromEventTypeKey(unsigned int eventTypeKey) const;


  protected:
    ParaverTraceConfig * traceConfig;


  public:
    friend std::ostream & operator<<(std::ostream & os, const UIParaverTraceConfig & ptraceConfig);

};

} // namespace libparaver
#endif
