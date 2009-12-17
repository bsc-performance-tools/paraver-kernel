/*****************************************************************************\
 *                        ANALYSIS PERFORMANCE TOOLS                         *
 *                               libparaver-api                              *
 *                       Paraver Main Computing Library                      *
 *****************************************************************************
 *     ___     This library is free software; you can redistribute it and/or *
 *    /  __         modify it under the terms of the GNU LGPL as published   *
 *   /  /  _____    by the Free Software Foundation; either version 2.1      *
 *  /  /  /     \   of the License, or (at your option) any later version.   *
 * (  (  ( B S C )                                                           *
 *  \  \  \_____/   This library is distributed in hope that it will be      *
 *   \  \__         useful but WITHOUT ANY WARRANTY; without even the        *
 *    \___          implied warranty of MERCHANTABILITY or FITNESS FOR A     *
 *                  PARTICULAR PURPOSE. See the GNU LGPL for more details.   *
 *                                                                           *
 * You should have received a copy of the GNU Lesser General Public License  *
 * along with this library; if not, write to the Free Software Foundation,   *
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA          *
 * The GNU LEsser General Public License is contained in the file COPYING.   *
 *                                 ---------                                 *
 *   Barcelona Supercomputing Center - Centro Nacional de Supercomputacion   *
\*****************************************************************************/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- *\
 | @file: $HeadURL$
 | @last_commit: $Date$
 | @version:     $Revision$
\* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#ifndef SEMANTICFUNCTION_H_INCLUDED
#define SEMANTICFUNCTION_H_INCLUDED

#include <vector>
#include <stack>
#include "memorytrace.h"
#include "semanticexception.h"
#include "semanticinfo.h"

class KWindow;

using namespace std;

class SemanticFunction
{
  public:
    SemanticFunction()
    {}
    virtual ~SemanticFunction()
    {}

    void setDefaultParam()
    {
      for ( TParamIndex i = 0; i < getMaxParam(); i++ )
      {
        parameters.push_back( getDefaultParam( i ) );
        parametersName.push_back( getDefaultParamName( i ) );
      }
    }

    const bool getInitFromBegin()
    {
      return getMyInitFromBegin();
    }

    virtual TParamIndex getMaxParam() const = 0;
    virtual const TParamValue& getParam( TParamIndex whichParam ) const
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return parameters[whichParam];
    }

    virtual void setParam( TParamIndex whichParam, const TParamValue& newValue )
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      parameters[whichParam] = newValue;
    }

    virtual string getParamName( TParamIndex whichParam ) const
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( SemanticException::maxParamExceeded );
      return parametersName[whichParam];
    }

    virtual TSemanticValue execute( const SemanticInfo *info ) = 0;

    virtual void init( KWindow *whichWindow ) = 0;

    virtual string getName() = 0;

    virtual SemanticFunction *clone() = 0;

    virtual vector<vector<TSemanticValue> > *getStack()
    {
      return NULL;
    }

    virtual SemanticInfoType getSemanticInfoType() const
    {
      return NO_TYPE;
    }

  protected:
    vector<TParamValue> parameters;
    vector<string> parametersName;

    virtual TParamValue getDefaultParam( TParamIndex whichParam ) = 0;
    virtual string getDefaultParamName( TParamIndex whichParam ) = 0;

    // Must initialize from the beginning of the trace
    virtual const bool getMyInitFromBegin() = 0;

  private:

};


#endif // SEMANTICFUNCTION_H_INCLUDED
