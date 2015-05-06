//--------------------------------------------------------------------------------------------------
//
//  File:       ChannelManagerApplication.cpp
//
//  Project:    M+M
//
//  Contains:   The class definition for the application object of the channel manager application.
//
//  Written by: Norman Jaffe
//
//  Copyright:  (c) 2014 by H Plus Technologies Ltd. and Simon Fraser University.
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

#include "ChannelManagerApplication.h"
#include "EntitiesPanel.h"
#include "PeekInputHandler.h"
#include "ScannerThread.h"
#include "YarpLaunchThread.h"

#include <mpm/M+MRequests.h>

//#include <odl/ODEnableLogging.h>
#include <odl/ODLogging.h>

#include <arpa/inet.h>

#if defined(__APPLE__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunknown-pragmas"
# pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif // defined(__APPLE__)
/*! @file
 
 @brief The class definition for the application object of the channel manager application. */
#if defined(__APPLE__)
# pragma clang diagnostic pop
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Namespace references
#endif // defined(__APPLE__)

using namespace ChannelManager;
using namespace MplusM;
using namespace std;

#if defined(__APPLE__)
# pragma mark Private structures, constants and variables
#endif // defined(__APPLE__)

/*! @brief @c true if an exit has been requested and @c false otherwise. */
static bool lExitRequested = false;

#if defined(__APPLE__)
# pragma mark Global constants and variables
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Local functions
#endif // defined(__APPLE__)

/*! @brief Check if YARP can be launched and if the user wishes it to be.
 @param yarpPath The file system path to the YARP executable.
 @returns @c true if the user requests that a private YARP network be set up and @ c false if the
 YARP executable is invalid or the user does not want a private YARP network. */
static bool validateYarp(const String & yarpPath)
{
    OD_LOG_ENTER(); //####
    bool         doLaunch = false;
    ChildProcess runYarp;
    String       appName(JUCEApplication::getInstance()->getApplicationName());
    StringArray  nameAndArgs(yarpPath);
    
    nameAndArgs.add("version");
    if (runYarp.start(nameAndArgs))
    {
        const String childOutput(runYarp.readAllProcessOutput());
        
        runYarp.waitForProcessToFinish(10000);
        if (0 < childOutput.length())
        {
            // We have a useable YARP executable - ask what the user wants to do.
            doLaunch = (1 == AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                                          "Do you wish to launch a private YARP "
                                                          "network?",
                                                          "If you do, it may take a few moments to "
                                                          "start, depending on network traffic and "
                                                          "system activity. "
                                                          "Also, the private YARP network will be "
                                                          "shut down when this application exits, "
                                                          "resulting in a potential loss of "
                                                          "connectivity to any M+M services that "
                                                          "were started after the private YARP "
                                                          "network was launched.", String::empty,
                                                          String::empty, nullptr, nullptr));
        }
        else
        {
            // The YARP executable can't actually be launched!
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, appName,
                                        "No YARP network was detected and the YARP executable "
                                        "found in the PATH system environment variable did not "
                                        "return valid data. "
                                        "Execution is not possible.");
        }
    }
    else
    {
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, appName,
                                    "No YARP network was detected and the YARP executable found in "
                                    "the PATH system environment variable could not be started. "
                                    "Execution is not possible.");
    }
    OD_LOG_EXIT_B(doLaunch); //####
    return doLaunch;
} // validateYarp

#if defined(__APPLE__)
# pragma mark Class methods
#endif // defined(__APPLE__)

#if defined(__APPLE__)
# pragma mark Constructors and Destructors
#endif // defined(__APPLE__)

ChannelManagerApplication::ChannelManagerApplication(void) :
    inherited(), _mainWindow(nullptr), _yarp(nullptr), _scanner(nullptr), _yarpLauncher(nullptr),
    _peeker(nullptr), _peekHandler(nullptr)
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

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
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
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

yarp::os::Network * ChannelManagerApplication::checkForYarpAndLaunchIfDesired(void)
{
    OD_LOG_OBJENTER(); //####
    yarp::os::ConstString    yarpPath(Utilities::FindPathToExecutable("yarp"));
    yarp::os::Network *      result = nullptr;
#if MAC_OR_LINUX_
    yarp::os::impl::Logger & theLogger = Common::GetLogger();
#endif // MAC_OR_LINUX_
    
    // No running YARP server was detected - first check if YARP is actually available:
    if (0 < yarpPath.length())
    {
        // If YARP is installed, give the option of running a private copy.
        if (validateYarp(yarpPath.c_str()))
        {
            struct in_addr       serverAddress;
            int                  serverPort;
            Common::StringVector ipAddresses;
            
#if MAC_OR_LINUX_
            theLogger.warning("Private YARP network being launched.");
#endif // MAC_OR_LINUX_
            Utilities::GetMachineIPs(ipAddresses);
            if (Utilities::GetCurrentYarpConfiguration(serverAddress, serverPort))
            {
                if (INADDR_NONE != serverAddress.s_addr)
                {
                    char         buffer[INET_ADDRSTRLEN + 1];
                    const char * addressAsString = inet_ntop(AF_INET, &serverAddress.s_addr, buffer,
                                                             sizeof(buffer));
                    if (addressAsString)
                    {
                        std::cerr << addressAsString << ":" << serverPort << std::endl;//$$$$
                    }
                }
            }


            _yarpLauncher = new YarpLaunchThread(yarpPath.c_str());
            if (_yarpLauncher)
            {
                _yarpLauncher->startThread();
                // Sleep for a little while and recheck if YARP is active.
                for ( ; ! result; )
                {
                    Thread::sleep(100);
                    result = new yarp::os::Network;
                }
            }
        }
    }
    else
    {
        // If YARP isn't installed, say so and leave.
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, getApplicationName(),
                                    "No YARP network was detected and a YARP executable could not "
                                    "be found in the PATH system environment variable. "
                                    "Execution is not possible.");
    }
    OD_LOG_OBJEXIT_P(result); //####
    return result;
} // ChannelManagerApplication::checkForYarpAndLaunchIfDesired

