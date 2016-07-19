#include "ParaverTraceConfig.h"
#include <sstream>
namespace libparaver {

ParaverTraceConfig::ParaverTraceConfig() {
    debug = false;
    level = "THREAD";
    units = "NANOSEC";
    look_back = 100;
    speed = 1;
    flag_icons = "ENABLED";
    num_of_state_colors = 1000;
    ymax_scale = 10;
    thread_func = "State As Is";
}

ParaverTraceConfig::~ParaverTraceConfig() {
    states_color_type::const_iterator states_color_begin = states_color.begin();
    states_color_type::const_iterator states_color_end = states_color.end();
    while (states_color_begin != states_color_end) {
        delete states_color_begin->second;
        states_color_begin++;
    }
}

void ParaverTraceConfig::setDebug(bool value) {
    debug = value;
}

void ParaverTraceConfig::setLevel(std::string value) {
    level = value;
}

void ParaverTraceConfig::setUnits(std::string value) {
    units = value;
}

void ParaverTraceConfig::setLook_back(int value) {
    look_back = value;
}

void ParaverTraceConfig::setSpeed(int value) {
    speed = value;
}

void ParaverTraceConfig::setFlag_icons(std::string value) {
    flag_icons = value;
}

void ParaverTraceConfig::setNum_of_state_colors(int value) {
    num_of_state_colors = value;
}

void ParaverTraceConfig::setYmax_scale(int value) {
    ymax_scale = value;
}

void ParaverTraceConfig::setThread_func(std::string value) {
    thread_func = value;
}

void ParaverTraceConfig::addState(int key, std::string value) {
    if (states.find(key) != states.end()) {
        LOG("STATE with key: " + boost::lexical_cast<std::string>(key)
            + " -> \"" + states[key] + "\" overwritten to \"" + value + "\"")
    }
    states[key] = value;
}

std::vector<int> ParaverTraceConfig::getStates() const {
    std::vector<int> keys;
    states_type::const_iterator begin = states.begin();
    states_type::const_iterator end = states.end();
    while (begin != end) {
        keys.push_back(begin->first);
        begin++;
    }
    return keys;
}

std::string ParaverTraceConfig::getState(int key) const {
    if (states.find(key) == states.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }
    return states.find(key)->second;
}

void ParaverTraceConfig::addStateColor(int key, int r, int g, int b) {
    StateColor * sc = new StateColor(r, g, b);
    if (states_color.find(key) != states_color.end()) {
        StateColor * oldSC = states_color[key];
        LOG("STATES_COLOR with key: " + boost::lexical_cast<std::string>(key)
            + " -> \"" + oldSC->toString() + "\" overwritten to \"" + sc->toString() + "\"")
        delete oldSC;
    }
    states_color[key] = sc;
}

std::vector<int> ParaverTraceConfig::getStateColors() const {
    std::vector<int> keys;
    states_color_type::const_iterator begin = states_color.begin();
    states_color_type::const_iterator end = states_color.end();
    while (begin != end) {
        keys.push_back(begin->first);
        begin++;
    }
    return keys;
}

UIParaverTraceConfig::StateColor ParaverTraceConfig::getStateColor(int key) const {
    if (states_color.find(key) == states_color.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }
    return *(states_color.find(key)->second);
}

void ParaverTraceConfig::addGradientName(int key, std::string value) {
    if (gradient_names.find(key) != gradient_names.end()) {
        LOG("GraDIENT_NAME with key: " + boost::lexical_cast<std::string>(key)
            + " -> \"" + gradient_names[key] + "\" overwritten to \"" + value + "\"")
    }
    gradient_names[key] = value;
}

std::vector<int> ParaverTraceConfig::getGradientNames() const {
    std::vector<int> keys;
    gradient_names_type::const_iterator begin = gradient_names.begin();
    gradient_names_type::const_iterator end = gradient_names.end();
    while (begin != end) {
        keys.push_back(begin->first);
        begin++;
    }
    return keys;
}

std::string ParaverTraceConfig::getGradientName(int key) const {
    if (gradient_names.find(key) == gradient_names.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }
    return gradient_names.find(key)->second;
}

void ParaverTraceConfig::addGradientColor(int key, int r, int g, int b) {
    GradientColor * gc = new GradientColor(r, g, b);
    if (gradient_colors.find(key) != gradient_colors.end()) {
        GradientColor * oldGC = gradient_colors[key];
        LOG("STATES_COLOR with key: " + boost::lexical_cast<std::string>(key)
            + " -> \"" + oldGC->toString() + "\" overwritten to \"" + gc->toString() + "\"")
        delete oldGC;
    }
    gradient_colors[key] = gc;
}

std::vector<int> ParaverTraceConfig::getGradientColors() const {
    std::vector<int> keys;
    gradient_color_type::const_iterator begin = gradient_colors.begin();
    gradient_color_type::const_iterator end = gradient_colors.end();
    while (begin != end) {
        keys.push_back(begin->first);
        begin++;
    }
    return keys;
}

UIParaverTraceConfig::GradientColor ParaverTraceConfig::getGradientColor(int key) const {
    if (gradient_colors.find(key) == gradient_colors.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }
    return *(gradient_colors.find(key)->second);
}

void ParaverTraceConfig::addEventTypes(std::vector<EventType *> & eventTypes_) {
    std::vector<EventType *>::const_iterator begin = eventTypes_.begin();
    std::vector<EventType *>::const_iterator end = eventTypes_.end();
    while (begin != end) {
        if (event_types.find((*begin)->getKey()) != event_types.end()) {
            EventType * oldEvent = event_types[(*begin)->getKey()];
            LOG("EVENT_TYPE with key: " + boost::lexical_cast<std::string>(oldEvent->getKey())
                + " -> \"" + oldEvent->toString() + "\" overwritten to \"" + (*begin)->toString() +"\"")
            if( (*begin)->getEventValues() != oldEvent->getEventValues() )
              delete oldEvent;
        }
        event_types[(*begin)->getKey()] = (*begin);
        begin++;
    }
}

bool ParaverTraceConfig::parse(string_iterator_type begin,string_iterator_type end, bool resend) {
    if(begin == end) {
        BOOST_THROW_EXCEPTION(std::runtime_error("There is nothing to parse!"));
    }
    bool r = false;
    typedef classic::position_iterator2<string_iterator_type> pos_iterator_type;
    pos_iterator_type position_begin( begin, end, "-");
    pos_iterator_type position_end;

    try {
        // This is necessary to track the line and column position
        ParaverTraceConfigGrammar<pos_iterator_type, ParaverTraceConfig> pcfGrammar(*this);
        r = qi::phrase_parse(position_begin, position_end, pcfGrammar, SKIPPER);
    } catch (const qi::expectation_failure<pos_iterator_type> & e) {
        if(resend) throw;
        const classic::file_position_base<std::string>&  pos =
            e.first.get_position();
        std::stringstream msg;
        msg <<
        "parse error at file " << pos.file <<
        " line " << pos.line << " column " << pos.column << std::endl <<
        "'" << e.first.get_currentline() << "'" << std::endl <<
        std::setw(pos.column) << " " << "^- here";
        msg<< "\nExpected: "<<e.what_<<std::endl;
        BOOST_THROW_EXCEPTION(std::runtime_error(msg.str()));
    }
    return r;
}

bool ParaverTraceConfig::parse(std::istream & input, const std::string filename, bool resend) {
    input.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        input.seekg (0, input.beg); // Checking the istream
    }catch (std::exception & e) {
        BOOST_THROW_EXCEPTION(std::runtime_error("Unable to parse " + filename));
    }

