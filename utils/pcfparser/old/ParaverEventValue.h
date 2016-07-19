#ifndef LIBPARAVER_PARAVEREVENTVALUE_H
#define LIBPARAVER_PARAVEREVENTVALUE_H


#include <string>
#include <iostream>

namespace libparaver {

class ParaverEventValue {
  protected:
    unsigned int key;

    std::string value;


  public:
    ParaverEventValue(int key, std::string value);

    virtual ~ParaverEventValue();

    inline const unsigned int get_key() const;

    void set_key(unsigned int value);

    inline const std::string get_value() const;

    void set_value(std::string new_value);

    friend std::ostream & operator<<(std::ostream & os, const ParaverEventValue & ptraceConfig);

};
inline const unsigned int ParaverEventValue::get_key() const {
  return key;
}

inline const std::string ParaverEventValue::get_value() const {
  return value;
}


} // namespace libparaver
#endif
