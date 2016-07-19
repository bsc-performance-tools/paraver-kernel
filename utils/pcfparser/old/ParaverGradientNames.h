#ifndef LIBPARAVER_PARAVERGRADIENTNAMES_H
#define LIBPARAVER_PARAVERGRADIENTNAMES_H


#include <string>

namespace libparaver {

class ParaverGradientNames {
  protected:
    int key;

    std::string value;


  public:
    ParaverGradientNames(int key, std::string value);

    virtual ~ParaverGradientNames();

};

} // namespace libparaver
#endif
