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

#if (! defined(HAVE_OWN_SCROLLBARS))
/*! @brief The initial thickness of the horizontal and vertical scrollbars. */
static const int kDefaultScrollbarThickness = 16;
#endif // ! defined(HAVE_OWN_SCROLLBARS)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ContentPanel::ContentPanel(void) :
    inherited(), _entitiesPanel(new EntitiesPanel)
{
    OD_LOG_ENTER(); //####
    _entitiesPanel->setContainer(this);
#if defined(HAVE_OWN_SCROLLBARS)
    addAndMakeVisible(_entitiesPanel);
#endif // ! defined(HAVE_OWN_SCROLLBARS)
    setSize(_entitiesPanel->getWidth(), _entitiesPanel->getHeight());
#if (! defined(HAVE_OWN_SCROLLBARS))
    setScrollBarsShown(true, true);
    setScrollBarThickness(kDefaultScrollbarThickness);
    _entitiesPanel->setVisible(true);
    setViewedComponent(_entitiesPanel);
//#if 0
    ScrollBar * horizBar = getHorizontalScrollBar();
    ScrollBar * vertBar = getVerticalScrollBar();
    
    OD_LOG_P2("horizBar = ", horizBar, "vertBar = ", vertBar); //####
    if (horizBar)
    {
        horizBar->setAutoHide(false);
    }
    if (vertBar)
    {
        vertBar->setAutoHide(false);
    }
//#endif // 0
#endif // ! defined(HAVE_OWN_SCROLLBARS)
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
//    repaint();
    OD_LOG_OBJEXIT(); //####
} // ContentPanel::resized