void ChannelManagerApplication::connectPeekChannel(void)
{
    OD_LOG_OBJENTER(); //####
    if (_peeker)
    {
        if (! Utilities::NetworkConnectWithRetries(MpM_REGISTRY_STATUS_NAME, _peeker->name(),
                                                   STANDARD_WAIT_TIME))
        {
            OD_LOG("(! Utilities::NetworkConnectWithRetries(MpM_REGISTRY_STATUS_NAME, " //####
                   "_peeker->name(), STANDARD_WAIT_TIME))"); //####
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::connectPeekChannel

void ChannelManagerApplication::doCleanupSoon(void)
{
    OD_LOG_OBJENTER(); //####
    if (_scanner)
    {
        _scanner->doScanSoon();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::doCleanupSoon

void ChannelManagerApplication::doScanSoon(void)
{
    OD_LOG_OBJENTER(); //####
    if (_scanner)
    {
        _scanner->doScanSoon();
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::doScanSoon

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

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
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
    Common::SetUpLogger(ProjectInfo::projectName);
#endif // MAC_OR_LINUX_
    Common::Initialize(ProjectInfo::projectName);
    Utilities::SetUpGlobalStatusReporter();
#if defined(MpM_ReportOnConnections)
    ChannelStatusReporter * reporter = Utilities::GetGlobalStatusReporter();
#endif // defined(MpM_ReportOnConnections)

    Utilities::CheckForNameServerReporter();
    if (Utilities::CheckForValidNetwork(true))
    {
        _yarp = new yarp::os::Network; // This is necessary to establish any connections to the YARP
                                       // infrastructure
    }
    else
    {
        OD_LOG("! (yarp::os::Network::checkNetwork())"); //####
#if MAC_OR_LINUX_
        yarp::os::impl::Logger & theLogger = Common::GetLogger();
#endif // MAC_OR_LINUX_

#if MAC_OR_LINUX_
        theLogger.warning("YARP network not running.");
#endif // MAC_OR_LINUX_
        _yarp = checkForYarpAndLaunchIfDesired();
    }
    _mainWindow = new ChannelManagerWindow(ProjectInfo::projectName);
    if (_yarp)
    {
        EntitiesPanel & entities = _mainWindow->getEntitiesPanel();
        
        entities.recallPositions();
        _peeker = new Common::AdapterChannel(false);
        _peekHandler = new PeekInputHandler;
        if (_peeker && _peekHandler)
        {
#if defined(MpM_ReportOnConnections)
            _peeker->setReporter(reporter);
            _peeker->getReport(reporter);
#endif // defined(MpM_ReportOnConnections)
            yarp::os::ConstString peekName = Common::GetRandomChannelName(HIDDEN_CHANNEL_PREFIX
                                                                          "peek_/"
                                                                          DEFAULT_CHANNEL_ROOT);
            
            if (_peeker->openWithRetries(peekName, STANDARD_WAIT_TIME))
            {
                _peeker->setReader(*_peekHandler);
                _scanner = new ScannerThread(*_mainWindow);
                _scanner->startThread();
            }
        }
    }
    OD_LOG_OBJEXIT(); //####
} // ChannelManagerApplication::initialise
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

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
    if (_scanner)
    {
        _scanner->stopNow();
        _scanner->stopThread(5000);
    }
    if (_yarpLauncher)
    {
        _yarpLauncher->stopNow();
        _yarpLauncher->stopThread(5000);
    }
    EntitiesPanel & entities = _mainWindow->getEntitiesPanel();

    entities.rememberPositions();
#if defined(MpM_DoExplicitClose)
    _peeker->close();
#endif // defined(MpM_DoExplicitClose)
    Common::AdapterChannel::RelinquishChannel(_peeker);
    _scanner = nullptr; // shuts down thread
	_yarpLauncher = nullptr; // shuts down thread
    _mainWindow = nullptr; // (deletes our window)
    yarp::os::Network::fini();
    _yarp = nullptr;
    Utilities::ShutDownGlobalStatusReporter();
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

#if (! MAC_OR_LINUX_)
# pragma warning(push)
# pragma warning(disable: 4100)
#endif // ! MAC_OR_LINUX_
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
#if (! MAC_OR_LINUX_)
# pragma warning(pop)
#endif // ! MAC_OR_LINUX_

void SetExitRequest(void)
{
    OD_LOG_ENTER(); //####
    lExitRequested = true;
    OD_LOG_EXIT(); //####
} // SetExitRequest
