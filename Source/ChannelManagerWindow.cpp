//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelManagerWindow.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the primary window of the channel manager application.
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
//  Created:    2014-07-14
//
//--------------------------------------------------------------------------------------------------

#include "ChannelManagerWindow.h"
#include "ContentPanel.h"

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the primary window of the channel manager application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and destructors
#endif // defined(__APPLE__)

ChannelManagerWindow::ChannelManagerWindow(const String & title)  :
    inherited(title, Colours::lightgrey, inherited::allButtons),
    _contentPanel(new ContentPanel(this)), _scannerThread(NULL)
{
    OD_LOG_ENTER(); //####
    setOpaque(true);
    setResizable(true, true);
    setContentOwned(_contentPanel, true);
    centreWithSize(getWidth(), getHeight());
    setVisible(true);
    OD_LOG_EXIT_P(this); //####
} // ChannelManagerWindow::ChannelManagerWindow

ChannelManagerWindow::~ChannelManagerWindow(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerWindow::~ChannelManagerWindow

#if defined(__APPLE__)
# pragma mark Actions
#endif // defined(__APPLE__)

void ChannelManagerWindow::closeButtonPressed(void)
{
    OD_LOG_OBJENTER(); //####
    // This is called when the user tries to close this window. Here, we'll just
    // ask the app to quit when this happens, but you can change this to do
    // whatever you need.
    JUCEApplication::getInstance()->systemRequestedQuit();
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerWindow::closeButtonPressed

#if defined(__APPLE__)
# pragma mark Accessors
#endif // defined(__APPLE__)

EntitiesPanel & ChannelManagerWindow::getEntitiesPanel(void)
const
{
    OD_LOG_OBJENTER(); //####
    EntitiesPanel & thePanel(_contentPanel->getEntitiesPanel());
    
    OD_LOG_OBJEXIT_P(&thePanel); //####
    return thePanel;
} // ChannelManagerWindow::getEntitiesPanel

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)
