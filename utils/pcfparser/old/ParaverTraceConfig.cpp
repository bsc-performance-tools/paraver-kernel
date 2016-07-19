
#include "ParaverTraceConfig.h"
#include "ParaverEventValue.h"
#include "ParaverState.h"
#include "ParaverStatesColor.h"
#include "ParaverEventType.h"
#include "ParaverGradientColor.h"
#include "ParaverGradientNames.h"
#include "LibException.h"
#include "ParaverTraceConfigGrammar.h"
#include <sstream>

using namespace std;


namespace libparaver {

ParaverTraceConfig::ParaverTraceConfig(string pcfFile):pcfFile(pcfFile) {
  parse();
}

ParaverTraceConfig::~ParaverTraceConfig() {
  for(unsigned int i = 0; i < states.size(); i++){
     delete states[i];
  }

  for(unsigned int i = 0; i < statesColor.size(); i++){
     delete statesColor[i];
  }

  for(unsigned int i = 0; i < eventTypes.size(); i++){
     delete eventTypes[i];
  }

  for(unsigned int i = 0; i < gradientColors.size(); i++){
     delete gradientColors[i];
  }

  for(unsigned int i = 0; i < gradientNames.size(); i++){
     delete gradientNames[i];
  }
}

void ParaverTraceConfig::parse() {
  iterator_t first(pcfFile);
  if (!first){
    stringstream ss;
    ss<<"Error opening "<<pcfFile;
    throw LibException(__FILE__, __LINE__, ss.str());
  }
  iterator_t last = first.make_end();
  ParaverTraceConfigGrammar<ParaverTraceConfig> pcfGrammar(*this);
  BOOST_SPIRIT_DEBUG_NODE(pcfGrammar);
  parse_info <iterator_t> info = BOOST_SPIRIT_CLASSIC_NS::parse(
        first,
        last,
        pcfGrammar, space_p - eol_p
     );

  // Oh! It's maaagic
  std::cout.flush();

/*  if (info.full)
  {
    std::cout << endl;
  }else{
    std::cout << endl;
  }
*/
/*  if (info.full){
     std::cout << "\n"<<pcfFile<<" parse succeeded!\n";
  }else{
    stringstream ss;
    ss<< "\n"<<pcfFile<<"PCF parse failed! at:"<< info.stop << ": " << last << std::endl;
    throw LibException(__FILE__, __LINE__, ss.str());
  }
*/
}

ParaverEventType * ParaverTraceConfig::getEventType(const string eventType) {
  string l_eventType(eventType);
  vector<ParaverEventType *>::iterator it = eventTypes.begin();
  while(it != eventTypes.end()){
     if((*it)->get_description().compare(eventType) == 0){
        return *it;
     }
     it++;
  }
  return 0;
}

ParaverEventType * ParaverTraceConfig::getEventType(int eventType) {
  vector<ParaverEventType *>::iterator it = eventTypes.begin();
  while(it != eventTypes.end()){
     if((*it)->get_key() == eventType){
        return *it;
     }
     it++;
  }
  return 0;
}

int ParaverTraceConfig::getNumValues() {
  return 0;
}

int ParaverTraceConfig::getValue() {
  return 0;
}

void ParaverTraceConfig::addState(const int key, const string value) {
  states.push_back(new ParaverState(key, value));
}

void ParaverTraceConfig::addStateColor(const int key, const int red, const int green, const int blue) {
  statesColor.push_back(new ParaverStatesColor(key, red, green, blue));
}

void ParaverTraceConfig::addEventType(const int color, const int key, const string description) {
  currentEvents.push_back(new ParaverEventType(key, description, color));
}

void ParaverTraceConfig::addGradientColor(const int key, const int red, const int green, const int blue) {
  gradientColors.push_back(new ParaverGradientColor(key, red, green, blue));
}

void ParaverTraceConfig::addGradientNames(const int key, const string value) {
  gradientNames.push_back(new ParaverGradientNames(key, value));
}

void ParaverTraceConfig::commitEventTypes() {
  for(unsigned int i = 0; i < currentEvents.size(); i++){
     eventTypes.push_back(currentEvents[i]);
     i_eventTypes.push_back(currentEvents[i]->get_key());
  }
  currentEvents.clear();
}

void ParaverTraceConfig::addValues(const int key, const string value) {
  eventValue = new ParaverEventValue(key, value);
  for(unsigned int i = 0; i < currentEvents.size(); i++){
     if(i==0) currentEvents[i]->addValues(eventValue, true);
     else currentEvents[i]->addValues(eventValue, false);
  }
}

ostream & operator<<(ostream & os, const ParaverTraceConfig & ptraceConfig) {
  for(uint i=0; i < ptraceConfig.eventTypes.size(); i++){
    os<<*ptraceConfig.eventTypes[i];
  }
  return os;
}


} // namespace libparaver
