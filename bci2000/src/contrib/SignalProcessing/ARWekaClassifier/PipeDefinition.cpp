////////////////////////////////////////////////////////////////////////////////
// $Id: PipeDefinition.cpp 1723 2008-01-16 17:46:33Z mellinger $
// Description: This file defines which filters will be used, and the sequence
//   in which they are applied.
//   Each Filter() entry consists of the name of the filter and a string which,
//   by lexical comparison, defines the relative position of the filter in the
//   sequence.
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
////////////////////////////////////////////////////////////////////////////////
#include "PCHIncludes.h"
#pragma hdrstop

#include "SpatialFilter.h"
#include "ARFilter.h"
#include "WekaClassifier.h"

Filter( SpatialFilter, 2.B );
Filter( ARFilter, 2.C );
Filter( WekaClassifier, 2.C4 );