    bool r = false;
    // iterate over stream input
    typedef std::istreambuf_iterator<char> base_iterator_type;
    base_iterator_type in_begin(input);

    // convert input iterator to forward iterator, usable by spirit parser
    typedef boost::spirit::multi_pass<base_iterator_type> forward_iterator_type;
    forward_iterator_type fwd_begin = boost::spirit::make_default_multi_pass(in_begin);
    forward_iterator_type fwd_end;

    // wrap forward iterator with position iterator, to record the position
    typedef classic::position_iterator2<forward_iterator_type> pos_iterator_type;
    pos_iterator_type position_begin(fwd_begin, fwd_end, filename);
    pos_iterator_type position_end;

    ParaverTraceConfigGrammar<pos_iterator_type, ParaverTraceConfig> pcfGrammar(*this);
    try {
        //r = qi::phrase_parse(fwd_begin, fwd_end, pcfGrammar, SKIPPER);
        r = qi::phrase_parse(position_begin, position_end, pcfGrammar, SKIPPER);
    } catch (const qi::expectation_failure<pos_iterator_type> & e) {
        if(resend) throw;
        const classic::file_position_base<std::string>&  pos =
            e.first.get_position();
        std::stringstream msg;
        msg <<
        "parse error at file " << pos.file <<
        " line " << pos.line << " column " << pos.column << std::endl <<
        "'" << e.first.get_currentline() << "'" << std::endl <<
        std::setw(pos.column) << " " << "^- here";
        msg<< "\nExpected: "<<e.what_<<std::endl;
        BOOST_THROW_EXCEPTION(std::runtime_error(msg.str()));
    }

