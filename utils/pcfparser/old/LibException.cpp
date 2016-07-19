
#include "LibException.h"
#include <sstream>

using namespace std;


namespace libparaver {

LibException::LibException() {
}

LibException::LibException(string file, int line, string message) {
  stringstream ss;
  ss<<"(FFF) ("<<file<<":"<<line<<") - "<<message<<endl;
  reason = ss.str();
}

LibException::~LibException() throw() {
}

const char * LibException::what() const throw() {
  return reason.c_str();
}

const char * LibException::where() const throw() {
  return stack.c_str();
}


} // namespace libparaver
