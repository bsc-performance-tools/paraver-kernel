
#include "ParaverEventValue.h"

using namespace std;

namespace libparaver {

ParaverEventValue::ParaverEventValue(int key, string value):key(key),value(value) {
}

ParaverEventValue::~ParaverEventValue() {
}

void ParaverEventValue::set_key(unsigned int value) {
  key = value;
}

void ParaverEventValue::set_value(string new_value) {
  value = new_value;
}

ostream & operator<<(ostream & os, const ParaverEventValue & ptraceConfig) {
  os<<"\t"<<ptraceConfig.key<<" -> "<<ptraceConfig.value<<endl;
  return os;
}


} // namespace libparaver