    return r;
}

bool ParaverTraceConfig::parse(const std::string & filename, bool resend) {
    typedef classic::position_iterator2<string_iterator_type> pos_iterator_type;
    std::vector<qi::expectation_failure<pos_iterator_type> > exceptions;
    std::ifstream fh(filename.c_str(), std::ios::in);
    if(!fh.is_open()) {
        BOOST_THROW_EXCEPTION(std::runtime_error("Unable to open " + filename + " file!"));
    }
    unsigned int nlines = 0;
    std::string line;
    while(fh.good()) {
        std::getline(fh, line);
        nlines++;
    }
    nlines--; // substract the eof
    bool endLine = false;
    unsigned int lineOffset = 0;
    while(!endLine) {
        fh.clear();
        fh.seekg(0, std::ios::beg);
        unsigned int readLines = 0;
        for (unsigned int i=0; i < lineOffset && fh.good(); i++)
            std::getline(fh, line);
        std::string fileContent;
        while(fh.good()) {
            std::getline(fh, line);
            fileContent += line + "\n";
            readLines++;
        }
        try {
            parse(fileContent.begin(), fileContent.end(), true);
            endLine = true;
        } catch(const qi::expectation_failure<pos_iterator_type> & e) {
            exceptions.push_back(e);
            const classic::file_position_base<std::string>&  pos =
                e.first.get_position();
            std::stringstream msg;
            msg <<
                "parse error at file " << pos.file <<
                " line " << pos.line + lineOffset << " column " << pos.column <<
                std::endl <<
                "'" << e.first.get_currentline() << "'" << std::endl <<
                std::setw(pos.column) << " " << "^- here";
                msg<< "\nExpected: "<<e.what_<<std::endl;
            if(!resend) std::cout << msg.str();
            lineOffset += pos.line;
            if(lineOffset>=nlines) endLine=true;
        }
    }
    fh.close();
    if(resend){
        std::vector<std::string> tmp;
        for(unsigned int i=0; i < exceptions.size(); i++){
            const classic::file_position_base<std::string>&  pos =
                exceptions[i].first.get_position();
            std::string stmp = pos.file;
            stmp += ";" + pos.line;
            stmp += ";" + pos.column;
            tmp.push_back(stmp);
        }
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::pcf_format_error(tmp));
    }
    return true;
}

