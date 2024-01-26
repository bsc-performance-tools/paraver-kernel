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

#pragma once

#include "semanticderivedhistogram.h"


class DerivedHistogramAdd: public SemanticDerivedHistogram
{
  public:
    typedef enum
    {
      MAXPARAM = 0
    } TParam;

    DerivedHistogramAdd()
    {
      setDefaultParam();
    }

    ~DerivedHistogramAdd()
    {}

    virtual TParamIndex getMaxParam() const override
    {
      return MAXPARAM;
    }

    // virtual TSemanticValue execute( const DerivedHistogramFunctionInfo *info ) override;
    virtual TSemanticValue execute( const SemanticInfo *info ) override;

    virtual void init( KHistogram *whichWindow ) override
    {}

    virtual std::string getName() override
    {
      return DerivedHistogramAdd::name;
    }

    virtual SemanticFunction *clone() override
    {
      return new DerivedHistogramAdd( *this );
    }

  protected:
    virtual const bool getMyInitFromBegin() override
    {
      return initFromBegin;
    }
    virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return ( TParamValue ) 0;
    }
    virtual std::string getDefaultParamName( TParamIndex whichParam ) override
    {
      if ( whichParam >= getMaxParam() )
        throw SemanticException( TSemanticErrorCode::maxParamExceeded );
      return "";
    }

  private:
    static const bool initFromBegin = false;
    static std::string name;
};


// class DerivedHistogramProduct: public SemanticDerivedHistogram
// {
//   public:
//     typedef enum
//     {
//       MAXPARAM = 0
//     } TParam;

//     DerivedHistogramProduct()
//     {
//       setDefaultParam();
//     }

//     ~DerivedHistogramProduct()
//     {}

//     virtual TParamIndex getMaxParam() const override
//     {
//       return MAXPARAM;
//     }

//     virtual bool isControlDerivedHistogram() override
//     {
//       return controlDerivedHistogram;
//     }

//     virtual TSemanticValue execute( const SemanticInfo *info ) override;

//     virtual void init( KTimeline *whichWindow ) override
//     {}

//     virtual std::string getName() override
//     {
//       return DerivedHistogramProduct::name;
//     }

//     virtual SemanticFunction *clone() override
//     {
//       return new DerivedHistogramProduct( *this );
//     }


//   protected:
//     virtual const bool getMyInitFromBegin() override
//     {
//       return initFromBegin;
//     }
//     virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return ( TParamValue ) 0;
//     }
//     virtual std::string getDefaultParamName( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return "";
//     }

//   private:
//     static const bool initFromBegin = false;
//     static const bool controlDerivedHistogram = false;
//     static std::string name;

// };


// class DerivedHistogramSubstract: public SemanticDerivedHistogram
// {
//   public:
//     typedef enum
//     {
//       MAXPARAM = 0
//     } TParam;

//     DerivedHistogramSubstract()
//     {
//       setDefaultParam();
//     }

//     ~DerivedHistogramSubstract()
//     {}

//     virtual TParamIndex getMaxParam() const override
//     {
//       return MAXPARAM;
//     }

//     virtual bool isControlDerivedHistogram() override
//     {
//       return controlDerivedHistogram;
//     }

//     virtual TSemanticValue execute( const SemanticInfo *info ) override;

//     virtual void init( KTimeline *whichWindow ) override
//     {}

//     virtual std::string getName() override
//     {
//       return DerivedHistogramSubstract::name;
//     }

//     virtual SemanticFunction *clone() override
//     {
//       return new DerivedHistogramSubstract( *this );
//     }


//   protected:
//     virtual const bool getMyInitFromBegin() override
//     {
//       return initFromBegin;
//     }
//     virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return ( TParamValue ) 0;
//     }
//     virtual std::string getDefaultParamName( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return "";
//     }

//   private:
//     static const bool initFromBegin = false;
//     static const bool controlDerivedHistogram = false;
//     static std::string name;

// };


// class DerivedHistogramDivide: public SemanticDerivedHistogram
// {
//   public:
//     typedef enum
//     {
//       MAXPARAM = 0
//     } TParam;

//     DerivedHistogramDivide()
//     {
//       setDefaultParam();
//     }

//     ~DerivedHistogramDivide()
//     {}

//     virtual TParamIndex getMaxParam() const override
//     {
//       return MAXPARAM;
//     }

//     virtual bool isControlDerivedHistogram() override
//     {
//       return controlDerivedHistogram;
//     }

//     virtual TSemanticValue execute( const SemanticInfo *info ) override;
//     virtual void init( KTimeline *whichWindow ) override
//     {}

