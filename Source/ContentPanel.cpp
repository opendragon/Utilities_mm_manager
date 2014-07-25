//--------------------------------------------------------------------------------------------------
//
//  File:       ContentPanel.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the content area of the primary window of the channel
//              manager application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by HPlus Technologies Ltd. and Simon Fraser University.
//
//              All rights reserved. Redistribution and use in source and binary forms, with or
//              without modification, are permitted provided that the following conditions are met:
//                * Redistributions of source code must retain the above copyright notice, this list
//                  of conditions and the following disclaimer.
//                * Redistributions in binary form must reproduce the above copyright notice, this
//                  list of conditions and the following disclaimer in the documentation and/or
//                  other materials provided with the distribution.
//                * Neither the name of the copyright holders nor the names of its contributors may
//                  be used to endorse or promote products derived from this software without
//                  specific prior written permission.
//
//              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//              EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//              OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//              SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//              INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//              TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//              BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//              CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//              ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//              DAMAGE.
//
//  Created:    2014-07-21
//
//--------------------------------------------------------------------------------------------------

#include "ContentPanel.h"
#include "ChannelManagerWindow.h"
#include "EntitiesPanel.h"

#include "ODEnableLogging.h"
#include "ODLogging.h"

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the content area of the primary window of the channel manager
 application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief The initial thickness of the horizontal and vertical scrollbars. */
static const int kDefaultScrollbarThickness = 16;

/*! @brief The initial single-step size of the horizontal and vertical scrollbars. */
static const int kDefaultSingleStepSize = 10;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ContentPanel::ContentPanel(ChannelManagerWindow * containingWindow) :
    inherited(), _entitiesPanel(new EntitiesPanel(this)), _containingWindow(containingWindow)
{
    OD_LOG_ENTER(); //####
    _entitiesPanel->setSize(_entitiesPanel->getWidth(),
                            _entitiesPanel->getHeight() -_containingWindow->getTitleBarHeight());
    setSize(_entitiesPanel->getWidth(), _entitiesPanel->getHeight());
    setScrollBarsShown(true, true);
    setScrollBarThickness(kDefaultScrollbarThickness);
    setSingleStepSizes(kDefaultSingleStepSize, kDefaultSingleStepSize);
    _entitiesPanel->setVisible(true);
    setViewedComponent(_entitiesPanel);
    OD_LOG_EXIT_P(this); //####
} // ContentPanel::ContentPanel

ContentPanel::~ContentPanel(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::~ContentPanel

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void ContentPanel::resized(void)
{
    OD_LOG_OBJENTER(); //####
    _entitiesPanel->setSize(getWidth(), getHeight());
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::resized

void ContentPanel::visibleAreaChanged(const Rectangle<int> & newVisibleArea)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_L4("nVA.x = ", newVisibleArea.getX(), "nVA.y = ", newVisibleArea.getY(), //####
              "nVA.w = ", newVisibleArea.getWidth(), "nVA.h = ", newVisibleArea.getHeight()); //####
#if 0
    OD_LOG("about to call adjustSize(true)"); //####
    _entitiesPanel->adjustSize(true);
    ScrollBar * horizBar = getHorizontalScrollBar();
    ScrollBar * vertBar = getVerticalScrollBar();
    
    if (horizBar)
    {
//                horizBar->setRangeLimits(minLeft, maxRight);
        horizBar->setCurrentRange(getViewPositionX(), getViewWidth());
//                horizBar->setCurrentRange(outerL, outerR - outerL);
    }
    if (vertBar)
    {
//                vertBar->setRangeLimits(minY, maxY);
//                vertBar->setCurrentRange(outerT, outerB - outerT);
    }
#endif // 0
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::visibleAreaChanged
