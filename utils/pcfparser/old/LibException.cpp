
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

LibException::~LibException() noexcept {
}

const char * LibException::what() const noexcept {
  return reason.c_str();
}

const char * LibException::where() const noexcept {
  return stack.c_str();
}


} // namespace libparaver
