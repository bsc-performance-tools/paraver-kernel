#include <fstream>
#include <sstream>
#include "rowlabels.h"
#include "paraverlabels.h"

RowLabels::RowLabels()
{}

RowLabels::RowLabels( const string& filename )
{
  ifstream rowFile( filename.c_str() );
  if ( !rowFile )
    return;

  while ( !rowFile.eof() )
  {
    string strLine;
    string strLevel;
    string strSize;
    vector<string> *tmpvector = NULL;

    getline( rowFile, strLine );
    if ( strLine.length() == 0 )
      continue;
    else if ( strLine[ 0 ] == '#' )
      continue;

    istringstream auxStream( strLine );
    getline( auxStream, strLevel, ' ' ); // 'LEVEL'
    getline( auxStream, strLevel, ' ' );

    if( strLevel == LEVEL_WORKLOAD )
      tmpvector = &workload;
    else if( strLevel == LEVEL_APPLICATION )
      tmpvector = &appl;
    else if( strLevel == LEVEL_TASK )
      tmpvector = &task;
    else if( strLevel == LEVEL_THREAD )
      tmpvector = &thread;
    else if( strLevel == LEVEL_SYSTEM )
      tmpvector = &system;
    else if( strLevel == LEVEL_NODE )
      tmpvector = &node;
    else if( strLevel == LEVEL_CPU )
      tmpvector = &cpu;
    else
      continue;

    getline( auxStream, strSize, 'S' ); // 'SIZE'
    getline( auxStream, strSize, ' ' ); // 'SIZE'
    getline( auxStream, strSize );

    istringstream sizeStream( strSize );
    int size;

    if ( !( sizeStream >> size ) )
      continue;

    int i = 0;
    while ( !rowFile.eof() && i < size )
    {
      getline( rowFile, strLine );
      tmpvector->push_back( strLine );
      i++;
    }

  }

  rowFile.close();
}

RowLabels::~RowLabels()
{}

string RowLabels::getRowLabel( TWindowLevel whichLevel, TObjectOrder whichRow ) const
{
  string tmpstr;
  const vector<string> *tmpvector = NULL;
  switch( whichLevel )
  {
    case WORKLOAD:
      tmpvector = &workload;
      break;
    case APPLICATION:
      tmpvector = &appl;
      break;
    case TASK:
      tmpvector = &task;
      break;
    case THREAD:
      tmpvector = &thread;
      break;
    case SYSTEM:
      tmpvector = &system;
      break;
    case NODE:
      tmpvector = &node;
      break;
    case CPU:
      tmpvector = &cpu;
      break;
    default:
      tmpvector = NULL;
  }

  if( tmpvector == NULL )
    tmpstr = "";
  else
  {
    if( tmpvector->begin() == tmpvector->end() ||
        whichRow > tmpvector->size() )
      tmpstr = "";
    else
      tmpstr = (*tmpvector)[ whichRow ];
  }

  return tmpstr;
}