std::string ParaverTraceConfig::toString() const {
    std::string str = "";

    //str += "--------------- PCF DUMP BEGIN -----------------\n";
    // DEFAULT_OPTIONS
    str += "DEFAULT_OPTIONS\n\n";
    str += "LEVEL               " + level + "\n";
    str += "UNITS               " + units + "\n";
    str += "LOOK_BACK           " + boost::lexical_cast<std::string>(look_back) + "\n";
    str += "SPEED               " + boost::lexical_cast<std::string>(speed) + "\n";
    str += "FLAG_ICONS          " + flag_icons + "\n";
    str += "NUM_OF_STATE_COLORS " + boost::lexical_cast<std::string>(num_of_state_colors) + "\n";
    str += "YMAX_SCALE          " + boost::lexical_cast<std::string>(ymax_scale) + "\n";
    str += "\n\n";

    str += "DEFAULT_SEMANTIC\n\n";
    str += "THREAD_FUNC         "  + thread_func + "\n";
    str += "\n\n";

    // STATES
    states_type::const_iterator states_begin = states.begin();
    states_type::const_iterator states_end = states.end();
    str += "STATES\n";
    while (states_begin != states_end) {
        std::stringstream ss;
        ss<<states_begin->first;
        str += ss.str();
        str += " ";
        str += states_begin->second;
        str += "\n";
        states_begin++;
    }

    str += "\n\n";

    // STATES COLOR
    str += "STATES_COLOR\n";
    states_color_type::const_iterator states_color_begin = states_color.begin();
    states_color_type::const_iterator states_color_end = states_color.end();
    while (states_color_begin != states_color_end) {
        str += boost::lexical_cast<std::string>(states_color_begin->first) + " " + states_color_begin->second->toString() + "\n";
        states_color_begin++;
    }

    str += "\n\n";

    // GRADIENT_COLOR
    str += "GRADIENT_COLOR\n";
    gradient_color_type::const_iterator gradient_color_begin = gradient_colors.begin();
    gradient_color_type::const_iterator gradient_color_end = gradient_colors.end();
    while (gradient_color_begin != gradient_color_end) {
        str += boost::lexical_cast<std::string>(gradient_color_begin->first) + " " + gradient_color_begin->second->toString() + "\n";
        gradient_color_begin++;
    }

    str += "\n\n";

    // GRADIENT_NAME
    str += "GRADIENT_NAMES\n";
    gradient_names_type::const_iterator gradient_name_begin = gradient_names.begin();
    gradient_names_type::const_iterator gradient_name_end = gradient_names.end();
    while (gradient_name_begin != gradient_name_end) {
        str += boost::lexical_cast<std::string>(gradient_name_begin->first) + " " + gradient_name_begin->second + "\n";
        gradient_name_begin++;
    }

    str += "\n\n";


    // EVENT_TYPE
    std::vector< std::vector< unsigned int > > groupedTypes = getGroupedEventTypes();
    for ( std::vector< std::vector< unsigned int > >::const_iterator itGroup = groupedTypes.begin(); itGroup != groupedTypes.end(); ++itGroup )
    {
      str += "EVENT_TYPE\n";

      for ( std::vector< unsigned int >::const_iterator itType = (*itGroup).begin(); itType != (*itGroup).end(); ++itType )
      {
        if ( itType == --(*itGroup).end() )
          str += event_types.at( (int)(*itType) )->toString() + "\n";
        else
          str += event_types.at( (int)(*itType) )->toStringWithoutValues() + "\n";
      }

      str += "\n\n";
/*
    event_types_type::const_iterator event_types_begin = event_types.begin();
    event_types_type::const_iterator event_types_end = event_types.end();
    while (event_types_begin != event_types_end) {
        str += "EVENT_TYPE\n";
        str += event_types_begin->second->toString() + "\n";
        event_types_begin++;
        str += "\n\n";
*/
    }


    //str += "--------------- PCF DUMP END   -----------------\n";
    return str;
}


