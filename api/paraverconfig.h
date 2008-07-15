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
    bool getShowUnits() const;

  private:
    static ParaverConfig *instance;

    ParaverConfig();

    UINT32 precision;
    TObjectOrder histoNumColumns;
    bool showUnits;

};

#endif // PARAVERCONFIG_H_INCLUDED
