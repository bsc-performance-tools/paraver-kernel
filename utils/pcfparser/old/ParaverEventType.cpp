
#include "ParaverEventType.h"
#include "ParaverEventValue.h"

using namespace std;

namespace libparaver {

ParaverEventType::ParaverEventType(int key, string description, int color):key(key),description(description),color(color) {
}

ParaverEventType::~ParaverEventType() {
  if(eventValueIsMine){
     for(unsigned int i = 0; i < eventValues.size(); i++){
        delete eventValues[i];
     }
  }
}

void ParaverEventType::addValues(ParaverEventValue * value, bool isMine) {
  eventValueIsMine = isMine;
  eventValues.push_back(value);
}

void ParaverEventType::set_eventValueIsMine(bool value) {
  eventValueIsMine = value;
}

void ParaverEventType::set_key(int value) {
  key = value;
}

void ParaverEventType::set_description(string value) {
  description = value;
}

void ParaverEventType::set_color(int value) {
  color = value;
}

void ParaverEventType::set_eventValues(vector<ParaverEventValue *> & value) {
  eventValues = value;
}

string ParaverEventType::get_eventValueOfKey(unsigned int key) {
  for(unsigned int i = 0; i < eventValues.size(); i++){
     if(eventValues[i]->get_key() == key) return eventValues[i]->get_value();
  }
  return "Not found";
}

int ParaverEventType::get_eventKeyOfValue(const string value) {
  for(unsigned int i = 0; i < eventValues.size(); i++){
     if(eventValues[i]->get_value() == value) return eventValues[i]->get_key();
  }
  return -1;
}

ostream & operator<<(ostream & os, const ParaverEventType & ptraceConfig) {
  os<<ptraceConfig.description<<":"<<endl;
  for(unsigned int i=0; i < ptraceConfig.eventValues.size(); i++){
    os<<*ptraceConfig.eventValues[i];
  }
  return os;
}


} // namespace libparaver
