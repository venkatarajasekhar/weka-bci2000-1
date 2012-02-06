/////////////////////////////////////////////////////////////////////////////
//
// File: GUIReinforcementView.cpp
//
// Date: Nov 7, 2001
//
// Author: Juergen Mellinger
//
// Description: The TGUIReinforcementView class implements the GUI specific details
//              of the TReinforcementView class.
//
// Changes:
//
// (C) 2000-2008, BCI2000 Project
// http://www.bci2000.org
//////////////////////////////////////////////////////////////////////////////

#ifdef __BORLANDC__
#include "PCHIncludes.h"
#pragma hdrstop
#endif // __BORLANDC__

#include "OSIncludes.h"

#ifndef VCL
# error This is the VCL implementation of TGUIReinforcementView.
#endif

#include <vector>
#include <sstream>

#include "GUIReinforcementView.h"
#include "Utils/Util.h"
#include "ViewsRes.h"

TGUIReinforcementView::TGUIReinforcementView()
: TGUIView( reinforcementViewZ ),
  numAnimationFrames( 0 ),
  animationBitmap( NULL ),
  animationCounter( 0 ),
  lastAnimationCounter( 0 ),
  timerID( NULL )
{
}

TGUIReinforcementView::~TGUIReinforcementView()
{
    if( timerID != NULL )
        timeKillEvent( timerID );

    for( std::vector<Graphics::TBitmap*>::iterator i = animFrames.begin();
                                        i != animFrames.end(); i++ )
        delete *i;
}

void
TGUIReinforcementView::Paint()
{
    if( animationCounter != lastAnimationCounter )
    {
        if( animationCounter < numAnimationFrames )
            animationBitmap = animFrames[ animationCounter ];
        else
            animationBitmap = NULL;
        lastAnimationCounter = animationCounter;
    }
    if( animationBitmap != NULL )
        GetCanvas()->Draw( animTRect.Left, animTRect.Top, animationBitmap );
}

void
TGUIReinforcementView::Resized()
{
    TGUIView::Resized();
    if( numAnimationFrames > 0 )
    {
        Graphics::TBitmap   *bitmap = animFrames[ 0 ];
        animTRect.Left = viewTRect.Left + ( viewTRect.Width() - bitmap->Width ) / 2;
        animTRect.Top = viewTRect.Top + ( viewTRect.Height() - bitmap->Height ) / 2;
        animTRect.Right = animTRect.Left + bitmap->Width;
        animTRect.Bottom = animTRect.Top + bitmap->Height;
    }
}

TPresError
TGUIReinforcementView::InitAnimation( /*char    *inFileName*/ )
{
    animationBitmap = NULL;
    for( std::vector<Graphics::TBitmap*>::iterator i = animFrames.begin();
                                        i != animFrames.end(); i++ )
        delete *i;
    animFrames.clear();
    numAnimationFrames = 0;

    // Build a list of animation frames.
    std::ostringstream  resName;
    resName << cRiAnimPrefix << numAnimationFrames + 1;
    HBITMAP bmpHandle = ::LoadBitmap( HInstance, resName.str().c_str() );

    while( bmpHandle != NULL )
    {
        Graphics::TBitmap   *bitmap = new Graphics::TBitmap;
        bitmap->Handle = bmpHandle;
        bitmap->Transparent = true;
        bitmap->TransparentMode = tmAuto;
        animFrames.push_back( bitmap );
        numAnimationFrames++;

        resName.str( "" );
        resName << cRiAnimPrefix << numAnimationFrames + 1;
        bmpHandle = ::LoadBitmap( HInstance, resName.str().c_str() );
    }

    Resized();

    return presNoError;
}

void
TGUIReinforcementView::PlayAnimation()
{
    if( timerID != NULL )
    {
        ::timeKillEvent( timerID );
        timerID = NULL;
    }
    if( numAnimationFrames > 0 )
    {
        animationBitmap = *animFrames.begin();
        animationCounter = 0;
        lastAnimationCounter = -1;
        TGUIView::InvalidateRect( animTRect );
        timerID = ::timeSetEvent( animFrameLength, animFrameLength / 2,
                                    AnimationCallback, ( DWORD )this, TIME_PERIODIC );
    }
}

void
CALLBACK
TGUIReinforcementView::AnimationCallback(   UINT    inTimerID,
                                            UINT,
                                            DWORD   inInstance,
                                            DWORD,
                                            DWORD )
{
    TGUIReinforcementView   *caller = ( TGUIReinforcementView* )inInstance;
    // We don't know whether the caller didn't start another sequence.
    if( inTimerID == caller->timerID )
    {
      ++caller->animationCounter;
      if( caller->animationCounter > caller->numAnimationFrames )
      {
          ::timeKillEvent( inTimerID );
          caller->timerID = NULL;
      }
    }
    else
      ::timeKillEvent( inTimerID );
      
    TGUIView::InvalidateRect( caller->animTRect );
}

