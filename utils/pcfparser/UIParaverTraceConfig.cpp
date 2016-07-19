
#include "UIParaverTraceConfig.h"
#include "ParaverTraceConfig.h"

namespace libparaver {

/**
 * \brief This is the constructor. The system parses and store internally the data defined in the Paraver Config File.
 * \return An instance of UIParaverTraceConfig
 */
UIParaverTraceConfig::UIParaverTraceConfig() {
    traceConfig = new ParaverTraceConfig();
}

/**
 * \brief This is the destructor.
 */
UIParaverTraceConfig::~UIParaverTraceConfig() {
    delete traceConfig;
}


/**
 * \brief Enables or diables the parsing output debug
 */
void UIParaverTraceConfig::setDebug(bool value) {
    traceConfig->setDebug(value);
}

/**
 * \brief Gets the parsing output debug value
 * \return Returns the debug value true/false
 */
bool UIParaverTraceConfig::getDebug() const {
    return traceConfig->getDebug();
}

/**
 * \brief Sets LEVEL in DEFAULT_OPTIONS context
 * \param The values can be:
 * CPU, PTASK, TASK, THREAD, NODE, WORKLOAD, SYSTEM
 */
void UIParaverTraceConfig::setLevel(std::string value) {
    traceConfig->setLevel(value);
}

/**
 * \brief Gets the LEVEL value in the PCF
 * \return The LEVEL value
 */
const std::string UIParaverTraceConfig::getLevel() const {
    return traceConfig->getLevel();
}

/**
 * \brief Set the UNITS value n the PCF
 * \param value can take next strings:
 * NANOSEC, MICROSEC, MILLISEC, SEC, HOUR
 */
void UIParaverTraceConfig::setUnits(std::string value) {
    traceConfig->setUnits(value);
}

/**
 * \brief Return the UNITS defined in the PCF file
 * \return A sting with the defined value
 */
const std::string UIParaverTraceConfig::getUnits() const {
    return traceConfig->getUnits();
}

/**
 * \brief Sets the LOOK_BACK value
 */
void UIParaverTraceConfig::setLook_back(int value) {
    traceConfig->setLook_back(value);
}

/**
 * \brief Gets the LOOK_BACK value
 */
int UIParaverTraceConfig::getLook_back() const {
    return traceConfig->getLook_back();
}

/**
 * \brief Sets the SPEED value
 */
void UIParaverTraceConfig::setSpeed(int value) {
    traceConfig->setSpeed(value);
}

/**
 * \brief Gets the SPEED value
 */
int UIParaverTraceConfig::getSpeed() const {
    return traceConfig->getSpeed();
}

/**
 * \brief Set the FLAG_ICONS value
 */
void UIParaverTraceConfig::setFlag_icons(std::string value) {
    traceConfig->setFlag_icons(value);
}

/**
 * \brief Gets the FLAG_ICONS value
 * \param True/False
 */
const std::string UIParaverTraceConfig::getFlag_icons() const {
    return traceConfig->getFlag_icons();
}

/**
 * \brief Sets the NUM_OF_STATE_COLORS value
 */
void UIParaverTraceConfig::setNum_of_state_colors(int value) {
    traceConfig->setNum_of_state_colors(value);
}

/**
 * \brief Gets the NUM_OF_STATE_COLORS value
 */
int UIParaverTraceConfig::getNum_of_state_colors() const {
    return traceConfig->getNum_of_state_colors();
}

/**
 * \brief Sets the YMAX_SCALE value
 */
void UIParaverTraceConfig::setYmax_scale(int value) {
    traceConfig->setYmax_scale(value);
}

/**
 * \brief Gets the YMAX_SCALE value
 */
int UIParaverTraceConfig::getYmax_scale() const {
    return traceConfig->getYmax_scale();
}

/**
 * \brief Sets the THREAD_FUNC value
 */
void UIParaverTraceConfig::setThread_func(std::string value) {
    traceConfig->setThread_func(value);
}


/**
 * \brief Gets the THREAD_FUNC value
 */
const std::string UIParaverTraceConfig::getThread_func() const {
    return traceConfig->getThread_func();
}

/**
 * \brief This parses the input and constructs an internal representation of the pcf file
 * \param std::string::const_iterator poiting to the first character.
 * \param std::string::const_iterator poiting to the last character.
 */
bool UIParaverTraceConfig::parse(std::string::const_iterator begin, std::string::const_iterator end) {
    return traceConfig->parse(begin, end);
}

/**
 * \brief Function to parse the input and construct an internal representation of the pcf file
 * \param std::istream object (file/network/mem/whatever...)
 * \brief A description of the representing istream: Example a filename.
 */
bool UIParaverTraceConfig::parse(std::istream & input, const std::string & filename) {
    return traceConfig->parse(input, filename);
}

