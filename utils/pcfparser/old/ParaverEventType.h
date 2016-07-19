#ifndef LIBPARAVER_PARAVEREVENTTYPE_H
#define LIBPARAVER_PARAVEREVENTTYPE_H


#include <string>
#include <vector>
#include <iostream>

namespace libparaver { class ParaverEventValue; }

namespace libparaver {

class ParaverEventType {
  protected:
    bool eventValueIsMine;

    int key;

    std::string description;

    int color;

    std::vector<ParaverEventValue *> eventValues;


  public:
    ParaverEventType(int key, std::string description, int color);

    virtual ~ParaverEventType();

    void addValues(ParaverEventValue * value, bool isMine);

    inline const bool get_eventValueIsMine() const;

    void set_eventValueIsMine(bool value);

    inline const int get_key() const;

    void set_key(int value);

    inline const std::string get_description() const;

    void set_description(std::string value);

    inline const int get_color() const;

    void set_color(int value);

    inline const std::vector<ParaverEventValue *> & get_eventValues() const;

    void set_eventValues(std::vector<ParaverEventValue *> & value);

    std::string get_eventValueOfKey(unsigned int key);

    int get_eventKeyOfValue(const std::string value);

    friend std::ostream & operator<<(std::ostream & os, const ParaverEventType & ptraceConfig);

};
inline const bool ParaverEventType::get_eventValueIsMine() const {
  return eventValueIsMine;
}

inline const int ParaverEventType::get_key() const {
  return key;
}

inline const std::string ParaverEventType::get_description() const {
  return description;
}

inline const int ParaverEventType::get_color() const {
  return color;
}

inline const std::vector<ParaverEventValue *> & ParaverEventType::get_eventValues() const {
  return eventValues;
}


} // namespace libparaver
#endif
