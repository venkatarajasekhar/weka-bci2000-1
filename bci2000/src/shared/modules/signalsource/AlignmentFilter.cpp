////////////////////////////////////////////////////////////////////////////////
// $Id: AlignmentFilter.cpp 1723 2008-01-16 17:46:33Z mellinger $
// Author: schalk@wadsworth.org, juergen.mellinger@uni-tuebingen.de
// Description: A BCI2000 filter performing temporal alignment of its input
//   data using linear interpolation between subsequent samples.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "AlignmentFilter.h"
#include "MeasurementUnits.h"

using namespace std;

RegisterFilter( AlignmentFilter, 1.1 );

AlignmentFilter::AlignmentFilter()
: mAlign( false )
{
 BEGIN_PARAMETER_DEFINITIONS

  "Source:Alignment int AlignChannels= 1 0 0 1 "
    "// align channels in time (0=no, 1=yes)",

  "Source:Alignment floatlist SourceChTimeOffset= 0 0 % % "
    "// time offsets for all source channels (may be empty)",

 END_PARAMETER_DEFINITIONS
}


AlignmentFilter::~AlignmentFilter()
{
}


void
AlignmentFilter::Preflight( const SignalProperties& Input,
                                  SignalProperties& Output ) const
{
  // Check the SourceChTimeOffset; only check if we do not use the default value.
  if( Parameter( "SourceChTimeOffset" )->NumValues() > 0 )
  {
    // We need to have as many entries in SourceChTimeOffset as we have digitized channels:
    if( Parameter( "SourceChTimeOffset" )->NumValues() < Input.Channels() )
      bcierr << "If not empty, the number of entries in SourceChTimeOffset must"
             << " match the number of input channels."
             << endl;
    // Each value in SourceChTimeOffset has to be >= 0 and < 1:
    for( int i = 0; i < Parameter( "SourceChTimeOffset" )->NumValues(); ++i )
      if( Parameter( "SourceChTimeOffset" )( i ) < 0
          || Parameter( "SourceChTimeOffset" )( i ) > 1 )
        bcierr << "Values in SourceChTimeOffset must be between 0 and 1." << endl;
  }
  // Requested output signal properties.
  Output = Input;
}


void
AlignmentFilter::Initialize( const SignalProperties& Input,
                             const SignalProperties& /*Output*/ )
{
  mPrevSample.clear();
  mWeightPrev.clear();
  mWeightCur.clear();

  // Do we want to align the samples in time ?
  mAlign = ( Parameter( "AlignChannels" ) == 1 );
  if( mAlign )
  {
    mPrevSample.resize( Input.Channels(), 0.0 );
    mWeightPrev.resize( Input.Channels(), 0.0 );
    mWeightCur.resize( Input.Channels(), 1.0 );

    // Calculate weight values for linear interpolation if we do not use the default value.
    if( Parameter( "SourceChTimeOffset" )->NumValues() > 0 )
    {
      for( int i = 0; i < Input.Channels(); ++i ) // get original channel position
      {
        mWeightPrev[ i ] = Parameter( "SourceChTimeOffset" )( i );
        mWeightCur[ i ] = 1.0 - mWeightPrev[ i ];
      }
    }
    // If we do use the default value, assume that all sampled channels are evenly distributed in time.
    else
    {
      float delta = 1.0 / Input.Channels();
      for( int i = 0; i < Input.Channels(); ++i ) // get original channel position
      {
        mWeightPrev[ i ] = delta * i;
        mWeightCur[ i ] = 1.0 - mWeightPrev[ i ];
      }
    }
  }
}


void
AlignmentFilter::Process( const GenericSignal& Input, GenericSignal& Output )
{
  if( mAlign ) // Perform the alignment on the input and write it into the output signal.
    for( int channel = 0; channel < Input.Channels(); ++channel )
      for( int sample = 0; sample < Input.Elements(); ++sample )
      {
        Output( channel, sample ) =
          Input( channel, sample ) * mWeightCur[ channel ]
            + mWeightPrev[ channel ] * mPrevSample[ channel ];
        mPrevSample[ channel ] = Input( channel, sample );
      }
  else // No alignment.
    Output = Input;
}



