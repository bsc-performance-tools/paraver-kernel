#ifndef PARAVERCONFIG_H_INCLUDED
#define PARAVERCONFIG_H_INCLUDED

#include "paraverkerneltypes.h"

class ParaverConfig
{
  public:
    static ParaverConfig *getInstance();

    static void readParaverConfigFile();
    static bool writeDefaultConfig();

    UINT32 getPrecision() const;
    TObjectOrder getHistoNumColumns() const;

  private:
    static ParaverConfig *instance;

    ParaverConfig();

    UINT32 precision;
    TObjectOrder histoNumColumns;

};

#endif // PARAVERCONFIG_H_INCLUDED
