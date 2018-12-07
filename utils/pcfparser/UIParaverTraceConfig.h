#ifndef _UIPARAVER_TRACE_CONFIG_H
#define _UIPARAVER_TRACE_CONFIG_H

#include <string>
#include <vector>
#include <map>

#include <boost/throw_exception.hpp>

namespace libparaver {
class ParaverTraceConfig;
}

namespace libparaver {
/**
 * \brief Class interface to read the values defined in a Paraver Trace config file.
 */
class UIParaverTraceConfig {
public:
    struct error : virtual std::exception, virtual boost::exception { };
    struct value_not_found : virtual error { };
    struct pcf_format_error : virtual error {
        // vector<"filename;line;column">
        std::vector<std::string> excps;
        pcf_format_error() {}
        pcf_format_error(std::vector<std::string> e) : excps(e) {}
        ~pcf_format_error() throw() {}
    };

    class Color {
    protected:
        int red;

        int green;

        int blue;
    public:
        Color(int red_, int green_, int blue_);

        virtual ~Color();

        inline int getRed() const;

        void setRed(int color);

        inline int getGreen() const;

        void setGreen(int color);

        inline int getBlue() const;

        void setBlue(int color);

        bool operator==(const Color & sc) const;

        bool operator!=(const Color & sc) const;

        virtual std::string toString() const;
    };

    class StateColor : public Color {
    public:
        StateColor(int red_, int green_, int blue_);
    };

    class GradientColor : public Color {
    public:
        GradientColor(int red_, int green_, int blue_);
    };

    class EventValue {
    protected:
        int key;

        std::string descr;

    public:
        EventValue(int key_, std::string descr_);

        inline int getKey() const;

        inline const std::string getDescription() const;

        virtual std::string toString() const;
    };

    class EventType {
    protected:
        int color;

        int key;

        std::string descr;

    public:
        EventType(int color_, int key_, std::string descr_);

        EventType(const EventType & eventType);

        virtual ~EventType();

        inline int getColor() const;

        inline int getKey() const;

        inline const std::string getDescription() const;

        virtual std::string toString() const;
        virtual std::string toStringWithoutValues() const;

        EventType & operator=(const EventType & eventType);
    };


    /**
     * \brief This is the constructor. The system parses and store internally the data defined in the Paraver Config File.
     * \return An instance of UIParaverTraceConfig
     */
    UIParaverTraceConfig();

    /**
     * \brief This is the destructor.
     */
    virtual ~UIParaverTraceConfig();

    /**
     * \brief Enables or diables the parsing output debug
     */
    void setDebug(bool value);

    /**
     * \brief Gets the parsing output debug value
     * \return Returns the debug value true/false
     */
    bool getDebug() const;

    /**
     * \brief Sets LEVEL in DEFAULT_OPTIONS context
     * \param The values can be:
     * CPU, PTASK, TASK, THREAD, NODE, WORKLOAD, SYSTEM
     */
    void setLevel(std::string value);

    /**
     * \brief Gets the LEVEL value in the PCF
     * \return The LEVEL value
     */
    const std::string getLevel() const;

    /**
     * \brief Set the UNITS value n the PCF
     * \param value can take next strings:
     * NANOSEC, MICROSEC, MILLISEC, SEC, HOUR
     */
    void setUnits(std::string value);

    /**
     * \brief Return the UNITS defined in the PCF file
     * \return A sting with the defined value
     */
    const std::string getUnits() const;

    /**
     * \brief Sets the LOOK_BACK value
     */
    void setLook_back(int value);

    /**
     * \brief Gets the LOOK_BACK value
     */
    int getLook_back() const;

    /**
     * \brief Sets the SPEED value
     */
    void setSpeed(int value);

    /**
     * \brief Gets the SPEED value
     */
    int getSpeed() const;

    /**
     * \brief Set the FLAG_ICONS value
     */
    void setFlag_icons(std::string value);

    /**
     * \brief Gets the FLAG_ICONS value
     * \param True/False
     */
    const std::string getFlag_icons() const;

    /**
     * \brief Sets the NUM_OF_STATE_COLORS value
     */
    void setNum_of_state_colors(int value);

    /**
     * \brief Gets the NUM_OF_STATE_COLORS value
     */
    int getNum_of_state_colors() const;

    /**
     * \brief Sets the YMAX_SCALE value
     */
    void setYmax_scale(int value);

    /**
     * \brief Gets the YMAX_SCALE value
     */
    int getYmax_scale() const;

    /**
     * \brief Sets the THREAD_FUNC value
     */
    void setThread_func(std::string value);

    /**
     * \brief Gets the THREAD_FUNC value
     */
    const std::string getThread_func() const;


    /**
     * \brief Dumps the PCF data into std::string format
     * \return The PCF data contained in the object
     */
    std::string toString() const;

    /**
     * \brief This parses the input and constructs an internal representation of the pcf file
     * \param std::string::const_iterator poiting to the first character.
     * \param std::string::const_iterator poiting to the last character.
     */
    bool parse(std::string::const_iterator begin, std::string::const_iterator end);

    /**
     * \brief Function to parse the input and construct an internal representation of the pcf file
     * \param std::istream object (file/network/mem/whatever...)
     * \brief A description of the representing istream: Example a filename.
     */
    bool parse(std::istream & input, const std::string & filename);

    /**
     * \brief Function to parse the input and construct an internal representation of the pcf file. This function skips the bad format parts of the file.
     * \param std::string object (filename)
     * \param bool resend Resend exceptions. It sends a vector of "file;line;column" indicating errors
     */
    bool parse(const std::string & filename, bool resend=false);

    /**
     * \brief Gets all the index keys of the STATES
     * \return A vector with the numeric indexes
     */
    std::vector<int> getStates() const;

    /**
     * \brief Gets the STATE description for a Key
     */
    std::string getState(int key) const;

    /**
     * \brief Gets all the index keys of the STATES_COLOR
     * \return A vector with the numeric indexes
     */
    std::vector<int> getStateColors() const;

    /**
     * \brief Gets a STATE_COLOR for a key
     */
    StateColor getStateColor(int key) const;

    /**
     * \brief Gets all the index keys of the GRADIENT_NAME
     * \return A vector with the numeric indexes
     */
    std::vector<int> getGradientNames() const;


    /**
     * \brief Gets the gradient name for a key
     */
    std::string getGradientName(int key) const;

    /**
     * \brief Gets all the index keys of the GRADIENT_COLOR
     * \return A vector with the numeric indexes
     */
    std::vector<int> getGradientColors() const;

    /**
     * \brief Gets the Gradient Color for a key
     */
    GradientColor getGradientColor(int key) const;

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
     * \brief This method returns the precision for event type key.
     * \param eventTypeKey This is the numerical key of the event type.
     * \return The precision expressed in number of decimals
     */
    int getEventTypePrecision( const int eventTypeKey ) const;

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

    void eraseEvent( unsigned int eventTypeKey );

    /**
     * \brief This method returns an integer vector with a list of event value keys from an event type key.
     * \return An integer vector with the list of event value keys.
     */
    std::vector<unsigned int > getEventValues(unsigned int eventTypeKey) const;

    std::vector< std::vector< unsigned int > > getGroupedEventTypes() const;


    void setEventValues( unsigned int eventTypeKey, std::map< unsigned int, std::string >& values );

protected:
    ParaverTraceConfig * traceConfig;

};

#include "UIParaverTraceConfig.ipp"
} // namespace libparaver

#endif