int ParaverTraceConfig::getEventType(const std::string eventTypeValue) const {
    event_types_type::const_iterator event_types_begin = event_types.begin();
    event_types_type::const_iterator event_types_end = event_types.end();
    while (event_types_begin != event_types_end) {
        if (event_types_begin->second->getDescription().compare(eventTypeValue)==0)
            return event_types_begin->second->getKey();
        event_types_begin++;
    }
    BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    return -1;
}

std::string ParaverTraceConfig::getEventType(const int eventTypeKey) const {
    if (event_types.find(eventTypeKey) == event_types.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }
    return event_types.find(eventTypeKey)->second->getDescription();
}

int ParaverTraceConfig::getEventValue(const int eventTypeKey, const std::string eventValue) const {
    if (event_types.find(eventTypeKey) == event_types.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
        return -1;
    }
    return event_types.find(eventTypeKey)->second->getEventValues()->getValue(eventValue);
}

std::string ParaverTraceConfig::getEventValue(const int eventTypeKey, const int eventValueKey) const {
    if (event_types.find(eventTypeKey) == event_types.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }
    return event_types.find(eventTypeKey)->second->getEventValues()->getValue(eventValueKey);
}

std::vector<unsigned int > ParaverTraceConfig::getEventTypes() const {
    std::vector<unsigned int> keys;
    event_types_type::const_iterator event_types_begin = event_types.begin();
    event_types_type::const_iterator event_types_end = event_types.end();
    while (event_types_begin != event_types_end) {
        keys.push_back(event_types_begin->first);
        event_types_begin++;
    }
    return keys;
}

void ParaverTraceConfig::eraseEvent( unsigned int eventTypeKey )
{
  event_types.erase( eventTypeKey );
}

std::vector<unsigned int > ParaverTraceConfig::getEventValues(unsigned int eventTypeKey) const {
    if (event_types.find(eventTypeKey) == event_types.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }
    return event_types.find(eventTypeKey)->second->getEventValues()->getValues();
}


std::vector< std::vector< unsigned int > > ParaverTraceConfig::getGroupedEventTypes() const
{
  std::vector< std::vector< unsigned int > > groupedEventTypes;
  std::vector< unsigned int > currentGroup;

  event_types_type::const_iterator firstType = event_types.begin();
  event_types_type::const_iterator currentType = firstType;

  EventType::EventValuesPtr firstTypeValues;

  currentGroup.push_back( (unsigned int)(*currentType).first );

  try
  {
    firstTypeValues = (*firstType).second->getEventValues();
  }
  catch ( UIParaverTraceConfig::value_not_found )
  {}

  while ( ++currentType != event_types.end() )
  {
    EventType::EventValuesPtr currentTypeValues;
    try
    {
      currentTypeValues = (*currentType).second->getEventValues();
    }
    catch ( UIParaverTraceConfig::value_not_found )
    {}

    if ( currentTypeValues != firstTypeValues )
    {
      groupedEventTypes.push_back( currentGroup );
      firstType = currentType;
      firstTypeValues = currentTypeValues;
      currentGroup.clear();
    }

    currentGroup.push_back( (unsigned int)(*currentType).first );
  }

  return groupedEventTypes;
}


void ParaverTraceConfig::setEventValues( unsigned int eventTypeKey, std::map< unsigned int, std::string > &values )
{
    if (event_types.find(eventTypeKey) == event_types.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }

    event_types[ eventTypeKey ]->setEventValues( values );
}


/* StateColor class */

ParaverTraceConfig::StateColor::StateColor(int red_, int green_, int blue_):
        UIParaverTraceConfig::StateColor(red_, green_, blue_) {
}

std::string ParaverTraceConfig::StateColor::toString() const {
    std::string str ="";
    str += "{" + boost::lexical_cast<std::string>(red) + ",";
    str += boost::lexical_cast<std::string>(green) + ",";
    str += boost::lexical_cast<std::string>(blue) + "}";
    return str;
}

/* GradientColor class */

