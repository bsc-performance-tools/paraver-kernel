#ifndef PARAVERTYPES_H_INCLUDED
#define PARAVERTYPES_H_INCLUDED

typedef unsigned char ParaverColor;

enum colorIndex { RED, GREEN, BLUE };

struct rgb
{
  ParaverColor red;
  ParaverColor green;
  ParaverColor blue;
};

#endif // PARAVERTYPES_H_INCLUDED