 /**
 * \brief Function to parse the input and construct an internal representation of the pcf file. This function skips the bad format parts of the file.
  * \param std::string object (filename)
  * \param bool resend Resend exceptions. It sends a vector of "file;line;column" indicating errors
  */
bool UIParaverTraceConfig::parse(const std::string & filename, bool resend) {
    return traceConfig->parse(filename, resend);
}

/**
 * \brief Dumps the PCF data into std::string format
 * \return The PCF data contained in the object
 */
std::string UIParaverTraceConfig::toString() const {
    return traceConfig->toString();
}

/**
 * \brief Gets all the index keys of the STATES
 * \return A vector with the numeric indexes
 */
std::vector<int> UIParaverTraceConfig::getStates() const {
    return traceConfig->getStates();
}

/**
 * \brief Gets the STATE description for a Key
 */
std::string UIParaverTraceConfig::getState(int key) const {
    return traceConfig->getState(key);
}

/**
 * \brief Gets all the index keys of the STATES_COLOR
 * \return A vector with the numeric indexes
 */
std::vector<int> UIParaverTraceConfig::getStateColors() const {
    return traceConfig->getStateColors();
}


/**
 * \brief Gets a STATE_COLOR for a key
 */
UIParaverTraceConfig::StateColor UIParaverTraceConfig::getStateColor(int key) const {
    return traceConfig->getStateColor(key);
}

/**
 * \brief Gets all the index keys of the GRADIENT_NAME
 * \return A vector with the numeric indexes
 */
std::vector<int> UIParaverTraceConfig::getGradientNames() const {
    return traceConfig->getGradientNames();
}

/**
 * \brief Gets the gradient name for a key
 */
std::string UIParaverTraceConfig::getGradientName(int key) const {
    return traceConfig->getGradientName(key);
}

/**
 * \brief Gets all the index keys of the GRADIENT_COLOR
 * \return A vector with the numeric indexes
 */
std::vector<int> UIParaverTraceConfig::getGradientColors() const {
    return traceConfig->getGradientColors();
}

/**
 * \brief Gets the Gradient Color for a key
 */
UIParaverTraceConfig::GradientColor UIParaverTraceConfig::getGradientColor(int key) const {
    return traceConfig->getGradientColor(key);
}

/**
 * \brief This method returns the numeric key from a textual event type.
 * \param eventTypeValue This is the textual description of the event type.
 * \return The numeric key from the eventTypeValue.
 */
int UIParaverTraceConfig::getEventType(const std::string eventTypeValue) const {
    return traceConfig->getEventType(eventTypeValue);
}

/**
 * \brief This method returns the textual description for event type key.
 * \param eventTypeKey This is the numerical key of the event type.
 * \return The textual description from eventTypeValue.
 */
std::string UIParaverTraceConfig::getEventType(const int eventTypeKey) const {
    return traceConfig->getEventType(eventTypeKey);
}

/**
 * \brief This method returns the numeric key for an event value that belongs to an event type key.
 * \param eventTypeKey This is the numerical key of the event type.
 * \param eventValue This is the textual description of the event value.
 * \return The numeric key from an eventValue belonging to an eventTypeKey
 */
int UIParaverTraceConfig::getEventValue(const int eventTypeKey, const std::string eventValue) const {
    return traceConfig->getEventValue(eventTypeKey, eventValue);
}

/**
 * \brief This method returns the textual description for an event value key that belongs to an event type key.
 * \param eventTypeKey This is the numerical ID of the event type.
 * \param eventValueKey This is the numeric ID of the event value.
 * \return The textual description from the eventValue belonging to an eventTypeKey.
 */
std::string UIParaverTraceConfig::getEventValue(const int eventTypeKey, const int eventValueKey) const {
    return traceConfig->getEventValue(eventTypeKey, eventValueKey);
}

/**
 * \brief This method returns an integer vector with a list of event type keys.
 * \return An integer vector with the list of event type keys.
 */
std::vector<unsigned int > UIParaverTraceConfig::getEventTypes() const {
    return traceConfig->getEventTypes();
}

void UIParaverTraceConfig::eraseEvent( unsigned int eventTypeKey )
{
  traceConfig->eraseEvent( eventTypeKey );
}

/**
 * \brief This method returns an integer vector with a list of event value keys from an event type key.
 * \return An integer vector with the list of event value keys.
 */
std::vector<unsigned int > UIParaverTraceConfig::getEventValues(unsigned int eventTypeKey) const {
    return traceConfig->getEventValues(eventTypeKey);
}

void UIParaverTraceConfig::setEventValues( unsigned int eventTypeKey, std::map< unsigned int, std::string >& values )
{
  traceConfig->setEventValues( eventTypeKey, values );
}


std::vector< std::vector< unsigned int > > UIParaverTraceConfig::getGroupedEventTypes() const
{
  return traceConfig->getGroupedEventTypes();
}


std::ostream & operator<<(std::ostream & os, const UIParaverTraceConfig & ptraceConfig) {
    return os<<ptraceConfig.toString();
}



/* Color class */

UIParaverTraceConfig::Color::Color(int red_, int green_, int blue_) {
    red = red_;
    green = green_;
    blue = blue_;
}

UIParaverTraceConfig::Color::~Color() {
}

std::string UIParaverTraceConfig::Color::toString() const {
    return "";
}

void UIParaverTraceConfig::Color::setRed(int color){
	red = color;
}

void UIParaverTraceConfig::Color::setGreen(int color){
	green = color;
}

void UIParaverTraceConfig::Color::setBlue(int color){
	blue = color;
}

bool UIParaverTraceConfig::Color::operator==(const Color & sc) const{
	return green == sc.getGreen() &&
		red == sc.getRed() &&
		blue == sc.getBlue();
}

bool UIParaverTraceConfig::Color::operator!=(const Color & sc) const{
	return green != sc.getGreen() ||
		red != sc.getRed() ||
		blue != sc.getBlue();
}


/* StateColor class */

UIParaverTraceConfig::StateColor::StateColor(int red_, int green_, int blue_):
        Color(red_, green_, blue_) {
}

/* GradientColor class */

UIParaverTraceConfig::GradientColor::GradientColor(int red_, int green_, int blue_):
        Color(red_, green_, blue_) {
}

/* EventType class */

UIParaverTraceConfig::EventType::EventType(int color_, int key_, std::string descr_) {
    color = color_;
    key = key_;
    descr = descr_;
}

UIParaverTraceConfig::EventType::EventType(const EventType & eventType) {
    if (this != &eventType) {
        *this = eventType;
    }
}

UIParaverTraceConfig::EventType & UIParaverTraceConfig::EventType::operator=(const UIParaverTraceConfig::EventType & eventType) {
    color = eventType.getColor();
    key = eventType.getKey();
    descr = eventType.getDescription();
    return *this;
}

UIParaverTraceConfig::EventType::~EventType() {
}

std::string UIParaverTraceConfig::EventType::toString() const {
    return "";
}

std::string UIParaverTraceConfig::EventType::toStringWithoutValues() const {
    return "";
}


/* EventValues class */

UIParaverTraceConfig::EventValue::EventValue(int key_, std::string descr_) {
    key = key_;
    descr = descr_;
}

std::string UIParaverTraceConfig::EventValue::toString() const {
    return "";
}

} // namespace libparaver

