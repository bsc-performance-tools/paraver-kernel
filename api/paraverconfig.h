#ifndef PARAVERCONFIG_H_INCLUDED
#define PARAVERCONFIG_H_INCLUDED

#include "paraverkerneltypes.h"

class ParaverConfig
{
  public:
    ParaverConfig();

    UINT32 getPrecision() const;

    static void readParaverConfigFile( ParaverConfig& config );

  private:
    UINT32 precision;

};

#endif // PARAVERCONFIG_H_INCLUDED
