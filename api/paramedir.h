#ifndef PARAMEDIR_H_INCLUDED
#define PARAMEDIR_H_INCLUDED

#include <vector>
#include <string>

using std::vector;
using std::string;

class Window;
class Histogram;

void dumpWindow( vector<Window *>& windows, string& strOutputFile );

void dumpHistogram( vector<Histogram *>& histograms, string& strOutputFile );

void printHelp();

void readParaverConfigFile();

#endif // PARAMEDIR_H_INCLUDED