//     virtual std::string getName() override
//     {
//       return DerivedHistogramDivide::name;
//     }

//     virtual SemanticFunction *clone() override
//     {
//       return new DerivedHistogramDivide( *this );
//     }


//   protected:
//     virtual const bool getMyInitFromBegin() override
//     {
//       return initFromBegin;
//     }
//     virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return ( TParamValue ) 0;
//     }
//     virtual std::string getDefaultParamName( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return "";
//     }

//   private:
//     static const bool initFromBegin = false;
//     static const bool controlDerivedHistogram = false;
//     static std::string name;

// };


// class DerivedHistogramMaximum: public SemanticDerivedHistogram
// {
//   public:
//     typedef enum
//     {
//       MAXPARAM = 0
//     } TParam;

//     DerivedHistogramMaximum()
//     {
//       setDefaultParam();
//     }

//     ~DerivedHistogramMaximum()
//     {}

//     virtual TParamIndex getMaxParam() const override
//     {
//       return MAXPARAM;
//     }

//     virtual bool isControlDerivedHistogram() override
//     {
//       return controlDerivedHistogram;
//     }

//     virtual TSemanticValue execute( const SemanticInfo *info ) override;
//     virtual void init( KTimeline *whichWindow ) override
//     {}

//     virtual std::string getName() override
//     {
//       return DerivedHistogramMaximum::name;
//     }

//     virtual SemanticFunction *clone() override
//     {
//       return new DerivedHistogramMaximum( *this );
//     }


//   protected:
//     virtual const bool getMyInitFromBegin() override
//     {
//       return initFromBegin;
//     }
//     virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return ( TParamValue ) 0;
//     }
//     virtual std::string getDefaultParamName( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return "";
//     }

//   private:
//     static const bool initFromBegin = false;
//     static const bool controlDerivedHistogram = false;
//     static std::string name;

// };


// class DerivedHistogramMinimum: public SemanticDerivedHistogram
// {
//   public:
//     typedef enum
//     {
//       MAXPARAM = 0
//     } TParam;

//     DerivedHistogramMinimum()
//     {
//       setDefaultParam();
//     }

//     ~DerivedHistogramMinimum()
//     {}

//     virtual TParamIndex getMaxParam() const override
//     {
//       return MAXPARAM;
//     }

//     virtual bool isControlDerivedHistogram() override
//     {
//       return controlDerivedHistogram;
//     }

//     virtual TSemanticValue execute( const SemanticInfo *info ) override;
//     virtual void init( KTimeline *whichWindow ) override
//     {}

//     virtual std::string getName() override
//     {
//       return DerivedHistogramMinimum::name;
//     }

//     virtual SemanticFunction *clone() override
//     {
//       return new DerivedHistogramMinimum( *this );
//     }


//   protected:
//     virtual const bool getMyInitFromBegin() override
//     {
//       return initFromBegin;
//     }
//     virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return ( TParamValue ) 0;
//     }
//     virtual std::string getDefaultParamName( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return "";
//     }

//   private:
//     static const bool initFromBegin = false;
//     static const bool controlDerivedHistogram = false;
//     static std::string name;

// };


// class DerivedHistogramDifferent: public SemanticDerivedHistogram
// {
//   public:
//     typedef enum
//     {
//       MAXPARAM = 0
//     } TParam;

//     DerivedHistogramDifferent()
//     {
//       setDefaultParam();
//     }

//     ~DerivedHistogramDifferent()
//     {}

//     virtual TParamIndex getMaxParam() const override
//     {
//       return MAXPARAM;
//     }

//     virtual bool isControlDerivedHistogram() override
//     {
//       return controlDerivedHistogram;
//     }

//     virtual TSemanticValue execute( const SemanticInfo *info ) override;
//     virtual void init( KTimeline *whichWindow ) override
//     {}

//     virtual std::string getName() override
//     {
//       return DerivedHistogramDifferent::name;
//     }

//     virtual SemanticFunction *clone() override
//     {
//       return new DerivedHistogramDifferent( *this );
//     }


//   protected:
//     virtual const bool getMyInitFromBegin() override
//     {
//       return initFromBegin;
//     }
//     virtual TParamValue getDefaultParam( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return ( TParamValue ) 0;
//     }
//     virtual std::string getDefaultParamName( TParamIndex whichParam ) override
//     {
//       if ( whichParam >= getMaxParam() )
//         throw SemanticException( TSemanticErrorCode::maxParamExceeded );
//       return "";
//     }

//   private:
//     static const bool initFromBegin = false;
//     static const bool controlDerivedHistogram = false;
//     static std::string name;

// };

