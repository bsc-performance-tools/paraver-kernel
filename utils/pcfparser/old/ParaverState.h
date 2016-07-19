#ifndef LIBPARAVER_PARAVERSTATE_H
#define LIBPARAVER_PARAVERSTATE_H


#include <string>

namespace libparaver {

class ParaverState {
  protected:
    int key;

    std::string value;


  public:
    ParaverState(int key, std::string value);

    virtual ~ParaverState();

    inline int get_key() const;

    inline std::string get_value() const;
};

inline int ParaverState::get_key() const
{
  return key;
}

inline std::string ParaverState::get_value() const
{
  return value;
}

} // namespace libparaver
#endif
