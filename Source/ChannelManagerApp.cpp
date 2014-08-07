//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelManagerApp.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the application object of the channel manager application.
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

#include "ChannelManagerApp.h"
#include "ChannelContainer.h"
#include "ScannerThread.h"

#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the application object of the channel manager application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief @c true if an exit has been requested and @c false otherwise. */
static bool lExitRequested = false;

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ChannelManagerApplication::ChannelManagerApplication(void) :
    inherited(), _mainWindow(NULL), _yarp(NULL), _scanner(NULL)
{
#if defined(MpM_ServicesLogToStandardError)
    OD_LOG_INIT(ProjectInfo::projectName, kODLoggingOptionIncludeProcessID | //####
                kODLoggingOptionIncludeThreadID | kODLoggingOptionWriteToStderr | //####
                kODLoggingOptionEnableThreadSupport); //####
#else // ! defined(MpM_ServicesLogToStandardError)
    OD_LOG_INIT(ProjectInfo::projectName, kODLoggingOptionIncludeProcessID | //####
                kODLoggingOptionIncludeThreadID | kODLoggingOptionEnableThreadSupport); //####
#endif // ! defined(MpM_ServicesLogToStandardError)
    OD_LOG_ENTER(); //####
    OD_LOG_EXIT_P(this); //####
} // ChannelManagerApplication::ChannelManagerApplication

ChannelManagerApplication::~ChannelManagerApplication(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::~ChannelManagerApplication

#if defined(__APPLE__)
# pragma mark Actions and Accessors
#endif // defined(__APPLE__)

void ChannelManagerApplication::anotherInstanceStarted(const String & commandLine)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(commandLine)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("commandLine = ", commandLine.toStdString()); //####
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::anotherInstanceStarted

const String ChannelManagerApplication::getApplicationName(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_S(ProjectInfo::projectName); //####
    return ProjectInfo::projectName;
} // ChannelManagerApplication::getApplicationName

const String ChannelManagerApplication::getApplicationVersion(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_S(ProjectInfo::versionString); //####
    return ProjectInfo::versionString;
} // ChannelManagerApplication::getApplicationVersion

void ChannelManagerApplication::initialise(const String & commandLine)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(commandLine)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_OBJENTER(); //####
    OD_LOG_S1s("commandLine = ", commandLine.toStdString()); //####
#if MAC_OR_LINUX_
    MplusM::Common::SetUpLogger(ProjectInfo::projectName);
#endif // MAC_OR_LINUX_
    MplusM::Common::Initialize(ProjectInfo::projectName);
    MplusM::Utilities::SetUpGlobalStatusReporter();
#if CheckNetworkWorks_
    if (yarp::os::Network::checkNetwork())
#endif // CheckNetworkWorks_
    {
        _yarp = new yarp::os::Network; // This is necessary to establish any connections to the YARP
                                       // infrastructure
    }
#if CheckNetworkWorks_
    else
    {
        OD_LOG("! (yarp::os::Network::checkNetwork())"); //####
# if MAC_OR_LINUX_
        yarp::os::impl::Logger & theLogger = MplusM::Common::GetLogger();
        
        theLogger.fail("YARP network not running.");
# endif // MAC_OR_LINUX_
    }
#endif // CheckNetworkWorks_
    _mainWindow = new ChannelManagerWindow(ProjectInfo::projectName);
    if (_yarp)
    {
        _scanner = new ScannerThread(ProjectInfo::projectName, *_mainWindow);
        _scanner->startThread();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::initialise

bool ChannelManagerApplication::moreThanOneInstanceAllowed(void)
{
    OD_LOG_OBJENTER(); //####
    OD_LOG_OBJEXIT_B(true); //####
    return true;
} // ChannelManagerApplication::moreThanOneInstanceAllowed

void ChannelManagerApplication::shutdown(void)
{
    OD_LOG_OBJENTER(); //####
    SetExitRequest();
    _scanner->stopThread(5000);
    _scanner = nullptr; // shuts down thread
    _mainWindow = nullptr; // (deletes our window)
    yarp::os::Network::fini();
    _yarp = nullptr;
    MplusM::Utilities::ShutDownGlobalStatusReporter();
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::shutdown

void ChannelManagerApplication::systemRequestedQuit(void)
{
    OD_LOG_OBJENTER(); //####
    // This is called when the app is being asked to quit: you can ignore this
    // request and let the app carry on running, or call quit() to allow the app to close.
    quit();
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::systemRequestedQuit

#if defined(__APPLE__)
# pragma mark Global functions
#endif // defined(__APPLE__)

#include <odl/ODDisableLogging.h>
#include <odl/ODLogging.h>
bool CheckForExit(void * stuff)
{
#if (! defined(OD_ENABLE_LOGGING))
# if MAC_OR_LINUX_
#  pragma unused(stuff)
# endif // MAC_OR_LINUX_
#endif // ! defined(OD_ENABLE_LOGGING)
    OD_LOG_ENTER(); //####
    OD_LOG_P1("stuff = ", stuff); //####
    OD_LOG_EXIT_B(lExitRequested); //####
    return lExitRequested;
} // CheckForExit
#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

void SetExitRequest(void)
{
    OD_LOG_ENTER(); //####
    lExitRequested = true;
    OD_LOG_EXIT(); //####
} // SetExitRequest