ParaverTraceConfig::GradientColor::GradientColor(int red_, int green_, int blue_):
        UIParaverTraceConfig::GradientColor(red_, green_, blue_) {
}


std::string ParaverTraceConfig::GradientColor::toString() const {
    std::string str ="";
    str += "{" + boost::lexical_cast<std::string>(red) + ",";
    str += boost::lexical_cast<std::string>(green) + ",";
    str += boost::lexical_cast<std::string>(blue) + "}";
    return str;
}

/* EventType class */

ParaverTraceConfig::EventType::EventType(int color_, int key_, std::string descr_):
        UIParaverTraceConfig::EventType(color_, key_, descr_) {
}


void ParaverTraceConfig::EventType::setEventValues(EventValues * eventValues_) {
    EventValuesPtr tmp(eventValues_);
    eventValues = tmp;
}


void ParaverTraceConfig::EventType::setEventValues( std::map< unsigned int, std::string > &values )
{
  eventValues->setEventValues( values );
}


std::string ParaverTraceConfig::EventType::toString() const {
    std::string str = "";
    str += boost::lexical_cast<std::string>(color) + "\t";
    str += boost::lexical_cast<std::string>(key) + "\t";
    str += descr;
    if (eventValues) {
        str += "\nVALUES\n";
        str += eventValues->toString();
    }
    return str;
}


std::string ParaverTraceConfig::EventType::toStringWithoutValues() const {
    std::string str = "";
    str += boost::lexical_cast<std::string>(color) + "\t";
    str += boost::lexical_cast<std::string>(key) + "\t";
    str += descr;

    return str;
}


const ParaverTraceConfig::EventType::EventValuesPtr ParaverTraceConfig::EventType::getEventValues() const {
	if (eventValues == NULL)
		BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());

  return eventValues;
}

/* EventValues class */

void ParaverTraceConfig::EventValues::addValue(int key, std::string value) {
    if (eventValues.find(key) == eventValues.end()) {
        eventValues[key] = value;
    } else {
        LOG("STATE with key: " + boost::lexical_cast<std::string>(key)
            + " -> \"" + eventValues[key] + "\" overwritten to \"" + value + "\"")
    }
}

std::string ParaverTraceConfig::EventValues::toString() const {
    std::string str = "";
    EventValues::eventValues_type::const_iterator begin = eventValues.begin();
    EventValues::eventValues_type::const_iterator end = eventValues.end();
    while (begin != end) {
        str += boost::lexical_cast<std::string>(begin->first) + "\t";
        str += begin->second + "\n";
        begin++;
    }
    return str;
}

int ParaverTraceConfig::EventValues::getValue(const std::string eventValue) const {
    EventValues::eventValues_type::const_iterator begin = eventValues.begin();
    EventValues::eventValues_type::const_iterator end = eventValues.end();
    while (begin != end) {
        if (begin->second.compare(eventValue)==0) {
            return begin->first;
        }
        begin++;
    }
    BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    return -1;
}

std::string ParaverTraceConfig::EventValues::getValue(const int eventKey) const {
    if (eventValues.find(eventKey) == eventValues.end()) {
        BOOST_THROW_EXCEPTION(UIParaverTraceConfig::value_not_found());
    }
    return eventValues.find(eventKey)->second;
}

std::vector<unsigned int> ParaverTraceConfig::EventValues::getValues() const {
    std::vector<unsigned int> keys;
    EventValues::eventValues_type::const_iterator begin = eventValues.begin();
    EventValues::eventValues_type::const_iterator end = eventValues.end();
    while (begin != end) {
        keys.push_back(begin->first);
        begin++;
    }
    return keys;
}

void ParaverTraceConfig::EventValues::setEventValues( std::map< unsigned int, std::string > &values )
{
  eventValues.clear();
  for( std::map< unsigned int, std::string >::iterator it = values.begin(); it != values.end(); ++it )
  {
    eventValues[ (int)(*it).first ] = (*it).second;
  }
}

}

